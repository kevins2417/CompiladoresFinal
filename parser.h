#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Variables globales del lexer */
extern int yylex();
extern char* yytext;
extern int line_num;
extern int col_num;
extern FILE* yyin;

/* Token actual y manejo de errores */
extern int current_token;
extern int error_count;
extern int max_errors;

/* Funciones del parser */
void init_parser(FILE* input);
void parse_program();
void parser_error(const char* expected);
void match(int expected_token);
void recover_from_error();
int get_error_count();
void print_ll1_table();

/* Producciones de la gramática */
void declarations();
void declaration_list();
void declaration();
void id_list();
void type();
void compound_stmt();
void stmt_list();
void stmt();
void assignment_stmt();
void if_stmt();
void while_stmt();
void read_stmt();
void write_stmt();
void expression();
void simple_expr();
void term();
void factor();
void relop();
void addop();
void mulop();

/* Funciones LL(1) */
void init_ll1_table();
void parse_ll1();
const char* get_non_terminal_name(int nt);

/* Utilidades */
const char* token_name(int token);
void print_first_sets();
void print_follow_sets();

#endif