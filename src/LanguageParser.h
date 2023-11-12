
// Generated from Language.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  LanguageParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, Identifier = 12, WS = 13, 
    Comment = 14, Comment2 = 15
  };

  enum {
    RuleTrueExpr = 0, RuleFalseExpr = 1, RuleVariableExpr = 2, RuleNotExpr = 3, 
    RuleAndExpr = 4, RuleOrExpr = 5, RuleImpliesExpr = 6, RuleEqualExpr = 7, 
    RuleGroupedExpr = 8, RuleBoolExpr = 9, RuleNode = 10, RuleNegNode = 11, 
    RuleAssertion = 12, RuleStatements = 13
  };

  explicit LanguageParser(antlr4::TokenStream *input);

  LanguageParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~LanguageParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class TrueExprContext;
  class FalseExprContext;
  class VariableExprContext;
  class NotExprContext;
  class AndExprContext;
  class OrExprContext;
  class ImpliesExprContext;
  class EqualExprContext;
  class GroupedExprContext;
  class BoolExprContext;
  class NodeContext;
  class NegNodeContext;
  class AssertionContext;
  class StatementsContext; 

  class  TrueExprContext : public antlr4::ParserRuleContext {
  public:
    TrueExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TrueExprContext* trueExpr();

  class  FalseExprContext : public antlr4::ParserRuleContext {
  public:
    FalseExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FalseExprContext* falseExpr();

  class  VariableExprContext : public antlr4::ParserRuleContext {
  public:
    VariableExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableExprContext* variableExpr();

  class  NotExprContext : public antlr4::ParserRuleContext {
  public:
    LanguageParser::GroupedExprContext *lhs = nullptr;
    NotExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GroupedExprContext *groupedExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NotExprContext* notExpr();

  class  AndExprContext : public antlr4::ParserRuleContext {
  public:
    AndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupedExprContext *> groupedExpr();
    GroupedExprContext* groupedExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AndExprContext* andExpr();

  class  OrExprContext : public antlr4::ParserRuleContext {
  public:
    OrExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupedExprContext *> groupedExpr();
    GroupedExprContext* groupedExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrExprContext* orExpr();

  class  ImpliesExprContext : public antlr4::ParserRuleContext {
  public:
    LanguageParser::GroupedExprContext *lhs = nullptr;
    LanguageParser::GroupedExprContext *rhs = nullptr;
    ImpliesExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupedExprContext *> groupedExpr();
    GroupedExprContext* groupedExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ImpliesExprContext* impliesExpr();

  class  EqualExprContext : public antlr4::ParserRuleContext {
  public:
    LanguageParser::GroupedExprContext *lhs = nullptr;
    LanguageParser::GroupedExprContext *rhs = nullptr;
    EqualExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupedExprContext *> groupedExpr();
    GroupedExprContext* groupedExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EqualExprContext* equalExpr();

  class  GroupedExprContext : public antlr4::ParserRuleContext {
  public:
    GroupedExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TrueExprContext *trueExpr();
    FalseExprContext *falseExpr();
    VariableExprContext *variableExpr();
    NotExprContext *notExpr();
    BoolExprContext *boolExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupedExprContext* groupedExpr();

  class  BoolExprContext : public antlr4::ParserRuleContext {
  public:
    BoolExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GroupedExprContext *groupedExpr();
    AndExprContext *andExpr();
    OrExprContext *orExpr();
    ImpliesExprContext *impliesExpr();
    EqualExprContext *equalExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BoolExprContext* boolExpr();

  class  NodeContext : public antlr4::ParserRuleContext {
  public:
    LanguageParser::BoolExprContext *lhs = nullptr;
    LanguageParser::BoolExprContext *rhs = nullptr;
    NodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BoolExprContext *> boolExpr();
    BoolExprContext* boolExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NodeContext* node();

  class  NegNodeContext : public antlr4::ParserRuleContext {
  public:
    LanguageParser::BoolExprContext *lhs = nullptr;
    LanguageParser::BoolExprContext *rhs = nullptr;
    NegNodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BoolExprContext *> boolExpr();
    BoolExprContext* boolExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NegNodeContext* negNode();

  class  AssertionContext : public antlr4::ParserRuleContext {
  public:
    AssertionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NodeContext *node();
    NegNodeContext *negNode();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssertionContext* assertion();

  class  StatementsContext : public antlr4::ParserRuleContext {
  public:
    StatementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AssertionContext *> assertion();
    AssertionContext* assertion(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementsContext* statements();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

