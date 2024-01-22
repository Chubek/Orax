%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



FILE* yyin = NULL;


%}



%x COMMAND
%x COMMENT
%x PARAMDEF
%x REGEX
%x DEF
%x PKG
%x TEXT
%x INFO_DEF
%x INSTALL_TEXT

ident [-a-zA-Z0-9]+


%%

"defn"		{ BEGIN(DEF); return DEFN; }
"pckg"		{ BEGIN(PKG); return PCKG; }


<DEF>{ident}	{ BEGIN(PARAMDEF); 
			yylval = strdup(yytext);
			return DEF_IDENT;	 }

<PARAMDEF>"..."		return ELLIPSE_PATT;

<PARAMDEF>{ident} { yylval = strdup(yytext);
			return IDENT_PATT;	}


<PARAMDEF>"/"	BEGIN(REGEX);
<REGEX>[^/]+	{ BEGIN(PARAMDEF);
			yylval = strdup(yytext);
			return REGEX_PATT;	}

<REGEX>"/"	BEGIN(PARAMDEF);
<PARAMDEF>"=>" { BEGIN(COMMAND);
			return MAPSTO;		}

<COMMAND>[^;]+ { yylval = strdup(yytext);
			return COMMAND_DEF;	}

<COMMAND>";"   { BEGIN(INITIAL); return *yytext; }


<PKG>{ident}   { yylval = strdup(yytext); return PKG_IDENT; }
<PKG>"="       { BEGIN(INFO_DEF); return *yytext;	    }


<INFO_DEF>[:|]  return *yytext;

<INFO_DEF>"Name"	{ BEGIN(TEXT); return NAME;        }
<INFO_DEF>"Author"	{ BEGIN(TEXT); return AUTHOR;      }
<INFO_DEF>"Repository"  { BEGIN(TEXT); return REPOSITORY;  }
<INFO_DEF>"Description" { BEGIN(TEXT); return DESCRIPTION; }
<INFO_DEF>"Install"	{ BEGIN(INSTALL_TEXT); return INSTALL; }

<TEXT>.+		{ yylval = strdup(yytext); return PACKAGE_TEXT; }
<TEXT>[\n\r]+		BEGIN(INFO_DEF);

<INSTALL_TEXT>"%"{ident}  { yylval = strdup(&yytext[1]); return CALL_IDENT; }
<INSTALL_TEXT>.+	  { yylval = strdup(yytext); return INSTALL_COMMAND; }
<INSTALL_TEXT>[\n\r]+	  BEGIN(INFO_DEF);

<INFO_DEF>"end"		{ BEGIN(INITIAL); return END; }



