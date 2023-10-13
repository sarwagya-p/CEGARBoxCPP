
// Generated from ../FormulaK.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "FormulaKListener.h"


/**
 * This class provides an empty implementation of FormulaKListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  FormulaKBaseListener : public FormulaKListener {
public:

  virtual void enterTermOp(FormulaKParser::TermOpContext * /*ctx*/) override { }
  virtual void exitTermOp(FormulaKParser::TermOpContext * /*ctx*/) override { }

  virtual void enterImpOp(FormulaKParser::ImpOpContext * /*ctx*/) override { }
  virtual void exitImpOp(FormulaKParser::ImpOpContext * /*ctx*/) override { }

  virtual void enterUnaryOp(FormulaKParser::UnaryOpContext * /*ctx*/) override { }
  virtual void exitUnaryOp(FormulaKParser::UnaryOpContext * /*ctx*/) override { }

  virtual void enterFormula(FormulaKParser::FormulaContext * /*ctx*/) override { }
  virtual void exitFormula(FormulaKParser::FormulaContext * /*ctx*/) override { }

  virtual void enterTerm(FormulaKParser::TermContext * /*ctx*/) override { }
  virtual void exitTerm(FormulaKParser::TermContext * /*ctx*/) override { }

  virtual void enterLiteral(FormulaKParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(FormulaKParser::LiteralContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

