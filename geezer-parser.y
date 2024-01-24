%{

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#include "geezer.h"


%}

%union
{
  char* sval;
  ASTNode* nodeval;
}


%token DEFCMD DEFINST DEFPKG FOR NEWLINE ELLIPSES


%token <sval> CMD_IDENT INST_IDENT PKG_IDENT PASCAL_CASE
%token <sval> INVOKE_IDENT SHELL_WORD QUOTED_SHELL_WORD REF_IDENT
%token <sval> INFO_TEXT COMMAND_TEXT


%type <nodeval> geezer defines instdef cmddef pkgdef params param infotxts
%type <nodeval> infotext invoke shword shwords command commands

%%

geezer   : /* empty */
	 | defines				{ $$ = $1; }
	 | geezer defines			{ $$ = ast_add_child(&$1, $2);  }
	 ;

defines  : cmddef	{ $$ = $1; }
	 | pkgdef	{ $$ = $1; }
	 | instdef	{ $$ = $1; }
	 ;

instdef  : DEFINST FOR INST_IDENT '=' invoke ';'    { ASTNode *leaf_id =
	 						new_ast_leaf(AST_INST_NAME, $3);
						      $$ = ast_add_child(&leaf_id, $5);  }

cmddef   : DEFCMD CMD_IDENT params '=' commands ';' { ASTNode *leaf_id =
	 						new_ast_leaf(AST_CMD_NAME, $2);
						     $$ = ast_add_child(&leaf_id, $4);   }

pkgdef   : DEFPKG PKG_IDENT '=' infotxts ';'    { ASTNode *leaf_id = 
	 					    new_ast_leaf(AST_PKG_NAME, $2);
						  $$ = ast_add_child(&leaf_id, $4);	 }

params   : /* empty */
	 | param				{ $$ = $1;				 }
	 | params param				{ $$ = ast_add_child(&$1, $2);		 }
	 ;

param    : CMD_IDENT				{ $$ = new_ast_leaf(AST_CMD_IDENT, $1);  }
	 | ELLIPSES				{ $$ = new_ast_leaf(AST_ELLIPSES, NULL); }
	 ;

infotxts : infotxt				{ $$ = $1; 				}
	 | infotxts '|' infotxt			{ $$ = ast_add_child(&$1, $2);		}
	 ;

infotxt  : PASCAL_CASE ':' INFO_TEXT NEWLINE    { $$ = new_ast_leaf(AST_INFO_TEXT, $1); }
         ;

invoke   : INVOKE_IDENT shwords
	 ;

shwords  : shword				{ $$ = $1;				 }
	 | shwords shword			{ $$ = ast_add_child(&$1, $2); 		 }
	 ;

shword   : SHELL_WORD				{ $$ = new_ast_leaf(AST_SHELL_WORD, $1); }
	 | QUOTED_SHELL_WORD			{ $$ = new_ast_leaf(AST_SHELL_WORD, $1); }
	 ;

commands : command				{ $$ = $1			         }
	 | commands command			{ $$ = ast_add_child(&$1, $2);		 }
	 ;

command  : COMMAND_TEXT				{ $$ = new_ast_leaf(AST_CMD_TEXT, $1);  }
	 | REF_IDENT				{ $$ = new_ast_leaf(AST_REF_IDENT, $1); }
	 ;
