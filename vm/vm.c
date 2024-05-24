#include <stdio.h>

#define STACK_CAPACITY 1024

typedef enum{
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_ILLEGAL_INST
}Err;

typedef struct{
    size_t stack[STACK_CAPACITY];
    int size_s;
}Stack;

typedef enum {
    INST_PUSH,
    INST_POP,
    INST_PLUS 
} Inst_type;


typedef struct {
    Inst_type type;
    int operand;
} Inst;


Err execute_vm(Stack *stack, Inst inst) {
    switch (inst.type) {
        case INST_PUSH:
            if(stack->size_s >= STACK_CAPACITY){
                return ERR_STACK_OVERFLOW;
            }
            stack->stack[stack->size_s++] = inst.operand;
            printf("pushed: %d\n", inst.operand); 
            break;
        case INST_POP:
            if(stack->size_s == 0){
                return ERR_STACK_UNDERFLOW;
            }
            stack->stack[stack->size_s--];
            break;
        case INST_PLUS:
            if(stack->size_s < 2){
                return ERR_STACK_OVERFLOW;
            }
            stack->stack[stack->size_s - 2] += stack->stack[stack->size_s - 1];
            stack->size_s--;
            break;
        default:
            return ERR_ILLEGAL_INST;
            break;
    }
    return ERR_OK;
}

void print_stack(Stack *stack){
    for(long i = 0; i < stack->size_s; i++){
        printf("%ld) %ld\n", i, stack->stack[i]);
    }
}

int main(void) {

    Stack stack = {0};
    Inst first = {INST_PUSH, 5};
    execute_vm(&stack, first);
    Inst push = {INST_PUSH, 10};
    execute_vm(&stack, push);
    Inst pop = {INST_POP, 0};
    execute_vm(&stack, pop);
    print_stack(&stack);
    return 0;
}

