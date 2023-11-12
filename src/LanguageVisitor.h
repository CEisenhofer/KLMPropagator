
// Generated from Language.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "LanguageParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LanguageParser.
 */
class  LanguageVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LanguageParser.
   */
    virtual std::any visitTrueExpr(LanguageParser::TrueExprContext *context) = 0;

    virtual std::any visitFalseExpr(LanguageParser::FalseExprContext *context) = 0;

    virtual std::any visitVariableExpr(LanguageParser::VariableExprContext *context) = 0;

    virtual std::any visitNotExpr(LanguageParser::NotExprContext *context) = 0;

    virtual std::any visitAndExpr(LanguageParser::AndExprContext *context) = 0;

    virtual std::any visitOrExpr(LanguageParser::OrExprContext *context) = 0;

    virtual std::any visitImpliesExpr(LanguageParser::ImpliesExprContext *context) = 0;

    virtual std::any visitEqualExpr(LanguageParser::EqualExprContext *context) = 0;

    virtual std::any visitGroupedExpr(LanguageParser::GroupedExprContext *context) = 0;

    virtual std::any visitBoolExpr(LanguageParser::BoolExprContext *context) = 0;

    virtual std::any visitNode(LanguageParser::NodeContext *context) = 0;

    virtual std::any visitNegNode(LanguageParser::NegNodeContext *context) = 0;

    virtual std::any visitAssertion(LanguageParser::AssertionContext *context) = 0;

    virtual std::any visitStatements(LanguageParser::StatementsContext *context) = 0;


};

