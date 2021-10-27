// Calculator Program
//
// Harshit Joshi

#include <stdio.h>
#include <string.h>
#include <ctype.h>
// https://www.programiz.com/c-programming/library-function/math.h/pow
// https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Pointers.html
// https://overiq.com/c-programming-101/array-of-pointers-in-c/
#include <math.h>

typedef enum {false, true} bool;

int match(char str1[], char str2[]) {
    return !strcmp(str1, str2);
}

unsigned int mystrlen(const int* arr)
 {
     unsigned int length = 0;
     while (*arr != 0)
     {
         arr++;
         length++;
     }
     return length;
 }


enum operations {ADD, SUB, MUL, DIV};

// NO BODMAS
// https://stackoverflow.com/questions/11656532/returning-an-array-using-c
int genByteCode(char string[], int len, int bytecode[1000]) {
    // loop over the string and retrun byte codes
    // int bytecode[1000]; // formatted like {value, inst_, value, inst_, value}
    int inst_counter = 0;
    int count = 0;
    // unsigned int len = strlen(string); // bad operation !
    int intBuff = 0;
    int intlen = 0;
    bool intSet = 0;

    while(count < len) {
        char c = string[count];
        //printf("char = %c\t ascii: %d\n", c, c);
        // whitespace | TODO REMOVE WHITESPACE PARSING CODE AND REMOVE WHITESPACES BEFOREHAND
        if (c == 0x20) {
            // add intBuff to bytecode array;
            if (intSet) {
                intSet = false;
                bytecode[inst_counter] = intBuff; // aah we need a flag for here for 10 + 20 => for whitespace after + .
                intBuff = 0;
                intlen = 0;
                inst_counter++;
            }
            count++;
            continue;
        }
        // ADD
        else if (c == 0x2b) bytecode[inst_counter] = ADD;
        // SUB
        else if (c == 0x2d) bytecode[inst_counter] = SUB;
        // MUL
        else if (c == 0x2a) bytecode[inst_counter] = MUL;
        // DIV
        else if (c == 0x2f) bytecode[inst_counter] = DIV;
        // likely int value
        else {
            if (!isdigit(c)) {
                fprintf(stderr, "ERROR ENCOUNTERED ! INVALID CHARACTERS IN PLACE OF DIGITS !\n");
                break;
            }
            intBuff = 10 * intBuff + (c - '0');
            intlen += 1;
            intSet = true;
            // we are not adding anything to the array so we have to counter blank array indexes;
            inst_counter--;
        }
        inst_counter++;
        count++;
    }
    if (intBuff != 1) {
        bytecode[inst_counter] = intBuff;
    }
    return inst_counter + 1;
}
int parseByteCode(int* bytecode, int len) {
    // odd indexes are operators and even indexes are operands.
    for (int i = 0; i < len; i++)
    {
        // but this method doesnt take operator precedence into account.
        if (i % 2 != 0) {
            // operator check | compute the value and put it in next index slot if it is available
            //enum operations {ADD, SUB, MUL, DIV};
            int op = bytecode[i];
            int op1 = bytecode[i - 1];
            int op2 = bytecode[i + 1];
            bool last_comp = (i + 2 == len);
            //printf("op = %d\nop1 = %d\nop2 = %d\n, slots=%d\n", op, op1, op2, last_comp);
            int res;
            switch (op) {
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
                default:
                    fprintf(stderr, "ERROR ENCOUNTERED ! INVALID OPERATOR CODE : %d", op);
                    return 1;
                    break;
            }
            if (last_comp) {
                return res;
            } else {
                bytecode[i+1] = res;
            }
        }
    }
}

// BODMAS SUPPORT !
int reduce_bytecode(int* bytecode, int len) {
    if (len == 1) return bytecode[0];
    // loop over instructions and find highest order function. [inst are at odd indexes]
    int i = 1;
    int max_inst=0;
    int max_inst_count=0;
    int max_inst_indexes[len]; // cannot be more than the $len in anyway.

    while(i < len) {
        int code = bytecode[i];
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
        int op = bytecode[i];
        int op1 = bytecode[i - 1];
        int op2 = bytecode[i + 1];
        int res;
        
        switch (op) {
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
            default:
                fprintf(stderr, "ERROR ENCOUNTERED ! INVALID OPERATOR CODE : %d", op);
                return 1;
                break;
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


void introduction() {
    printf("Welcome to C_Calculator ! you can perform simple operations like +,-,/,*\n");
    printf("built By @HarshitJoshi9152, report issues at \n");
}

int main(int argc, char* argv[]) {
    introduction();
    char input[100];
    // store history
    while(1) {
        // getting user input
        printf(">> ");
        scanf("%[^\n]%*c", input);
        // check if user wants to exit
        if (match(input, "exit")) {
            break;
        }
        
        int bytecode[1000];
        unsigned int len = strlen(input);
        int byte_code_len = genByteCode(input, len, bytecode);

        if (argc > 1) {
            printf("Printing Byte Code !\n");
            for (int i = 0; i < byte_code_len; i++) {
                printf("%d  ", bytecode[i]);
            }
            printf("----------\n");
        }

        //int result = parseByteCode(bytecode, byte_code_len); 
        //printf("%d\n", result);

        int res = reduce_bytecode(bytecode, byte_code_len);
        printf("%d\n", res);
    }
    return 0;
}

// http://www.robelle.com/library/smugbook/ascii.html
// https://www.tutorialspoint.com/cprogramming/c_structures.htm
// https://www.programiz.com/c-programming/examples/access-array-pointer
// https://stackoverflow.com/questions/628761/convert-a-character-digit-to-the-corresponding-integer-in-c
// https://www.programiz.com/c-programming/library-function/ctype.h/isdigit
// https://stackoverflow.com/questions/27731351/c-pass-array-to-a-function-and-iterate-over-in-recipient-function

// i think i have understood a few things about pointers and passing around arrays in C with this project !
// 
// bad pointer address value (when [,immaturely] trying to return arr pointer from a function) // memory for that array gets deallocated once the function has completed / returned.
// bytecode pointer =>> 0x7
//
// correct ig?
// bytecode pointer =>> 0x7fff09f46f20

// assigning items to int array passed as pointer to function
// https://beginnersbook.com/2014/01/c-passing-array-to-function-example/

/* TODO
 * ADD PARENTHESIS SUPPORT 
 * ADD BODMAS
 * ADD INTERACTIVE BPYTHON LIKE CONSOLE
 * USE DOUBLES FOR DIVISION
 * ADD SOME REDUCE LIKE FUNCTION TO EVALUATE THE BYTECODE IN BODMAS ORDER
 */
