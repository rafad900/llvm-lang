%{
    #include "parser.tab.hpp"
    #include <cstring>
    extern "C" int yylex();  
    char* scan_string(const char* s, int length);
%}
%option noyywrap
%option yylineno

%%
[0-9]+              { yylval.intVal     = atoi(yytext); return INTEGER_LITERAL; }
[0-9]+.[0-9]+       { yylval.floatVal   = atoi(yytext); return FLOAT_LITERAL; }
\"(\\.|[^\\])*\"    { yylval.strVal     = scan_string(yytext, yyleng); return STRING_LITERAL; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MULT; }
"/"                 { return DIV; }
";"                 { return SEMICOLON; }
[ \t\r\n\f]         ; /* ignore whitespaces */
%%

char* scan_string(const char* s, int length) {
    if (length >= 2) {
        size_t len = length - 2;
        char* copy = (char*)malloc(len+1);
        memcpy(copy, s+1, len);
        s[len] = '\0';
        return s;
    } else {
        return strdup("");
    }
}
