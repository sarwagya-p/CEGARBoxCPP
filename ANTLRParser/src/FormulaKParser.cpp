
// Generated from FormulaK.g4 by ANTLR 4.13.1


#include "FormulaKVisitor.h"

#include "FormulaKParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct FormulaKParserStaticData final {
  FormulaKParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FormulaKParserStaticData(const FormulaKParserStaticData&) = delete;
  FormulaKParserStaticData(FormulaKParserStaticData&&) = delete;
  FormulaKParserStaticData& operator=(const FormulaKParserStaticData&) = delete;
  FormulaKParserStaticData& operator=(FormulaKParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag formulakParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
FormulaKParserStaticData *formulakParserStaticData = nullptr;

void formulakParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (formulakParserStaticData != nullptr) {
    return;
  }
#else
  assert(formulakParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<FormulaKParserStaticData>(
    std::vector<std::string>{
      "termOp", "impOp", "unaryOp", "formula", "term", "literal"
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
  	4,1,14,44,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,1,0,1,0,1,1,
  	1,1,1,2,1,2,1,3,1,3,1,3,1,3,5,3,23,8,3,10,3,12,3,26,9,3,1,4,1,4,1,4,1,
  	4,3,4,32,8,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,42,8,5,1,5,0,0,6,0,2,
  	4,6,8,10,0,3,1,0,5,6,1,0,7,8,1,0,9,13,41,0,12,1,0,0,0,2,14,1,0,0,0,4,
  	16,1,0,0,0,6,18,1,0,0,0,8,27,1,0,0,0,10,41,1,0,0,0,12,13,7,0,0,0,13,1,
  	1,0,0,0,14,15,7,1,0,0,15,3,1,0,0,0,16,17,7,2,0,0,17,5,1,0,0,0,18,24,3,
  	8,4,0,19,20,3,0,0,0,20,21,3,8,4,0,21,23,1,0,0,0,22,19,1,0,0,0,23,26,1,
  	0,0,0,24,22,1,0,0,0,24,25,1,0,0,0,25,7,1,0,0,0,26,24,1,0,0,0,27,31,3,
  	10,5,0,28,29,3,2,1,0,29,30,3,10,5,0,30,32,1,0,0,0,31,28,1,0,0,0,31,32,
  	1,0,0,0,32,9,1,0,0,0,33,42,5,14,0,0,34,35,3,4,2,0,35,36,3,10,5,0,36,42,
  	1,0,0,0,37,38,5,1,0,0,38,39,3,6,3,0,39,40,5,2,0,0,40,42,1,0,0,0,41,33,
  	1,0,0,0,41,34,1,0,0,0,41,37,1,0,0,0,42,11,1,0,0,0,3,24,31,41
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  formulakParserStaticData = staticData.release();
}

}

FormulaKParser::FormulaKParser(TokenStream *input) : FormulaKParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

FormulaKParser::FormulaKParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  FormulaKParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *formulakParserStaticData->atn, formulakParserStaticData->decisionToDFA, formulakParserStaticData->sharedContextCache, options);
}

FormulaKParser::~FormulaKParser() {
  delete _interpreter;
}

const atn::ATN& FormulaKParser::getATN() const {
  return *formulakParserStaticData->atn;
}

std::string FormulaKParser::getGrammarFileName() const {
  return "FormulaK.g4";
}

const std::vector<std::string>& FormulaKParser::getRuleNames() const {
  return formulakParserStaticData->ruleNames;
}

const dfa::Vocabulary& FormulaKParser::getVocabulary() const {
  return formulakParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FormulaKParser::getSerializedATN() const {
  return formulakParserStaticData->serializedATN;
}


//----------------- TermOpContext ------------------------------------------------------------------

FormulaKParser::TermOpContext::TermOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaKParser::TermOpContext::AND() {
  return getToken(FormulaKParser::AND, 0);
}

tree::TerminalNode* FormulaKParser::TermOpContext::OR() {
  return getToken(FormulaKParser::OR, 0);
}


size_t FormulaKParser::TermOpContext::getRuleIndex() const {
  return FormulaKParser::RuleTermOp;
}


std::any FormulaKParser::TermOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitTermOp(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::TermOpContext* FormulaKParser::termOp() {
  TermOpContext *_localctx = _tracker.createInstance<TermOpContext>(_ctx, getState());
  enterRule(_localctx, 0, FormulaKParser::RuleTermOp);
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
    setState(12);
    _la = _input->LA(1);
    if (!(_la == FormulaKParser::AND

    || _la == FormulaKParser::OR)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ImpOpContext ------------------------------------------------------------------

FormulaKParser::ImpOpContext::ImpOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaKParser::ImpOpContext::IMP() {
  return getToken(FormulaKParser::IMP, 0);
}

tree::TerminalNode* FormulaKParser::ImpOpContext::IFF() {
  return getToken(FormulaKParser::IFF, 0);
}


size_t FormulaKParser::ImpOpContext::getRuleIndex() const {
  return FormulaKParser::RuleImpOp;
}


std::any FormulaKParser::ImpOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitImpOp(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::ImpOpContext* FormulaKParser::impOp() {
  ImpOpContext *_localctx = _tracker.createInstance<ImpOpContext>(_ctx, getState());
  enterRule(_localctx, 2, FormulaKParser::RuleImpOp);
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
    setState(14);
    _la = _input->LA(1);
    if (!(_la == FormulaKParser::IMP

    || _la == FormulaKParser::IFF)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOpContext ------------------------------------------------------------------

FormulaKParser::UnaryOpContext::UnaryOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaKParser::UnaryOpContext::NOT() {
  return getToken(FormulaKParser::NOT, 0);
}

tree::TerminalNode* FormulaKParser::UnaryOpContext::BOX() {
  return getToken(FormulaKParser::BOX, 0);
}

tree::TerminalNode* FormulaKParser::UnaryOpContext::DIA() {
  return getToken(FormulaKParser::DIA, 0);
}

tree::TerminalNode* FormulaKParser::UnaryOpContext::IDXBOX() {
  return getToken(FormulaKParser::IDXBOX, 0);
}

tree::TerminalNode* FormulaKParser::UnaryOpContext::IDXDIA() {
  return getToken(FormulaKParser::IDXDIA, 0);
}


size_t FormulaKParser::UnaryOpContext::getRuleIndex() const {
  return FormulaKParser::RuleUnaryOp;
}


std::any FormulaKParser::UnaryOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitUnaryOp(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::UnaryOpContext* FormulaKParser::unaryOp() {
  UnaryOpContext *_localctx = _tracker.createInstance<UnaryOpContext>(_ctx, getState());
  enterRule(_localctx, 4, FormulaKParser::RuleUnaryOp);
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
    setState(16);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 15872) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormulaContext ------------------------------------------------------------------

FormulaKParser::FormulaContext::FormulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FormulaKParser::TermContext *> FormulaKParser::FormulaContext::term() {
  return getRuleContexts<FormulaKParser::TermContext>();
}

FormulaKParser::TermContext* FormulaKParser::FormulaContext::term(size_t i) {
  return getRuleContext<FormulaKParser::TermContext>(i);
}

std::vector<FormulaKParser::TermOpContext *> FormulaKParser::FormulaContext::termOp() {
  return getRuleContexts<FormulaKParser::TermOpContext>();
}

FormulaKParser::TermOpContext* FormulaKParser::FormulaContext::termOp(size_t i) {
  return getRuleContext<FormulaKParser::TermOpContext>(i);
}


size_t FormulaKParser::FormulaContext::getRuleIndex() const {
  return FormulaKParser::RuleFormula;
}


std::any FormulaKParser::FormulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitFormula(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::FormulaContext* FormulaKParser::formula() {
  FormulaContext *_localctx = _tracker.createInstance<FormulaContext>(_ctx, getState());
  enterRule(_localctx, 6, FormulaKParser::RuleFormula);
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
    setState(18);
    term();
    setState(24);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == FormulaKParser::AND

    || _la == FormulaKParser::OR) {
      setState(19);
      termOp();
      setState(20);
      term();
      setState(26);
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

//----------------- TermContext ------------------------------------------------------------------

FormulaKParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<FormulaKParser::LiteralContext *> FormulaKParser::TermContext::literal() {
  return getRuleContexts<FormulaKParser::LiteralContext>();
}

FormulaKParser::LiteralContext* FormulaKParser::TermContext::literal(size_t i) {
  return getRuleContext<FormulaKParser::LiteralContext>(i);
}

FormulaKParser::ImpOpContext* FormulaKParser::TermContext::impOp() {
  return getRuleContext<FormulaKParser::ImpOpContext>(0);
}


size_t FormulaKParser::TermContext::getRuleIndex() const {
  return FormulaKParser::RuleTerm;
}


std::any FormulaKParser::TermContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitTerm(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::TermContext* FormulaKParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 8, FormulaKParser::RuleTerm);
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
    setState(27);
    literal();
    setState(31);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaKParser::IMP

    || _la == FormulaKParser::IFF) {
      setState(28);
      impOp();
      setState(29);
      literal();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

FormulaKParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaKParser::LiteralContext::NAME() {
  return getToken(FormulaKParser::NAME, 0);
}

FormulaKParser::UnaryOpContext* FormulaKParser::LiteralContext::unaryOp() {
  return getRuleContext<FormulaKParser::UnaryOpContext>(0);
}

FormulaKParser::LiteralContext* FormulaKParser::LiteralContext::literal() {
  return getRuleContext<FormulaKParser::LiteralContext>(0);
}

FormulaKParser::FormulaContext* FormulaKParser::LiteralContext::formula() {
  return getRuleContext<FormulaKParser::FormulaContext>(0);
}


size_t FormulaKParser::LiteralContext::getRuleIndex() const {
  return FormulaKParser::RuleLiteral;
}


std::any FormulaKParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::LiteralContext* FormulaKParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 10, FormulaKParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(41);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FormulaKParser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(33);
        match(FormulaKParser::NAME);
        break;
      }

      case FormulaKParser::NOT:
      case FormulaKParser::BOX:
      case FormulaKParser::DIA:
      case FormulaKParser::IDXBOX:
      case FormulaKParser::IDXDIA: {
        enterOuterAlt(_localctx, 2);
        setState(34);
        unaryOp();
        setState(35);
        literal();
        break;
      }

      case FormulaKParser::T__0: {
        enterOuterAlt(_localctx, 3);
        setState(37);
        match(FormulaKParser::T__0);
        setState(38);
        formula();
        setState(39);
        match(FormulaKParser::T__1);
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

void FormulaKParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  formulakParserInitialize();
#else
  ::antlr4::internal::call_once(formulakParserOnceFlag, formulakParserInitialize);
#endif
}
