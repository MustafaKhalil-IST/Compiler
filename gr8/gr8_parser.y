%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#include <cdk/basic_type.h>
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!

int Unspec = -1, Use = 0, Public = 1, Local = 2;

%}
%union {
  int i;
  double d;
  std::string *s;
  cdk::basic_node *node;
  cdk::sequence_node *sequence;
  cdk::expression_node *expression;
  basic_type *t;
  gr8::variable_node *var;
  cdk::lvalue_node     *lvalue;
};



%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tIF tINIT tFUNCTION tPROCEDURE tAS tTWEET tSTOP tAGAIN tRETURN tSWEEP tPOST tPUBLIC tASSIGN tTO tTHEN tFROM tBY tUSES tINPUT tCELL tAT tDO tUSE tFOR tDEFINE tON tNULL tSMALL tHUGE tNEWS tFAKE tMINUS tPLUS tCHL tIN tOUT tSEP

%nonassoc tIFX
%nonassoc tELSE tELSIF

%left ','
%left tASSIGN
%left tEQUALS
%left tOR
%left tAND
%nonassoc tNOT
%left tBELOW 
%left tABOVE
%left tPLUS
%left tMINUS
%left tTIMES 
%left tOVER
%left tMODULUS 
%nonassoc tAT
%nonassoc tUNARY '?' 
%nonassoc '(' ')' tOBJECTS

%type <node> file instruction declaration function_declaration block conditional_instruction iteration_instruction void_function_decl non_void_function_decl optional_conditional_instruction
%type <var> block_variable_declaration variable_declaration
%type <sequence> instructions declarations block_variable_declarations expressions function_arguments
%type <expression> expression
%type <lvalue> lval
%type <s> string
%type <t> type small huge fake


%{
//-- The rules below will be included in yyparse, the main parsing function.
%}

%%

file:	        declarations			{ compiler->ast($1); }
 		;   		

declarations:	declarations declaration 	{ $$ = new cdk::sequence_node(LINE, $2, $1); }			
                |			        { $$ = new cdk::sequence_node(LINE, new cdk::nil_node(LINE)); }
		;

declaration:	variable_declaration tSEP   	{ $1->scope(Public); $$ = $1; }
     		| function_declaration       	{ $$ = $1; }
		;
 
variable_declaration:	block_variable_declaration  // not complete $$ = $1
				   { $$ =  new gr8::variable_node(LINE, $1->type(), $1->identifier(), $1->initializeExpression(), Local); } 
                        | tPUBLIC block_variable_declaration           
                                   { $$ =  new gr8::variable_node(LINE, $2->type(), $2->identifier(), $2->initializeExpression(), Public); }
         		| tUSE type tIDENTIFIER				   
                                   { $$ = new gr8::variable_node(LINE, $2, $3, nullptr, Use); }
			;

block_variable_declaration: 	type tIDENTIFIER "(" tINIT expression ")"    { $$ = new gr8::variable_node(LINE, $1, $2, $5, Unspec); }
				| type tIDENTIFIER 			     { $$ = new gr8::variable_node(LINE, $1, $2, nullptr, Unspec); }
				;

block_variable_declarations:	block_variable_declarations block_variable_declaration tSEP 
                                                                       { $$ = new cdk::sequence_node(LINE, $2, $1); }
                		|                                      { $$ = new cdk::sequence_node(LINE, new cdk::nil_node(LINE)); }
				;

block: 			 tIN block_variable_declarations instructions tOUT         { $$ = new gr8::block_node(LINE, $2, $3); }
			 ; //here

function_arguments:	function_arguments ',' block_variable_declaration  
                                                                       { $$ = new cdk::sequence_node(LINE, $3, $1); }
                	| block_variable_declaration                   { $$ = new cdk::sequence_node(LINE, $1); }
			;
non_void_function_decl:  tPUBLIC type tFUNCTION tIDENTIFIER tUSES function_arguments tSEP
  					{ $$ = new gr8::function_node(LINE, $2, $4, $6, nullptr, Public); }
                    	 | tDEFINE tPUBLIC type tFUNCTION tIDENTIFIER tON function_arguments tAS tSEP block
					{ $$ = new gr8::function_node(LINE, $3, $5, $7, $10, Public); }
   			 | tPUBLIC type tFUNCTION tIDENTIFIER tSEP  
				 	{ $$ = new gr8::function_node(LINE, $2, $4, nullptr, nullptr, Public); }
			 | tDEFINE tPUBLIC type tFUNCTION tIDENTIFIER tAS tSEP block 
					{ $$ = new gr8::function_node(LINE, $3, $5, nullptr, $8, Public); }
			 | type tFUNCTION tIDENTIFIER tUSES function_arguments tSEP
  					{ $$ = new gr8::function_node(LINE, $1, $3, $5, nullptr, Local); }
                    	 | tDEFINE type tFUNCTION tIDENTIFIER tON function_arguments tAS tSEP block
					{ $$ = new gr8::function_node(LINE, $2, $4, $6, $9, Local); }
   			 | type tFUNCTION tIDENTIFIER tSEP   
				 	{ $$ = new gr8::function_node(LINE, $1, $3, nullptr, nullptr, Local); }
			 | tDEFINE type tFUNCTION tIDENTIFIER tAS tSEP block
				 	{ $$ = new gr8::function_node(LINE, $2, $4, nullptr, $7, Local); }
                         ;

void_function_decl:      tPUBLIC tPROCEDURE tIDENTIFIER tUSES function_arguments tSEP
				        { $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID)
									  , $3, $5, nullptr, Public); }
			 | tDEFINE tPUBLIC tPROCEDURE tIDENTIFIER tON function_arguments tAS tSEP block
          			        { $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID), $4, $6, $9, Public); }
			 | tDEFINE tPROCEDURE tIDENTIFIER tON function_arguments tAS tSEP block
          				{ $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID), $3, $5, $8, Local); }
			 | tDEFINE tPROCEDURE tIDENTIFIER tAS tSEP block
          				{ $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID)
											    , $3, nullptr, $6, Local); }
			 | tPROCEDURE tIDENTIFIER tSEP
				        { $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID),
												 $2, nullptr, nullptr, Local); }
          		 | tPUBLIC tPROCEDURE tIDENTIFIER tSEP
				        { $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID),
												 $3, nullptr, nullptr, Public); }
			 | tDEFINE tPROCEDURE tIDENTIFIER tSEP
				        { $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID),
												 $3, nullptr, nullptr, Local); }
			 | tDEFINE tPUBLIC tPROCEDURE tIDENTIFIER tAS tSEP block
          				{ $$ = new gr8::function_node(LINE, new basic_type(4, basic_type::TYPE_VOID)
									  , $4, nullptr, $7, Public); }
			 ;

function_declaration:    void_function_decl				    { $$ = $1; }
 			 | non_void_function_decl			    { $$ = $1; }
			 ;		


type:			 tSMALL         { $$ = new basic_type(4, basic_type::TYPE_INT); }
			 | small tFAKE  { $$ = new basic_type(4, basic_type::TYPE_POINTER); 
				   	  $$->_subtype = $1;  }
         		 | tHUGE        { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
			 | huge tFAKE   { $$ = new basic_type(4, basic_type::TYPE_POINTER); 
					  $$->_subtype = $1; }
         		 | tNEWS        { $$ = new basic_type(4, basic_type::TYPE_STRING); }
			 | fake tNEWS   { $$ = new basic_type(4, basic_type::TYPE_POINTER); 
 					  $$->_subtype = new basic_type(4, basic_type::TYPE_STRING); }
			 ;

small:			 tSMALL 	{ $$ = new basic_type(4, basic_type::TYPE_INT); }
			 | small tSMALL { $$ = new basic_type(4, basic_type::TYPE_POINTER); 
					  $$->_subtype = $1; }
			 ;

huge: 			 tHUGE		{ $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
			 | huge tHUGE   { $$ = new basic_type(4, basic_type::TYPE_POINTER); 
					  $$->_subtype = $1; }
			 ; 

fake:			 tFAKE        	{ $$ = new basic_type(4, basic_type::TYPE_POINTER); } 
			 | fake tFAKE   { $$ = new basic_type(4, basic_type::TYPE_POINTER); 
					  $$->_subtype = $1; }
			 ;


instructions:		 instructions instruction    { $$ = new cdk::sequence_node(LINE, $2, $1); } //here
                         | instruction             	  { $$ = new cdk::sequence_node(LINE, $1); }
			 ;

instruction:		 tASSIGN expression tTO lval tSEP       	{ $$ = new cdk::assignment_node(LINE, $4, $2); }
			 | tTWEET expression tSEP			{ $$ = new gr8::tweet_node(LINE, $2); }
			 | tPOST expression tSEP			{ $$ = new gr8::post_node(LINE, $2); }
			 | tAGAIN tSEP					{ $$ = new gr8::again_node(LINE, 0); }
			 | tSTOP tSEP					{ $$ = new gr8::stop_node(LINE, 0); }
			 | tRETURN expression tSEP			{ $$ = new gr8::return_node(LINE, $2); }
			 | conditional_instruction 			{ $$ = $1; }	
			 | iteration_instruction 			{ $$ = $1; }
			 | expression tSEP				{ $$ = new gr8::evaluation_node(LINE, $1); }
			 ;


conditional_instruction:     tIF expression tTHEN tSEP block %prec tIFX    	
				{ $$ = new gr8::if_node(LINE, $2, $5);  }
			     |  tIF expression tTHEN tSEP block tELSIF optional_conditional_instruction 
				{ $$ = new gr8::if_else_node(LINE, $2, $5, $7);  }
			     |  tIF expression tTHEN tSEP block tELSE tSEP block
			 	{ $$ = new gr8::if_else_node(LINE, $2, $5, $8);  }
			     ;


optional_conditional_instruction:   expression tTHEN tSEP block %prec tIFX
					{ $$ = new gr8::if_node(LINE, $1, $4);  }
				    |  expression tTHEN tSEP block tELSIF optional_conditional_instruction
					{ $$ = new gr8::if_else_node(LINE, $1, $4, $6);  }
				    |  expression tTHEN tSEP block tELSE tSEP block
					{ $$ = new gr8::if_else_node(LINE, $1, $4, $7);  }
				    ;


iteration_instruction:	  tSWEEP lval tFROM expression tTO expression tBY expression tDO tSEP block
                             { $$ = new gr8::sweep_by_node(LINE, $2, $4, $6, $8, $11); } 
			  | tSWEEP lval tFROM expression tTO expression tDO tSEP block   
                             { $$ = new gr8::sweep_by_node(LINE, $2, $4, $6, new cdk::integer_node(LINE,1) , $9); }
			  ;
expression:		  tINTEGER                		{ $$ = new cdk::integer_node(LINE, $1); }
     			  | tDOUBLE                	 	{ $$ = new cdk::double_node(LINE, $1); }
       			  | string                  		{ $$ = new cdk::string_node(LINE, $1); }
     			  | tNULL                  		{ $$ = new gr8::null_node(LINE); }
			  | tUSE expressions tFOR tIDENTIFIER   { $$ = new gr8::function_call_node(LINE, $4, $2); }
			  | tDO tIDENTIFIER			{ $$ = new gr8::function_call_node(LINE, $2, nullptr);}
     			  | tPLUS expression 	%prec tUNARY	{ $$ = new gr8::identity_node(LINE, $2); }
     			  | tMINUS expression   %prec tUNARY	{ $$ = new cdk::neg_node(LINE, $2); } 
			  | tNOT expression                	{ $$ = new cdk::not_node(LINE, $2); } 
     			  | expression tTIMES expression        { $$ = new cdk::mul_node(LINE, $1, $3); }
         		  | expression tOVER expression         { $$ = new cdk::div_node(LINE, $1, $3); }
     			  | expression tMODULUS expression      { $$ = new cdk::mod_node(LINE, $1, $3); } 
     			  | expression tPLUS expression         { $$ = new cdk::add_node(LINE, $1, $3); }
     			  | expression tMINUS expression        { $$ = new cdk::sub_node(LINE, $1, $3); } 
     			  | expression tBELOW expression        { $$ = new cdk::lt_node(LINE, $1, $3); }
     			  | expression tABOVE expression        { $$ = new cdk::gt_node(LINE, $1, $3); }
     			  | expression tEQUALS expression       { $$ = new cdk::eq_node(LINE, $1, $3); } 
     			  | expression tAND expression          { $$ = new cdk::and_node(LINE, $1, $3); } 
     			  | expression tOR expression           { $$ = new cdk::or_node(LINE, $1, $3); }
			  | '(' expression ')'                  { $$ = $2; }
			  | expression tOBJECTS	%prec tUNARY	{ $$ = new gr8::allocation_node(LINE, $1);}
			  | lval  		       		{ $$ = new cdk::rvalue_node(LINE, $1); } //not complete
			  | expression '?'	         	{ $$ = new gr8::address_node(LINE, $1); }
     			  | tINPUT                    		{ $$ = new gr8::read_node(LINE); }
			  ;

expressions:		  expression   		         	{ $$ = new cdk::sequence_node(LINE, $1); }
     			  | expressions ',' expression   	{ $$ = new cdk::sequence_node(LINE, $3, $1); }
			  ;	  

lval:	 		  tCELL expression tAT expression    	{ $$ = new gr8::index_node(LINE, $4, $2); }
			  | tIDENTIFIER				{ $$ = new cdk::identifier_node(LINE, $1); }
     			  ;

string : 		  tSTRING               		{ $$ = $1;}
       			  | string tSTRING        		{ $$ = new std::string(*$1 + *$2);}
       			  ;


%%


