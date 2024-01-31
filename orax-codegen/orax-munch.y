%{

// The following grammar parses a Tree and a list of instructions. The Trees make up a 'tile' (as in instruction selection tile) and the generated functions use Maximal Munch to arrive at the optimal (not optimum!) code

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "orax-munch.h"

extern int yylex(void);
extern void yyerror(const char*);


%}

%union {
   char *string_term;
   MunchNode *ast;
   MunchList list;
}


%token <string_term> TERMINAL NON_TERMINAL SEMANTIC_ACTION INSTRUCTION HEADER FOOTER

%type <ast> OraxMunch Rule SemAction Decl TreeAct Tree
%type <list> TreeAlt Decls Rules

%start OraxMunch

%%


OraxMunch : / * empty */i 
	  | HEADER Decls PCNT_PCNT Rules PCNT_PCNT FOOTER
	  ;

Rules	  : Rule			{ $$ = munch_new_list($1); }
	  | Rules ';' Rule		{ $$ = munch_list_append($1, $3); }
	  ;

Rule	  : NON_TERMINAL ':' TreeAlt	{ $$ = munch_ast_rule($3, $1); }
          ;

Decls	  : Decl			{ $$ = new_munch_list($1); }
	  | Decls ',' Decl		{ $$ = munch_list_append($1, $3); }
	  ;

Decl	  : '%' INSTRUCTION		{ $$ = munch_ast_new_inst($2); }
       	  ;

TreeAlt   : TreeAct			{ $$ = new_munch_list($1); }
	  | TreeAlt '|' TreeAct		{ $$ = munch_list_append($1, $3); }
	  ;

TreeAct   : Tree SEMANTIC_ACTION	{ $$ = munch_ast_tree_action($1, $2); }

Tree	  : TERMINAL			{ $$ = munch_ast_new_term($1);	}
          | NON_TERMINAL		{ $$ = munch_ast_new_non_term($1); }
	  | '(' Tree ')'		{ $$ = munch_ast_munch_tree($2, NULL); }
	  | '(' Tree ',' Tree ')'	{ $$ = munch_ast_munch_tree($2, $4); }
	  ;


