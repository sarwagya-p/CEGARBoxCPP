
// Generated from FormulaK.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "FormulaKVisitor.h"


/**
 * This class provides an empty implementation of FormulaKVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  FormulaKBaseVisitor : public FormulaKVisitor {
public:

  virtual std::any visitTermOp(FormulaKParser::TermOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitImpOp(FormulaKParser::ImpOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdxbox(FormulaKParser::IdxboxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIdxdia(FormulaKParser::IdxdiaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryOp(FormulaKParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFormula(FormulaKParser::FormulaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTerm(FormulaKParser::TermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(FormulaKParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }


};

