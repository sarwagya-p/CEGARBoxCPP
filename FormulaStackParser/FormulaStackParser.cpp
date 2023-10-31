#include "FormulaStackParser.h"

FormulaStackParser::FormulaStackParser(string filename){
    reader = make_shared<StreamReader>(filename);
}

FormulaStackParser::~FormulaStackParser() {}

FormulaStackParser::StreamReader::StreamReader(string filename): line_number(1), index(1) {
    input_file.open(filename);
    buffer_char = input_file.get();
}

FormulaStackParser::StreamReader::~StreamReader() {
    input_file.close();
}

char FormulaStackParser::StreamReader::getChar() {
    char next_char = buffer_char;
    buffer_char = input_file.get();

    if(next_char == '\n'){
        line_number++;
        index = 1;
    } else {
        index++;
    }

    return next_char;
}

char FormulaStackParser::StreamReader::lookahead() {
    return buffer_char;
}

pair<int, int> FormulaStackParser::StreamReader::getPos() {
    return make_pair(line_number, index);
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
                subf_set.insert(subf_stack.back());
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
                subf_set.insert(subf_stack.back());
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
        }

        case IFF:{
            shared_ptr<Formula> right = subf_stack.back();
            subf_stack.pop_back();

            shared_ptr<Formula> left = subf_stack.back();
            subf_stack.pop_back();

            subf_stack.push_back(Or::create({Not::create(left), right}));
            subf_stack.push_back(Or::create({Not::create(right), left}));

            op_stack.pop_back();
            op_stack.push_back(AND);
        }
    }
}

shared_ptr<Formula> FormulaStackParser::parse(){
    bool expect_binary_op = false;
    for (char c = reader->getChar(); c != EOF; c = reader->getChar()){
        switch (c){
            case ' ':
            case '\t':
            case '\n':
            case '\r':{
                continue;
            } break;

            case '&':{
                if (!expect_binary_op){
                    pair<int, int> pos = reader->getPos();
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
                    pair<int, int> pos = reader->getPos();
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
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                op_stack.push_back(NOT);
            } break;

            case '[':{
                if (expect_binary_op){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                for (c = reader->getChar(); c != EOF; c = reader->getChar()){
                    if (c == ']'){
                        op_stack.push_back(BOX);
                        break;
                    }

                    if (!isdigit(c)){
                        pair<int, int> pos = reader->getPos();
                        cerr << "Error: invalid modality in box at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }
                }
            } break;

            case '<':{
                c = reader->getChar();

                if (c == '='){
                    c = reader->getChar();
                    if (c != '>'){
                        pair<int, int> pos = reader->getPos();
                        cerr << "Error: Expected char '>' for IFF at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }

                    if (!expect_binary_op){
                        pair<int, int> pos = reader->getPos();
                        cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }
                    expect_binary_op = false;
                    op_stack.push_back(IFF);
                    continue;
                }

                if (expect_binary_op){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                for (c = reader->getChar(); c != EOF; c = reader->getChar()){
                    if (c == '>'){
                        op_stack.push_back(DIA);
                        break;
                    }

                    if (!isdigit(c)){
                        pair<int, int> pos = reader->getPos();
                        cerr << "Error: invalid modality in diamond at line number " << pos.first << " index " << pos.second << endl;
                        exit(1);
                    }
                }
            } break;

            case '(':{
                if (expect_binary_op){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                op_stack.push_back(LPAREN);
            } break;

            case ')':{
                if (!expect_binary_op){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                }
                while (!op_stack.empty() && op_stack.back() != LPAREN){
                    reduceStack();
                }

                if (op_stack.empty()){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Mismatched parenthesis at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                op_stack.pop_back();
            } break;

            case '=':{
                c = reader->getChar();

                if (c != '>'){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Expected char '>' for IMP at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                if (!expect_binary_op){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Expected unary operator or atom at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }
                expect_binary_op = false;
                op_stack.push_back(IMP);
            } break;

            default:{
                if (!isalnum(c)){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: invalid character:  at line number " << pos.first << " index " << pos.second << c << endl;
                    exit(1);
                }

                if (expect_binary_op){
                    pair<int, int> pos = reader->getPos();
                    cerr << "Error: Binary operator or EOF expected at line number " << pos.first << " index " << pos.second << endl;
                    exit(1);
                }

                string atom_name;

                for (; isalnum(reader->lookahead()); c = reader->getChar()){
                    atom_name.push_back(c);
                }
                atom_name.push_back(c);
                subf_stack.push_back(Atom::create(atom_name));
                expect_binary_op = true;
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