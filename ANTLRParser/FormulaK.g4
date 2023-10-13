grammar FormulaK;

/* Operators */
INT: [0-9]+ ;

AND: '&' ;
OR: '|';

termOp: AND | OR ;

IMP: '=>' ;
IFF: '<=>' ;

impOp: IMP | IFF ;

NOT: '~' ;

BOX: '[]' ;
DIA: '<>' ;
IDXBOX: '[' INT? ']' ;
IDXDIA: '<' INT? '>' ;

unaryOp: NOT | BOX | DIA | IDXBOX | IDXDIA ;

/* Variable Name */
NAME: [a-zA-Z]([a-zA-Z0-9])* ;

/* Formula Structure */

formula: term (termOp term)* ;
term: literal (impOp literal)? ;
literal: NAME | unaryOp literal | '(' formula ')' ;
