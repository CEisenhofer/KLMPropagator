#pragma once

#include "KLMPropagator.h"
#include "random_formula.h"

enum language {
    lang_smtlib2,
    lang_custom,
    lang_auto,
};

int main(int argc, char* argv[]);

void Solve(context& context, const std::string& smtlib2, unsigned timeout, Logic logic, language outputLanguage, bool visual, bool checkResult);

expr replace_nodes(const expr& e, const func_decl& oldDecl, const func_decl& newDecl, const sort& nodeSort,
                   std::unordered_map<expr, expr_template*>& abstraction,
                   std::unordered_map<expr, std::optional<expr>>& abstractionInv, bool& has_theories);

std::string to_SMTLIB(const expr_vector& assertions);

void benchmark(Logic logic);
