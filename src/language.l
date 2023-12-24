%option noyywrap
%option nounistd
%option never-interactive

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "language.tab.h"

%}

%%

[ \t\r\n]					; // ignore all whitespace
"true"					{return T_TRUE;}
"false"					{return T_FALSE;}
([A-Z]|[a-z]|[0-9]|_)+	{
							yylval.cval = new char[strlen(yytext)+1];
							strcpy(yylval.cval, yytext); 
							return T_ID;
						}
"~"						{return T_NOT;}
"&"						{return T_AND;}
"|"						{return T_OR;}
"=>"					{return T_IMPL;}
"="						{return T_EQ;}
"->"					{return T_EDGE;}
"-/>"					{return T_NEDGE;}
"("						{return T_OPEN;}
")"						{return T_CLOSE;}

%%

void set_input_string(const char* in) {
  yy_scan_string(in);
}

void end_lexical_scan(void) {
  yy_delete_buffer(YY_CURRENT_BUFFER);
}