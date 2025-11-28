#ifndef TOKENS_H
#define TOKENS_H

/* Palabras reservadas */
#define PROGRAM     256
#define VAR         257
#define BEGIN_KW    258
#define END         259
#define IF          260
#define THEN        261
#define ELSE        262
#define WHILE       263
#define DO          264
#define READ        265
#define WRITE       266
#define INTEGER     267
#define BOOLEAN     268
#define TRUE        269
#define FALSE       270
#define AND         271
#define OR          272
#define NOT         273

/* Identificadores y números */
#define IDENTIFIER  274
#define NUMBER      275

/* Operadores y símbolos */
#define ASSIGN      276
#define SEMICOLON   277
#define COMMA       278
#define COLON       279
#define DOT         280
#define LPAREN      281
#define RPAREN      282
#define PLUS        283
#define MINUS       284
#define MULTIPLY    285
#define DIVIDE      286
#define LT          287
#define LE          288
#define GT          289
#define GE          290
#define EQ          291
#define NE          292

/* Estructura para valores semánticos */
typedef union {
    int num;
    char* str;
} YYSTYPE;

/* Declaraciones externas - se definen en tokens.c */
extern YYSTYPE yylval;

#endif