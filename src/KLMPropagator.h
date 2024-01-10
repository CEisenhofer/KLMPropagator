#pragma once

#include "node.h"
#include <iostream>
#include <optional>

class expr_template;

typedef std::function<void()> action;

class KLMPropagator : public user_propagator_base {

    const func_decl& nodeFct;
    std::unordered_map<expr, node*> exprToNode;
    std::unordered_map<expr, edge*> exprToEdge;
    std::unordered_map<expr, expr_template*> abstraction;

    std::vector<unsigned> undoStackSize;
    std::vector<action> undoStack;

    std::unordered_map<expr, bool> interpretation;

    const Logic logic;

    const bool hasLoop[5]
            {
                    false, true, true, true, true,
            };
    const bool hasTrans[5]
            {
                    false, false, true, false, true,
            };
    const bool hasOr[5]
            {
                    false, false, false, true, true,
            };

    bool HasLoop() const {
        return hasLoop[(int) logic];
    }

    bool HasTrans() const {
        return hasTrans[(int) logic];
    }

    bool HasOr() const {
        return hasOr[(int) logic];
    }

public:
    KLMPropagator(solver& s, const func_decl& nodeFct, Logic logic,
                  std::unordered_map<expr, expr_template*> abstraction);

#ifndef NDEBUG
#define Log(X) std::cout << X << std::endl
#else
#define Log(X) do { } while (false)
#endif

    void push() override;

    void pop(unsigned n) override;

    user_propagator_base* fresh(context& ctx) override { return this; }

    void fixed(const expr& e, const expr& v) override;

    void AddPosConnection(edge* e1);

    void AddNegConnection(edge* edge);

    void PropagateMissingTrue(edge* baseEdge, edge* connectedEdge);

    void PropagateMissingFalse(edge* baseEdge, edge* nonConnectedEdge);

    bool ignoreCreated;

    void created(const expr& e) override;

    std::string ExprToString(const node& node, bool smtlib) const;

    Z3_lbool eval(const expr& e);

    Z3_lbool eval(node* n, edge* e);

    void check_model();

    std::string get_model(const model& m, bool smtlib) const;

    std::string display_model(const model& m, bool smtlib) const;
};

class expr_template {

    expr m_template;
    std::unordered_map<edge*, std::optional<expr>> m_instances; // we assume edge pointers to be unique
    const std::vector<std::pair<std::string, std::optional<sort>>> m_variables;

public:
    expr_template(expr temp, std::vector<std::pair<std::string, std::optional<sort>>>&& variables);

    expr get_instance(edge* edge);

    std::string to_string() const;

    std::string ExprToString() const;

    std::string ExprToString(const expr& expr) const;
};
