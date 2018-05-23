%option c++ prefix="gr8_scanner_" outfile="gr8_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <stack>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "gr8_parser.tab.h"

int ind = 0;
std::stack<int>lvls;

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT_OP X_COMMENT_EX X_NORMAL X_INDENT X_SPECIAL

D		[0-9]
LIT		[0-9a-fA-F]
INTDEC 		0|([1-9][0-9]*)
INTHIP		0|(0[0-6]*)
CHL		\n
%%

.			unput(*yytext); BEGIN(X_NORMAL);lvls.push(0);
<X_NORMAL>" "		;

"!!"			yy_push_state(X_COMMENT_EX);
<X_COMMENT_EX>"\n"	yy_pop_state();

<X_NORMAL>"<<"          yy_push_state(X_COMMENT_OP);
<X_COMMENT_OP>"<<"	yy_push_state(X_COMMENT_OP);
<X_COMMENT_OP>">>"	yy_pop_state();BEGIN(X_NORMAL);
<X_COMMENT_OP>.|\n 	;

<X_NORMAL>"if"                  std::cout<<"if ";return tIF; 
<X_NORMAL>"else" 		return tELSE;
<X_NORMAL>"elsif"		return tELSIF;
<X_NORMAL>"small"		return tSMALL; 
<X_NORMAL>"huge"		return tHUGE;
<X_NORMAL>"below"		return tBELOW; 
<X_NORMAL>"above"		return tABOVE;
<X_NORMAL>"equals"	        return tEQUALS;
<X_NORMAL>"not"		        return tNOT;
<X_NORMAL>"times"		return tTIMES;
<X_NORMAL>"over"		return tOVER;
<X_NORMAL>"modulus"	        return tMODULUS;
<X_NORMAL>"or"		        return tOR;
<X_NORMAL>"and"		        return tAND;
<X_NORMAL>"assign"	        return tASSIGN;
<X_NORMAL>"to"		        return tTO;
<X_NORMAL>"then"		return tTHEN;
<X_NORMAL>"from"		return tFROM;
<X_NORMAL>"do"		        return tDO;
<X_NORMAL>"by"		        return tBY;
<X_NORMAL>"for"		        return tFOR;
<X_NORMAL>"initially"	        return tINIT;
<X_NORMAL>"function"	        return tFUNCTION;
<X_NORMAL>"procedure"	        return tPROCEDURE;
<X_NORMAL>"define" 	        return tDEFINE;
<X_NORMAL>"on"		        return tON;
<X_NORMAL>"as"		        return tAS;
<X_NORMAL>"fake"		return tFAKE;
<X_NORMAL>"news"		return tNEWS;
<X_NORMAL>"tweet"		return tTWEET;
<X_NORMAL>"stop"		return tSTOP;
<X_NORMAL>"again"		return tAGAIN;
<X_NORMAL>"return"	        return tRETURN;
<X_NORMAL>"sweeping"	        return tSWEEP;
<X_NORMAL>"post"		return tPOST;
<X_NORMAL>"public"              return tPUBLIC;
<X_NORMAL>"use"		        return tUSE;
<X_NORMAL>"uses"		return tUSES;
<X_NORMAL>"input"		return tINPUT;
<X_NORMAL>"cell"		return tCELL;
<X_NORMAL>"at"		        return tAT;
<X_NORMAL>"objects"	        return tOBJECTS;
<X_NORMAL>"null"		return tNULL;
<X_NORMAL>"minus"		return tMINUS;
<X_NORMAL>"plus"		return tPLUS;

<X_NORMAL>[A-Za-z][A-Za-z0-9-]* yylval.s = new std::string(yytext); return tIDENTIFIER;
<X_NORMAL>\n		BEGIN(X_INDENT); ind = 0;return tSEP;

<X_INDENT>" "     	ind++;
<X_INDENT>\t		ind += 8;
<X_INDENT>.		{ 
				unput(*yytext);
				if(ind > lvls.top()){
					lvls.push(ind);
					BEGIN(X_NORMAL);
					std::cout<<"in ";return tIN;
				}
				else if(ind < lvls.top()){
					lvls.pop();
					
					std::cout<<"out ";return tOUT;
				}
				else{
					BEGIN(X_NORMAL);
				}
			}
<X_INDENT><<EOF>>	{
				if(lvls.top() != 0){
					lvls.pop();
					std::cout<<"out ";return tOUT;
					yyterminate();
				}
				yyterminate();
			}

<X_NORMAL>{INTDEC}	      yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
<X_NORMAL>{INTHIP}	      yylval.i = strtol(yytext, nullptr, 7); return tINTEGER;

<X_NORMAL>({D}+\.{D}*|{D}*\.{D}+)([eE][-+]?{D}+)?		yylval.d = strtod(yytext, nullptr); return tDOUBLE;
<X_NORMAL>[-+]?{D}+[eE][-+]?{D} 				yylval.d = strtod(yytext, nullptr); return tDOUBLE; 

<X_NORMAL>[-()+!,~|?\<>]    	return *yytext;

<X_NORMAL>\"                    yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\~ 			yy_push_state(X_SPECIAL);
<X_SPECIAL>[0-7]{1,3}           *yylval.s += (char)strtol(yytext,nullptr, 7); yy_pop_state();
<X_SPECIAL>"n"		        yy_pop_state();
<X_STRING>\"      		yy_pop_state(); BEGIN(X_NORMAL); return tSTRING;
<X_STRING>\\\\			*yylval.s += "\\";
<X_STRING>.             	*yylval.s += yytext;
<X_STRING>\n			yyerror("newline in string");
"...\n"				;
<X_NORMAL>.                      	yyerror("Unknown character");



%%


