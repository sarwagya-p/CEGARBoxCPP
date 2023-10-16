#include "ANTLRParser.h"

std::any FormulaTreeVisitor::visitTermOp(FormulaKParser::TermOpContext *context){
    if (context->AND() != nullptr)
        return make_any<bool>(true);
    return make_any<bool>(false);
}

std::any FormulaTreeVisitor::visitImpOp(FormulaKParser::ImpOpContext *context){
    if (context->IMP() != nullptr)
        return make_any<bool>(true);
    return make_any<bool>(false);
}

enum unary_op {NOTOP, BOXOP, DIAOP, IDXBOXOP, IDXDIAOP};

std::any FormulaTreeVisitor::visitUnaryOp(FormulaKParser::UnaryOpContext *context){
    if (context->NOT() != nullptr)
        return make_any<unary_op>(NOTOP);

    if (context->BOX() != nullptr)
        return make_any<unary_op>(BOXOP);

    if (context->DIA() != nullptr)
        return make_any<unary_op>(DIAOP);

    if (context->IDXBOX() != nullptr)
        return make_any<unary_op>(IDXBOXOP);

    return make_any<unary_op>(IDXDIAOP);
}

std::any FormulaTreeVisitor::visitFormula(FormulaKParser::FormulaContext *context){
    formula_set subf_set;
    bool cur_type_and = true;

    auto all_term = context->term();
    auto all_term_op = context->termOp();

    subf_set.insert(any_cast<shared_ptr<Formula>>(visit(all_term[0])));

    for (size_t i = 1; i < context->term().size(); i++){
        shared_ptr<Formula> cur_term = any_cast<shared_ptr<Formula>>(visit(all_term[i]));

        if (cur_type_and == any_cast<bool>(visit(all_term_op[i-1]))){
            subf_set.insert(cur_term);
            continue;
        }

        shared_ptr<Formula> new_formula;
        if (cur_type_and){
            new_formula = And::create(subf_set);
        }
        else{
            new_formula = Or::create(subf_set);
        }

        subf_set.clear();
        subf_set.insert(new_formula);
        subf_set.insert(cur_term);

        cur_type_and = !cur_type_and;
    }

    if (subf_set.size() == 1)
        return *subf_set.begin();

    if (cur_type_and)
        return make_any<shared_ptr<Formula>>(And::create(subf_set));
    return make_any<shared_ptr<Formula>>(Or::create(subf_set));
}

std::any FormulaTreeVisitor::visitTerm(FormulaKParser::TermContext *context){
    auto sub_literals = context->literal();

    if (sub_literals.size() == 1)
        return visit(sub_literals[0]);

    shared_ptr<Formula> left_lit = any_cast<shared_ptr<Formula>>(visit(sub_literals[0]));
    shared_ptr<Formula> right_lit = any_cast<shared_ptr<Formula>>(visit(sub_literals[1]));

    if (any_cast<bool>(visit(context->impOp()))){
        return make_any<shared_ptr<Formula>>(Or::create({Not::create(left_lit), right_lit}));
    }

    return make_any<shared_ptr<Formula>>(And::create({
        Or::create({Not::create(left_lit), right_lit}),
        Or::create({Not::create(right_lit), left_lit})
    }));
}

std::any FormulaTreeVisitor::visitLiteral(FormulaKParser::LiteralContext *context){
    if (context->formula() != nullptr)
        return visit(context->formula());

    if (context->unaryOp() != nullptr){
        shared_ptr<Formula> sub_lit = any_cast<shared_ptr<Formula>>(visit(context->literal()));

        switch (any_cast<unary_op>(visit(context->unaryOp())))
        {
        case NOTOP:
            return make_any<shared_ptr<Formula>>(Not::create(sub_lit));
            break;

        case BOXOP:
        case IDXBOXOP:
            return make_any<shared_ptr<Formula>>(Box::create({1}, sub_lit));
            break;

        case DIAOP:
        case IDXDIAOP:
            return make_any<shared_ptr<Formula>>(Diamond::create({1}, sub_lit));
            break;
        }
    }

    return make_any<shared_ptr<Formula>>(Atom::create(context->NAME()->getText()));
}

ANTLRParser::ANTLRParser(const string& filename){
    input.loadFromFile(filename);
}

ANTLRParser::~ANTLRParser(){

}
shared_ptr<Formula> ANTLRParser::parse(){
    FormulaKLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    FormulaKParser parser(&tokens);
    FormulaKParser::FormulaContext* tree = parser.formula();
    FormulaTreeVisitor visitor;
    
    return any_cast<shared_ptr<Formula>>(visitor.visit(tree));
}