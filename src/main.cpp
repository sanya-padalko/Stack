#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errors.h"
#include "stack.h"
#include "vars.h"
#include "verify.h"

int main() {

    srand(time(NULL));

    StackErr_t* code_error = (StackErr_t*)calloc(1, sizeof(StackErr_t));
    *code_error = NOTHING;
    
    make_stack(stack, 2);

    for (int i = 0; i < 400; ++i) {
        if (i % 5 == 3) {
            StackPop(stack, code_error);
            continue;
        }
        StackPush(stack, rand() % (unsigned int)rand(), code_error);
    }

    for (int i = 0; i < 40; ++i) {
        StackPop(stack, code_error);
    }

    stackverify(stack, 0);

    stackdump(stack);

    StackDtor(stack, code_error);
}