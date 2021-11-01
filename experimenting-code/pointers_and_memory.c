#include <stdio.h>
// wait is it a const pointer ?

void p_addr();

int main() {
    char str[] = "naem!";
    char str2[] = "name2";

    // mem addrs change on assignment check ARRAYS cant be assigned only initialised. !! NOTE
    p_addr(str);
    str[1] = 'B'; // this doesnt change the memory address; nor would `memcpy` i suppose !
    p_addr(str);
    p_addr(str2);
    
    typedef struct {char name[100]; int age;} Person;

    // how to reassign mem addrs to pointers ?
    char *name = "Harshit Joshi";
    printf("%c\n", *(name + 2));
    printf("%p\n", &name);
    name = "NAGASKI"; // like this !
    printf("%p\n", &name);                  // didnt change !
    printf("%c\n", *(name + 2));

    // pointers to string literals/string constants <NOT GOOD> ! str is CONST !
    //char *bad = "bad";
    //*(bad) = 's';                       // segmentation FAULT
    //printf("%s\n", bad);
    

    // http://www.c-faq.com/charstring/assign.html | read the entire website !
    // https://stackoverflow.com/questions/32705419/const-arrays-in-c
    // 
    // str = str2; // arrays are consts in C | arrays cannot be reassigned !
    // structs can be reassigned but the rvalue must be a struct too !
    // and C doesnt have assignment literals for structs only initialization literals 
    // `struct car = {.model, .color }` 
    //
    // TODO: make differnet types of variables and check their mutability and mem_address
    // change when reassigned & on value change !
    // learn more about variables, consts, constants, stack, heap, how memory is set,
    // memory layout of a C program, pointers, 1st, 2nd class citizens, initialization
    // vs reassignment vs assignment, strings and struct literals[done],  !
    // pointers are first class, structs are seconds class ? Q CAN THEY BE PASSED AS
    // ARGUMENTS AND RETURNED FROM FUNCTIONS (iread somewhere that they can are copied to functions)
    // https://stackoverflow.com/questions/32705419/const-arrays-in-c
    return 0;
}

void p_addr(char *str) {
    printf("address is %p\n", str);
}

