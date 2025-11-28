#include "parser.h"
#include "tokens.h"
#include <stdio.h>
#include <string.h>

#define MAX_TOKENS 300
#define MAX_NON_TERMINALS 25

typedef enum {
    NT_PROGRAM, NT_DECLARATIONS, NT_DECLARATION_LIST, NT_DECLARATION,
    NT_ID_LIST, NT_TYPE, NT_COMPOUND_STMT, NT_STMT_LIST, NT_STMT,
    NT_ASSIGNMENT, NT_IF_STMT, NT_WHILE_STMT, NT_READ_STMT, NT_WRITE_STMT,
    NT_EXPRESSION, NT_SIMPLE_EXPR, NT_TERM, NT_FACTOR,
    NT_EXPRESSION_PRIME, NT_SIMPLE_EXPR_PRIME, NT_TERM_PRIME,
    NT_STMT_LIST_PRIME, NT_DECLARATION_LIST_PRIME, NT_ID_LIST_PRIME
} NonTerminal;

int ll1_table[MAX_NON_TERMINALS][MAX_TOKENS];

void init_ll1_table() {
    for (int i = 0; i < MAX_NON_TERMINALS; i++) {
        for (int j = 0; j < MAX_TOKENS; j++) {
            ll1_table[i][j] = -1;
        }
    }

    // PROGRAM -> PROGRAM IDENTIFIER SEMICOLON declarations compound_stmt DOT
    ll1_table[NT_PROGRAM][PROGRAM] = 1;

    // DECLARATIONS -> VAR declaration_list | ε
    ll1_table[NT_DECLARATIONS][VAR] = 1;
    ll1_table[NT_DECLARATIONS][BEGIN_KW] = 0;

    // DECLARATION_LIST -> declaration declaration_list'
    ll1_table[NT_DECLARATION_LIST][IDENTIFIER] = 1;

    // DECLARATION -> id_list COLON type SEMICOLON
    ll1_table[NT_DECLARATION][IDENTIFIER] = 1;

    // ID_LIST -> IDENTIFIER id_list'
    ll1_table[NT_ID_LIST][IDENTIFIER] = 1;

    // TYPE -> INTEGER | BOOLEAN
    ll1_table[NT_TYPE][INTEGER] = 1;
    ll1_table[NT_TYPE][BOOLEAN] = 2;

    // COMPOUND_STMT -> BEGIN stmt_list END
    ll1_table[NT_COMPOUND_STMT][BEGIN_KW] = 1;

    // STMT_LIST -> stmt stmt_list'
    ll1_table[NT_STMT_LIST][IDENTIFIER] = 1;
    ll1_table[NT_STMT_LIST][BEGIN_KW] = 1;
    ll1_table[NT_STMT_LIST][IF] = 1;
    ll1_table[NT_STMT_LIST][WHILE] = 1;
    ll1_table[NT_STMT_LIST][READ] = 1;
    ll1_table[NT_STMT_LIST][WRITE] = 1;

    // STMT -> assignment_stmt | compound_stmt | if_stmt | while_stmt | read_stmt | write_stmt
    ll1_table[NT_STMT][IDENTIFIER] = 1;
    ll1_table[NT_STMT][BEGIN_KW] = 2;
    ll1_table[NT_STMT][IF] = 3;
    ll1_table[NT_STMT][WHILE] = 4;
    ll1_table[NT_STMT][READ] = 5;
    ll1_table[NT_STMT][WRITE] = 6;

    // EXPRESSION -> simple_expr expression'
    ll1_table[NT_EXPRESSION][IDENTIFIER] = 1;
    ll1_table[NT_EXPRESSION][NUMBER] = 1;
    ll1_table[NT_EXPRESSION][TRUE] = 1;
    ll1_table[NT_EXPRESSION][FALSE] = 1;
    ll1_table[NT_EXPRESSION][LPAREN] = 1;
    ll1_table[NT_EXPRESSION][NOT] = 1;

    printf("✅ Tabla LL(1) inicializada\n");
}

const char* get_non_terminal_name(int nt) {
    switch (nt) {
        case NT_PROGRAM: return "PROGRAM";
        case NT_DECLARATIONS: return "DECLARATIONS";
        case NT_DECLARATION_LIST: return "DECLARATION_LIST";
        case NT_DECLARATION: return "DECLARATION";
        case NT_ID_LIST: return "ID_LIST";
        case NT_TYPE: return "TYPE";
        case NT_COMPOUND_STMT: return "COMPOUND_STMT";
        case NT_STMT_LIST: return "STMT_LIST";
        case NT_STMT: return "STMT";
        case NT_EXPRESSION: return "EXPRESSION";
        case NT_SIMPLE_EXPR: return "SIMPLE_EXPR";
        case NT_TERM: return "TERM";
        case NT_FACTOR: return "FACTOR";
        default: return "UNKNOWN";
    }
}

void print_ll1_table() {
    printf("\n📊 === TABLA LL(1) ===\n\n");
    
    int important_tokens[] = {
        PROGRAM, VAR, BEGIN_KW, END, IF, THEN, ELSE, WHILE, DO,
        READ, WRITE, INTEGER, BOOLEAN, IDENTIFIER, SEMICOLON,
        COLON, DOT, COMMA, LPAREN, RPAREN, ASSIGN
    };
    int num_tokens = sizeof(important_tokens) / sizeof(important_tokens[0]);
    
    printf("%-20s", "No Terminal");
    for (int i = 0; i < num_tokens; i++) {
        printf(" %-10s", token_name(important_tokens[i]));
    }
    printf("\n");
    
    for (int i = 0; i < 70; i++) printf("─");
    printf("\n");
    
    for (int nt = NT_PROGRAM; nt <= NT_STMT; nt++) {
        printf("%-20s", get_non_terminal_name(nt));
        for (int t = 0; t < num_tokens; t++) {
            int token = important_tokens[t];
            int production = ll1_table[nt][token];
            if (production == -1) {
                printf(" %-10s", "---");
            } else if (production == 0) {
                printf(" %-10s", "ε");
            } else {
                printf(" %-10d", production);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void parse_ll1() {
    printf("🔧 === Iniciando Análisis LL(1) Predictivo ===\n");
    init_ll1_table();
    print_ll1_table();
    
    printf("📝 Nota: Usando parser recursivo descendente con tabla LL(1) como referencia\n");
    parse_program();
}