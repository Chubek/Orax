%{

// The following grammar parses a Tree and a list of instructions. The Trees make up a 'tile' (as in instruction selection tile) and the generated functions use Maximal Munch to arrive at the optimal (not optimum!) code

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "orax-munch.h"

extern int yylex(void);
extern void yyerror(const char*);


struct MunchNode {
   MunchNodeType type;
   MunchNode *left, *righti;
   const char *value;
};

%}

%union {
   char *string_term;
   MunchNode *ast;
}


%token <string_term> TERMINAL NON_TERMINAL SEMANTIC_ACTION INSTRUCTION HEADER FOOTER

%type <ast> OraxMunch Rules Rule SemAction Decls Decl TreeAlt TreeAct Tree

%start OraxMunch

%%


OraxMunch : HEADER Decls PCNT_PCNT Rules PCNT_PCNT FOOTER
	  {
	    $$ = munch_ast_orax_munch($1, $2, $4, $6);
	  }
	  ;

Rules	  : Rule			{ $$ = $1; }
	  | Rules ';' Rule		{ $$ = munch_ast_append(&$1, $3); }
	  ;

Rule	  : NON_TERMINAL ':' TreeAlt	{ $$ = munch_ast_rule($1, $3); }
          ;

SemAction : /* empty */
	  | DOUBLE_CURLY_OPEN EPSILON DOUBLE_CURLY_CLOSE 
	  { 
	    $$ = munch_ast_semantic_action($2) 
	  }
	  ;

Decls	  : Decl			{ $$ = $1; }
	  | Decls ',' Decl		{ $$ = munch_ast_append(&$1, $3); }
	  ;

Decl	  : '%' INSTRUCTION		{ $$ = munch_ast_new_inst($2); }
       	  ;

TreeAlt   : TreeAct			{ $$ = $1; }
	  | TreeAlt '|' TreeAct		{ $$ = munch_ast_append(&$1, $3); }
	  ;

TreeAct   : Tree SemAction		{ $$ = munch_ast_tree_action($1, $2); }

Tree	  : TERMINAL			{ $$ = munch_ast_new_term($1);	}
          | NON_TERMINAL		{ $$ = munch_ast_new_non_term($1); }
	  | '(' Tree ')'		{ $$ = munch_ast_single_tree($2); }
	  | '(' Tree ',' Tree ')'	{ $$ = munch_ast_pair_trees($2, $4); }
	  ;


