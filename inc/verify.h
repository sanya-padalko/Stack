#ifndef _VERIFY_H_
#define _VERIFY_H_

#include <stdio.h>
#include <ctype.h>
#include "vars.h"
#include "errors.h"

typedef int StackElem_t;

struct stack_t {
    StackElem_t* data = NULL;

    long hash = 0;

    ssize_t size      = 0;
    ssize_t capacity  = 0;
};

const int POIZON = 146988;
const int CANARY = 52954; // CEDA
const int CANADA = 65242; // FEDA

const long mod = 998244353;

#define stackdump(name) StackDump(name, code_error, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__));

#define stackverify(stack, ret_value)   if (code_error == NULL) \
                                            code_error = (StackErr_t*)calloc(1, sizeof(StackErr_t)); \
                                        *code_error = StackVerify(stack); \
                                        if (*code_error != NOTHING) { \
                                            stackdump(stack); \
                                            ParseErr(*code_error); \
                                            return ret_value; \
                                        }

const int MaxPrintedCount = 15;

StackErr_t StackVerify(stack_t* stack);

void StackDump(stack_t *stack, StackErr_t* code_error, VarInfo varinfo);

#endif