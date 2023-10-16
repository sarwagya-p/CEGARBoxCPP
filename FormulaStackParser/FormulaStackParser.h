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

#include <fstream>

using namespace std;

class FormulaStackParser {
private:
    enum FormulaOps {
        AND, OR, NOT, BOX, DIA, IMP, IFF, LPAREN, RPAREN
    };

    struct StreamReader {
        char buffer_char;
        ifstream input_file;
        
        StreamReader(string filename);
        ~StreamReader();
        char getChar();
        char lookahead();
    };

    vector<FormulaOps> op_stack;
    vector<shared_ptr<Formula>> subf_stack;
    shared_ptr<StreamReader> reader;

    void reduceStack();
public:
    FormulaStackParser(string filename);
    ~FormulaStackParser();

    shared_ptr<Formula> parse();
};