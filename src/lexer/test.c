#include <stdio.h>

const char* keywords[] = {
    "kappa",
    "kippo"
};




int main(void){


    for(int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); ++i){
    
    printf("%s\n", keywords[i]); 

    }

    return 0;
}
