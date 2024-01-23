%{

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "geezer.h"

size_t line_num = 0;
size_t column_num = 0;

%}



identifier 	[a-zA-Z_][-a-zA-Z0-9_]*
pascal_case	[A-Z][a-z]*([A-Z][a-z]*)*


command_text	[^;$\n]+
info_text	[^\n]+

shell_word	[[:alnum:]_./-]+|'[^']*'|"[^"]*

%x DEF_CMD_COND
%x DEF_INST_COND
%x DEF_PKG_COND

%%

"defcmd"		{
			   BEGIN(DEF_CMD_COND);
			   column_name += yyleng;
			   return DEFCMD;
			}
"definst"		{
			   BEGIN(DEF_INST_COND);
			   column_num += yyleng;
 		    	   return DEFINST;
			}
"defpkg"		{
			   BEGIN(DEF_PKG_COND);
			   column_num += yyleng;
			   return DEFPKG;
			}

<DEF_CMD_COND>{identifier}	{
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return CMD_IDENT;
				}

<DEF_INST_COND>{identifier}     {
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return INST_IDENT;
				}
<DEF_PKG_COND>{identifier}	{
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return PKG_IDENT;
				}

<DEf_PKG_COND>{pascal_case}	{
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return PASCAL_CASE;
				}

<DEF_INST_COND>"%"{identifier}  {
				   yylval.sval = strndup(&yytext[1], yyleng - 1);
				   column_num += yyleng;
				   return INVOKE_IDENT;
				}

<DEF_INST_COND>{shell_word}	{
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return SHELL_WORD;
				}
<DEF_INST_COND>["']{shell_word}['"] {
				      yylval.sval = strndup(&yytext[1], yyleng - 2);
				      column_num += yyleng;
				      return QUOTED_SHELL_WORD;
				    }
<DEF_PKG_COND,DEF_INST_COND,DEF_CMD_COND>[;] {
						BEGIN(INITIAL);
						column_num += yyleng;
						return *yytext;
					     }
<DEF_CMD_COND>"$"{identifier}   {
				   yylval.sval = strndup(&yytext[1], yyleng - 1);
				   column_num += yyleng;
				   return REF_IDENT;
		                }

<DEF_CMD_COND,DEF_INST_COND,DEF_PKG_COND>[=] { column_num += yyleng; return *yytext; }
<DEF_PKG_COND>[\n]+		{ line_num += yyleng;	return NEWLINE; }

<DEF_PKG_COND>{info_text}	{
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return INFO_TEXT;
				}
<DEF_CMD_COND>{command_text}	{
				   yylval.sval = strndup(yytext, yyleng);
				   column_num += yyleng;
				   return COMMAND_TEXT;
				}

<DEF_INST_COND>"for"		{ column_name += yyleng; return FOR; }

<DEF_CMD_COND>"..."		{ column_name += yyleng; return ELLIPSES; }

<DEF_PKG_COND>[:|]		{ column_name += yyleng; return *yytext; }



";;".+				{  column_name += yyleng;  }
[\n]+				{  line_num += yyleng;     }

%%

