#pragma once

#include <cstdlib>
#include <random>

#include "helper.h"

class Formula;

class And;

class Or;

class NodeSpecification {
    Formula* m_lhs;
    Formula* m_rhs;

public:

    NodeSpecification() = default;

    NodeSpecification(Formula* lhs, Formula* rhs);

    NodeSpecification(const NodeSpecification &other) = default;

    NodeSpecification(NodeSpecification &&other);

    NodeSpecification &operator=(NodeSpecification &&other) noexcept;

    ~NodeSpecification();

    Formula* get_lhs() const {
        return m_lhs;
    }

    Formula* get_rhs() const {
        return m_rhs;
    }

    bool operator==(const NodeSpecification &other) const;

    bool operator!=(const NodeSpecification &other) const {
        return !operator==(other);
    }

    unsigned operator()() const;

    std::string to_string() const;
};

template<>
struct std::hash<NodeSpecification> {
    unsigned operator()(const NodeSpecification &n) const {
        return n();
    }
};

class Specification {
    std::vector<NodeSpecification*> m_positive;
    NodeSpecification* m_negative;

public:

    Specification(std::vector<NodeSpecification*> &&positive, NodeSpecification* negative);

    Specification(Specification &&other) : m_positive(other.m_positive), m_negative(other.m_negative) {
        other.m_positive.resize(0);
        other.m_negative = nullptr;
    }

    const std::vector<NodeSpecification*> &get_positive() const {
        return m_positive;
    }

    const NodeSpecification* get_negative() const {
        return m_negative;
    }

    std::string to_string() const;
};

class RandomFormula {
    const int m_maxVars;
    const int m_maxDepth;
    const int m_maxLength;
    const int m_maxNodes;

    int m_currentVariables = 0;

    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;

    Formula* gen_random();

    Formula* gen_literal();

    Formula* GenAnd(int depth);

    Formula* GenOr(int depth);

public:

    RandomFormula(int seed, int maxVars, int maxDepth, int maxLength, int maxNodes);

    Specification gen_spec();
};

class Formula {
public:
    virtual ~Formula() = default;

    virtual expr ToZ3(context &context) const = 0;

    virtual bool operator==(const Formula* other) const = 0;

    bool operator!=(const Formula* const other) const {
        return !this->operator==(other);
    }

    virtual unsigned operator()() const = 0;

    virtual std::string to_string() const = 0;
};

class Atom : public Formula {
    const unsigned m_id;

public:
    Atom(unsigned id) : m_id(id) {}

    expr ToZ3(context &ctx) const override;

    bool operator==(const Formula* other) const override {
        if (typeid(*other) == typeid(Atom))
            return operator==(*(const Atom*) other);
        return false;
    }

    bool operator==(const Atom &other) const {
        return m_id == other.m_id;
    }

    bool operator!=(const Atom &other) const {
        return !operator==(other);
    }

    unsigned operator()() const override {
        return m_id;
    }

    std::string to_string() const override {
        return "V" + std::to_string(m_id + 1);
    }
};

class Negation : public Formula {
    Formula* m_formula;

public:
    Negation(Formula* formula);

    Negation(Negation &&other);

    Negation& operator=(Negation &&other);

    ~Negation() override {
        delete m_formula;
        m_formula = nullptr;
    }

    const Formula* get_subformula() const {
        return m_formula;
    }

    expr ToZ3(context &ctx) const override;

    bool operator==(const Formula* other) const override;

    bool operator==(const Negation &other) const;

    unsigned operator()() const override;

    std::string to_string() const override;
};

class And : public Formula {
    std::vector<Formula*> m_args;

public:
    And(std::vector<Formula*> &&args);

    And(And &&other);

    ~And() override {
        for (auto &arg: m_args)
            delete arg;
        m_args.resize(0);
    }

    And& operator=(And &&other);

    expr ToZ3(context &ctx) const override;

    bool operator==(const Formula* other) const override;

    bool operator==(const And &other) const;

    bool operator!=(const And &other) const;

    unsigned operator()() const override;

    std::string to_string() const override;
};

class Or : public Formula {
    std::vector<Formula*> m_args;

public:
    Or(std::vector<Formula*> &&args);

    Or(Or &&other);

    ~Or() override {
        for (auto &arg: m_args)
            delete arg;
        m_args.resize(0);
    }

    Or& operator=(Or &&other);

    expr ToZ3(context &ctx) const override;

    bool operator==(const Formula* other) const override;

    bool operator==(const Or &other) const;

    bool operator!=(const Or &other) const;

    unsigned operator()() const override;

    std::string to_string() const override;
};
