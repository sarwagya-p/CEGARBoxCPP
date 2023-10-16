
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
      "termOp", "impOp", "idxbox", "idxdia", "unaryOp", "formula", "term", 
      "literal"
    },
    std::vector<std::string>{
      "", "'['", "']'", "'<'", "'>'", "'('", "')'", "", "", "'&'", "'|'", 
      "'=>'", "'<=>'", "'~'", "'[]'", "'<>'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "WS", "INT", "AND", "OR", "IMP", "IFF", 
      "NOT", "BOX", "DIA", "NAME", "IDXBOX", "IDXDIA"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,18,56,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,1,0,1,0,1,1,1,1,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,4,1,4,1,5,1,5,1,
  	5,1,5,5,5,35,8,5,10,5,12,5,38,9,5,1,6,1,6,1,6,1,6,3,6,44,8,6,1,7,1,7,
  	1,7,1,7,1,7,1,7,1,7,1,7,3,7,54,8,7,1,7,0,0,8,0,2,4,6,8,10,12,14,0,3,1,
  	0,9,10,1,0,11,12,2,0,13,15,17,18,51,0,16,1,0,0,0,2,18,1,0,0,0,4,20,1,
  	0,0,0,6,24,1,0,0,0,8,28,1,0,0,0,10,30,1,0,0,0,12,39,1,0,0,0,14,53,1,0,
  	0,0,16,17,7,0,0,0,17,1,1,0,0,0,18,19,7,1,0,0,19,3,1,0,0,0,20,21,5,1,0,
  	0,21,22,5,8,0,0,22,23,5,2,0,0,23,5,1,0,0,0,24,25,5,3,0,0,25,26,5,8,0,
  	0,26,27,5,4,0,0,27,7,1,0,0,0,28,29,7,2,0,0,29,9,1,0,0,0,30,36,3,12,6,
  	0,31,32,3,0,0,0,32,33,3,12,6,0,33,35,1,0,0,0,34,31,1,0,0,0,35,38,1,0,
  	0,0,36,34,1,0,0,0,36,37,1,0,0,0,37,11,1,0,0,0,38,36,1,0,0,0,39,43,3,14,
  	7,0,40,41,3,2,1,0,41,42,3,14,7,0,42,44,1,0,0,0,43,40,1,0,0,0,43,44,1,
  	0,0,0,44,13,1,0,0,0,45,54,5,16,0,0,46,47,3,8,4,0,47,48,3,14,7,0,48,54,
  	1,0,0,0,49,50,5,5,0,0,50,51,3,10,5,0,51,52,5,6,0,0,52,54,1,0,0,0,53,45,
  	1,0,0,0,53,46,1,0,0,0,53,49,1,0,0,0,54,15,1,0,0,0,3,36,43,53
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
    setState(16);
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
    setState(18);
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

//----------------- IdxboxContext ------------------------------------------------------------------

FormulaKParser::IdxboxContext::IdxboxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaKParser::IdxboxContext::INT() {
  return getToken(FormulaKParser::INT, 0);
}


size_t FormulaKParser::IdxboxContext::getRuleIndex() const {
  return FormulaKParser::RuleIdxbox;
}


std::any FormulaKParser::IdxboxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitIdxbox(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::IdxboxContext* FormulaKParser::idxbox() {
  IdxboxContext *_localctx = _tracker.createInstance<IdxboxContext>(_ctx, getState());
  enterRule(_localctx, 4, FormulaKParser::RuleIdxbox);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(20);
    match(FormulaKParser::T__0);
    setState(21);
    match(FormulaKParser::INT);
    setState(22);
    match(FormulaKParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdxdiaContext ------------------------------------------------------------------

FormulaKParser::IdxdiaContext::IdxdiaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* FormulaKParser::IdxdiaContext::INT() {
  return getToken(FormulaKParser::INT, 0);
}


size_t FormulaKParser::IdxdiaContext::getRuleIndex() const {
  return FormulaKParser::RuleIdxdia;
}


std::any FormulaKParser::IdxdiaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<FormulaKVisitor*>(visitor))
    return parserVisitor->visitIdxdia(this);
  else
    return visitor->visitChildren(this);
}

FormulaKParser::IdxdiaContext* FormulaKParser::idxdia() {
  IdxdiaContext *_localctx = _tracker.createInstance<IdxdiaContext>(_ctx, getState());
  enterRule(_localctx, 6, FormulaKParser::RuleIdxdia);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(24);
    match(FormulaKParser::T__2);
    setState(25);
    match(FormulaKParser::INT);
    setState(26);
    match(FormulaKParser::T__3);
   
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
  enterRule(_localctx, 8, FormulaKParser::RuleUnaryOp);
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
    setState(28);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 450560) != 0))) {
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
  enterRule(_localctx, 10, FormulaKParser::RuleFormula);
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
    setState(30);
    term();
    setState(36);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == FormulaKParser::AND

    || _la == FormulaKParser::OR) {
      setState(31);
      termOp();
      setState(32);
      term();
      setState(38);
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
  enterRule(_localctx, 12, FormulaKParser::RuleTerm);
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
    setState(39);
    literal();
    setState(43);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == FormulaKParser::IMP

    || _la == FormulaKParser::IFF) {
      setState(40);
      impOp();
      setState(41);
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
  enterRule(_localctx, 14, FormulaKParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(53);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case FormulaKParser::NAME: {
        enterOuterAlt(_localctx, 1);
        setState(45);
        match(FormulaKParser::NAME);
        break;
      }

      case FormulaKParser::NOT:
      case FormulaKParser::BOX:
      case FormulaKParser::DIA:
      case FormulaKParser::IDXBOX:
      case FormulaKParser::IDXDIA: {
        enterOuterAlt(_localctx, 2);
        setState(46);
        unaryOp();
        setState(47);
        literal();
        break;
      }

      case FormulaKParser::T__4: {
        enterOuterAlt(_localctx, 3);
        setState(49);
        match(FormulaKParser::T__4);
        setState(50);
        formula();
        setState(51);
        match(FormulaKParser::T__5);
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
