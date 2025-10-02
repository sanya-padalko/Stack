#include "stack.h"

void fill_poizon(stack_t *stack, int left, int right) {
    for (int i = left; i < right; ++i) {
        stack->data[i] = POIZON_VALUE;
    }
}

stack_t* StackCtor(ssize_t capacity ON_DEBUG(, VarInfo varinfo)) {
    ON_DEBUG(PrintVarInfo(varinfo));

    stack_t *stack = (stack_t*)calloc(1, sizeof(stack_t));

    if (stack == NULL) {
        printerr(RED_COLOR "Calloc return NULL (during creating struct) in Stack's Constuctor\n" RESET_COLOR);
        return NULL;
    }

    if (capacity > MaxCapacity || capacity < 0) {
        printerr(RED_COLOR "Unavailable size of stack in Stack's Constructor\n" RESET_COLOR);
        return NULL;
    }

    stack->size = 0;
    stack->capacity = capacity;

    stack->data = (StackElem_t*)calloc(stack->capacity + 2, sizeof(StackElem_t));
    if (stack->data == NULL) {
        printerr(RED_COLOR "Calloc return NULL in Stack Constructor\n" RESET_COLOR);
        return NULL;
    }

    stack->data[0] = CANARY_LEFT;
    ++stack->data;
    stack->data[stack->capacity] = CANARY_RIGHT;

    fill_poizon(stack, 0, stack->capacity);

    stack->hash = calc_hash(stack);

    return stack;
}

StackErr_t StackDtor(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    free(stack->data);
    stack->data = NULL;
    free(stack);
    stack = NULL;

    return NOTHING;
}

static int StackRealloc(stack_t *stack, ssize_t new_size) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, BadSize);

    my_assert(new_size >= stack->size, SIZE_ERR, BadSize);

    --stack->data;
    stack->data = (StackElem_t*)realloc(stack->data, (new_size + 2) * sizeof(StackElem_t));

    my_assert(stack->data, REALLOC_ERR, BadSize);

    stack->data[0] = CANARY_LEFT;
    ++stack->data;

    fill_poizon(stack, stack->size, new_size);
    
    stack->capacity = new_size;
    stack->data[stack->capacity] = CANARY_RIGHT;

    stack->hash = calc_hash(stack);

    return stack->capacity;
}

StackErr_t StackPush(stack_t *stack, StackElem_t new_value) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    if (stack->size == stack->capacity) {
        int new_capacity = StackRealloc(stack, stack->size * ExpandMn);

        my_assert(new_capacity != BadSize, REALLOC_ERR, REALLOC_ERR);
    }

    *(stack->data + stack->size) = new_value;
    
    stack->hash = calc_hash(stack);

    ++stack->size;

    return NOTHING;
}

StackElem_t StackPop(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, POIZON_VALUE);

    if (stack->size == 0) {
        ON_DEBUG(stackdump(stack));
        ParseErr(EMPTY_STACK);
        printerr(RED_COLOR "You cannot pop from empty stack\n\n" RESET_COLOR);
        return POIZON_VALUE;
    }

    --stack->size;
    StackElem_t last = *(stack->data + stack->size);
    *(stack->data + stack->size) = POIZON_VALUE;

    stack->hash = calc_hash(stack);

    int new_capacity = stack->capacity;
    if (stack->size * CheckMn == stack->capacity)
        new_capacity = StackRealloc(stack, stack->capacity / NarrowMn);

    if (new_capacity == BadSize)
        return POIZON_VALUE;

    return last;
}