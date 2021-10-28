#include "bytecode.h"
// includes helper function to work with bytecode instructions !
#include <stdio.h>
#include <string.h>


void print_inst_op_code(inst *OP_CODE) {
    printf("type <inst>\n");
    char type[100];
    if ((*OP_CODE).type == 0) strcpy(type,"INT");
    else strcpy(type,"INSTRUCTION");
    printf("    %s\n", type);
    printf("    %d\n", (*OP_CODE).value);
    printf("    %p\n", OP_CODE);
}