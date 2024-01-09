#pragma once

#include "KLMPropagator.h"
#include "random_formula.h"


int main(int argc, char* argv[]);

void Solve(context& context, const std::string& smtlib2, unsigned timeout, Logic logic, bool smtlibOutput, bool visual);

expr replace_nodes(const expr& e, const func_decl& oldDecl, const func_decl& newDecl, const sort& nodeSort,
                   std::unordered_map<expr, expr_template*, expr_hash, expr_eq>& abstraction,
                   std::unordered_map<expr, std::optional<expr>, expr_hash, expr_eq>& abstractionInv);

std::string to_SMTLIB(const expr_vector& assertions);

void benchmark(Logic logic);
