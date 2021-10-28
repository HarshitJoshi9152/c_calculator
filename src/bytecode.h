#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

enum inst_type {
    INSTRUCTION,
    INT
};

typedef struct {
    enum inst_type type;
    int value;
} inst;


void print_inst_op_code(inst *OP_CODE);

#endif