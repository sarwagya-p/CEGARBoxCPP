
// Generated from FormulaK.g4 by ANTLR 4.13.1


#include "FormulaKLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct FormulaKLexerStaticData final {
  FormulaKLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FormulaKLexerStaticData(const FormulaKLexerStaticData&) = delete;
  FormulaKLexerStaticData(FormulaKLexerStaticData&&) = delete;
  FormulaKLexerStaticData& operator=(const FormulaKLexerStaticData&) = delete;
  FormulaKLexerStaticData& operator=(FormulaKLexerStaticData&&) = delete;

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

::antlr4::internal::OnceFlag formulaklexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
FormulaKLexerStaticData *formulaklexerLexerStaticData = nullptr;

void formulaklexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (formulaklexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(formulaklexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<FormulaKLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "WS", "INT", "AND", "OR", "IMP", "IFF", "NOT", "BOX", 
      "DIA", "IDXBOX", "IDXDIA", "NAME"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'('", "')'", "", "", "'&'", "'|'", "'=>'", "'<=>'", "'~'", "'[]'", 
      "'<>'"
    },
    std::vector<std::string>{
      "", "", "", "WS", "INT", "AND", "OR", "IMP", "IFF", "NOT", "BOX", 
      "DIA", "IDXBOX", "IDXDIA", "NAME"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,14,83,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,1,0,
  	1,0,1,1,1,1,1,2,4,2,35,8,2,11,2,12,2,36,1,2,1,2,1,3,4,3,42,8,3,11,3,12,
  	3,43,1,4,1,4,1,5,1,5,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,8,1,8,1,9,1,9,1,9,
  	1,10,1,10,1,10,1,11,1,11,3,11,67,8,11,1,11,1,11,1,12,1,12,3,12,73,8,12,
  	1,12,1,12,1,13,1,13,5,13,79,8,13,10,13,12,13,82,9,13,0,0,14,1,1,3,2,5,
  	3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,1,0,4,3,0,
  	9,10,13,13,32,32,1,0,48,57,2,0,65,90,97,122,3,0,48,57,65,90,97,122,87,
  	0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,
  	0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,
  	0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,1,29,1,0,0,0,3,31,1,0,0,0,5,34,
  	1,0,0,0,7,41,1,0,0,0,9,45,1,0,0,0,11,47,1,0,0,0,13,49,1,0,0,0,15,52,1,
  	0,0,0,17,56,1,0,0,0,19,58,1,0,0,0,21,61,1,0,0,0,23,64,1,0,0,0,25,70,1,
  	0,0,0,27,76,1,0,0,0,29,30,5,40,0,0,30,2,1,0,0,0,31,32,5,41,0,0,32,4,1,
  	0,0,0,33,35,7,0,0,0,34,33,1,0,0,0,35,36,1,0,0,0,36,34,1,0,0,0,36,37,1,
  	0,0,0,37,38,1,0,0,0,38,39,6,2,0,0,39,6,1,0,0,0,40,42,7,1,0,0,41,40,1,
  	0,0,0,42,43,1,0,0,0,43,41,1,0,0,0,43,44,1,0,0,0,44,8,1,0,0,0,45,46,5,
  	38,0,0,46,10,1,0,0,0,47,48,5,124,0,0,48,12,1,0,0,0,49,50,5,61,0,0,50,
  	51,5,62,0,0,51,14,1,0,0,0,52,53,5,60,0,0,53,54,5,61,0,0,54,55,5,62,0,
  	0,55,16,1,0,0,0,56,57,5,126,0,0,57,18,1,0,0,0,58,59,5,91,0,0,59,60,5,
  	93,0,0,60,20,1,0,0,0,61,62,5,60,0,0,62,63,5,62,0,0,63,22,1,0,0,0,64,66,
  	5,91,0,0,65,67,3,7,3,0,66,65,1,0,0,0,66,67,1,0,0,0,67,68,1,0,0,0,68,69,
  	5,93,0,0,69,24,1,0,0,0,70,72,5,60,0,0,71,73,3,7,3,0,72,71,1,0,0,0,72,
  	73,1,0,0,0,73,74,1,0,0,0,74,75,5,62,0,0,75,26,1,0,0,0,76,80,7,2,0,0,77,
  	79,7,3,0,0,78,77,1,0,0,0,79,82,1,0,0,0,80,78,1,0,0,0,80,81,1,0,0,0,81,
  	28,1,0,0,0,82,80,1,0,0,0,6,0,36,43,66,72,80,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  formulaklexerLexerStaticData = staticData.release();
}

}

FormulaKLexer::FormulaKLexer(CharStream *input) : Lexer(input) {
  FormulaKLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *formulaklexerLexerStaticData->atn, formulaklexerLexerStaticData->decisionToDFA, formulaklexerLexerStaticData->sharedContextCache);
}

FormulaKLexer::~FormulaKLexer() {
  delete _interpreter;
}

std::string FormulaKLexer::getGrammarFileName() const {
  return "FormulaK.g4";
}

const std::vector<std::string>& FormulaKLexer::getRuleNames() const {
  return formulaklexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& FormulaKLexer::getChannelNames() const {
  return formulaklexerLexerStaticData->channelNames;
}

const std::vector<std::string>& FormulaKLexer::getModeNames() const {
  return formulaklexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& FormulaKLexer::getVocabulary() const {
  return formulaklexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FormulaKLexer::getSerializedATN() const {
  return formulaklexerLexerStaticData->serializedATN;
}

const atn::ATN& FormulaKLexer::getATN() const {
  return *formulaklexerLexerStaticData->atn;
}




void FormulaKLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  formulaklexerLexerInitialize();
#else
  ::antlr4::internal::call_once(formulaklexerLexerOnceFlag, formulaklexerLexerInitialize);
#endif
}
