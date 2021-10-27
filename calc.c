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

enum operations {ADD, SUB, MUL, DIV};

// NO BODMAS
// https://stackoverflow.com/questions/11656532/returning-an-array-using-c
int genByteCode(char string[], int len, int bytecode[1000]) {
    // int bytecode[1000]; // formatted like {value, inst_, value, inst_, value}
    int inst_counter = 0;  // no of instructions in bytecode
    int count = 0;         // loop counter
    // unsigned int len = strlen(string); // bad operation !
    int intBuff = 0; // stores accumulated integer value when reading int by int
    int intlen = 0;  // stored length of int currently being read.
    bool intSet = 0;

    while(count < len) {
        char c = string[count];
        // ADD
        if (c == 0x2b) {
            if (intSet) {
                bytecode[inst_counter++] = intBuff;
                intSet = false;
                intBuff = 0;
                intlen = 0;
            } else {
                fprintf(stderr, "ERROR BAD SYNTAX !\n");
                break;
            }
            bytecode[inst_counter++] = ADD;
            
        }
        // SUB
        else if (c == 0x2d) {
            if (intSet) {
                bytecode[inst_counter++] = intBuff;
                intSet = false;
                intBuff = 0;
                intlen = 0;
            } else {
                fprintf(stderr, "ERROR BAD SYNTAX !\n");
                break;
            }
            bytecode[inst_counter++] = SUB;
            
        }
        // MUL
        else if (c == 0x2a) {
            if (intSet) {
                bytecode[inst_counter++] = intBuff;
                intSet = false;
                intBuff = 0;
                intlen = 0;
            } else {
                fprintf(stderr, "ERROR BAD SYNTAX !\n");
                break;
            }
            bytecode[inst_counter++] = MUL;
            
        }
        // DIV
        else if (c == 0x2f) {
            if (intSet) {
                bytecode[inst_counter++] = intBuff;
                intSet = false;
                intBuff = 0;
                intlen = 0;
            } else {
                fprintf(stderr, "ERROR BAD SYNTAX !\n");
                break;
            }
            bytecode[inst_counter++] = DIV;
            
        }
        // integer value
        else if(isdigit(c)) {
            intSet = true;
            intBuff = 10 * intBuff + (c - '0');
            intlen += 1;
        } else {
            fprintf(stderr, "ERROR ENCOUNTERED ! INVALID CHARACTERS IN PLACE OF DIGITS !\n");
            break;
        }
        count++;
    }

    if (intSet) {
        bytecode[inst_counter] = intBuff;
    }
    return inst_counter + 1;
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

// void check_commands(char *input) {
//     if (match(input, "exit")) {
//         break
//     }
//     else if(match(input, "history"))
//     {
//         for (int i = 0; i < history_counter; i++) {
//             printf("%s\n", history[i].question);
//             printf("%d\n", history[i].answer);
//         }
//     }
// }

int main(int argc, char* argv[]) {
    //printf("%d\n",argc);
    //for (int i = 0; i < argc; i++) {
    //    printf("%s ", argv[i]);
    //}
    //printf("\n");
    introduction();
    char input[100];
    // store history
    struct CALCULATION {
        char question[1000];
        int answer;
    };
    unsigned char history_counter = 0; // what chars are signed too ?
    struct CALCULATION history[1000];

    while(1) {
        // getting user input
        printf(">> ");
        scanf("%[^\n]%*c", input);

        // strip user input | remove whitespace characters.
        strip(input, strlen(input));

        // check if user wants to exit
        if (match(input, "exit")) {
            break;
        }
        else if(match(input, "history")) {
            for (int i = 0; i < history_counter; i++) {
                printf("%s\n", history[i].question);
                // printf("%d\n", strlen(history[i].question));
                printf("%d\n", history[i].answer);
            }
            continue;
        }

        // bytecode generation and evaluation process starts

        unsigned int len = strlen(input);

        int bytecode[1000];
        int byte_code_len = genByteCode(input, len, bytecode);

        if (argc > 1) {
            printf("Printing Byte Code !\n");
            for (int i = 0; i < byte_code_len; i++) {
                printf("%d  ", bytecode[i]);
            }
            printf("\n----------\n");
        }

        int res = reduce_bytecode(bytecode, byte_code_len);
        printf("%d\n", res);

        // history[history_counter++].question = input // wouldnt Work because strings cannot be reassigned wholely like this
        strcpy(history[history_counter++].question, input);
        history[history_counter++].answer = res;

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