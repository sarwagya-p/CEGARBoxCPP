
// Generated from FormulaK.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "FormulaKParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by FormulaKParser.
 */
class  FormulaKVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by FormulaKParser.
   */
    virtual std::any visitTermOp(FormulaKParser::TermOpContext *context) = 0;

    virtual std::any visitImpOp(FormulaKParser::ImpOpContext *context) = 0;

    virtual std::any visitIdxbox(FormulaKParser::IdxboxContext *context) = 0;

    virtual std::any visitIdxdia(FormulaKParser::IdxdiaContext *context) = 0;

    virtual std::any visitUnaryOp(FormulaKParser::UnaryOpContext *context) = 0;

    virtual std::any visitFormula(FormulaKParser::FormulaContext *context) = 0;

    virtual std::any visitTerm(FormulaKParser::TermContext *context) = 0;

    virtual std::any visitLiteral(FormulaKParser::LiteralContext *context) = 0;


};

