#include <stdio.h>
#include "lexer.h"

int main() {
    Lexer_Results result = lexer_tokenize("test.gr");
    if (result == TOKEN_ERROR) {
        printf("Failed to tokenize the file.\n");
        return 1;
    }
    return 0;
}

