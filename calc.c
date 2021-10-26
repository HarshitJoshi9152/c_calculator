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

    int byte_len = 0;

    while(count < len) {
        char c = string[count];
        printf("char = %c\t ascii: %d\n", c, c);
        // whitespace
        if (c == 0x20) {
            // add intBuff to bytecode array;
            if (intSet) {
                intSet = false;
                bytecode[inst_counter] = intBuff; // aah we need a flag for here for 10 + 20 => for whitespace after + .
                intBuff = 0;
                intlen = 0;
                inst_counter++;
                byte_len += 1;
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
            printf("\n");
            printf("10 * intBuff + (c - '0');\n");
            printf("\t%lf * %d + %d\n", pow(10, intlen), intBuff, (c - '0'));
            intBuff = 10 * intBuff + (c - '0');
            printf("intbuff = %d\n", intBuff);
            printf("\n");
            intlen += 1;
            intSet = true;
            // we are not adding anything to the array so we have to counter blank array indexes;
            inst_counter--;
            byte_len -= 1;
        }
        byte_len += 1;
        inst_counter++;
        count++;
    }
    if (intBuff != 1) {
        bytecode[inst_counter] = intBuff;
    }
    return inst_counter + 1;
}
int parseByteCode(int* bytecode, int len) {
    printf("\nPARSING STARTS:\n");
    // odd indexes are operators and even indexes are operands.
    int a, b, c, d;
    bool a_set, b_set;
    for (int i = 0; i < len; i++) {
        // even index meaning operand value
        printf("i is = %d\n", i);
        printf("index is = %d\n", bytecode[i]);
        if (i % 2 == 0) {
            // this logic feels useless now lmao
            if (!a_set) {
                a = bytecode[i];
                //a_set = true;
            } else {
                b = bytecode[i];
            }
        }
        else {
            // operator check | compute the value and put it in next index slot if it is available
            //enum operations {ADD, SUB, MUL, DIV};
            int op = bytecode[i];
            int op1 = bytecode[i - 1];
            int op2 = bytecode[i + 1];
            bool last_comp = (i + 2 == len);
            printf("op = %d\nop1 = %d\nop2 = %d\n, slots=%d\n", op, op1, op2, last_comp);
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
void printBytecode(int list[1000], int len) {
    printf("PRINTING BYTECODE\n");
    for (int i = 0; i < len; i++) {
        printf("%d ", *(list + i));
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
        printf("GOT INPUT !\n");
        // check if user wants to exit
        if (match(input, "exit")) {
            break;
        }
        //parseInput
        printf("%s\n", input);
        //int z[] = {1,8,2,3,4,5};
        //printBytecode(z, 6);
        
        int bytecode[1000];
        unsigned int len = strlen(input);
        int byte_len = genByteCode(input, len, bytecode);

        // int length = sizeof(bytecode) / sizeof(bytecode[0]);
        printf("len is %d", byte_len); // length is 1000 lol
        int res = parseByteCode(bytecode, byte_len); // ya length determination is the only task left now !
        printf("res = %d", res);

        printf("\nbytecode pointer =>> %p\n", bytecode);

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
