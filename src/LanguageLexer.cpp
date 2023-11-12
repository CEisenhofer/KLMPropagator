
// Generated from Language.g4 by ANTLR 4.10.1


#include "LanguageLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct LanguageLexerStaticData final {
  LanguageLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  LanguageLexerStaticData(const LanguageLexerStaticData&) = delete;
  LanguageLexerStaticData(LanguageLexerStaticData&&) = delete;
  LanguageLexerStaticData& operator=(const LanguageLexerStaticData&) = delete;
  LanguageLexerStaticData& operator=(LanguageLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag languagelexerLexerOnceFlag;
LanguageLexerStaticData *languagelexerLexerStaticData = nullptr;

void languagelexerLexerInitialize() {
  assert(languagelexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<LanguageLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "Identifier", "WS", "Comment", "Comment2"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,15,96,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,3,1,3,1,4,
  	1,4,1,5,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,9,1,10,1,10,1,10,1,
  	10,1,11,4,11,66,8,11,11,11,12,11,67,1,12,4,12,71,8,12,11,12,12,12,72,
  	1,12,1,12,1,13,1,13,5,13,79,8,13,10,13,12,13,82,9,13,1,13,1,13,1,14,1,
  	14,1,14,1,14,5,14,90,8,14,10,14,12,14,93,9,14,1,14,1,14,0,0,15,1,1,3,
  	2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,
  	1,0,3,5,0,43,46,48,57,65,90,95,95,97,122,3,0,9,10,13,13,32,32,2,0,10,
  	10,13,13,99,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,
  	0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,
  	21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,1,31,1,
  	0,0,0,3,36,1,0,0,0,5,42,1,0,0,0,7,44,1,0,0,0,9,46,1,0,0,0,11,48,1,0,0,
  	0,13,51,1,0,0,0,15,53,1,0,0,0,17,55,1,0,0,0,19,57,1,0,0,0,21,60,1,0,0,
  	0,23,65,1,0,0,0,25,70,1,0,0,0,27,76,1,0,0,0,29,85,1,0,0,0,31,32,5,116,
  	0,0,32,33,5,114,0,0,33,34,5,117,0,0,34,35,5,101,0,0,35,2,1,0,0,0,36,37,
  	5,102,0,0,37,38,5,97,0,0,38,39,5,108,0,0,39,40,5,115,0,0,40,41,5,101,
  	0,0,41,4,1,0,0,0,42,43,5,126,0,0,43,6,1,0,0,0,44,45,5,38,0,0,45,8,1,0,
  	0,0,46,47,5,124,0,0,47,10,1,0,0,0,48,49,5,61,0,0,49,50,5,62,0,0,50,12,
  	1,0,0,0,51,52,5,61,0,0,52,14,1,0,0,0,53,54,5,40,0,0,54,16,1,0,0,0,55,
  	56,5,41,0,0,56,18,1,0,0,0,57,58,5,45,0,0,58,59,5,62,0,0,59,20,1,0,0,0,
  	60,61,5,45,0,0,61,62,5,47,0,0,62,63,5,62,0,0,63,22,1,0,0,0,64,66,7,0,
  	0,0,65,64,1,0,0,0,66,67,1,0,0,0,67,65,1,0,0,0,67,68,1,0,0,0,68,24,1,0,
  	0,0,69,71,7,1,0,0,70,69,1,0,0,0,71,72,1,0,0,0,72,70,1,0,0,0,72,73,1,0,
  	0,0,73,74,1,0,0,0,74,75,6,12,0,0,75,26,1,0,0,0,76,80,5,59,0,0,77,79,8,
  	2,0,0,78,77,1,0,0,0,79,82,1,0,0,0,80,78,1,0,0,0,80,81,1,0,0,0,81,83,1,
  	0,0,0,82,80,1,0,0,0,83,84,6,13,0,0,84,28,1,0,0,0,85,86,5,47,0,0,86,87,
  	5,47,0,0,87,91,1,0,0,0,88,90,8,2,0,0,89,88,1,0,0,0,90,93,1,0,0,0,91,89,
  	1,0,0,0,91,92,1,0,0,0,92,94,1,0,0,0,93,91,1,0,0,0,94,95,6,14,0,0,95,30,
  	1,0,0,0,5,0,67,72,80,91,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  languagelexerLexerStaticData = staticData.release();
}

}

LanguageLexer::LanguageLexer(CharStream *input) : Lexer(input) {
  LanguageLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *languagelexerLexerStaticData->atn, languagelexerLexerStaticData->decisionToDFA, languagelexerLexerStaticData->sharedContextCache);
}

LanguageLexer::~LanguageLexer() {
  delete _interpreter;
}

std::string LanguageLexer::getGrammarFileName() const {
  return "Language.g4";
}

const std::vector<std::string>& LanguageLexer::getRuleNames() const {
  return languagelexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& LanguageLexer::getChannelNames() const {
  return languagelexerLexerStaticData->channelNames;
}

const std::vector<std::string>& LanguageLexer::getModeNames() const {
  return languagelexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& LanguageLexer::getVocabulary() const {
  return languagelexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView LanguageLexer::getSerializedATN() const {
  return languagelexerLexerStaticData->serializedATN;
}

const atn::ATN& LanguageLexer::getATN() const {
  return *languagelexerLexerStaticData->atn;
}




void LanguageLexer::initialize() {
  std::call_once(languagelexerLexerOnceFlag, languagelexerLexerInitialize);
}
