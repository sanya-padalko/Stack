#include "stack.h"

stack_t* StackCtor(ssize_t capacity, StackErr_t *code_error ON_DEBUG(, VarInfo varinfo)) {
    ON_DEBUG(PrintVarInfo(varinfo));

    stack_t *stack = (stack_t*)calloc(1, sizeof(stack_t));

    if (stack == NULL) {
        *code_error = NULLPTR;
        printerr(RED_COLOR "Calloc return NULL (during creating struct) in Stack's Constuctor\n" RESET_COLOR);
        return NULL;
    }

    if (capacity > 10000000 || capacity < 2) {
        *code_error = SIZE_ERR;
        printerr(RED_COLOR "Unavailable size of stack in Stack's Constructor\n" RESET_COLOR);
        return NULL;
    }

    stack->hash = 0;

    stack->size = 2;

    stack->capacity = capacity;

    void* calloc_ptr = calloc(stack->capacity, sizeof(StackElem_t));
    if (calloc_ptr == NULL) {
        *code_error = NULLPTR;
        printerr(RED_COLOR "Calloc return NULL in Stack Constructor\n" RESET_COLOR);
        return NULL;
    }

    stack->data = (StackElem_t*)calloc_ptr;

    stack->data[0] = CANARY_LEFT;

    stack->data[stack->capacity - 1] = CANARY_RIGHT;

    for (size_t i = 1; i < stack->capacity - 1; ++i) {
        stack->data[i] = POIZON_VALUE;
        stack->hash = (stack->hash + i * POIZON_VALUE % mod) % mod;
    }

    return stack;
}

StackErr_t StackDtor(stack_t *stack, StackErr_t *code_error) {
    stackverify(stack, *code_error);

    free(stack->data); // check free arguments
    stack->data = NULL;
    free(stack);
    stack = NULL;

    return NOTHING;
}

static int StackRealloc(stack_t *stack, ssize_t new_size, StackErr_t *code_error) {
    stackverify(stack, -1);

    if (new_size < stack->size) {
        printerr(RED_COLOR "Unavailable realloc-size of stack\n" RESET_COLOR);
        return -1;
    }

    void* realloc_ptr = realloc(stack->data, new_size * sizeof(StackElem_t));
    if (realloc_ptr == NULL) {
        *code_error = REALLOC_ERR;
        ON_DEBUG(stackdump(stack));
        ParseErr(*code_error);
        return -1;
    }

    stack->data = (StackElem_t*)realloc_ptr;

    stack->data[stack->capacity - 1] = POIZON_VALUE;
    stack->hash = (stack->hash + (stack->capacity - 1) * POIZON_VALUE % mod) % mod;

    while (stack->capacity < new_size) {
        ++stack->capacity;
        stack->hash = (stack->hash + (stack->capacity - 1) * POIZON_VALUE) % mod;
        stack->data[stack->capacity - 1] = POIZON_VALUE;
    }

    while (stack->capacity > new_size) {
        --stack->capacity;
        stack->hash = (stack->hash + mod - stack->capacity * POIZON_VALUE % mod) % mod;
    }

    stack->data[stack->capacity - 1] = CANARY_RIGHT;
    stack->hash = (stack->hash + mod - (stack->capacity - 1) * POIZON_VALUE % mod) % mod;

    return stack->capacity;
}

void StackPush(stack_t *stack, StackElem_t x, StackErr_t *code_error) {
    stackverify(stack, );

    if (stack->size == stack->capacity) {
        StackRealloc(stack, stack->size * 2, code_error);

        if (*code_error != NOTHING) {
            printerr(RED_COLOR "Push was not completed\n" RESET_COLOR);
            return;
        }
    }

    *(stack->data + stack->size - 1) = x;
    stack->hash = (stack->hash + mod - (stack->size - 1) * POIZON_VALUE % mod) % mod;
    stack->hash = (stack->hash + (stack->size - 1) * x) % mod;

    ++stack->size;

    return;
}

StackElem_t StackPop(stack_t *stack, StackErr_t *code_error) {
    stackverify(stack, POIZON_VALUE);

    if (stack->size == 2) {
        *code_error = EMPTY_STACK;
        ON_DEBUG(stackdump(stack));
        ParseErr(*code_error);
        printerr(RED_COLOR "You cannot pop from empty stack\n\n" RESET_COLOR);
        return POIZON_VALUE;
    }

    --stack->size;
    StackElem_t last = *(stack->data + stack->size - 1);
    *(stack->data + stack->size - 1) = POIZON_VALUE;
    stack->hash = (stack->hash + mod - (stack->size - 1) * last % mod) % mod;
    stack->hash = (stack->hash + (stack->size - 1) * POIZON_VALUE % mod) % mod;

    if (stack->size * 4 == stack->capacity)
        StackRealloc(stack, stack->size, code_error);

    if (*code_error != NOTHING)
        return POIZON_VALUE;

    return last;
}