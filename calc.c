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

// rewrite SHIT ! JUMP HERE 
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

        int result = parseByteCode(bytecode, byte_code_len); 
        printf("%d\n", result);
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
 */
