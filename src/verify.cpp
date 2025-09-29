#include "verify.h"

StackErr_t StackVerify(stack_t* stack) {
    if (!stack) 
        return NULLPTR;

    if (stack->size < 2)
        return SIZE_ERR;
    
    if (stack->capacity <= 2)
        return CAPACITY_ERR;

    if (stack->size > stack->capacity)
        return SIZE_ERR;

    if (stack->data[0] != CANARY || stack->data[stack->capacity - 1] != CANADA)
        return CANARY_ERR;

    long real_hash = 0;
    for (int i = 1; i < stack->capacity - 1; ++i)
        real_hash = (real_hash + i * stack->data[i]) % mod;

    if (real_hash != stack->hash)
        return HASH_ERR;

    return NOTHING;
}

void StackDump(stack_t* stack, StackErr_t* code_error, VarInfo varinfo) {
    printerr("\nStackDump called from %s: in function %s, line %d\n", varinfo.file_name, varinfo.func_name, varinfo.line_ind);
    printerr("%s ", varinfo.stack_name);

    printerr("[");
    if (stack == NULL) {
        printerr(RED_COLOR "NULL" RESET_COLOR);
        printerr("]");
        printerr(RED_COLOR "(!!!! BAD !!!!)\n" RESET_COLOR);
    }
    else {
        printerr(GREEN_COLOR "%x" RESET_COLOR, stack);
        printerr("]\n");
    }

    printerr("\t{\n");

    if (stack) {
        const int delta = (stack->size >= 2 ? -2 : 0);
        
        if (stack->size > stack->capacity || stack->size < 2) {
            printerr(RED_COLOR "\tsize = %d    (!!! BAD !!!)\n" RESET_COLOR, stack->size + delta); 
        }
        else {
            printerr(GREEN_COLOR "\tsize = %d\n" RESET_COLOR, stack->size + delta);
        }

        if (stack->capacity < 2) {
            printerr(RED_COLOR "\tcapacity = %d (!!! BAD !!!)\n" RESET_COLOR, stack->capacity);
        }
        else {
            printerr(GREEN_COLOR "\tcapacity = %d\n" RESET_COLOR, stack->capacity);
        }

        printerr(BLUE_COLOR "\tHash = %d\n" RESET_COLOR, stack->hash);
        
        printerr("\tdata [");
        if (stack->data == NULL) {
            printerr(RED_COLOR "NULL" RESET_COLOR);
            printerr("]");
            printerr(RED_COLOR "(!!!! BAD !!!!)\n" RESET_COLOR);
        }
        else {
            printerr(GREEN_COLOR "%x" RESET_COLOR, stack->data);
            printerr("]\n");
        }
        
        printerr("\t\t{\n");

        if ((stack->capacity) >= 0) {
            printerr((stack->data[0] == CANARY) ? GREEN_COLOR : RED_COLOR);
            printerr("\t\t   [%2d] = %10d (CANARY)\n" RESET_COLOR, 0, *stack->data);
            for (ssize_t i = 1; i < stack->capacity - 1; ++i) {
                if (i >= MaxPrintedCount) {
                    printerr("\t\t\t...\n\n");
                    break;
                }

                if (*(stack->data + i) == POIZON)
                    printerr("\t\t   [%2d] = %10d (POIZON)\n", i, POIZON);
                else
                    printerr("\t\t * [%2d] = %10d\n", i, *(stack->data + i));
            }
            
            printerr((stack->data[stack->capacity - 1] == CANADA) ? GREEN_COLOR : RED_COLOR);
            printerr("\t\t   [%2d] = %10d (CANARY)\n" RESET_COLOR, stack->capacity - 1, stack->data[stack->capacity - 1]);
        }

        printerr("\t\t}\n");
    }

    printerr("\t}\n");
}