#include "Node.h"

expr_vector node::get_transitive_out_just(node* n) {
	assert(!is_transitive_out(n));
	expr_vector just(m_label.ctx());
	for (auto e : m_transitive_out[n])
		just.push_back(e->get_expr());
	return just;
}

expr_vector node::get_transitive_in_just(node* n) {
	assert(!is_transitive_in(n));
	expr_vector just(m_label.ctx());
	for (auto e : m_transitive_in[n])
		just.push_back(e->get_expr());
	return just;
}

std::string node::to_string() const {
    std::string n = m_label.to_string();
    if (n.length() > 2 && n[0] == '|' && n[n.length() - 1] == '|')
        return n.substr(1, n.length() - 2);
    return n;
}

expr edge::GetVariable(const std::string& variable) {
    if (m_variables.find(variable) != m_variables.cend())
        return *m_variables[variable];
    auto value = expr(m_expr.ctx(), Z3_mk_fresh_const(m_expr.ctx(), variable.c_str(), m_expr.ctx().bool_sort()));
    sort_vector domain(m_expr.ctx());
    value = m_expr.ctx().user_propagate_function(value.decl().name(), domain, m_expr.ctx().bool_sort())();
    m_variables[variable] = value;
    return value;
}

std::string edge::to_string() const {
    return m_from->to_string() + " -> " + m_to->to_string() + " " + m_expr.to_string();
}
