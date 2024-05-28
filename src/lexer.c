#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdbool.h>
#include "lexer.h"

const char *keywords[] = {
    "ἀκέραιος",
};


bool isGreekCharacter(unsigned char c) {
    return (c >= 0x0370 && c <= 0x03FF);
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
    while (isspace(l->character) && l->character != EOF) {
        lexer_next_char(l);
    }
}


Token *generate_int(Lexer *l) {
    Token *token = malloc(sizeof(Token));
    token->text_len = malloc(sizeof(int));
    token->text_len = l->line; 
    token->type = INTEGER;
    char *value = malloc(sizeof(char) * 8);
    int value_index = 0;
    while (isGreekCharacter(l->character) && l->character != '\0') {
        value[value_index++] = l->character;
        lexer_next_char(l);
    }
    value[value_index] = '\0';
    token->text = value;
    return token;
}




Token lexer_next_token(Lexer *l) {
    skip_whitespace(l);
    
    Token token;
   
    if (isdigit(l->character)) {
        token.type = INTEGER;
        token.text = &l->content[l->pos - 1];
        token.text_len = 1;
        while (isdigit(lexer_peek_char(l))) {
            lexer_next_char(l);
            token.text_len++;
        }
        lexer_next_char(l);  
    } else if (isalpha(l->character) || isGreekCharacter(l->character)) {
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
            case EOF:
                token.type = END;
                token.text = "EOF";
                token.text_len = 3;
                break;
                
          default:
              printf("Unknown character: %c\n", l->character);
              token.type = UNKNOWN;
              token.text = &l->content[l->pos - 1];
              token.text_len = 1;
              lexer_next_char(l);
              break;
        }
    }
    
    if (token.type == IDENTIFIER && strncmp(token.text, "ακ", token.text_len) == 0) {
        return *generate_int(l);
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
        case EQUALS: return "EQUALS";
        case SEMICOLON: return "SEMICOLON";
        case UNKNOWN: return "UNKNOWN";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

