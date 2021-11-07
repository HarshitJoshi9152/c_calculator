// Made for testing !
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <termios.h>

#define LIMIT 10000

// working !!!
void resetCursor() {
    //printf("\n");
    // clear the line and take cursor to left ???
    printf("\x1b[2K");
    printf("\x1b[1000D");
}

/* 
Up: \u001b[{n}A
Down: \u001b[{n}B
Right: \u001b[{n}C
Left: \u001b[{n}D
*/

typedef enum {false, true} bool;
typedef enum directions {up, down, right, left} DIRECTION;
// http://www.robelle.com/library/smugbook/ascii.html

typedef enum ControlChars {
    NUL,
    ETX, // end of text ???   ctrl -c 
    EOT, // end of data ???     ctrl - d
    BACKSPACE,
    ENTER,
    LEFT,
    RIGHT,
    DOWN,
    UP,
    C_LEFT,
    C_RIGHT,
    C_DOWN,
    C_UP,
    
    // ^[[1;5D `ctrl + arrow keys`
} CONTROLCHAR;

// up is 27; 91; 65; ???? nani
// down is 27; 91; 66; ???? nani
// right is 27; 91; 67; ???? nani
// left is 27; 91; 68; ???? nani
void ReverseArray(int arr[], int size)
{
    for (int i = 0; i < size/2; i++)
    {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

void move(int steps, DIRECTION dir) {
    char dirchar;
    switch (dir) {
        case up:
            dirchar = 'A';
            break;
        case down:
            dirchar = 'B';
            break;
        case right:
            dirchar = 'C';
            break;
        case left:
            dirchar = 'D';
            break;
        default:
            break;
    }
    printf("\x1b[%d%c", steps, dirchar);
}

void print_array_until(int *intList, int l) {
    for (int i = 0; i < l; i++) {
        printf("%d ", intList[i]);
    }
    printf("\n");
}

static int charbuff[LIMIT] = {};
char str[100] = "";

bool buffer_matches(const int *sequence, int seq_len, bool *indentifies) {
    // change buffer to be a global variable
    // is it bad to multiply $seq_len with sizeof(int) implicitly ?????
    // what if we change the type of the array items to soemthing else
    // like a `short int` or `long long` ;
    *indentifies = (!memcmp(charbuff, sequence, sizeof(int) * seq_len));
    return *indentifies;
}
bool keyis(char* key)  {
    return !strncmp(str, key, strlen(key));
}

// lets make it such that if it identifies the input and returns successfully
// it must always be a control character ! or any other char that need interpretation
// YES LETS DO THIS ! HAVE A SPECIAL VALUE IN THE ENUM FOR RECORDING STATE TOO !
int identify_input(char inputChar, char* str, int *is_control_char) {
    // printf("<%d>", (int)inputChar);

    //use time to clear the buffer
    //static time_t seconds;
    //time(&seconds);
    //printf("<%ld seconds>", seconds);
    // or maybe match until you find a match from the right sifde of the array
    // tab is 9


    static int c = 0;

    // code to check time interval and clear cache if interval is more than 1 seconds.
    // TODO: iknow make it smaller
    // static struct timeval cache, thistime;
    // gettimeofday(&thistime, NULL); // is seconds 
    // if (thistime.tv_sec - cache.tv_sec >= 1) {
    //     //printf("CLEARING CACHE"); //IS THIS CODE REALLY REQUIRED TODO
    //     // print_array_until(charbuff, c);
    //     memset(charbuff, 0, sizeof(int) * c);
    //     c = 0;
    // }
    // cache.tv_sec = thistime.tv_sec;

    static int recording = 0;

    if (inputChar == 27 && !recording) {
        // start recording on ESCAPE (coz thats when escape sequences start
        // stop when a sequence has been completed or too much time has passed idk ? maybe 2 secs
        // add them to a complete history 
        recording = 1;
    }

    if (recording) charbuff[c++] = (int)inputChar;

    // character code sequences for control Keys !
    // TODO i guess they use a lot of stack space.
    const int up_[]  = {27, 91, 65};
    const int down_[]  = {27, 91, 66};
    const int right_[]  = {27, 91, 67};
    const int left_[]  = {27, 91, 68};
    const int up_control_pressed[] = {27, 91, 49, 59, 53, 65}; // 5 chars ^[[1;5A
    const int down_control_pressed[] = {27, 91, 49, 59, 53, 66}; // 5 chars ^[[1;5A
    const int right_control_pressed[] = {27, 91, 49, 59, 53, 67}; // 5 chars ^[[1;5A
    const int left_control_pressed[] = {27, 91, 49, 59, 53, 68}; // 5 chars ^[[1;5A

    // const int escape[] = {27, 91, 51, 126};

    // const int back_space =    127;
    // const int space =         32;
    // const int tab =           9;
    // const int enter1 =        13;
    // const int enter2 =        10;


    // for inputs that dont occupy more than one character.  | SWITCH STATEMENTS ARE ESSENTIALLY `GOTO`
    switch((int)inputChar) {
        case 13:    {strcpy(str, "ENTER");break;}
        case 10:    {strcpy(str, "ENTER");break;}
        case 127:   {strcpy(str, "BACKSPACE");break;}
        case 9:     {strcpy(str, "TAB");break;}
        // for sequences of characters !
        default: {
            bool identified = false;
            if (buffer_matches(up_, 3, &identified)) {
                strcpy(str, "UP_ARROW");
            }
            else if (buffer_matches(down_, 3, &identified)) {
                strcpy(str, "DOWN_ARROW");
            }
            else if (buffer_matches(right_, 3, &identified)) {
                strcpy(str, "RIGHT_ARROW");
            }
            else if (buffer_matches(left_, 3, &identified)) {
                strcpy(str, "LEFT_ARROW");
            }
            // Control Pressed Arrow Keys !
            else if (buffer_matches(up_control_pressed, 6, &identified)) {
                strcpy(str, "UP_ARROW_CTRL");
            }
            else if (buffer_matches(down_control_pressed, 6, &identified)) {
                strcpy(str, "DOWN_ARROW_CTRL");
            }
            else if (buffer_matches(right_control_pressed, 6, &identified)) {
                strcpy(str, "RIGHT_ARROW_CTRL");
            }
            if (buffer_matches(left_control_pressed, 6, &identified)) {
                strcpy(str, "LEFT_ARROW_CTRL");
            }

            if (identified)
            {
                memset(charbuff, 0, sizeof(int)* (c + 1)); // remember c is an index and indexes start at zero
                c = 0;
                recording = 0;
                // *is_control_char = 1;
            }
        }
    }

    if (!strcmp(str, "")) {
        *is_control_char = 0;
    } else {
        *is_control_char = 1;
    }

    if (!recording) { 
        memset(charbuff, 0, sizeof(int) * c);
        c = 0;
    }
    return recording;
}

void render(char *buffer) {
    resetCursor();
    printf("%s", buffer);
    //move(strlen(buffer), right); no need that happens automatically when we print the string in buffer.
}

int main(void)
{
    system("stty raw");
    char buffer[LIMIT];
    int c = 0;

    while(1) {
        char inputchar = getc(stdin);
        int is_control_char = 0;
        //get cursor postion and clear the places written by the control characters
        // <ctrl-c> ^c
        if (!inputchar || inputchar == 3) break;

        strcpy(str, "");
        int recording = identify_input(inputchar, str, &is_control_char);
        //printf("is_control_char => %d", is_control_char);
        // ok so the input sink in not a continuous sink .... this code executes
        // before the next character ex: 97 after 27 is read from stdin.
        // return watching too !!!
        if (recording) continue;
        if (is_control_char) {
            // backspace, arrow keys etc ! (enter is not a control chracter but still needs to be cleared  how about we just clear it all and rewrite anyways ?)
            // the input is a control character ! that we need to react to.

            // the first tab is smaller as compared to next tabs !
            if (keyis("TAB")) {
                buffer[c++] = '\t';
            }
            else if (keyis("BACKSPACE")) {
                if (c == 0) continue;
                buffer[--c] = '\000';
            }
            else if (keyis("ENTER")) {
                render(buffer); // if we dont rerender the buffer once before moving to next line
                                // the ugly control character mark will be left ! (^M)
                // evaluate the result and move to a newline for further evaluation
                printf("\n");         // comment this and the cursor wont go down !
                printf("\x1b[1000D"); // comment this and the cursor wont go left ! (before the next render !)

                // clearing the buffer as we wont have to edit the line anymore !
                memset(buffer, '\000', c); // or memset maybe
                c = 0;
                continue;
            }
        }
        else {
            // add character to buffer !
            buffer[c++] = inputchar;
        }
        // now all the printed control characters get cleared because we rerender the buffer
        // on each input character !
        render(buffer);
        //move(1, up);
    }    
    // set cursor position to left most point on newline when exiting 
    printf("\n");
    printf("\x1b[1000D");

    system("stty sane");
    return 0;
}

/*
 * https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
 * https://espterm.github.io/docs/VT100%20escape%20codes.html
 * https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#cursor-navigation
 * https://www.includehelp.com/c-programs/gotoxy-clrscr-getch-getche-for-gcc-linux.aspx
 *
 */

