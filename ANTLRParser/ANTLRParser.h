#include "src/FormulaKLexer.h"
#include "src/FormulaKParser.h"
#include "src/FormulaKBaseVisitor.h"
#include "src/FormulaKVisitor.h"

#include "../Formula/And/And.h"
#include "../Formula/Atom/Atom.h"
#include "../Formula/Box/Box.h"
#include "../Formula/Diamond/Diamond.h"
#include "../Formula/FEnum/FEnum.h"
#include "../Formula/False/False.h"
#include "../Formula/Formula/Formula.h"
#include "../Formula/Not/Not.h"
#include "../Formula/Or/Or.h"
#include "../Formula/True/True.h"

#include <any>

using namespace std;
using namespace antlr4;

class FormulaTreeVisitor : public FormulaKVisitor {
public:
    virtual std::any visitTermOp(FormulaKParser::TermOpContext *context);

    virtual std::any visitImpOp(FormulaKParser::ImpOpContext *context);

    virtual std::any visitUnaryOp(FormulaKParser::UnaryOpContext *context);

    virtual std::any visitFormula(FormulaKParser::FormulaContext *context);

    virtual std::any visitTerm(FormulaKParser::TermContext *context);

    virtual std::any visitLiteral(FormulaKParser::LiteralContext *context);
};

class ANTLRParser {
private:
    ANTLRFileStream input;
public:
    ANTLRParser(const string& filename);
    ~ANTLRParser();
    shared_ptr<Formula> parse();
};