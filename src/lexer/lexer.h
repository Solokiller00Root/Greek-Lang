#ifndef LEXER_H_
#define LEXER_H_

#include <stdlib.h>

typedef enum {
    TOKEN_OK = 0,
    TOKEN_ERROR
} Lexer_Results;

typedef enum {
    INTEGER,
    INTEGER_LITERAL,
    FLOAT,
    FLOAT_LITERAL,
    IDENTIFIER,
    KEYWORD,
    STRING,
    STRING_LITERAL,
    BOOL,
    BOOL_LITERAL,
    PLUS,
    IF,
    FOR,
    LESS_THAN_EQ,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    EQUALS,
    SEMICOLON,
    COLON,
    ASIGN,
    UNKNOWN,
    END
} Token_type;

const char *get_token_strings(Token_type type);

typedef struct {
    Token_type type;
    const char *text;
    int text_len;
} Token;

typedef struct {
    const char *content;
    size_t content_len;
    size_t pos;
    size_t line;
    size_t cursor;
    size_t token_count;
    char character;
} Lexer;

void lexer_init(Lexer *l, const char *content, size_t content_len);
Lexer_Results lexer_tokenize(const char *filename);
void print_tokens(const Token *tokens);
char lexer_next_char(Lexer *l);
char lexer_peek_char(const Lexer *l);
Token lexer_next_token(Lexer *l);

#endif // LEXER_H_

