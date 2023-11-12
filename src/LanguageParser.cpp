
// Generated from Language.g4 by ANTLR 4.10.1


#include "LanguageVisitor.h"

#include "LanguageParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct LanguageParserStaticData final {
  LanguageParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LanguageParserStaticData(const LanguageParserStaticData&) = delete;
  LanguageParserStaticData(LanguageParserStaticData&&) = delete;
  LanguageParserStaticData& operator=(const LanguageParserStaticData&) = delete;
  LanguageParserStaticData& operator=(LanguageParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag languageParserOnceFlag;
LanguageParserStaticData *languageParserStaticData = nullptr;

void languageParserInitialize() {
  assert(languageParserStaticData == nullptr);
  auto staticData = std::make_unique<LanguageParserStaticData>(
    std::vector<std::string>{
      "trueExpr", "falseExpr", "variableExpr", "notExpr", "andExpr", "orExpr", 
      "impliesExpr", "equalExpr", "groupedExpr", "boolExpr", "node", "negNode", 
      "assertion", "statements"
    },
    std::vector<std::string>{
      "", "'true'", "'false'", "'~'", "'&'", "'|'", "'=>'", "'='", "'('", 
      "')'", "'->'", "'-/>'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "Identifier", "WS", 
      "Comment", "Comment2"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,15,99,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,1,0,1,0,1,
  	1,1,1,1,2,1,2,1,3,1,3,1,3,1,4,1,4,1,4,4,4,41,8,4,11,4,12,4,42,1,4,1,4,
  	1,5,1,5,1,5,4,5,50,8,5,11,5,12,5,51,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,
  	7,1,7,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,3,8,72,8,8,1,9,1,9,1,9,1,9,1,9,
  	3,9,79,8,9,1,10,1,10,1,10,1,10,1,11,1,11,1,11,1,11,1,12,1,12,3,12,91,
  	8,12,1,13,5,13,94,8,13,10,13,12,13,97,9,13,1,13,0,0,14,0,2,4,6,8,10,12,
  	14,16,18,20,22,24,26,0,0,96,0,28,1,0,0,0,2,30,1,0,0,0,4,32,1,0,0,0,6,
  	34,1,0,0,0,8,40,1,0,0,0,10,49,1,0,0,0,12,55,1,0,0,0,14,59,1,0,0,0,16,
  	71,1,0,0,0,18,78,1,0,0,0,20,80,1,0,0,0,22,84,1,0,0,0,24,90,1,0,0,0,26,
  	95,1,0,0,0,28,29,5,1,0,0,29,1,1,0,0,0,30,31,5,2,0,0,31,3,1,0,0,0,32,33,
  	5,12,0,0,33,5,1,0,0,0,34,35,5,3,0,0,35,36,3,16,8,0,36,7,1,0,0,0,37,38,
  	3,16,8,0,38,39,5,4,0,0,39,41,1,0,0,0,40,37,1,0,0,0,41,42,1,0,0,0,42,40,
  	1,0,0,0,42,43,1,0,0,0,43,44,1,0,0,0,44,45,3,16,8,0,45,9,1,0,0,0,46,47,
  	3,16,8,0,47,48,5,5,0,0,48,50,1,0,0,0,49,46,1,0,0,0,50,51,1,0,0,0,51,49,
  	1,0,0,0,51,52,1,0,0,0,52,53,1,0,0,0,53,54,3,16,8,0,54,11,1,0,0,0,55,56,
  	3,16,8,0,56,57,5,6,0,0,57,58,3,16,8,0,58,13,1,0,0,0,59,60,3,16,8,0,60,
  	61,5,7,0,0,61,62,3,16,8,0,62,15,1,0,0,0,63,72,3,0,0,0,64,72,3,2,1,0,65,
  	72,3,4,2,0,66,72,3,6,3,0,67,68,5,8,0,0,68,69,3,18,9,0,69,70,5,9,0,0,70,
  	72,1,0,0,0,71,63,1,0,0,0,71,64,1,0,0,0,71,65,1,0,0,0,71,66,1,0,0,0,71,
  	67,1,0,0,0,72,17,1,0,0,0,73,79,3,16,8,0,74,79,3,8,4,0,75,79,3,10,5,0,
  	76,79,3,12,6,0,77,79,3,14,7,0,78,73,1,0,0,0,78,74,1,0,0,0,78,75,1,0,0,
  	0,78,76,1,0,0,0,78,77,1,0,0,0,79,19,1,0,0,0,80,81,3,18,9,0,81,82,5,10,
  	0,0,82,83,3,18,9,0,83,21,1,0,0,0,84,85,3,18,9,0,85,86,5,11,0,0,86,87,
  	3,18,9,0,87,23,1,0,0,0,88,91,3,20,10,0,89,91,3,22,11,0,90,88,1,0,0,0,
  	90,89,1,0,0,0,91,25,1,0,0,0,92,94,3,24,12,0,93,92,1,0,0,0,94,97,1,0,0,
  	0,95,93,1,0,0,0,95,96,1,0,0,0,96,27,1,0,0,0,97,95,1,0,0,0,6,42,51,71,
  	78,90,95
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  languageParserStaticData = staticData.release();
}

}

LanguageParser::LanguageParser(TokenStream *input) : LanguageParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

LanguageParser::LanguageParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  LanguageParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *languageParserStaticData->atn, languageParserStaticData->decisionToDFA, languageParserStaticData->sharedContextCache, options);
}

LanguageParser::~LanguageParser() {
  delete _interpreter;
}

const atn::ATN& LanguageParser::getATN() const {
  return *languageParserStaticData->atn;
}

std::string LanguageParser::getGrammarFileName() const {
  return "Language.g4";
}

const std::vector<std::string>& LanguageParser::getRuleNames() const {
  return languageParserStaticData->ruleNames;
}

const dfa::Vocabulary& LanguageParser::getVocabulary() const {
  return languageParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LanguageParser::getSerializedATN() const {
  return languageParserStaticData->serializedATN;
}


//----------------- TrueExprContext ------------------------------------------------------------------

LanguageParser::TrueExprContext::TrueExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LanguageParser::TrueExprContext::getRuleIndex() const {
  return LanguageParser::RuleTrueExpr;
}


std::any LanguageParser::TrueExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitTrueExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::TrueExprContext* LanguageParser::trueExpr() {
  TrueExprContext *_localctx = _tracker.createInstance<TrueExprContext>(_ctx, getState());
  enterRule(_localctx, 0, LanguageParser::RuleTrueExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(28);
    match(LanguageParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FalseExprContext ------------------------------------------------------------------

LanguageParser::FalseExprContext::FalseExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LanguageParser::FalseExprContext::getRuleIndex() const {
  return LanguageParser::RuleFalseExpr;
}


std::any LanguageParser::FalseExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitFalseExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::FalseExprContext* LanguageParser::falseExpr() {
  FalseExprContext *_localctx = _tracker.createInstance<FalseExprContext>(_ctx, getState());
  enterRule(_localctx, 2, LanguageParser::RuleFalseExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(30);
    match(LanguageParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableExprContext ------------------------------------------------------------------

LanguageParser::VariableExprContext::VariableExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LanguageParser::VariableExprContext::Identifier() {
  return getToken(LanguageParser::Identifier, 0);
}


size_t LanguageParser::VariableExprContext::getRuleIndex() const {
  return LanguageParser::RuleVariableExpr;
}


std::any LanguageParser::VariableExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitVariableExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::VariableExprContext* LanguageParser::variableExpr() {
  VariableExprContext *_localctx = _tracker.createInstance<VariableExprContext>(_ctx, getState());
  enterRule(_localctx, 4, LanguageParser::RuleVariableExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(32);
    match(LanguageParser::Identifier);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NotExprContext ------------------------------------------------------------------

LanguageParser::NotExprContext::NotExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LanguageParser::GroupedExprContext* LanguageParser::NotExprContext::groupedExpr() {
  return getRuleContext<LanguageParser::GroupedExprContext>(0);
}


size_t LanguageParser::NotExprContext::getRuleIndex() const {
  return LanguageParser::RuleNotExpr;
}


std::any LanguageParser::NotExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitNotExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::NotExprContext* LanguageParser::notExpr() {
  NotExprContext *_localctx = _tracker.createInstance<NotExprContext>(_ctx, getState());
  enterRule(_localctx, 6, LanguageParser::RuleNotExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(34);
    match(LanguageParser::T__2);
    setState(35);
    antlrcpp::downCast<NotExprContext *>(_localctx)->lhs = groupedExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AndExprContext ------------------------------------------------------------------

LanguageParser::AndExprContext::AndExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::GroupedExprContext *> LanguageParser::AndExprContext::groupedExpr() {
  return getRuleContexts<LanguageParser::GroupedExprContext>();
}

LanguageParser::GroupedExprContext* LanguageParser::AndExprContext::groupedExpr(size_t i) {
  return getRuleContext<LanguageParser::GroupedExprContext>(i);
}


size_t LanguageParser::AndExprContext::getRuleIndex() const {
  return LanguageParser::RuleAndExpr;
}


std::any LanguageParser::AndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitAndExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::AndExprContext* LanguageParser::andExpr() {
  AndExprContext *_localctx = _tracker.createInstance<AndExprContext>(_ctx, getState());
  enterRule(_localctx, 8, LanguageParser::RuleAndExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(40); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(37);
              groupedExpr();
              setState(38);
              match(LanguageParser::T__3);
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(42); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
    setState(44);
    groupedExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OrExprContext ------------------------------------------------------------------

LanguageParser::OrExprContext::OrExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::GroupedExprContext *> LanguageParser::OrExprContext::groupedExpr() {
  return getRuleContexts<LanguageParser::GroupedExprContext>();
}

LanguageParser::GroupedExprContext* LanguageParser::OrExprContext::groupedExpr(size_t i) {
  return getRuleContext<LanguageParser::GroupedExprContext>(i);
}


size_t LanguageParser::OrExprContext::getRuleIndex() const {
  return LanguageParser::RuleOrExpr;
}


std::any LanguageParser::OrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitOrExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::OrExprContext* LanguageParser::orExpr() {
  OrExprContext *_localctx = _tracker.createInstance<OrExprContext>(_ctx, getState());
  enterRule(_localctx, 10, LanguageParser::RuleOrExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(49); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(46);
              groupedExpr();
              setState(47);
              match(LanguageParser::T__4);
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(51); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
    setState(53);
    groupedExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ImpliesExprContext ------------------------------------------------------------------

LanguageParser::ImpliesExprContext::ImpliesExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::GroupedExprContext *> LanguageParser::ImpliesExprContext::groupedExpr() {
  return getRuleContexts<LanguageParser::GroupedExprContext>();
}

LanguageParser::GroupedExprContext* LanguageParser::ImpliesExprContext::groupedExpr(size_t i) {
  return getRuleContext<LanguageParser::GroupedExprContext>(i);
}


size_t LanguageParser::ImpliesExprContext::getRuleIndex() const {
  return LanguageParser::RuleImpliesExpr;
}


std::any LanguageParser::ImpliesExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitImpliesExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::ImpliesExprContext* LanguageParser::impliesExpr() {
  ImpliesExprContext *_localctx = _tracker.createInstance<ImpliesExprContext>(_ctx, getState());
  enterRule(_localctx, 12, LanguageParser::RuleImpliesExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(55);
    antlrcpp::downCast<ImpliesExprContext *>(_localctx)->lhs = groupedExpr();
    setState(56);
    match(LanguageParser::T__5);
    setState(57);
    antlrcpp::downCast<ImpliesExprContext *>(_localctx)->rhs = groupedExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualExprContext ------------------------------------------------------------------

LanguageParser::EqualExprContext::EqualExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::GroupedExprContext *> LanguageParser::EqualExprContext::groupedExpr() {
  return getRuleContexts<LanguageParser::GroupedExprContext>();
}

LanguageParser::GroupedExprContext* LanguageParser::EqualExprContext::groupedExpr(size_t i) {
  return getRuleContext<LanguageParser::GroupedExprContext>(i);
}


size_t LanguageParser::EqualExprContext::getRuleIndex() const {
  return LanguageParser::RuleEqualExpr;
}


std::any LanguageParser::EqualExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitEqualExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::EqualExprContext* LanguageParser::equalExpr() {
  EqualExprContext *_localctx = _tracker.createInstance<EqualExprContext>(_ctx, getState());
  enterRule(_localctx, 14, LanguageParser::RuleEqualExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(59);
    antlrcpp::downCast<EqualExprContext *>(_localctx)->lhs = groupedExpr();
    setState(60);
    match(LanguageParser::T__6);
    setState(61);
    antlrcpp::downCast<EqualExprContext *>(_localctx)->rhs = groupedExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroupedExprContext ------------------------------------------------------------------

LanguageParser::GroupedExprContext::GroupedExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LanguageParser::TrueExprContext* LanguageParser::GroupedExprContext::trueExpr() {
  return getRuleContext<LanguageParser::TrueExprContext>(0);
}

LanguageParser::FalseExprContext* LanguageParser::GroupedExprContext::falseExpr() {
  return getRuleContext<LanguageParser::FalseExprContext>(0);
}

LanguageParser::VariableExprContext* LanguageParser::GroupedExprContext::variableExpr() {
  return getRuleContext<LanguageParser::VariableExprContext>(0);
}

LanguageParser::NotExprContext* LanguageParser::GroupedExprContext::notExpr() {
  return getRuleContext<LanguageParser::NotExprContext>(0);
}

LanguageParser::BoolExprContext* LanguageParser::GroupedExprContext::boolExpr() {
  return getRuleContext<LanguageParser::BoolExprContext>(0);
}


size_t LanguageParser::GroupedExprContext::getRuleIndex() const {
  return LanguageParser::RuleGroupedExpr;
}


std::any LanguageParser::GroupedExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitGroupedExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::GroupedExprContext* LanguageParser::groupedExpr() {
  GroupedExprContext *_localctx = _tracker.createInstance<GroupedExprContext>(_ctx, getState());
  enterRule(_localctx, 16, LanguageParser::RuleGroupedExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(71);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LanguageParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(63);
        trueExpr();
        break;
      }

      case LanguageParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(64);
        falseExpr();
        break;
      }

      case LanguageParser::Identifier: {
        enterOuterAlt(_localctx, 3);
        setState(65);
        variableExpr();
        break;
      }

      case LanguageParser::T__2: {
        enterOuterAlt(_localctx, 4);
        setState(66);
        notExpr();
        break;
      }

      case LanguageParser::T__7: {
        enterOuterAlt(_localctx, 5);
        setState(67);
        match(LanguageParser::T__7);
        setState(68);
        boolExpr();
        setState(69);
        match(LanguageParser::T__8);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BoolExprContext ------------------------------------------------------------------

LanguageParser::BoolExprContext::BoolExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LanguageParser::GroupedExprContext* LanguageParser::BoolExprContext::groupedExpr() {
  return getRuleContext<LanguageParser::GroupedExprContext>(0);
}

LanguageParser::AndExprContext* LanguageParser::BoolExprContext::andExpr() {
  return getRuleContext<LanguageParser::AndExprContext>(0);
}

LanguageParser::OrExprContext* LanguageParser::BoolExprContext::orExpr() {
  return getRuleContext<LanguageParser::OrExprContext>(0);
}

LanguageParser::ImpliesExprContext* LanguageParser::BoolExprContext::impliesExpr() {
  return getRuleContext<LanguageParser::ImpliesExprContext>(0);
}

LanguageParser::EqualExprContext* LanguageParser::BoolExprContext::equalExpr() {
  return getRuleContext<LanguageParser::EqualExprContext>(0);
}


size_t LanguageParser::BoolExprContext::getRuleIndex() const {
  return LanguageParser::RuleBoolExpr;
}


std::any LanguageParser::BoolExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitBoolExpr(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::BoolExprContext* LanguageParser::boolExpr() {
  BoolExprContext *_localctx = _tracker.createInstance<BoolExprContext>(_ctx, getState());
  enterRule(_localctx, 18, LanguageParser::RuleBoolExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(78);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(73);
      groupedExpr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(74);
      andExpr();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(75);
      orExpr();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(76);
      impliesExpr();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(77);
      equalExpr();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NodeContext ------------------------------------------------------------------

LanguageParser::NodeContext::NodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::BoolExprContext *> LanguageParser::NodeContext::boolExpr() {
  return getRuleContexts<LanguageParser::BoolExprContext>();
}

LanguageParser::BoolExprContext* LanguageParser::NodeContext::boolExpr(size_t i) {
  return getRuleContext<LanguageParser::BoolExprContext>(i);
}


size_t LanguageParser::NodeContext::getRuleIndex() const {
  return LanguageParser::RuleNode;
}


std::any LanguageParser::NodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitNode(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::NodeContext* LanguageParser::node() {
  NodeContext *_localctx = _tracker.createInstance<NodeContext>(_ctx, getState());
  enterRule(_localctx, 20, LanguageParser::RuleNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    antlrcpp::downCast<NodeContext *>(_localctx)->lhs = boolExpr();
    setState(81);
    match(LanguageParser::T__9);
    setState(82);
    antlrcpp::downCast<NodeContext *>(_localctx)->rhs = boolExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NegNodeContext ------------------------------------------------------------------

LanguageParser::NegNodeContext::NegNodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::BoolExprContext *> LanguageParser::NegNodeContext::boolExpr() {
  return getRuleContexts<LanguageParser::BoolExprContext>();
}

LanguageParser::BoolExprContext* LanguageParser::NegNodeContext::boolExpr(size_t i) {
  return getRuleContext<LanguageParser::BoolExprContext>(i);
}


size_t LanguageParser::NegNodeContext::getRuleIndex() const {
  return LanguageParser::RuleNegNode;
}


std::any LanguageParser::NegNodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitNegNode(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::NegNodeContext* LanguageParser::negNode() {
  NegNodeContext *_localctx = _tracker.createInstance<NegNodeContext>(_ctx, getState());
  enterRule(_localctx, 22, LanguageParser::RuleNegNode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(84);
    antlrcpp::downCast<NegNodeContext *>(_localctx)->lhs = boolExpr();
    setState(85);
    match(LanguageParser::T__10);
    setState(86);
    antlrcpp::downCast<NegNodeContext *>(_localctx)->rhs = boolExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssertionContext ------------------------------------------------------------------

LanguageParser::AssertionContext::AssertionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LanguageParser::NodeContext* LanguageParser::AssertionContext::node() {
  return getRuleContext<LanguageParser::NodeContext>(0);
}

LanguageParser::NegNodeContext* LanguageParser::AssertionContext::negNode() {
  return getRuleContext<LanguageParser::NegNodeContext>(0);
}


size_t LanguageParser::AssertionContext::getRuleIndex() const {
  return LanguageParser::RuleAssertion;
}


std::any LanguageParser::AssertionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitAssertion(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::AssertionContext* LanguageParser::assertion() {
  AssertionContext *_localctx = _tracker.createInstance<AssertionContext>(_ctx, getState());
  enterRule(_localctx, 24, LanguageParser::RuleAssertion);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(90);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(88);
      node();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(89);
      negNode();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementsContext ------------------------------------------------------------------

LanguageParser::StatementsContext::StatementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LanguageParser::AssertionContext *> LanguageParser::StatementsContext::assertion() {
  return getRuleContexts<LanguageParser::AssertionContext>();
}

LanguageParser::AssertionContext* LanguageParser::StatementsContext::assertion(size_t i) {
  return getRuleContext<LanguageParser::AssertionContext>(i);
}


size_t LanguageParser::StatementsContext::getRuleIndex() const {
  return LanguageParser::RuleStatements;
}


std::any LanguageParser::StatementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LanguageVisitor*>(visitor))
    return parserVisitor->visitStatements(this);
  else
    return visitor->visitChildren(this);
}

LanguageParser::StatementsContext* LanguageParser::statements() {
  StatementsContext *_localctx = _tracker.createInstance<StatementsContext>(_ctx, getState());
  enterRule(_localctx, 26, LanguageParser::RuleStatements);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(95);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LanguageParser::T__0)
      | (1ULL << LanguageParser::T__1)
      | (1ULL << LanguageParser::T__2)
      | (1ULL << LanguageParser::T__7)
      | (1ULL << LanguageParser::Identifier))) != 0)) {
      setState(92);
      assertion();
      setState(97);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void LanguageParser::initialize() {
  std::call_once(languageParserOnceFlag, languageParserInitialize);
}
