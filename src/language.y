// %skeleton "lalr1.cc"
// %language "c++"
%parse-param {unsigned* result}

%code requires {
#include <z3++.h>
}

%{

#include <stdio.h>
#include <stdlib.h>
#include <z3++.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern z3::context* g_ctx;
extern z3::func_decl* g_node_fct;
extern z3::expr_vector* g_subterms;

void yyerror(unsigned* result, const char* s);
%}

%union {
	char* cval;
	unsigned exprIdx;
}

%token T_TRUE T_FALSE
%token<cval> T_ID
%token T_NOT T_AND T_OR T_IMPL T_EQ T_EDGE T_NEDGE T_OPEN T_CLOSE

%left T_EQ
%left T_IMPL
%left T_OR
%left T_AND

%type<exprIdx> trueExpr falseExpr variableExpr notExpr andExpr orExpr impliesExpr equalExpr groupedExpr boolExpr node negNode assertion

%start assertion

%%

trueExpr : T_TRUE { $$ = g_subterms->size(); g_subterms->push_back(g_ctx->bool_val(true)); };

falseExpr : T_FALSE { $$ = g_subterms->size(); g_subterms->push_back(g_ctx->bool_val(false)); }

variableExpr : T_ID { $$ = g_subterms->size(); g_subterms->push_back(g_ctx->bool_const($1)); };

notExpr	: T_NOT groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(!g_subterms->operator[]($2));};

andExpr
	: groupedExpr T_AND groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(g_subterms->operator[]($1) && g_subterms->operator[]($3));};
	| andExpr T_AND groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(g_subterms->operator[]($1) && g_subterms->operator[]($3));};
	;

orExpr
	: groupedExpr T_OR groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(g_subterms->operator[]($1) || g_subterms->operator[]($3));};
	| orExpr T_AND groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(g_subterms->operator[]($1) || g_subterms->operator[]($3));};
	;

impliesExpr
	: groupedExpr T_IMPL groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(implies(g_subterms->operator[]($1), g_subterms->operator[]($3)));};
	;

equalExpr
	: groupedExpr T_EQ groupedExpr {$$ = g_subterms->size(); g_subterms->push_back(g_subterms->operator[]($1) == g_subterms->operator[]($3));};
	;

groupedExpr
	: trueExpr { $$ = $1; }
	| falseExpr { $$ = $1; }
	| variableExpr { $$ = $1; }
	| notExpr { $$ = $1; }
	| T_OPEN boolExpr T_CLOSE { $$ = $2; }
	;

boolExpr
	:  groupedExpr { $$ = $1; }
	| andExpr { $$ = $1; }
	| orExpr { $$ = $1; }
	| impliesExpr { $$ = $1; }
	| equalExpr { $$ = $1; }
	;
	
node 
	: boolExpr T_EDGE boolExpr {$$ = g_subterms->size(); g_subterms->push_back(g_node_fct->operator()(g_subterms->operator[]($1), g_subterms->operator[]($3)));};

negNode 
	: boolExpr T_NEDGE boolExpr {$$ = g_subterms->size(); g_subterms->push_back(!g_node_fct->operator()(g_subterms->operator[]($1), g_subterms->operator[]($3)));};

assertion
	: node { $$ = $1; *result = $1; }
	| negNode { $$ = $1; *result = $1; }
	;

%% 

void yyerror(unsigned* result, const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(254);
}

/* Declarations */
void set_input_string(const char* in);
void end_lexical_scan(void);

/* This function parses a string */
int parse_string(const char* in) {
  set_input_string(in);
  unsigned result = (unsigned)-1;
  yyparse(&result);
  end_lexical_scan();
  return result;
}
