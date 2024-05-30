#ifndef PARSER_H_
#define PARSER_H_

#include "../lexer/lexer.h"
#include <stdlib.h>

typedef enum {
    NODE_INTEGER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_FOR,
    NODE_BLOCK,
    NODE_PROGRAM
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            int value;
        } integer;

        struct {
            char *name;
        } identifier;

        struct {
            char op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;

        struct {
            struct ASTNode *left;
            struct ASTNode *right;
        } assignment;

        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;

        struct {
            struct ASTNode *init;
            struct ASTNode *condition;
            struct ASTNode *increment;
            struct ASTNode *body;
        } for_stmt;

        struct {
            struct ASTNode **statements;
            size_t statement_count;
        } block;
    };
} ASTNode;

typedef struct {
    Lexer *lexer;
    Token current_token;
} Parser;

ASTNode *parse_program(Parser *parser);

#endif // PARSER_H_

