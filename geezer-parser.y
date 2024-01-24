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
%token <sval> INFO_TEXT COMMAND_TEXT TERM


%type <nodeval> geezer defines instdef cmddef pkgdef params param infotxts
%type <nodeval> infotext invoke shword shwords command commands

%%

geezer   : /* empty */
	 | defines				{ $$ = $1; }
	 | geezer defines			{ $$ = ast_add_child(&$1, $2);  }
	 | geezer TERM				{ $$ = ast_add_child(&$1, 
	 							new_ast_terminate()); }
	 ;

defines  : cmddef	{ $$ = $1; }
	 | pkgdef	{ $$ = $1; }
	 | instdef	{ $$ = $1; }
	 ;

instdef  : DEFINST FOR 
	 	  INST_IDENT '=' invoke ';'    { $$ = new_ast_inst($3, $5);		 }

cmddef   : DEFCMD CMD_IDENT 
	 	  params '=' commands ';'	{ $$ = new_ast_cmd($2, $3, $4);	 	 }

pkgdef   : DEFPKG PKG_IDENT '=' infotxts ';'    { $$ = new_ast_pkg($2, $4);		 }

params   : /* empty */
	 | param				{ $$ = $1;				 }
	 | params param				{ $$ = ast_add_child(&$1, $2);		 }
	 ;

param    : CMD_IDENT				{ $$ = new_ast_cmdident($1);  		 }
	 | ELLIPSES				{ $$ = new_ast_ellipse();		 }
	 ;

infotxts : infotxt				{ $$ = $1; 				 }
	 | infotxts '|' infotxt			{ $$ = ast_add_child(&$1, $2);		 }
	 ;

infotxt  : PASCAL_CASE ':' INFO_TEXT NEWLINE    { $$ = new_ast_info($1, $3);		 }
         ;

invoke   : INVOKE_IDENT shwords
	 ;

shwords  : shword				{ $$ = $1;				 }
	 | shwords shword			{ $$ = ast_add_child(&$1, $2); 		 }
	 ;

shword   : SHELL_WORD				{ $$ = new_ast_shword($1);		 }
	 | QUOTED_SHELL_WORD			{ $$ = new_ast_shword($1); 		 }
	 ;

commands : command				{ $$ = $1			         }
	 | commands command			{ $$ = ast_add_child(&$1, $2);		 }
	 ;

command  : COMMAND_TEXT				{ $$ = new_ast_cmdtxt($1); 		 }
	 | REF_IDENT				{ $$ = new_ast_refident($1); 		 }
	 ;
