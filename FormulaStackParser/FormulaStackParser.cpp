#include "FormulaStackParser.h"
#include <filesystem>

string FormulaStackParser::ops_str(FormulaOps op){
    switch (op){
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case BOX: return "BOX";
        case DIA: return "DIA";
        case IMP: return "IMP";
        case IFF: return "IFF";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
    }
}

bool FormulaStackParser::is_unary(FormulaOps op){
    return op == NOT || op == BOX || op == DIA;
}

FormulaStackParser::FormulaStackParser(string filename): line_number(1), index(0){
    ifstream formula_file(filename);

    const auto file_size = filesystem::file_size(filename);
    formula_str = string(file_size, '\0');
    formula_file.read(formula_str.data(), file_size);
}

FormulaStackParser::~FormulaStackParser() {}

char FormulaStackParser::getChar() {
    if (index >= formula_str.size())return EOF;

    char c = formula_str[index];
    if (c == '\n'){
        ++line_number;
    }
    ++index;
    return c;
}

char FormulaStackParser::lookahead() {
    if (index >= formula_str.size())return EOF;

    return formula_str[index];
}

pair<int, int> FormulaStackParser::getPos() {
    return make_pair(line_number, index+1);
}

void FormulaStackParser::reduceStack() {
    if (op_stack.empty())
        return;

    switch (op_stack.back()){
        case AND:{
            formula_set subf_set;
            subf_set.insert(subf_stack.back());
            subf_stack.pop_back();

            while (!op_stack.empty() && op_stack.back() == AND){
                shared_ptr<Formula> subf = subf_stack.back();

                if (subf->getType() == FAnd){
                    And* and_subf = dynamic_cast<And*>(subf.get());
                    for (auto sub_subf : and_subf->getSubformulas()){
                        subf_set.insert(sub_subf);
                    }
                } else {
                    subf_set.insert(subf);
                }
                subf_stack.pop_back();
                op_stack.pop_back();
            }

            subf_stack.push_back(And::create(subf_set));
        } break;

        case OR:{
            formula_set subf_set;
            subf_set.insert(subf_stack.back());
            subf_stack.pop_back();

            while (!op_stack.empty() && op_stack.back() == OR){
                shared_ptr<Formula> subf = subf_stack.back();

                if (subf->getType() == FOr){
                    Or* or_subf = dynamic_cast<Or*>(subf.get());
                    for (auto sub_subf : or_subf->getSubformulas()){
                        subf_set.insert(sub_subf);
                    }
                } else {
                    subf_set.insert(subf);
                }
                subf_stack.pop_back();
                op_stack.pop_back();
            }

            subf_stack.push_back(Or::create(subf_set));
        } break;

        case NOT:{
            shared_ptr<Formula> back_subf = subf_stack.back();
            subf_stack.pop_back();

            subf_stack.push_back(Not::create(back_subf));
            op_stack.pop_back();
        } break;

        case BOX:{
            shared_ptr<Formula> back_subf = subf_stack.back();
            subf_stack.pop_back();

            subf_stack.push_back(Box::create({1}, back_subf));
            op_stack.pop_back();
        } break;

        case DIA:{
            shared_ptr<Formula> back_subf = subf_stack.back();
            subf_stack.pop_back();

            subf_stack.push_back(Diamond::create({1}, back_subf));
            op_stack.pop_back();
        } break;

        case RPAREN:{
            cerr << "Mismatched parenthesis" << endl;
            exit(1);
        } break;

        case LPAREN:{
            return;
        } break;

        case IMP:{
            shared_ptr<Formula> right = subf_stack.back();
            subf_stack.pop_back();

            shared_ptr<Formula> left = subf_stack.back();
            subf_stack.pop_back();

            subf_stack.push_back(Not::create(left));
            subf_stack.push_back(right);

            op_stack.pop_back();
            op_stack.push_back(OR);
        } break;

        case IFF:{
            shared_ptr<Formula> right = subf_stack.back();
            subf_stack.pop_back();

            shared_ptr<Formula> left = subf_stack.back();
            subf_stack.pop_back();

            subf_stack.push_back(Or::create({Not::create(left), right}));
            subf_stack.push_back(Or::create({Not::create(right), left}));

            op_stack.pop_back();
            op_stack.push_back(AND);
        } break;
    }
}

shared_ptr<Formula> FormulaStackParser::parse(){
    bool expect_binary_op = false;
    for (char c = getChar(); c != EOF; c = getChar()){
        switch (c){
            case ' ':
            case '\t':
            case '\n':
            case '\r':{
                continue;
            } break;

            case '&':{
                if (!expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                while (!op_stack.empty() && op_stack.back() != LPAREN && op_stack.back() != AND){
                    reduceStack();
                }
                op_stack.push_back(AND);
                expect_binary_op = false;
            } break;

            case '|':{
                if (!expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                while (!op_stack.empty() && op_stack.back() != LPAREN && op_stack.back() != OR){
                    reduceStack();
                }

                op_stack.push_back(OR);
                expect_binary_op = false;
            } break;

            case '~':{
                if (expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                op_stack.push_back(NOT);
            } break;

            case '[':{
                if (expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                for (c = getChar(); c != EOF; c = getChar()){
                    if (c == ']'){
                        op_stack.push_back(BOX);
                        break;
                    }

                    if (!isdigit(c)){
                        pair<int, int> pos = getPos();
                        cerr << "Error: invalid modality in box at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }
                }
            } break;

            case '<':{
                c = getChar();

                if (c == '='){
                    c = getChar();
                    if (c != '>'){
                        pair<int, int> pos = getPos();
                        cerr << "Error: Expected char '>' for IFF at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }

                    if (!expect_binary_op){
                        pair<int, int> pos = getPos();
                        cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }
                    expect_binary_op = false;
                    op_stack.push_back(IFF);
                    continue;
                }

                if (expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                for (; c != EOF; c = getChar()){
                    if (c == '>'){
                        op_stack.push_back(DIA);
                        break;
                    }

                    if (!isdigit(c)){
                        pair<int, int> pos = getPos();
                        cerr << "Error: invalid modality in diamond at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }
                }
            } break;

            case '(':{
                if (expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                op_stack.push_back(LPAREN);
            } break;

            case ')':{
                if (!expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                }
                while (!op_stack.empty() && op_stack.back() != LPAREN){
                    reduceStack();
                }

                if (op_stack.empty()){
                    pair<int, int> pos = getPos();
                    cerr << "Mismatched parenthesis at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                op_stack.pop_back();

                while (!op_stack.empty() && is_unary(op_stack.back())){
                    reduceStack();
                }
            } break;

            case '=':{
                c = getChar();

                if (c != '>'){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Expected char '>' for IMP at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                if (!expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                expect_binary_op = false;
                op_stack.push_back(IMP);
            } break;

            default:{
                if (!isalnum(c) && c != '$'){
                    pair<int, int> pos = getPos();
                    cerr << "Error: invalid character:  at line number " << pos.first << " index " << pos.second << c << endl;
                    exit(1);
                }

                if (expect_binary_op){
                    pair<int, int> pos = getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                string atom_name;

                for (; isalnum(lookahead())  || lookahead() == '$'; c = getChar()){
                    atom_name.push_back(c);
                }
                atom_name.push_back(c);

                if (atom_name == "$true"){
                    subf_stack.push_back(True::create());
                    expect_binary_op = true;
                }
                else if (atom_name == "$false"){
                    subf_stack.push_back(False::create());
                    expect_binary_op = true;
                }
                else {
                    subf_stack.push_back(Atom::create(atom_name));
                    expect_binary_op = true;
                }

                while (!op_stack.empty() && is_unary(op_stack.back())){
                    reduceStack();
                }
            }
        }
    }

    while (!op_stack.empty()){
        reduceStack();
    }

    if (subf_stack.size() != 1){
        cerr << "Error: invalid formula" << endl;
        exit(1);
    }

    return subf_stack.back();
}