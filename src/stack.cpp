#include "stack.h"

StackErr_t StackCtor(stack_t *stack, ssize_t capacity ON_DEBUG(, VarInfo varinfo)) {
    ON_DEBUG(PrintVarInfo(varinfo));

    if (stack == NULL) {
        printerr(RED_COLOR "Pointer went equal NULL in Stack's Constuctor\n" RESET_COLOR);
        return NULLPTR;
    }

    if (capacity > 10000000 || capacity < 2) {
        printerr(RED_COLOR "Unavailable size of stack in Stack's Constructor\n" RESET_COLOR);
        return SIZE_ERR;
    }

    stack->hash = 0;

    stack->size = 2;

    stack->capacity = capacity;

    void* calloc_ptr = calloc(stack->capacity, sizeof(StackElem_t));
    if (calloc_ptr == NULL) {
        printerr(RED_COLOR "Calloc return NULL in Stack Constructor\n" RESET_COLOR);
        return NULLPTR;
    }

    stack->data = (StackElem_t*)calloc_ptr;

    stack->data[0] = CANARY;

    stack->data[stack->capacity - 1] = CANADA;

    for (size_t i = 1; i < stack->capacity - 1; ++i) {
        stack->data[i] = POIZON;
        stack->hash = (stack->hash + i * POIZON % mod) % mod;
    }

    return NOTHING;
}

StackErr_t StackDtor(stack_t *stack, StackErr_t *code_error) {
    stackverify(stack, *code_error);

    free(stack->data); // check free arguments
    stack->data = NULL;
    free(stack);
    stack = NULL;

    return NOTHING;
}

static int expand(stack_t *stack, StackErr_t *code_error) {
    stackverify(stack, -1);

    void* realloc_ptr = realloc(stack->data, stack->capacity * 2 * sizeof(StackElem_t));
    if (realloc_ptr == NULL) {
        *code_error = REALLOC_ERR;
        ON_DEBUG(stackdump(stack));
        ParseErr(*code_error);
        return -1;
    }

    stack->capacity *= 2;

    stack->data = (StackElem_t*)realloc_ptr;

    stack->data[stack->capacity - 1] = CANADA;

    for (int i = stack->capacity / 2 - 1; i < stack->capacity - 1; ++i) {
        stack->data[i] = POIZON;
        stack->hash = (stack->hash + i * POIZON % mod) % mod;
    }

    return stack->capacity;
}

static int narrow(stack_t *stack, StackErr_t *code_error) {
    stackverify(stack, -1);

    stack->data[stack->capacity - 1] = POIZON;

    for (int i = stack->size - 1; i < stack->capacity - 1; ++i) {
        stack->hash = (stack->hash + mod - i * POIZON % mod) % mod;
    }

    stack->capacity = stack->size;

    stack->data = (StackElem_t*)realloc(stack->data, stack->capacity * sizeof(StackElem_t));

    stack->data[stack->capacity - 1] = CANADA;
    
    return stack->capacity;
}

void StackPush(stack_t *stack, StackElem_t x, StackErr_t *code_error) {
    stackverify(stack, );

    if (stack->size == stack->capacity) {
        expand(stack, code_error);

        if (*code_error != NOTHING) {
            printerr(RED_COLOR "Push was not completed\n" RESET_COLOR);
            return;
        }
    }

    *(stack->data + stack->size - 1) = x;
    stack->hash = (stack->hash + mod - (stack->size - 1) * POIZON % mod) % mod;
    stack->hash = (stack->hash + (stack->size - 1) * x) % mod;

    ++stack->size;

    return;
}

StackElem_t StackPop(stack_t *stack, StackErr_t *code_error) {
    stackverify(stack, POIZON);

    if (stack->size == 2) {
        *code_error = EMPTY_STACK;
        ON_DEBUG(stackdump(stack));
        ParseErr(*code_error);
        printerr(RED_COLOR "You cannot pop from empty stack\n\n" RESET_COLOR);
        return POIZON;
    }

    --stack->size;
    StackElem_t last = *(stack->data + stack->size - 1);
    *(stack->data + stack->size - 1) = POIZON;
    stack->hash = (stack->hash + mod - (stack->size - 1) * last % mod) % mod;
    stack->hash = (stack->hash + (stack->size - 1) * POIZON % mod) % mod;

    if (stack->size * 4 == stack->capacity)
        narrow(stack, code_error);

    if (*code_error != NOTHING)
        return POIZON;

    return last;
}