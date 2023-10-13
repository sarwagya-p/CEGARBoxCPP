
// Generated from ../FormulaK.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "FormulaKParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by FormulaKParser.
 */
class  FormulaKListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterTermOp(FormulaKParser::TermOpContext *ctx) = 0;
  virtual void exitTermOp(FormulaKParser::TermOpContext *ctx) = 0;

  virtual void enterImpOp(FormulaKParser::ImpOpContext *ctx) = 0;
  virtual void exitImpOp(FormulaKParser::ImpOpContext *ctx) = 0;

  virtual void enterUnaryOp(FormulaKParser::UnaryOpContext *ctx) = 0;
  virtual void exitUnaryOp(FormulaKParser::UnaryOpContext *ctx) = 0;

  virtual void enterFormula(FormulaKParser::FormulaContext *ctx) = 0;
  virtual void exitFormula(FormulaKParser::FormulaContext *ctx) = 0;

  virtual void enterTerm(FormulaKParser::TermContext *ctx) = 0;
  virtual void exitTerm(FormulaKParser::TermContext *ctx) = 0;

  virtual void enterLiteral(FormulaKParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(FormulaKParser::LiteralContext *ctx) = 0;


};

