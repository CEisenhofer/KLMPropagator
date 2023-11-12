grammar Language;

options {
	// language = CSharp;
    // visitor=true;
    // listener=true;
}

Identifier
	: [a-zA-Z0-9_+-.]+
	;

WS : [ \t\r\n]+ -> skip;

Comment
    :  ';' (~[\r\n])* -> skip;

Comment2
    :  '//' (~[\r\n])* -> skip;

trueExpr 
    : 'true'
    ;

falseExpr 
    : 'false'
    ;

variableExpr
	: Identifier
    ;

notExpr
	: '~' lhs=groupedExpr
	;

andExpr
	: (groupedExpr '&')+ groupedExpr
	;

orExpr
	: (groupedExpr '|')+ groupedExpr
	;

impliesExpr
	: lhs=groupedExpr '=>' rhs=groupedExpr
	;

equalExpr
	: lhs=groupedExpr '=' rhs=groupedExpr
	;

groupedExpr
	: trueExpr
	| falseExpr
	| variableExpr
	| notExpr
	| '(' boolExpr ')'
	;

boolExpr
	:  groupedExpr
	| andExpr
	| orExpr
	| impliesExpr
	| equalExpr
	;
	
node 
	: lhs=boolExpr '->' rhs=boolExpr
	;

negNode 
	: lhs=boolExpr '-/>' rhs=boolExpr
	;

assertion
	: node
	| negNode
	;

statements
    : assertion*
    ;
