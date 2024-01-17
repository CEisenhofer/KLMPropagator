#include "random_formula.h"

NodeSpecification::NodeSpecification(Formula* lhs, Formula* rhs) : m_lhs(lhs), m_rhs(rhs) {
}

NodeSpecification::NodeSpecification(NodeSpecification&& other) : m_lhs(other.m_lhs), m_rhs(other.m_rhs) {
    other.m_lhs = nullptr;
    other.m_rhs = nullptr;
}

NodeSpecification& NodeSpecification::operator=(NodeSpecification&& other) noexcept {
    m_lhs = other.m_lhs;
    m_rhs = other.m_rhs;
    other.m_lhs = nullptr;
    other.m_rhs = nullptr;
    return *this;
}

NodeSpecification::~NodeSpecification() {
    delete m_lhs;
    delete m_rhs;
    m_lhs = m_rhs = nullptr;
}

bool NodeSpecification::operator==(const NodeSpecification& other) const {
    return other.m_lhs->operator==(m_lhs) && other.m_rhs->operator==(m_rhs);
}

unsigned NodeSpecification::operator()() const {
    return 41 * m_lhs->operator()() + m_rhs->operator()();
}

std::string NodeSpecification::to_string() const {
    return "(" + m_lhs->to_string() + ", " + m_rhs->to_string() + ")";
}

Specification::Specification(std::vector<NodeSpecification*>&& positive, NodeSpecification* negative) :
        m_positive(positive), m_negative(negative) {
}

std::string Specification::to_string() const {
    std::stringstream ss;
    if (m_positive.empty())
        ss << "true";
    else
        ss << m_positive[0]->to_string();
    for (int i = 1; i < m_positive.size(); i++) {
        ss << ", " << m_positive[i]->to_string();
    }
    ss << " => " << m_negative->to_string();
    return ss.str();
}

RandomFormula::RandomFormula(int seed, int maxVars, int maxDepth, int maxLength, int maxNodes) :
        m_maxVars(maxVars), m_maxDepth(maxDepth), m_maxLength(maxLength), m_maxNodes(maxNodes), rng(seed) {
}

Specification RandomFormula::gen_spec() {
    const unsigned nodes = 1 + (dist(rng) % m_maxNodes);
    std::unordered_set<NodeSpecification*, pointer_hash<NodeSpecification>, pointer_eq<NodeSpecification>> added;
    std::vector<NodeSpecification*> positive;
    NodeSpecification* nodeSpecification = nullptr;

    for (int i = 0; i < nodes; i++) {
        do {
            delete nodeSpecification;
            const auto lhs = gen_random();
            const auto rhs = gen_random();
            nodeSpecification = new NodeSpecification(lhs, rhs);
        } while (added.find(nodeSpecification) != added.end());
        added.insert(nodeSpecification);
        positive.push_back(nodeSpecification);
        nodeSpecification = nullptr;
    }
    do {
        delete nodeSpecification;
        const auto lhs = gen_random();
        const auto rhs = gen_random();
        nodeSpecification = new NodeSpecification(lhs, rhs);
    } while (added.find(nodeSpecification) != added.end());
    added.insert(nodeSpecification);

    return Specification(std::move(positive), nodeSpecification);
}

Formula* RandomFormula::gen_random() {
    m_currentVariables = 0;

    int res = dist(rng) % 5;
    if (res == 0)
        return gen_literal();
    if (res >= 1 && res <= 2)
        return GenAnd(0);
    if (res >= 3 && res <= 4)
        return GenOr(0);
    throw std::exception();
}

Formula* RandomFormula::gen_literal() {
    bool neg = (dist(rng) % 2) == 0;
    auto variable = dist(rng) % m_maxVars;
    if (variable >= m_currentVariables)
        variable = m_currentVariables++;
    Formula* literal = new Atom(variable);
    if (neg)
        literal = new Negation(literal);
    return literal;
}

Formula* RandomFormula::GenAnd(int depth) {
    if (depth >= m_maxDepth)
        return gen_literal();

    int length = 2 + dist(rng) % (m_maxLength - 1);
    std::vector<Formula*> args(length);
    std::unordered_set<Atom*, pointer_hash<Atom>, pointer_eq<Atom>> atoms;
    for (int i = 0; i < length; i++) {
        if (dist(rng) % 3 == 0) {
            Atom* atom;
            do {
                args[i] = gen_literal();
                if (typeid(*args[i]) == typeid(Negation))
                    atom = (Atom*) ((Negation*) args[i])->get_subformula();
                else
                    atom = (Atom*) args[i];
            } while (atoms.find(atom) != atoms.end());
            atoms.insert(atom);
        }
        else
            args[i] = GenOr(depth + 1);
    }
    return new And(std::move(args));
}

Formula* RandomFormula::GenOr(int depth) {
    if (depth >= m_maxDepth)
        return gen_literal();

    int length = 2 + dist(rng) % (m_maxLength - 1);
    std::vector<Formula*> args(length);
    std::unordered_set<Atom*, pointer_hash<Atom>, pointer_eq<Atom>> atoms;
    for (int i = 0; i < length; i++) {
        if (dist(rng) % 3 == 0) {
            Atom* atom;
            do {
                args[i] = gen_literal();
                if (typeid(*args[i]) == typeid(Negation))
                    atom = (Atom*) ((Negation*) args[i])->get_subformula();
                else
                    atom = (Atom*) args[i];
            } while (atoms.find(atom) != atoms.end());
            atoms.insert(atom);
        }
        else
            args[i] = GenAnd(depth + 1);
    }
    return new Or(std::move(args));
}

expr Atom::ToZ3(context& ctx) const {
    return ctx.bool_const(("V" + std::to_string(m_id + 1)).c_str());
}

Negation::Negation(Formula* formula) : m_formula(formula) {
}

Negation::Negation(Negation&& other) : m_formula(other.m_formula) {
    m_formula = nullptr;
}

Negation& Negation::operator=(Negation&& other) {
    m_formula = other.m_formula;
    other.m_formula = nullptr;
    return *this;
}

expr Negation::ToZ3(context& ctx) const {
    return !m_formula->ToZ3(ctx);
}

bool Negation::operator==(const Formula* other) const {
    if (typeid(*other) == typeid(Negation))
        return operator==(*(const Negation*) other);
    return false;
}

bool Negation::operator==(const Negation& other) const {
    return m_formula->operator==(other.m_formula);
}

unsigned Negation::operator()() const {
    return 17 ^ m_formula->operator()();
}

std::string Negation::to_string() const {
    return "!" + m_formula->to_string();
}

And::And(std::vector<Formula*>&& args) : m_args(args) {}

And::And(And&& other) : m_args(other.m_args) {
    other.m_args.resize(0);
}

And& And::operator=(And&& other) {
    m_args = std::move(other.m_args);
    other.m_args.resize(0);
    return *this;
}

expr And::ToZ3(context& ctx) const {
    expr_vector z3Args(ctx);
    for (unsigned i = 0; i < m_args.size(); i++) {
        expr arg = m_args[i]->ToZ3(ctx);
        if (arg.is_and()) {
            const unsigned arg_cnt = arg.num_args();
            for (unsigned j = 0; j < arg_cnt; j++) {
                z3Args.push_back(arg.arg(j));
            }
        }
        else
            z3Args.push_back(arg);
    }
    return mk_and(z3Args);
}

bool And::operator==(const Formula* other) const {
    if (typeid(*other) == typeid(And))
        return operator==(*(const And*) other);
    return false;
}

bool And::operator==(const And& other) const {
    if (m_args.size() != other.m_args.size())
        return false;
    for (unsigned i = 0; i < m_args.size(); i++) {
        if (m_args[i]->operator!=(other.m_args[i]))
            return false;
    }
    return true;
}

bool And::operator!=(const And& other) const {
    return !operator==(other);
}

unsigned And::operator()() const {
    unsigned res = 19;
    for (const auto& arg: m_args) {
        res = 23 * res + arg->operator()();
    }
    return res;
}

std::string And::to_string() const {
    if (m_args.empty())
        return "true";
    std::stringstream ss;
    ss << "(" << m_args[0]->to_string();
    for (unsigned i = 1; i < m_args.size(); i++) {
        ss << " && " << m_args[i]->to_string();
    }
    ss << ")";
    return ss.str();
}

Or::Or(std::vector<Formula*>&& args) : m_args(args) {}

Or::Or(Or&& other) : m_args(other.m_args) {
    other.m_args.resize(0);
}

Or& Or::operator=(Or&& other) {
    m_args = std::move(other.m_args);
    other.m_args.resize(0);
    return *this;
}

expr Or::ToZ3(context& ctx) const {
    expr_vector z3Args(ctx);
    for (unsigned i = 0; i < m_args.size(); i++) {
        expr arg = m_args[i]->ToZ3(ctx);
        if (arg.is_and()) {
            const unsigned arg_cnt = arg.num_args();
            for (unsigned j = 0; j < arg_cnt; j++) {
                z3Args.push_back(arg.arg(j));
            }
        }
        else
            z3Args.push_back(arg);
    }
    return mk_or(z3Args);
}

bool Or::operator==(const Formula* other) const {
    if (typeid(*other) == typeid(Or))
        return operator==(*(const Or*) other);
    return false;
}

bool Or::operator==(const Or& other) const {
    if (m_args.size() != other.m_args.size())
        return false;
    for (unsigned i = 0; i < m_args.size(); i++) {
        if (m_args[i]->operator!=(other.m_args[i]))
            return false;
    }
    return true;
}

bool Or::operator!=(const Or& other) const {
    return !operator==(other);
}

unsigned Or::operator()() const {
    unsigned res = 29;
    for (const auto& arg: m_args) {
        res = 31 * res + arg->operator()();
    }
    return res;
}

std::string Or::to_string() const {
    if (m_args.empty())
        return "false";

    std::stringstream ss;
    ss << "(" << m_args[0]->to_string();
    for (unsigned i = 1; i < m_args.size(); i++) {
        ss << " || " << m_args[i]->to_string();
    }
    ss << ")";
    return ss.str();
}
