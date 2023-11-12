
// Generated from Language.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "LanguageVisitor.h"


/**
 * This class provides an empty implementation of LanguageVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LanguageBaseVisitor : public LanguageVisitor {
public:

  virtual std::any visitTrueExpr(LanguageParser::TrueExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFalseExpr(LanguageParser::FalseExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariableExpr(LanguageParser::VariableExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNotExpr(LanguageParser::NotExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExpr(LanguageParser::AndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOrExpr(LanguageParser::OrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitImpliesExpr(LanguageParser::ImpliesExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqualExpr(LanguageParser::EqualExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGroupedExpr(LanguageParser::GroupedExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBoolExpr(LanguageParser::BoolExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNode(LanguageParser::NodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegNode(LanguageParser::NegNodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssertion(LanguageParser::AssertionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatements(LanguageParser::StatementsContext *ctx) override {
    return visitChildren(ctx);
  }


};

