# things i learn while making this

gcc -03 // optimisation level 3
-lm
static inline functions & how the compiler reduces
size by inlining functions in main function

[tsoding](https://www.youtube.com/watch?v=sJuA5OPvABM&t=2355s)

C
some knowledge about working with address and arrays.
making an interactive console

# Language

https://cheatography.com/ashlyn-black/cheat-sheets/c-reference/

- In C when declaring a variable always explicitly initialise it to 0.
    - in most of the cases the compiler automatically assigns the value of 0 to the variables 
    - stored on the stack frame, but we shouldnt rely on it !
- Data types 
    - https://www.tutorialspoint.com/cprogramming/c_data_types.htm
    - https://en.wikipedia.org/wiki/C_data_types#Main_types
    - chars can be signed too lol

- Pointers and addresses (* => derefrencing operator, & to get the address of a variable).
- C lang memory allocation on stack, different functions code is on diff stack, primary data
- types are copied to functions parameters, but C doesnt copy secondary data types/ complex data types
- like strings (which are just arrays of characters terminated with '\x00' [null character]), arrays,
- structures etc, so instead C uses pointers to allow other functions to be able to access those
- variables in another stack frame !
- the stack has predefiend space for return value, function parameters, etc
    - https://www.youtube.com/watch?v=Rxvv9krECNw | (also copying large chunks of memory is ridiculous)
    - https://themightyprogrammer.dev/article/pointer-primer
    - has passing functions as arguments using pointers and pointer arithmetic.

- [Structures in C](https://www.youtube.com/watch?v=qqtmtuedaBM)
- zero initialization
    struct Point p = {0} // automatically initialize all members to zero.

- [Programs are Data Too](https://www.youtube.com/watch?v=BYPj1IBUeu4&list=PL76809ED684A081F3&index=10)

- make file  https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

-   In C and in C++ single quotes identify a single character, while double quotes
    create a string literal. 'a' is a single a character literal, while a is a
    string literal containing an 'a' and a null terminator (that is a 2 char
    array).
    "" -> char* (char pointer) with implicit NULL byte at the end to signify end of string!
- we cannot return complex data types declared within functions !
    - memory for that array gets automatically deallocated once the function has completed / returned.

- switch can only loop over integers
- syntax for structs, enums, typedef, static keyword
- includeguard !

but strings cannot be reassigned like 
str1 = str2;

instead we have to use `strcpy` and watch out for overflow !

same goes for structs

toread https://nullprogram.com/blog/2021/07/30/

#include just copies the code of the header file in the current file !

TODO: add synatx
    : https://www.youtube.com/watch?v=VV2sgTT48tw&list=PL76809ED684A081F3&index=11
    : learn about pointers reassigning !, pointer arithmetic, practice learnt stuff
    : eg: consts, learnt but unused stuff, union, etc 
    : READ ONLY MEMORY??

to make a complete list check C related search history !

```C
// SOLUTION :: `String literals are by default constant, when assigned to an array it copies the data over `
// http://c-faq.com/decl/strlitinit.html
void rename_string(char *s);

int main() {
    char str[] = "Harshit Joshi\n";
    *str = '9';
    printf("%s", str);

    char *string="testing";    // doesnt run ! 
        /* http://c-faq.com/decl/strlitinit.html
        it turns into an unnamed, static array of characters, and this unnamed array may be stored in read-only memory, and which therefore cannot necessarily be modified. In an expression context, the array is converted at once to a pointer, as usual (see section 6), so the second declaration initializes p to point to the unnamed array's first element. 
        */
    // char []string="testing"; runs 
    rename_string(string);
    return 0;
}

void rename_string(char *s)
{
    printf("Character 1: %c\n",*s);
    // *(s) = 'a';
    *s = '2';
    printf("Character 1 now: %c\n",*s);
}
```

    char string[] = "testing";

You just used a string literal to initialize an array.

so in C when we use ""(s) a string literal gets created as a const string (non modifiable)
(read only) and then the data is copied over to the array !

question : is it located in a different place in memory than other variables ? i guess so


### const pointer and const variables

```C
*
 * const int vs. int const as function parameter in C++ and C
 * 
 * The trick is to read the declaration backwards (right-to-left):
 */

const int a = 1; // read as "a is an integer which is constant"
int const a = 1; // read as "a is a constant integer"

/* Both are the same thing. Therefore: */

a = 2; // Can't do because a is constant

/*
 * The reading backwards trick especially comes in handy when you're
 * dealing with more complex declarations such as:
 */

const char *s;      // read as "s is a pointer to a char that is constant"
char c;
char *const t = &c; // read as "t is a constant pointer to a char"

*s = 'A'; // Can't do because the char is constant
s++;      // Can do because the pointer isn't constant
*t = 'A'; // Can do because the char isn't constant
t++;      // Can't do because the pointer is constant

/* [Ates Goral] [so/q/162480] [cc by-sa 3.0] */

```

read: http://www.cs.fsu.edu/~myers/cgs4406/notes/pointers.html
      bitwise tricks:
        https://graphics.stanford.edu/~seander/bithacks.html
