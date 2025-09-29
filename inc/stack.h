#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include "errors.h"
#include "vars.h"
#include "verify.h"

#ifdef DEBUG
#define make_stack(name, capacity) \
        stack_t* name = (stack_t*)calloc(1, sizeof(stack_t)); \
        if (code_error == NULL) \
            code_error = (StackErr_t*)calloc(1, sizeof(StackErr_t)); \
        *code_error = StackCtor(name, capacity + 2, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__));
#define ON_DEBUG(...) __VA_ARGS__
#else
#define make_stack(name, capacity) \
        stack_t* name = (stack_t*)calloc(1, sizeof(stack_t)); \
        if (code_error == NULL) \
            code_error = (StackErr_t*)calloc(1, sizeof(StackErr_t)); \
        *code_error = StackCtor(name, capacity + 2);
#define ON_DEBUG(...)
#endif

#define delete_stack(stack) StackDtor(stack, code_error);

#define get_name(x) #x

#define base(type) *((type*)calloc(1, sizeof(type)))

StackErr_t StackCtor(stack_t *stack, ssize_t capacity ON_DEBUG(, VarInfo varinfo));

StackErr_t StackDtor(stack_t *stack, StackErr_t *code_error);

void StackPush(stack_t *stack, StackElem_t x, StackErr_t *code_error);

StackElem_t StackPop(stack_t *stack, StackErr_t *code_error);

#endif