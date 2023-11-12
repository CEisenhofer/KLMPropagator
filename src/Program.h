#pragma once

#include "KLMPropagator.h"
#include "LanguageBaseVisitor.h"
#include "random_formula.h"
#include "LanguageParser.h"


int main(int argc, char* argv[]);

void Solve(context &context, const std::string &smtlib2, unsigned timeout, Logic logic, bool smtlibOutput, bool visual);

expr replace_nodes(const expr &e, const func_decl &oldDecl, const func_decl &newDecl, const sort &nodeSort,
                   std::unordered_map<expr, expr_template*, expr_hash, expr_eq> &abstraction,
                   std::unordered_map<expr, std::optional<expr>, expr_hash, expr_eq> &abstractionInv);

std::string to_SMTLIB(const expr_vector &assertions);

void benchmark(Logic logic);

class LanguageVisitorImpl : public LanguageBaseVisitor {

    context &m_ctx;
    func_decl m_nodeFct;

public:
    LanguageVisitorImpl(context &ctx) : m_ctx(ctx), m_nodeFct(ctx) {
        sort_vector domain(ctx);
        domain.push_back(ctx.bool_sort());
        domain.push_back(ctx.bool_sort());
        m_nodeFct = ctx.function("node", domain, ctx.bool_sort());
    }

    std::any visitFalseExpr(LanguageParser::FalseExprContext*) override { return m_ctx.bool_val(false); }

    std::any visitTrueExpr(LanguageParser::TrueExprContext*) override { return m_ctx.bool_val(true); }

    std::any visitNotExpr(LanguageParser::NotExprContext* context) override {
        return !std::any_cast<expr>(context->lhs->accept(this));
    }

    std::any visitAndExpr(LanguageParser::AndExprContext* context) override {
        expr_vector args(m_ctx);
        for (auto child: context->children) {
            if (child->getTreeType() != antlr4::tree::ParseTreeType::RULE)
                continue;
            auto res = child->accept(this);
            args.push_back(std::any_cast<expr>(res));
        }
        return mk_and(args);
    }

    std::any visitOrExpr(LanguageParser::OrExprContext* context) override {
        expr_vector args(m_ctx);
        for (auto child: context->children) {
            if (child->getTreeType() != antlr4::tree::ParseTreeType::RULE)
                continue;
            auto res = child->accept(this);
            args.push_back(std::any_cast<expr>(res));
        }
        return mk_or(args);
    }

    std::any visitImpliesExpr(LanguageParser::ImpliesExprContext* context) override {
        return implies(std::any_cast<expr>(context->lhs->accept(this)),
                       std::any_cast<expr>(context->rhs->accept(this)));
    }

    std::any visitEqualExpr(LanguageParser::EqualExprContext* context) override {
        return std::any_cast<expr>(context->lhs->accept(this)) == std::any_cast<expr>(context->rhs->accept(this));
    }

    std::any visitVariableExpr(LanguageParser::VariableExprContext* context) override {
        const char* name = context->getText().c_str();
        return m_ctx.bool_const(name);
    }

    std::any visitNode(LanguageParser::NodeContext* context) override {
        return m_nodeFct(std::any_cast<expr>(context->lhs->accept(this)),
                         std::any_cast<expr>(context->rhs->accept(this)));
    }

    std::any visitNegNode(LanguageParser::NegNodeContext* context) override {
        return !m_nodeFct(std::any_cast<expr>(context->lhs->accept(this)),
                          std::any_cast<expr>(context->rhs->accept(this)));
    }

    std::any visitStatements(LanguageParser::StatementsContext* context) override {
        expr_vector args(m_ctx);
        for (auto &child: context->children) {
            args.push_back(std::any_cast<expr>(child->accept(this)));
        }
        return mk_and(args);
    }
};
