%{
#include "y.tab.h"
%}

%%
"%%"        { return PERCENT_PERCENT; }
"%{"        { return PERCENT_OPEN_BRACE; }
"%}"        { return PERCENT_CLOSE_BRACE; }
";"         { return SEMICOLON; }
","         { return COMMA; }
":"         { return COLON; }
"|"         { return PIPE; }
"{{"        { return DOUBLE_OPEN_BRACE; }
"}}"        { return DOUBLE_CLOSE_BRACE; }
"("         { return OPEN_PAREN; }
")"         { return CLOSE_PAREN; }
[A-Z0-9_]+  { yylval.strval = strdup(yytext); return INSTRUCTION; }
[a-z0-9]+   { yylval.strval = strdup(yytext); return NON_TERMINAL; }
[A-Za-z_][A-Za-z0-9]*  { yylval.strval = strdup(yytext); return TERMINAL; }
[ \t\r\n]+  ;  /* Skip whitespace */
.           { fprintf(stderr, "Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

