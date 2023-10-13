
// Generated from ../FormulaK.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  FormulaKParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, INT = 3, AND = 4, OR = 5, IMP = 6, IFF = 7, NOT = 8, 
    BOX = 9, DIA = 10, IDXBOX = 11, IDXDIA = 12, NAME = 13
  };

  enum {
    RuleTermOp = 0, RuleImpOp = 1, RuleUnaryOp = 2, RuleFormula = 3, RuleTerm = 4, 
    RuleLiteral = 5
  };

  explicit FormulaKParser(antlr4::TokenStream *input);

  FormulaKParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~FormulaKParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class TermOpContext;
  class ImpOpContext;
  class UnaryOpContext;
  class FormulaContext;
  class TermContext;
  class LiteralContext; 

  class  TermOpContext : public antlr4::ParserRuleContext {
  public:
    TermOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *OR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TermOpContext* termOp();

  class  ImpOpContext : public antlr4::ParserRuleContext {
  public:
    ImpOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMP();
    antlr4::tree::TerminalNode *IFF();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ImpOpContext* impOp();

  class  UnaryOpContext : public antlr4::ParserRuleContext {
  public:
    UnaryOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *BOX();
    antlr4::tree::TerminalNode *DIA();
    antlr4::tree::TerminalNode *IDXBOX();
    antlr4::tree::TerminalNode *IDXDIA();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  UnaryOpContext* unaryOp();

  class  FormulaContext : public antlr4::ParserRuleContext {
  public:
    FormulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TermContext *> term();
    TermContext* term(size_t i);
    std::vector<TermOpContext *> termOp();
    TermOpContext* termOp(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  FormulaContext* formula();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LiteralContext *> literal();
    LiteralContext* literal(size_t i);
    ImpOpContext *impOp();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  TermContext* term();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NAME();
    UnaryOpContext *unaryOp();
    LiteralContext *literal();
    FormulaContext *formula();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  LiteralContext* literal();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

