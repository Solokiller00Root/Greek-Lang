#include <stdio.h>
#include <stdlib.h>
#include "./lexer/lexer.h"

int main(int argv, char **argc) {
    if(argv < 2){
        printf("Usage: ./main <file>");
        exit(1);
    }
    Lexer_Results result = lexer_tokenize(argc[1]);
    if (result == TOKEN_ERROR) {
        printf("Failed to tokenize the file.\n");
        return 1;
    }
    return 0;
}

