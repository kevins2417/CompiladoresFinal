%{
#include "tokens.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // <- AÑADIR ESTA LÍNEA

int line_num = 1;
int col_num = 1;

void update_position() {
    col_num += yyleng;
}

void lex_error(const char* msg) {
    fprintf(stderr, "Error léxico en línea %d, columna %d: %s\n", 
            line_num, col_num, msg);
    exit(1);
}
%}

%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT}|_)*
NUMBER      {DIGIT}+
WHITESPACE  [ \t\r]+

%%

"program"       { update_position(); return PROGRAM; }
"var"           { update_position(); return VAR; }
"begin"         { update_position(); return BEGIN_KW; }
"end"           { update_position(); return END; }
"if"            { update_position(); return IF; }
"then"          { update_position(); return THEN; }
"else"          { update_position(); return ELSE; }
"while"         { update_position(); return WHILE; }
"do"            { update_position(); return DO; }
"read"          { update_position(); return READ; }
"write"         { update_position(); return WRITE; }
"integer"       { update_position(); return INTEGER; }
"boolean"       { update_position(); return BOOLEAN; }
"true"          { update_position(); return TRUE; }
"false"         { update_position(); return FALSE; }
"and"           { update_position(); return AND; }
"or"            { update_position(); return OR; }
"not"           { update_position(); return NOT; }

{ID}            { update_position(); 
                  yylval.str = strdup(yytext);  // <- CORREGIR: yytext NO yylext
                  return IDENTIFIER; }

{NUMBER}        { update_position(); 
                  yylval.num = atoi(yytext); 
                  return NUMBER; }

":="            { update_position(); return ASSIGN; }
";"             { update_position(); return SEMICOLON; }
","             { update_position(); return COMMA; }
":"             { update_position(); return COLON; }
"."             { update_position(); return DOT; }
"("             { update_position(); return LPAREN; }
")"             { update_position(); return RPAREN; }
"+"             { update_position(); return PLUS; }
"-"             { update_position(); return MINUS; }
"*"             { update_position(); return MULTIPLY; }
"/"             { update_position(); return DIVIDE; }
"<="            { update_position(); return LE; }
">="            { update_position(); return GE; }
"<>"            { update_position(); return NE; }
"<"             { update_position(); return LT; }
">"             { update_position(); return GT; }
"="             { update_position(); return EQ; }

"{"[^}]*"}"     { 
                  for (int i = 0; i < yyleng; i++) {
                      if (yytext[i] == '\n') {
                          line_num++;
                          col_num = 1;
                      } else {
                          col_num++;
                      }
                  }
                }

"{"[^}]*$       { lex_error("Comentario no cerrado"); }

{WHITESPACE}    { col_num += yyleng; }

"\n"           { line_num++; col_num = 1; }

.               { 
                  char msg[100];
                  sprintf(msg, "Caracter inválido '%s'", yytext);
                  lex_error(msg);
                }

%%