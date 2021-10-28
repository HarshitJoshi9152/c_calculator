#include "bytecode.h"
#include <stdio.h>

int main() {
    inst x = {INT, 2000};
    print_inst_op_code(&x);
    return 1;
}