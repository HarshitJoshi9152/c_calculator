#include<stdio.h>

typedef enum {false, true} bool;

enum operations {ADD, SUB, MUL, DIV};

void print(char* str) {
    printf("%s\n", str);
}

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
    printf("max_inst_count: %d\n", max_inst_count);
    printf("max_inst: %d\n", max_inst);

    printf("INDEXES => ");
    for (int j = 0; j < max_inst_count; j++) {
        printf("%d  ", max_inst_indexes[j]);
    }

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

        printf("RESULTS ARE => ");
        printf("%d  ", res);
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

int main(int argc, char** argv) {
    print("Testing Structs in C");
    struct Address {
        char name[50];
        char address[100];
        unsigned age;
        unsigned marks;
        bool vaccinated;
    };

    struct Address h_addr = {"harshit", "address_string", 20, 100, true};
    
    //printf("%s\n", h_addr.name);
    //printf("%s\n", h_addr.address);
    //printf("%d\n", h_addr.age);
    //printf("%d\n", h_addr.marks);
    //printf("%d\n", h_addr.vaccinated);

    int bytecode[] = {2, 0, 2, 3, 2, 0, 10, 3, 10}; 
    int len = 9;
    int res = reduce_bytecode(bytecode, len);
    printf("res: %d\n", res);

    return 0;
}

// todo counting with static
