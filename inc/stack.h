#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include "errors.h"
#include "verify.h"

#ifdef DEBUG
#define make_stack(name, capacity) \
        stack_t* name = StackCtor(capacity, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__));
#define ON_DEBUG(...) __VA_ARGS__
#else
#define make_stack(name, capacity) \
        stack_t* name = StackCtor(capacity);
#define ON_DEBUG(...)
#endif

const ssize_t MaxCapacity = 1000000;

const int ExpandMn =  2;

const int CheckMn  =  4;

const int NarrowMn =  3;

const int BadSize  = -1;

stack_t* StackCtor(ssize_t capacity ON_DEBUG(, VarInfo varinfo));

StackErr_t StackDtor(stack_t *stack);

StackErr_t StackPush(stack_t *stack, StackElem_t new_value);

StackElem_t StackPop(stack_t *stack);

#endif