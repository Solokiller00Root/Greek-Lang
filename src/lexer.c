#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "lexer.h"

const Token keywords[] = {
    {INTEGER, "ἀκέραιος", 17},
    {STRING, "χαρακτήρ", 16},
    {BOOL, "ἀπόλυτος", 17},
    {IF, "ἐάν", 7},
    {FOR, "ἕνεκα", 11}
};

bool isGreekCharacter(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc >= 0x370 && uc <= 0x3FF);
}

void lexer_init(Lexer *l, const char *content, size_t content_len) {
    l->content = content;
    l->content_len = content_len;
    l->pos = 0;
    l->line = 1;
    l->cursor = 0;
    l->character = 0;

    lexer_next_char(l); 
}

char lexer_next_char(Lexer *l) {
    if (l->pos >= l->content_len) {
        l->character = EOF;
    } else {
        l->character = l->content[l->pos++];
        l->cursor++;
    }
    return l->character;
}

char lexer_peek_char(const Lexer *l) {
    if (l->pos >= l->content_len) {
        return EOF;
    }
    return l->content[l->pos];
}

void skip_whitespace(Lexer *l) {
    while (isspace((unsigned char)l->character) && l->character != EOF) {
        lexer_next_char(l);
    }
}

Token lexer_next_token(Lexer *l) {
    skip_whitespace(l);
    
    Token token;
    token.text = NULL;
    token.text_len = 0;
    
        for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        if (strncmp(&l->content[l->pos - 1], keywords[i].text, keywords[i].text_len) == 0) {
            token = keywords[i];
            l->pos += keywords[i].text_len - 1;
            lexer_next_char(l); 

            return token;
        }
    }
    
    if (isdigit((unsigned char)l->character)) {
        token.type = INTEGER;
        token.text = &l->content[l->pos - 1];
        token.text_len = 1;
        while (isdigit((unsigned char)lexer_peek_char(l))) {
            lexer_next_char(l);
            token.text_len++;
        }
        lexer_next_char(l);
    } else if (isalpha((unsigned char)l->character) || isGreekCharacter(l->character)) {
        token.type = IDENTIFIER;
        token.text = &l->content[l->pos - 1];
        token.text_len = 1;
        while (isalnum(lexer_peek_char(l)) || isGreekCharacter(lexer_peek_char(l))) {
            lexer_next_char(l);
            token.text_len++;
        }
        lexer_next_char(l);
    } else {
        switch (l->character) {
            case '=':
                token.type = EQUALS;
                token.text = "=";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case ';':
                token.type = SEMICOLON;
                token.text = ";";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case ':':
                token.type = VAR_TYPE;
                token.text = ":";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case '+':
                token.type = PLUS;
                token.text = "+";
                token.text_len = 1;
                lexer_next_char(l);
                break;
             case '<':
                token.type = LESS_THAN_EQ;
                token.text = "<";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case '(':
                token.type = LPAREN;
                token.text = "(";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case ')':
                token.type = RPAREN;
                token.text = ")";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case '{':
                token.type = LBRACE;
                token.text = "{";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case '}':
                token.type = RBRACE;
                token.text = "}";
                token.text_len = 1;
                lexer_next_char(l);
                break;
            case EOF:
                token.type = END;
                token.text = "EOF";
                token.text_len = 3;
                break;
            default:
                token.type = UNKNOWN;
                token.text = &l->content[l->pos - 1];
                token.text_len = 1;
                lexer_next_char(l);
                break;
        }
    }
    return token;
}

Lexer_Results lexer_tokenize(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return TOKEN_ERROR;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (!content) {
        fclose(file);
        return TOKEN_ERROR;
    }

    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);

    Lexer lexer;
    lexer_init(&lexer, content, length);

    Token token;
    do {
        token = lexer_next_token(&lexer);
        printf("Token: %s, Text: %.*s\n",
               get_token_strings(token.type),
               (int)token.text_len,
               token.text);
    } while (token.type != END);

    free(content);
    return TOKEN_OK;
}

void print_tokens(const Token *tokens) {
    for (size_t i = 0; tokens[i].type != END; i++) {
        printf("Token: %s, Text: %.*s\n",
               get_token_strings(tokens[i].type),
               (int)tokens[i].text_len,
               tokens[i].text);
    }
}

const char *get_token_strings(Token_type type) {
    switch (type) {
        case INTEGER: return "INTEGER";
        case IDENTIFIER: return "IDENTIFIER";
        case VAR_TYPE: return "VAR_TYPE";
        case KEYWORD: return "KEYWORD";
        case STRING: return "STRING";
        case BOOL: return "BOOL";
        case IF: return "IF";
        case FOR: return "FOR";
        case EQUALS: return "EQUALS";
        case PLUS: return "PLUS";
        case LESS_THAN_EQ: return "LESS_THAN_EQ";
        
        case SEMICOLON: return "SEMICOLON";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case UNKNOWN: return "UNKNOWN";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

