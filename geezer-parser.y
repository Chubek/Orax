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
	 | defines
	 | defines geezer
	 ;

defines  : cmddef
	 | pkgdef
	 | instdef
	 ;

instdef  : DEFINST FOR INST_IDENT '=' invoke ';'

cmddef   : DEFCMD CMD_IDENT params '=' commands ';'

pkgdef   : DEFPKG PKG_IDENT '=' infotxts ';'

params   : /* empty */
	 | param
	 | param params
	 ;

param    : CMD_IDENT
	 | ELLIPSES
	 ;

infotxts : infotxt
	 | infotxt '|' infotxts
	 ;

infotxt  : PASCAL_CASE ':' INFO_TEXT NEWLINE
         ;

invoke   : INVOKE_IDENT shwords
	 ;

shwords  : shword
	 | shword shwords
	 ;

shword   : SHELL_WORD
	 | QUOTED_SHELL_WORD
	 ;

commands : command
	 | command commands
	 ;

command  : COMMAND_TEXT
	 | REF_IDENT
	 ;
