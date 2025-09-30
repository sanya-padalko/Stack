#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include "errors.h"
#include "verify.h"

#ifdef DEBUG
#define make_stack(name, capacity) \
        if (code_error == NULL) \
            code_error = (StackErr_t*)calloc(1, sizeof(StackErr_t)); \
        stack_t* name = StackCtor(capacity + 2, code_error, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__));
#define ON_DEBUG(...) __VA_ARGS__
#else
#define make_stack(name, capacity) \
        if (code_error == NULL) \
            code_error = (StackErr_t*)calloc(1, sizeof(StackErr_t)); \
        stack_t* name = StackCtor(capacity + 2, code_error);
#define ON_DEBUG(...)
#endif

stack_t* StackCtor(ssize_t capacity, StackErr_t *code_erro ON_DEBUG(, VarInfo varinfo));

StackErr_t StackDtor(stack_t *stack, StackErr_t *code_error);

void StackPush(stack_t *stack, StackElem_t x, StackErr_t *code_error);

StackElem_t StackPop(stack_t *stack, StackErr_t *code_error);

#endif