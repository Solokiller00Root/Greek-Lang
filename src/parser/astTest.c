#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"


typedef enum {
    NODE_VARIABLE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_INTEGER_LITERAL,
    NODE_STRING_LITERAL,
    NODE_BINARY_EXPRESSION,
    NODE_IDENTIFIER
} NodeType;

typedef struct AstNode{
    NodeType type;

    union {
        struct {
            Token_type var_type;
            char *name;
            struct AstNode *initializer;
        } variable_decl;

    } data;

}AstNode;


typedef struct{
    Lexer lexer;
    Token current_token;
    size_t index;
}Parser;

int parser_advance(Parser *p){
    if(p->index >= p->lexer.token_count){
        return 1;
    }



    p->index++;
    return 0;
}   


AstNode *new_variable(char *name, Token_type type) {
    AstNode *node = malloc(sizeof(AstNode));

    if (!node)
        return NULL;

    node->type = NODE_VARIABLE_DECLARATION;

    node->data.variable_decl.name = name;
    node->data.variable_decl.var_type = type;
    node->data.variable_decl.initializer = NULL;

    return node;
}



int main(int argv, char **argc) {
   
    
    AstNode *node = new_variable("hello", STRING);

    printf("%s %d\n", node->data.variable_decl.name, node->data.variable_decl.var_type);

    return 0;
}
