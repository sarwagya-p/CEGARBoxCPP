
// Generated from FormulaK.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  FormulaKParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, WS = 7, 
    INT = 8, AND = 9, OR = 10, IMP = 11, IFF = 12, NOT = 13, BOX = 14, DIA = 15, 
    NAME = 16, IDXBOX = 17, IDXDIA = 18
  };

  enum {
    RuleTermOp = 0, RuleImpOp = 1, RuleIdxbox = 2, RuleIdxdia = 3, RuleUnaryOp = 4, 
    RuleFormula = 5, RuleTerm = 6, RuleLiteral = 7
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
  class IdxboxContext;
  class IdxdiaContext;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TermOpContext* termOp();

  class  ImpOpContext : public antlr4::ParserRuleContext {
  public:
    ImpOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMP();
    antlr4::tree::TerminalNode *IFF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ImpOpContext* impOp();

  class  IdxboxContext : public antlr4::ParserRuleContext {
  public:
    IdxboxContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdxboxContext* idxbox();

  class  IdxdiaContext : public antlr4::ParserRuleContext {
  public:
    IdxdiaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdxdiaContext* idxdia();

  class  UnaryOpContext : public antlr4::ParserRuleContext {
  public:
    UnaryOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *BOX();
    antlr4::tree::TerminalNode *DIA();
    antlr4::tree::TerminalNode *IDXBOX();
    antlr4::tree::TerminalNode *IDXDIA();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FormulaContext* formula();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LiteralContext *> literal();
    LiteralContext* literal(size_t i);
    ImpOpContext *impOp();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

