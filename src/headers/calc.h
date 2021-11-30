#pragma once

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "./types.h"


typedef enum errors {
    E_SUCCESSFUL,
    E_EMPTY_INPUT,
    E_BAD_SYNTAX,
    E_INVCHAR_POD, // invalid char in Place Of Digit
} Gen_Error;

//  The exponentiation operator is non-associative. | https://www.cs.utah.edu/~zachary/isp/worksheets/operprec/operprec.html
enum operations {ADD, SUB, MUL, DIV, POW};

static const char* OperationStrings[] = {
    [ADD] = "ADD",
    [SUB] = "SUB",
    [MUL] = "MUL",
    [DIV] = "DIV",
    [POW] = "POW"
};

int match(char str1[], char str2[]) {
    return !strcmp(str1, str2);
}

void strip(char* str, int len) {
    // str is just a pointer so we cant use strlen here !
    for (int i=0; i < len; i++) {
        int char_ascii = str[i];
        bool remove = false;
        switch (char_ascii)
        {
            case 0x20:  // space
                remove = true;
                break;
            case 0x9: // tabs
                remove = true;
                break;
            case 0x0a: // newline
                remove = true;
                break;
            case 0x0d: // carriage return
                remove = true;
                break;
            case 0x0c: // form feed
                remove = true;
                break;
            case 0x0b: // vertical tab
                remove = true;
                break;
            case 0x2c: // comma
                remove = true;
                break;
            default:
                break;
        }
        if (remove) {
            // shift all values to the left to fill the removed spot;
            for (int j = i; j < len; j++) {
                str[j] = str[j+1];
            }
        }
    }
}

int setCounterAfterFloat(char *str) {
    int i = 0;
    for (; str[i] != '\x00'; i++) {
        char c = str[i];
        if (c != '.' && !isdigit(c)) {
            return i;
        }
    }
    return i;
}

// https://stackoverflow.com/questions/11656532/returning-an-array-using-c
Gen_Error genByteCode(char string[], int len, float bytecode[1000], int *byte_code_len) {
    if (len == 0) return E_EMPTY_INPUT;
    // int bytecode[1000]; // formatted like {value, inst_, value, inst_, value}
    int inst_counter = 0;  // no of instructions in bytecode
    int count = 0;         // loop counter
    // unsigned int len = strlen(string); // bad operation !
    float floatBuff = 0; // stores accumulated integer value when reading int by int

    while(count < len) {
        char c = string[count];
        // ADD
        if (c == 0x2b)         bytecode[inst_counter++] = ADD;
        // SUB
        else if (c == 0x2d)    bytecode[inst_counter++] = SUB;
        // MUL
        else if (c == 0x2a)    bytecode[inst_counter++] = MUL;
        // DIV
        else if (c == 0x2f)    bytecode[inst_counter++] = DIV;
        // POW
        else if (c == 0x5e)    bytecode[inst_counter++] = POW;
        // integer value
        else if(isdigit(c)) {
            int floatsRead = sscanf(string + count, "%f", &floatBuff);
            if (floatsRead != 1) return E_INVCHAR_POD;
            bytecode[inst_counter++] = floatBuff;
            count += setCounterAfterFloat(string + count) - 1;
        } else {
            return E_BAD_SYNTAX;
        }
        count++;
    }

    *byte_code_len = inst_counter;
    return E_SUCCESSFUL;
}

float reduce_bytecode(float* bytecode, int len) {
    if (len == 1) return bytecode[0];
    // loop over instructions and find highest order function. [inst are at odd indexes]
    int i = 1;
    int max_inst=0;
    int max_inst_count=0;
    int max_inst_indexes[len]; // cannot be more than the $len in anyway.

    while(i < len) {
        float code = bytecode[i];
        if (code > max_inst) {
            max_inst = code;
            max_inst_count = 0;
            // clear the indexes_list; maybe we dont need this because we are also using length ! think !
            //for (int j = 0; j < max_inst_count; j++) {
            //    max_inst_indexes[j] = 0;
            //}
        }
        if (code == max_inst) {
            max_inst_indexes[max_inst_count++] = i;
        }
        i += 2;
    }

    // evaluate the indexes where max_values are;
    //printf("max_inst_count: %d\n", max_inst_count);
    //printf("max_inst: %d\n", max_inst);

    //printf("INDEXES => ");
    //for (int j = 0; j < max_inst_count; j++) {
    //    printf("%d  ", max_inst_indexes[j]);
    //}

    // evaluate the highest order bytecode operation indexes and store the result in left side index
    // and shift all the values after the index to the left;
    int offset = 0;
    for (int counter = 0; counter < max_inst_count; counter++) {
        int i = max_inst_indexes[counter] - offset;
        // evaluate the operation

        //enum operations {ADD, SUB, MUL, DIV};
        float op = bytecode[i];
        float op1 = bytecode[i - 1];
        float op2 = bytecode[i + 1];
        float res;

        switch ((int)op) {
            case ADD: 
                res = op1 + op2;
                break; // will this break break the for loop too ?
            case SUB:
                res = op1 - op2;
                break;
            case MUL:
                res = op1 * op2;
                break;
            case DIV:
                res = op1 / op2;
                break;
            case POW:
                res = pow(op1, op2);
                break;
            default:
                fprintf(stderr, "ERROR ENCOUNTERED ! INVALID OPERATOR CODE : %f", op);
                return 1;
        }

        //printf("RESULTS ARE => ");
        //printf("%d  ", res);
        //
        // shift instructions to the right to the left side !
        bytecode[i-1] = res;

        for (int jj=i; jj < len; jj++) {
            if (len <= jj + 2) {
                break;
            }
            bytecode[jj] = bytecode[jj + 2];
        }

        offset += 2; // index offset for max_inst_indexes

    }
    return reduce_bytecode(bytecode, len - offset);
}

void printByteCode(float* bytecode, int len) {
    // doesnt include "test.h" is that BAD? 
    printf("Printing Byte Code !");
    newline();
    for (int i = 0; i < len; i++) {
        if (i % 2 != 0) printf("%s  ", OperationStrings[(int)bytecode[i]]);
        else printf("%f  ", bytecode[i]);
    }
    newline();
    printf("----------");
    newline();
}

void introduction() {
    printf("Welcome to C_Calculator ! you can perform simple operations like +,-,/,*\n");
    printf("built By @HarshitJoshi9152, report issues at \n");
}

void handleError(Gen_Error e_code) {
    fprintf(stderr, "ERROR : <%d>", e_code);
}

