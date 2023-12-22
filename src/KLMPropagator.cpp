#include "KLMPropagator.h"
#include <utility>
#include <stack>
#include <unordered_map>

KLMPropagator::KLMPropagator(solver &s, const func_decl &nodeFct, Logic logic,
                             std::unordered_map<expr, expr_template*, expr_hash, expr_eq> abstraction) :
        user_propagator_base(&s), nodeFct(nodeFct), abstraction(std::move(abstraction)), logic(logic) {
    register_fixed();
    register_created();
}

void KLMPropagator::push() {
    //Log("Push");
    undoStackSize.push_back(undoStack.size());
}

void KLMPropagator::pop(unsigned n) {
    // Log("Pop: " + n);
    int size = undoStackSize[undoStackSize.size() - n];
    undoStackSize.erase(undoStackSize.end() - (int) n, undoStackSize.end());
    for (int i = undoStack.size(); i > size; i--) {
        undoStack[i - 1]();
    }
    undoStack.erase(undoStack.begin() + size, undoStack.end());
}

void KLMPropagator::fixed(const expr &e, const expr &v) {
    try {
        // Log("Fixed: " << e << " = " << v);
        assert(!(v.is_true() && v.is_false()));
        bool val = v.is_true();
        assert(interpretation.find(e) == interpretation.end());
        interpretation[e] = val; // To keep the expression alive

        undoStack.emplace_back([this, e] {
            interpretation.erase(e);
        });

        if (!Z3_is_eq_func_decl(ctx(), e.decl(), nodeFct))
            return;
        const auto it = exprToEdge.find(e);
        assert(it != exprToEdge.end());
        auto edge = it->second;
        edge->set_state(val ? Connected : NonConnected);

        undoStack.emplace_back([edge] {
            edge->set_state(Unknown);
        });

        if (val)
            AddPosConnection(edge);
        else
            AddNegConnection(edge);
    }
    catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        exit(-1);
    }
}

void KLMPropagator::AddPosConnection(edge* e1) {
    if (e1->get_from()->is_connected(e1->get_to()))
        return;

    e1->get_from()->add_connected(e1->get_to(), e1);
    undoStack.emplace_back([e1] { e1->get_from()->remove_connected(e1->get_to()); });

    // Eq
    if (e1->get_to()->is_connected(e1->get_from())) {
        const auto backEdge = e1->get_to()->get_connected(e1->get_from());
        for (const auto &e2: e1->get_to()->get_connected()) {
            if (e1->get_from() == e2.second->get_to())
                continue;
            expr_vector just(ctx());
            just.push_back(e1->get_expr());
            just.push_back(backEdge->get_expr());
            just.push_back(e2.second->get_expr());
            //std::cout << "Connecting: " << e1->get_from()->to_string() << " with " << e2.second->get_to()->to_string()
            //          << " due to eq1" << std::endl;
            propagate(just, nodeFct(e1->get_from()->get_label(), e2.second->get_to()->get_label()));
        }

        // TODO: check
        for (const auto &e2: e1->get_from()->get_connected()) {
            if (e1->get_to() == e2.second->get_to())
                continue;
            expr_vector just(ctx());
            just.push_back(e1->get_expr());
            just.push_back(backEdge->get_expr());
            just.push_back(e2.second->get_expr());
            //std::cout << "Connecting: " << e1->get_to()->to_string() << " with " << e2.second->get_to()->to_string()
            //          << " due to eq2" << std::endl;
            propagate(just, nodeFct(e1->get_to()->get_label(), e2.second->get_to()->get_label()));
        }
    }

    // Propagate to already existing equalities
    for (const auto &e2: e1->get_from()->get_connected()) {
        if (e2.second->get_to()->is_connected(e1->get_from())) {
            expr_vector just(ctx());
            just.push_back(e1->get_expr());
            just.push_back(e2.second->get_expr());
            just.push_back(e2.second->get_to()->get_connected(e1->get_from())->get_expr());
            //std::cout << "Connecting: " << e2.second->get_to()->to_string() << " with " << e1->get_to()->to_string()
            //          << " due to eq3" << std::endl;
            propagate(just, nodeFct(e2.second->get_to()->get_label(), e1->get_to()->get_label()));
        }
    }

    // Non-Entailment (II)
    // We might propagate new stuff now
    for (const auto &nonConnected: e1->get_from()->get_none_connected()) {
        PropagateMissingTrue(nonConnected.second, e1);
    }

    assert(!HasTrans() || HasLoop());

    if (HasLoop()) {
        if (!HasTrans()) { // Trans subsumes Loop; just apply only Trans

            if (e1->get_to()->is_transitive_out(e1->get_from())) {
                const expr_vector cycleJustExpr = e1->get_to()->get_transitive_out_just(e1->get_from());
                // We now have at least one cycle

                for (auto &r1: e1->get_to()->get_transitive_out()) {
                    if (r1.first->is_transitive_out(e1->get_from())) {
                        // r1 seems to be on one cycle as well

                        for (auto &r2: r1.first->get_connected()) {
                            if (r2.first->is_transitive_out(e1->get_from())) {
                                // ... and r2 as well
                                if (!r2.first->is_connected(r1.first)) {
                                    // And there is no edge yet between r1 and r2
                                    propagate(cycleJustExpr, nodeFct(r2.first->get_label(), r1.first->get_label()));
                                }
                            }
                        }
                    }
                }
            }
        }

        // Maintain transitive closure
        for (auto &r1: e1->get_from()->get_transitive_in()) {
            for (auto &r2: e1->get_to()->get_transitive_out()) {
                auto s1 = r1;
                auto s2 = r2;
                if (!s1.first->is_transitive_out(s2.first)) {
                    assert(!s2.first->is_transitive_in(s1.first));
                    std::vector<::edge*> just(s1.first->get_transitive_out(e1->get_from()));
                    just.push_back(e1);
                    for (const auto &e2: e1->get_to()->get_transitive_out(r2.first)) {
                        just.push_back(e2);
                    }

                    if (HasTrans()) {
                        expr_vector cycleJustExpr(ctx());
                        for (const auto &expr: just) {
                            cycleJustExpr.push_back(expr->get_expr());
                        }
                        propagate(cycleJustExpr, nodeFct(s1.first->get_label(), s2.first->get_label()));
                    }

                    auto l1 = s1.first;
                    auto l2 = s2.first;
                    l1->add_transitive_out(l2, just);
                    l2->add_transitive_in(l1, just);
                    undoStack.emplace_back([l1, l2] {
                        l1->remove_transitive_out(l2);
                        l2->remove_transitive_in(l1);
                    });
                }
            }
        }
    }
}

void KLMPropagator::AddNegConnection(edge* edge) {
    if (edge->get_from()->is_none_connected(edge->get_to()))
        return;

    edge->get_from()->add_none_connected(edge->get_to(), edge);
    undoStack.emplace_back([edge] { edge->get_from()->remove_none_connected(edge->get_to()); });

    // Non-Entailment (I)
    // The eager part
    expr_vector just(ctx());
    just.push_back(edge->get_expr());
    propagate(just, !abstraction[edge->get_to()->get_label()]->get_instance(edge));

    // Propagate everything we know already as well
    // We might propagate other stuff later on
    for (const auto &connected: edge->get_from()->get_connected())
        PropagateMissingTrue(edge, connected.second);
    if (HasOr()) {
        for (const auto &nonConnected: edge->get_from()->get_none_connected())
            PropagateMissingFalse(edge, nonConnected.second);

        // We might propagate pending non-entailment for other edges
        for (const auto nonConnected: edge->get_from()->get_none_connected()) {
            PropagateMissingFalse(nonConnected.second, edge);
        }
    }
}

void KLMPropagator::PropagateMissingTrue(edge* baseEdge, edge* connectedEdge) {
    assert(connectedEdge->get_from()->is_connected(connectedEdge->get_to()));
    expr_vector just(ctx());
    just.push_back(baseEdge->get_expr());
    just.push_back(connectedEdge->get_expr());
    propagate(just, abstraction[connectedEdge->get_to()->get_label()]->get_instance(baseEdge));
}

void KLMPropagator::PropagateMissingFalse(edge* baseEdge, edge* nonConnectedEdge) {
    assert(nonConnectedEdge->get_from()->is_none_connected(nonConnectedEdge->get_to()));
    expr_vector just(ctx());
    just.push_back(baseEdge->get_expr());
    just.push_back(nonConnectedEdge->get_expr());
    propagate(just, !abstraction[nonConnectedEdge->get_to()->get_label()]->get_instance(baseEdge));
}

void KLMPropagator::created(const expr &e) {
    try {
        if (!Z3_is_eq_func_decl(ctx(), e.decl(), nodeFct))
            return;
        auto e1 = e.arg(0);
        auto e2 = e.arg(1);
        // Log("Created: " << e);
        if (!ignoreCreated) {
            e1 = e1.simplify();
            e2 = e2.simplify();
        }

        node* n1;
        node* n2;
        auto it = exprToNode.find(e1);

        if (it == exprToNode.end()) {
            n1 = new node(e1);
            exprToNode[e1] = n1;
            ignoreCreated = true;
            for (const auto &existing: exprToNode) {
                add(nodeFct(n1->get_label(), existing.second->get_label()));
                add(nodeFct(existing.second->get_label(), n1->get_label()));
            }
            ignoreCreated = false;

            // Reflexivity
            auto self = nodeFct(e1, e1);
            // Debug.assert(exprToEdge.ContainsKey(self)); -- could have been in the input already
            n1->add_transitive_in(n1, std::vector<edge*>());
            n1->add_transitive_out(n1, std::vector<edge*>());
            add(self);
            const expr_vector empty(ctx());
            propagate(empty, self);
        } else
            n1 = it->second;

        it = exprToNode.find(e2);
        if (it == exprToNode.end()) {
            n2 = new node(e2);
            exprToNode[e2] = n2;
            ignoreCreated = true;
            for (const auto &existing: exprToNode) {
                add(nodeFct(n2->get_label(), existing.second->get_label()));
                add(nodeFct(existing.second->get_label(), n2->get_label()));
            }
            ignoreCreated = false;

            // Reflexivity
            auto self = nodeFct(e2, e2);
            // Debug.assert(exprToEdge.ContainsKey(self));
            n2->add_transitive_in(n2, std::vector<edge*>());
            n2->add_transitive_out(n2, std::vector<edge*>());
            add(self);
            const expr_vector empty(ctx());
            propagate(empty, self);
        } else
            n2 = it->second;

        exprToEdge[e] = new edge(n1, n2, e);
    }
    catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        exit(-1);
    }
}

std::string KLMPropagator::ExprToString(const node &node, bool smtlib) const {
    if (smtlib)
        return '"' + node.to_string() + '"';
    return '"' + abstraction.find(node.get_label())->second->ExprToString() + '"';
}

static void model_failed(std::string msg) {
    std::cerr << "Model checking failed: " << msg << std::endl;
    exit(-2);
}

Z3_lbool KLMPropagator::eval(const expr &e) {
    if (e.is_true())
        return Z3_L_TRUE;
    if (e.is_false())
        return Z3_L_FALSE;
    if (e.decl().decl_kind() == Z3_OP_UNINTERPRETED) {
        auto it = interpretation.find(e);
        if (it == interpretation.end())
            return Z3_L_UNDEF;
        return it->second ? Z3_L_TRUE : Z3_L_FALSE;
    }
    if (e.is_not()) {
        auto res = eval(e.arg(0));
        if (res == Z3_L_TRUE)
            return Z3_L_FALSE;
        if (res == Z3_L_FALSE)
            return Z3_L_TRUE;
        return Z3_L_UNDEF;
    }
    if (e.is_and()) {
        auto val = Z3_L_TRUE;
        for (auto arg: e.args()) {
            auto arg_eval = eval(arg);
            if (arg_eval == Z3_L_FALSE)
                return Z3_L_FALSE;
            if (arg_eval == Z3_L_UNDEF)
                val = Z3_L_UNDEF;
        }
        return val;
    }
    if (e.is_or()) {
        auto val = Z3_L_FALSE;
        for (auto arg: e.args()) {
            auto arg_eval = eval(arg);
            if (arg_eval == Z3_L_TRUE)
                return Z3_L_TRUE;
            if (arg_eval == Z3_L_UNDEF)
                val = Z3_L_UNDEF;
        }
        return val;
    }
    if (e.is_implies())
        return eval(!e.arg(0) || e.arg(1));
    if (e.is_eq())
        return eval((!e.arg(0) || e.arg(1)) && (!e.arg(1) || e.arg(0)));

    model_failed("Unexpected expression: " + e);
    assert(false);
    return Z3_L_UNDEF;
}

Z3_lbool KLMPropagator::eval(node* n, edge* edge) {
    expr e = abstraction[n->get_label()]->get_instance(edge);
    return eval(e);
}

void KLMPropagator::check_model() {
    if (logic != C && logic != CL)
        std::cout << "Warning: Currently only checking conditions for C and CL" << std::endl;
    for (auto np: exprToNode) {
        auto n = np.second;
        if (n->get_connected().size() + n->get_none_connected().size() != exprToNode.size())
            model_failed("outgoing edges for " + n->to_string() + " are not fully defined");
        for (auto e: n->get_connected()) {
            if (e.second->get_state() != Connected)
                model_failed("edge " + e.second->get_from()->to_string() + " -> " + e.second->get_to()->to_string() +
                             " is both connected and not");
        }
        for (auto e: n->get_none_connected()) {
            if (e.second->get_state() != NonConnected)
                model_failed("edge " + e.second->get_from()->to_string() + " -> " + e.second->get_to()->to_string() +
                             " is both connected and not");
        }

    }
    // Refl
    for (const auto &n: exprToNode) {
        if (!n.second->is_connected(n.second) || n.second->is_none_connected(n.second))
            model_failed("Node " + n.second->to_string() + " is not reflexive");
    }
    // Eq
    for (const auto &e: exprToEdge) {
        if (
                e.second->get_state() == Connected &&
                e.second->get_to()->is_connected(e.second->get_from())) {
            for (auto &con: e.second->get_from()->get_connected()) {
                if (!e.second->get_to()->is_connected(con.second->get_to()) ||
                    e.second->get_to()->is_none_connected(con.second->get_to()))
                    model_failed(
                            "Node " + e.second->get_from()->to_string() + " and " + e.second->get_to()->to_string() +
                            " are mutually connected and " +
                            e.second->get_from()->to_string() + " with " + con.second->to_string() + " but not " +
                            e.second->get_to()->to_string());
            }
        }
    }
    // Eval
    for (const auto &e: exprToEdge) {
        if (e.second->get_state() == NonConnected) {
            if (eval(e.second->get_to(), e.second) != Z3_L_FALSE)
                model_failed("No edge between " + e.second->get_from()->to_string() + " and " +
                             e.second->get_to()->to_string() + " requires " + e.second->get_to()->to_string() +
                             " to evaluate to false");

            for (auto &con: e.second->get_from()->get_connected()) {
                if (eval(con.second->get_to(), e.second) != Z3_L_TRUE)
                    model_failed("No edge between " + e.second->get_from()->to_string() + " and " +
                                 e.second->get_to()->to_string() + " requires " + con.second->get_to()->to_string() +
                                 " to evaluate to true");
            }
        }
    }

    if (HasLoop()) {
        // maybe unsigned and bor the already visited rows
        bool* reachable = new bool[exprToNode.size() * exprToNode.size()];
        unsigned cnt = exprToNode.size();
        auto is_reachable = [reachable, cnt](unsigned from, unsigned to) {
            return reachable[cnt * from + to];
        };
        unsigned i = 0;
        std::unordered_map<node*, unsigned> nodeToId;
        for (const auto &n: exprToNode) {
            nodeToId[n.second] = i++;
        }

        for (const auto &np: exprToNode) {
            auto n1 = np.second;
            unsigned id1 = nodeToId[n1];
            std::stack<node*> toProcess;
            bool* r = reachable + (cnt * id1);
            toProcess.push(n1);
            while (!toProcess.empty()) {
                auto n2 = toProcess.top();
                unsigned id2 = nodeToId[n2];
                toProcess.pop();
                if (r[id2])
                    continue;
                r[id2] = true;
                for (const auto &n3: n2->get_connected()) {
                    toProcess.push(n3.second->get_to());
                }
            }
        }

        //if (!HasTrans()) { // Trans subsumes Loop; just apply only Trans

        for (const auto &e: exprToEdge) {
            if (e.second->get_state() != Connected)
                continue;
            if (is_reachable(nodeToId[e.second->get_to()], nodeToId[e.second->get_from()])) {
                for (auto &r1: exprToNode) {
                    if (!is_reachable(nodeToId[e.second->get_to()], nodeToId[r1.second]) ||
                        !is_reachable(nodeToId[r1.second], nodeToId[e.second->get_to()]))
                        continue;

                    for (auto &r2: r1.second->get_connected()) {
                        if (is_reachable(nodeToId[r2.first], nodeToId[e.second->get_from()]) &&
                            !r2.first->is_connected(r1.second)) {
                            if (!r2.first->is_connected(r1.second))
                                model_failed("node " + r2.first->to_string() + " should be connected to " +
                                             r1.second->to_string() + " because of loop");
                        }
                    }
                }
            }
        }
        //}

        delete[] reachable;
    }
}

std::string KLMPropagator::get_model(bool smtlib) const {
    std::stringstream sb;
    sb << "Edges:\n";
    for (const auto &edge: exprToEdge) {
        if (edge.second->get_state() == Connected)
            sb << ExprToString(*edge.second->get_from(), smtlib) << " -> "
               << ExprToString(*edge.second->get_to(), smtlib) << "\n";
        else if (edge.second->get_state() == NonConnected) {
            sb << ExprToString(*edge.second->get_from(), smtlib) << " -/> "
               << ExprToString(*edge.second->get_to(), smtlib) << "\n";
            for (const auto &variable: edge.second->get_variable()) {
                auto it = interpretation.find(*variable.second);
                if (it != interpretation.end())
                    sb << "\t" << variable.first << " = " << (it->second ? "true" : "false") << "\n";
            }
            sb << "\n";
        }
    }
    return sb.str();
}

template<typename T>
struct ptr_pair {
    T* p1;
    T* p2;

    ptr_pair() = default;

    ptr_pair(T* p1, T* p2) noexcept: p1(p1), p2(p2) {}

    ptr_pair(ptr_pair &&other) noexcept: p1(other.p1), p2(other.p2) {
        other.p1 = nullptr;
        other.p2 = nullptr;
    }

    ptr_pair(const ptr_pair &other) : p1(other.p1), p2(other.p2) {}

    bool operator==(const ptr_pair<T> &other) const {
        return p1 == other.p1 && p2 == other.p2;
    }

    bool operator!=(const ptr_pair<T> &other) const {
        return !this->operator==(other);
    }
};

template<>
struct std::hash<ptr_pair<node>> {
    size_t operator()(const ptr_pair<node> &o) const {
        return (size_t) ((uint64_t) o.p1 * 17 ^ (uint64_t) o.p2);
    }
};


std::string KLMPropagator::display_model(bool smtlib) const {
    std::stringstream ss;
    ss << "digraph G {\n";

    std::unordered_set<ptr_pair<node>> covered;

    std::unordered_map<node*, std::string> nodeMap;
    for (const auto &edge: exprToEdge) {
        if (covered.find(ptr_pair<node>(edge.second->get_to(), edge.second->get_from())) != covered.end())
            continue;

        std::string from_node, to_node;
        auto from_it = nodeMap.find(edge.second->get_from());
        if (from_it == nodeMap.end()) {
            from_node = ExprToString(*edge.second->get_from(), smtlib);
            ss << from_node << ";\n";
            nodeMap[edge.second->get_from()] = from_node;
        } else
            from_node = from_it->second;
        auto to_it = nodeMap.find(edge.second->get_to());
        if (to_it == nodeMap.end()) {
            to_node = ExprToString(*edge.second->get_to(), smtlib);
            ss << to_node << ";\n";
            nodeMap[edge.second->get_to()] = to_node;
        } else
            to_node = to_it->second;

        std::string att;

        if (edge.second->get_state() == Connected) {
            att = "color=\"#0000FF\"";

            if (edge.second->get_to()->is_connected(edge.second->get_from()) &&
                edge.second->get_to()->get_connected(edge.second->get_from())->get_state() == Connected) {

                covered.insert(ptr_pair<node>(edge.second->get_from(), edge.second->get_to()));
                att = "dir=none color=\"#00FF00\"";
            }
        } else if (edge.second->get_state() == NonConnected) {
            att = "color=\"#FF0000\"";
            /*std::stringstream ss("");
            for (const auto &variable: edge.second->get_variable()) {
                auto it = interpretation.find(*variable.second);
                if (it != interpretation.end()) {
                    ss << variable.first << " = " << (it->second ? "true" : "false") << "\n";
                }
            }*/
        }

        ss << from_node << " -> " << to_node << " [" << att << "];\n";
    }
    ss << "}\n";
    return ss.str();
}

expr_template::expr_template(expr temp, std::vector<std::string> &&variables) : m_template(std::move(temp)),
                                                                                m_variables(variables) {
}

expr expr_template::get_instance(edge* edge) {
    const auto it = m_instances.find(edge);
    if (it != m_instances.end())
        return *it->second;

    auto replacements = expr_vector(m_template.ctx());
    for (unsigned i = 0; i < m_variables.size(); i++) {
        replacements.push_back(edge->get_variable(m_variables[i]));
    }
    auto instance = m_template.substitute(replacements);
    m_instances[edge] = instance;
    return instance;
}

std::string expr_template::to_string() const {
    return m_template.to_string();
}

std::string expr_template::ExprToString() const {
    return ExprToString(m_template);
}

static std::string escape(const std::string& s) {
    bool failed = false;
    for (int i = 0; !failed && i < s.length(); i++) {
        if (s[i] == '\\' || s[i] == '"')
            failed = true;
    }
    std::stringstream ss;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != '\\' && s[i] != '"')
            ss << s[i];
        else {
            ss << '\\' << s[i];
        }
    }
    return ss.str();
}

std::string expr_template::ExprToString(const expr &expr) const {
    if (expr.is_false())
        return "false";
    if (expr.is_true())
        return "true";
    if (expr.is_not())
        return "~" + ExprToString(expr.arg(0));
    if (expr.is_implies())
        return "(" + ExprToString(expr.arg(0)) + " => " + ExprToString(expr.arg(1)) + ")";
    if (expr.is_eq())
        return "(" + ExprToString(expr.arg(0)) + " = " + ExprToString(expr.arg(1)) + ")";
    if (expr.is_and()) {
        if (expr.num_args() == 0)
            return "true";
        if (expr.num_args() == 1)
            return ExprToString(expr.arg(0));
        std::stringstream ss;
        ss << "(" << ExprToString(expr.arg(0));
        for (unsigned i = 1; i < expr.num_args(); i++)
            ss << " & " << ExprToString(expr.arg(i));
        ss << ")";
        return ss.str();
    }
    if (expr.is_or()) {
        if (expr.num_args() == 0)
            return "false";
        if (expr.num_args() == 1)
            return ExprToString(expr.arg(0));
        std::stringstream ss;
        ss << "(" << ExprToString(expr.arg(0));
        for (unsigned i = 1; i < expr.num_args(); i++)
            ss << " | " << ExprToString(expr.arg(i));
        ss << ")";
        return ss.str();
    }
    if (expr.is_var())
        return escape(m_variables[Z3_get_index_value(expr.ctx(), expr)]);
    std::cerr << "Unknown connective in: " << expr.to_string() << std::endl;
    exit(-1);
}
