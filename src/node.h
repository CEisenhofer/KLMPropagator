#pragma once
#include <utility>

#include "Helper.h"

class edge;

class node {
    const expr m_label;

    std::unordered_map<node*, edge*> m_connected;
    std::unordered_map<node*, edge*> m_none_connected;

    std::unordered_map<node*, std::vector<edge*>> m_transitive_in;
    std::unordered_map<node*, std::vector<edge*>> m_transitive_out;

public:

    node(expr label) : m_label(std::move(label)) {}

    expr get_label() const { return m_label; }

    bool is_connected(node* other) const {
    	return m_connected.find(other) != m_connected.end();
	}

    bool is_none_connected(node* other) const {
    	return m_none_connected.find(other) != m_none_connected.end();
	}

	bool is_transitive_in(node* other) const {
        return m_transitive_in.find(other) != m_transitive_in.end();
    }

	bool is_transitive_out(node* other) const {
        return m_transitive_out.find(other) != m_transitive_out.end();
    }

    void add_connected(node* n, edge* c) {
        assert(!is_connected(n));
        assert(!is_none_connected(n));
        m_connected[n] = c;
	}

    void add_none_connected(node* n, edge* c) {
        assert(!is_connected(n));
        assert(!is_none_connected(n));
        m_none_connected[n] = c;
	}

    void add_transitive_in(node* n, std::vector<edge*> c) {
        assert(!is_transitive_in(n));
        m_transitive_in[n] = std::move(c);
	}

    void add_transitive_out(node* n, std::vector<edge*> c) {
        assert(!is_transitive_out(n));
        m_transitive_out[n] = std::move(c);
    }

    void remove_connected(node* n) {
        assert(is_connected(n));
        m_connected.erase(n);
    }

    void remove_none_connected(node* n) {
        assert(is_none_connected(n));
        m_none_connected.erase(n);
    }

    void remove_transitive_in(node* n) {
        assert(is_transitive_in(n));
        m_transitive_in.erase(n);
    }

    void remove_transitive_out(node* n) {
        assert(is_transitive_out(n));
        m_transitive_out.erase(n);
    }

    edge* get_connected(node* n) {
        assert(is_connected(n));
        return m_connected[n];
    }

    std::unordered_map<node*, edge*>& get_connected() {
        return m_connected;
    }

    edge* get_none_connected(node* n) {
        assert(is_none_connected(n));
        return m_none_connected[n];
    }

    std::unordered_map<node*, edge*>& get_none_connected() {
        return m_none_connected;
    }

    const std::vector<edge*>& get_transitive_out(node* n) {
        return m_transitive_out[n];
    }

    std::unordered_map<node*, std::vector<edge*>>& get_transitive_out() {
        return m_transitive_out;
    }

	const std::vector<edge*>& get_transitive_in(node* n) {
        return m_transitive_in[n];
    }

    std::unordered_map<node*, std::vector<edge*>>& get_transitive_in() {
        return m_transitive_in;
    }

    expr_vector get_transitive_out_just(node* n);
    expr_vector get_transitive_in_just(node* n);

    unsigned operator()() const {
        return m_label.hash();
    }
    bool operator==(const node& other) const {
        return m_label == other.m_label;
    }
    bool operator!=(const node& other) const {
        return !this->operator==(other);
    }
    std::string to_string() const;
};

enum bool_state : unsigned char {
    Unknown = 0,
    Connected = 1,
    NonConnected = 2,
};

class edge {
    node* m_from;
    node* m_to;
    expr m_expr;
    bool_state m_state = Unknown;

    std::unordered_map<std::string, std::optional<expr>> m_variables;

public:

    node* get_from() const { return m_from; }
    node* get_to() const { return m_to; }
    expr get_expr() const { return m_expr; }
    bool_state get_state() const { return m_state; }

    const std::unordered_map<std::string, std::optional<expr>>& get_variable() const {
        return m_variables;
    }

    void set_state(bool_state state) {
        m_state = state;
    }

    edge(node* from, node* to, expr e) : m_from(from), m_to(to), m_expr(e), m_variables() {
    }

    expr GetVariable(const std::string& variable);

    bool operator==(const edge& other) const {
        return m_to == other.m_to;
    }
    bool operator!=(const edge& other) const {
        return !this->operator==(other);
    }
    unsigned operator()() const {
        return m_to->operator()();
    }

    std::string to_string() const;
};
