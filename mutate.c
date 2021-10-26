#include <stdio.h>
#include <math.h>


void mut(int *intPoint, int n) {
    for (int i =0 ; i < n; i++) {
        // intPoint = 10; // this means pointer resetting not assigning value to any index !!!
        //*intPoint = 10; // setting the value at the pointer !
        //intPoint++;
        intPoint[i] = 10; // what this works too !!
    }
}

void print(int *list, int n)
{
    for (int i =0; i < n; i++) {
        printf("%d\n", list[i]); // when printing this works okay 
        printf("%d\n", *(list+i)); // this works too !
    }
}


void print2(int *list)
{
    while(*list != 0x00) {
        printf("%d ", *list);
        list++;
    }
}

int main() {
    int names[] = {1,2,3,4};
    mut(names, 4);
    print(names, 4);

    printf("\n"); // ok so we cant use null termination in int arrays becuase 0x00 is an int itself 0;
    int lst2[] = {1,2,3,4,5,6,7,8,9, 0x00};
    print2(lst2);
    // example here !
    printf("unusable null termination in int array\n"); // ok so we cant use null termination in int arrays becuase 0x00 is an int itself 0;
    int lst3[] = {0, 1,2,3,4,5,6,7,8,9, 0x00};
    print2(lst3);
}

