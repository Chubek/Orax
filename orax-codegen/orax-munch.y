%{

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


%token <string_term> TERMINAL NON_TERMINAL SEMANTIC_ACTION IR HEADER FOOTER

%type <ast> MaxMunch Rule SemAction Decl TreeAct Tree
%type <list> TreeAlt Decls Rules

%start MaxMunch

%%


MaxMunch  : / * empty */ 		  {   return;   }
	  | HEADER Decls PCNT_PCNT Rules PCNT_PCNT FOOTER
	  {
		MunchNode *decls_list = munch_ast_new_decl_list($2);
		MunchNode *rules_list = munch_ast_new_rules_list($4);
		MunchState *munch_state = create_munch_state($1, $6, decls_list, rules_list);
		bind_yyout(munch_state);
		install_maximal_munch(munch_state);
		free_munch_state(munch_state);
	  }
	  ;

Rules	  : Rule			{ $$ = munch_new_list($1); }
	  | Rules ';' Rule		{ $$ = munch_list_append($1, $3); }
	  ;

Rule	  : NON_TERMINAL ':' TreeAlt	{ $$ = munch_ast_rule($3, $1); }
          ;

Decls	  : Decl			{ $$ = new_munch_list($1); }
	  | Decls ',' Decl		{ $$ = munch_list_append($1, $3); }
	  ;

OpOpCodes : PCNT_INST INSTRUCTION	{ $$ = munch_ast_new_inst($2); }
       	  ;

TreeAlt   : TreeAct			{ $$ = new_munch_list($1); }
	  | TreeAlt '|' TreeAct		{ $$ = munch_list_append($1, $3); }
	  ;

TreeAct   : Tree SEMANTIC_ACTION	{ $$ = munch_ast_tree_action($1, $2); }

Tree     : TERMINAL                  { $$ = new_munch_list(munch_ast_new_term($1)); }
         | NON_TERMINAL              { $$ = new_munch_list(munch_ast_new_non_term($1)); }
         | '(' TreeList ')'          { $$ = munch_ast_new_tree_list($2); }
         ;

TreeList : Tree                      { $$ = new_munch_list($1); }
         | TreeList ',' Tree         { $$ = munch_list_append($1, $3); }
         ;

%%



