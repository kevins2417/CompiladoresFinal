#include "parser.h"
#include "tokens.h"

/* Variables globales del parser */
int current_token;
int error_count = 0;
int max_errors = 20;

void init_parser(FILE* input) {
    yyin = input;
    current_token = yylex();
    error_count = 0;
}

void recover_from_error() {
    fprintf(stderr, "   ↳ Intentando recuperación...\n");
    
    int sync_tokens[] = {SEMICOLON, END, ELSE, THEN, DO, BEGIN_KW, DOT, EOF};
    int num_sync = sizeof(sync_tokens) / sizeof(sync_tokens[0]);
    
    int recovery_found = 0;
    while (current_token != EOF && !recovery_found && error_count < max_errors) {
        for (int i = 0; i < num_sync; i++) {
            if (current_token == sync_tokens[i]) {
                recovery_found = 1;
                fprintf(stderr, "   ↳ Punto de recuperación: '%s'\n", token_name(current_token));
                break;
            }
        }
        
        if (!recovery_found) {
            fprintf(stderr, "   ↳ Descartando token: '%s'\n", token_name(current_token));
            current_token = yylex();
        }
    }
    
    if (current_token == EOF) {
        fprintf(stderr, "   ↳ Fin de archivo alcanzado durante recuperación.\n");
    }
}

void parser_error(const char* expected) {
    fprintf(stderr, "❌ Error sintáctico en línea %d, columna %d:\n", 
            line_num, col_num);
    fprintf(stderr, "   Esperaba: %s\n", expected);
    fprintf(stderr, "   Encontró: '%s' (%s)\n", 
            yytext, token_name(current_token));
    
    error_count++;
    
    if (error_count >= max_errors) {
        fprintf(stderr, "\n🛑 Demasiados errores (%d). Deteniendo análisis.\n", error_count);
        exit(1);
    }
    
    recover_from_error();
}

void match(int expected_token) {
    if (current_token == expected_token) {
        current_token = yylex();
    } else {
        char expected_str[100];
        sprintf(expected_str, "'%s'", token_name(expected_token));
        parser_error(expected_str);
    }
}

/* program -> PROGRAM IDENTIFIER SEMICOLON declarations compound_stmt DOT */
void parse_program() {
    printf("🏁 Iniciando análisis del programa...\n");
    
    if (current_token == PROGRAM) {
        match(PROGRAM);
    } else {
        parser_error("PROGRAM");
    }
    
    if (current_token == IDENTIFIER) {
        printf("   📝 Programa: %s\n", yytext);
        match(IDENTIFIER);
    } else {
        parser_error("IDENTIFIER");
    }
    
    if (current_token == SEMICOLON) {
        match(SEMICOLON);
    } else {
        parser_error(";");
    }
    
    declarations();
    compound_stmt();
    
    if (current_token == DOT) {
        match(DOT);
        printf("   ✅ Punto final encontrado\n");
    } else {
        parser_error(".");
    }
    
    if (current_token != EOF && error_count == 0) {
        fprintf(stderr, "⚠️  Advertencia: Tokens adicionales después del programa\n");
    }
}

/* declarations -> VAR declaration_list | epsilon */
void declarations() {
    if (current_token == VAR) {
        printf("   📋 Procesando declaraciones VAR\n");
        match(VAR);
        declaration_list();
    } else {
        printf("   📋 Sin declaraciones VAR\n");
    }
}

/* declaration_list -> declaration declaration_list' */
void declaration_list() {
    while (current_token == IDENTIFIER && error_count < max_errors) {
        declaration();
    }
}

/* declaration -> id_list COLON type SEMICOLON */
void declaration() {
    printf("     ├── Declaración\n");
    
    id_list();
    
    if (current_token == COLON) {
        match(COLON);
    } else {
        parser_error(":");
    }
    
    type();
    
    if (current_token == SEMICOLON) {
        match(SEMICOLON);
    } else {
        parser_error(";");
        recover_from_error();
    }
}

/* id_list -> IDENTIFIER id_list' */
void id_list() {
    if (current_token == IDENTIFIER) {
        printf("     │   ├── ID: %s\n", yytext);
        match(IDENTIFIER);
        
        while (current_token == COMMA) {
            match(COMMA);
            if (current_token == IDENTIFIER) {
                printf("     │   ├── ID: %s\n", yytext);
                match(IDENTIFIER);
            } else {
                parser_error("IDENTIFIER después de coma");
                break;
            }
        }
    } else {
        parser_error("IDENTIFIER");
    }
}

/* type -> INTEGER | BOOLEAN */
void type() {
    if (current_token == INTEGER) {
        printf("     │   └── Tipo: INTEGER\n");
        match(INTEGER);
    } else if (current_token == BOOLEAN) {
        printf("     │   └── Tipo: BOOLEAN\n");
        match(BOOLEAN);
    } else {
        parser_error("INTEGER o BOOLEAN");
        if (current_token != SEMICOLON && current_token != BEGIN_KW) {
            current_token = yylex();
        }
    }
}

/* compound_stmt -> BEGIN stmt_list END */
void compound_stmt() {
    printf("   🔨 Procesando bloque BEGIN-END\n");
    
    if (current_token == BEGIN_KW) {
        match(BEGIN_KW);
    } else {
        parser_error("BEGIN");
        return;
    }
    
    stmt_list();
    
    if (current_token == END) {
        match(END);
        printf("   ✅ Bloque END correcto\n");
    } else {
        parser_error("END");
        recover_from_error();
    }
}

/* stmt_list -> stmt stmt_list' */
void stmt_list() {
    while (current_token != END && current_token != EOF && 
           current_token != ELSE && error_count < max_errors) {
        
        if (current_token == IDENTIFIER || current_token == BEGIN_KW ||
            current_token == IF || current_token == WHILE ||
            current_token == READ || current_token == WRITE) {
            stmt();
            
            if (current_token == SEMICOLON) {
                match(SEMICOLON);
            }
        } else if (current_token != END && current_token != ELSE) {
            parser_error("inicio de sentencia");
            recover_from_error();
        }
    }
}

/* stmt -> assignment | compound | if | while | read | write */
void stmt() {
    printf("     ├── Sentencia: ");
    
    switch (current_token) {
        case IDENTIFIER:
            printf("Asignación\n");
            assignment_stmt();
            break;
        case BEGIN_KW:
            printf("Bloque compuesto\n");
            compound_stmt();
            break;
        case IF:
            printf("IF\n");
            if_stmt();
            break;
        case WHILE:
            printf("WHILE\n");
            while_stmt();
            break;
        case READ:
            printf("READ\n");
            read_stmt();
            break;
        case WRITE:
            printf("WRITE\n");
            write_stmt();
            break;
        default:
            parser_error("IDENTIFIER, BEGIN, IF, WHILE, READ o WRITE");
            return;
    }
}

/* assignment_stmt -> IDENTIFIER ASSIGN expression */
void assignment_stmt() {
    printf("     │   ├── Asignar a: %s\n", yytext);
    match(IDENTIFIER);
    
    if (current_token == ASSIGN) {
        match(ASSIGN);
    } else {
        parser_error(":=");
        return;
    }
    
    expression();
}

/* if_stmt -> IF expression THEN stmt [ELSE stmt] */
void if_stmt() {
    match(IF);
    expression();
    
    if (current_token == THEN) {
        match(THEN);
    } else {
        parser_error("THEN");
        recover_from_error();
    }
    
    stmt();
    
    if (current_token == ELSE) {
        match(ELSE);
        stmt();
    }
}

/* while_stmt -> WHILE expression DO stmt */
void while_stmt() {
    match(WHILE);
    expression();
    
    if (current_token == DO) {
        match(DO);
    } else {
        parser_error("DO");
        recover_from_error();
    }
    
    stmt();
}

/* read_stmt -> READ LPAREN id_list RPAREN */
void read_stmt() {
    match(READ);
    
    if (current_token == LPAREN) {
        match(LPAREN);
    } else {
        parser_error("(");
        return;
    }
    
    id_list();
    
    if (current_token == RPAREN) {
        match(RPAREN);
    } else {
        parser_error(")");
    }
}

/* write_stmt -> WRITE LPAREN expression RPAREN */
void write_stmt() {
    match(WRITE);
    
    if (current_token == LPAREN) {
        match(LPAREN);
    } else {
        parser_error("(");
        return;
    }
    
    expression();
    
    if (current_token == RPAREN) {
        match(RPAREN);
    } else {
        parser_error(")");
    }
}

/* expression -> simple_expr [relop simple_expr] */
void expression() {
    simple_expr();
    
    if (current_token >= LT && current_token <= NE) {
        printf("     │   │   └── Operador relacional: %s\n", token_name(current_token));
        relop();
        simple_expr();
    }
}

/* simple_expr -> term {addop term} */
void simple_expr() {
    term();
    
    while ((current_token == PLUS || current_token == MINUS || 
            current_token == OR) && error_count < max_errors) {
        printf("     │   │   └── Operador aditivo: %s\n", token_name(current_token));
        addop();
        term();
    }
}

/* term -> factor {mulop factor} */
void term() {
    factor();
    
    while ((current_token == MULTIPLY || current_token == DIVIDE || 
            current_token == AND) && error_count < max_errors) {
        printf("     │   │   └── Operador multiplicativo: %s\n", token_name(current_token));
        mulop();
        factor();
    }
}

/* factor -> IDENTIFIER | NUMBER | TRUE | FALSE | 
            LPAREN expression RPAREN | NOT factor */
void factor() {
    switch (current_token) {
        case IDENTIFIER:
            printf("     │   │   │   ├── Factor ID: %s\n", yytext);
            match(IDENTIFIER);
            break;
        case NUMBER:
            printf("     │   │   │   ├── Factor NUM: %s\n", yytext);
            match(NUMBER);
            break;
        case TRUE:
            printf("     │   │   │   ├── Factor: TRUE\n");
            match(TRUE);
            break;
        case FALSE:
            printf("     │   │   │   ├── Factor: FALSE\n");
            match(FALSE);
            break;
        case LPAREN:
            printf("     │   │   │   ├── Factor: ( expresión )\n");
            match(LPAREN);
            expression();
            if (current_token == RPAREN) {
                match(RPAREN);
            } else {
                parser_error(")");
            }
            break;
        case NOT:
            printf("     │   │   │   ├── Factor: NOT\n");
            match(NOT);
            factor();
            break;
        default:
            parser_error("IDENTIFIER, NUMBER, TRUE, FALSE, '(' o NOT");
            if (current_token != SEMICOLON && current_token != END && 
                current_token != THEN && current_token != DO && 
                current_token != RPAREN) {
                current_token = yylex();
            }
    }
}

void relop() {
    if (current_token >= LT && current_token <= NE) {
        match(current_token);
    } else {
        parser_error("operador relacional");
    }
}

void addop() {
    if (current_token == PLUS || current_token == MINUS || current_token == OR) {
        match(current_token);
    } else {
        parser_error("operador aditivo");
    }
}

void mulop() {
    if (current_token == MULTIPLY || current_token == DIVIDE || current_token == AND) {
        match(current_token);
    } else {
        parser_error("operador multiplicativo");
    }
}

int get_error_count() {
    return error_count;
}

const char* token_name(int token) {
    switch (token) {
        case PROGRAM: return "PROGRAM";
        case VAR: return "VAR";
        case BEGIN_KW: return "BEGIN";
        case END: return "END";
        case IF: return "IF";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case DO: return "DO";
        case READ: return "READ";
        case WRITE: return "WRITE";
        case INTEGER: return "INTEGER";
        case BOOLEAN: return "BOOLEAN";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case IDENTIFIER: return "IDENTIFIER";
        case NUMBER: return "NUMBER";
        case ASSIGN: return ":=";
        case SEMICOLON: return ";";
        case COLON: return ":";
        case DOT: return ".";
        case COMMA: return ",";
        case LPAREN: return "(";
        case RPAREN: return ")";
        case PLUS: return "+";
        case MINUS: return "-";
        case MULTIPLY: return "*";
        case DIVIDE: return "/";
        case LT: return "<";
        case LE: return "<=";
        case GT: return ">";
        case GE: return ">=";
        case EQ: return "=";
        case NE: return "<>";
        case EOF: return "EOF";
        default: return "token desconocido";
    }
}