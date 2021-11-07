// Made for testing !
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <termios.h>

#define LIMIT 10000

typedef enum {false, true} bool;
typedef enum directions {up, down, right, left} DIRECTION;  // for the move function.

typedef enum ControlChars {
    NOT_A_CONTROL_CHAR,
    RECORDING,
    UP_ARROW,
    DOWN_ARROW,
    RIGHT_ARROW,
    LEFT_ARROW,
    UP_ARROW_CTRL,
    DOWN_ARROW_CTRL,
    RIGHT_ARROW_CTRL,
    LEFT_ARROW_CTRL,
    BACKSPACE,
    ENTER,
    TAB,
} CONTROLCHAR;

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

bool buffer_matches(const int *sequence, int seq_len, bool *indentifies) {
    // change buffer to be a global variable
    // is it bad to multiply $seq_len with sizeof(int) implicitly ?????
    // what if we change the type of the array items to soemthing else
    // like a `short int` or `long long` ;
    *indentifies = (!memcmp(charbuff, sequence, sizeof(int) * seq_len));
    return *indentifies;
}

CONTROLCHAR identify_input(char inputChar) {
    // printf("<%d>", (int)inputChar);

    CONTROLCHAR key = NOT_A_CONTROL_CHAR;
    static int charCounter = 0;
    static int recording = 0;

    if (inputChar == 27 && !recording) {
        // start recording on ESCAPE (coz thats when escape sequences start
        // stop when a sequence has been completed or too much time has passed idk ? maybe 2 secs
        // add them to a complete history 
        recording = 1;
        key = RECORDING;
    }

    if (recording) charbuff[charCounter++] = (int)inputChar;

    // character code sequences for control Keys !
    // TODO i guess they use a lot of stack space.
    const int up_[]  = {27, 91, 65};
    const int down_[]  = {27, 91, 66};
    const int right_[]  = {27, 91, 67};
    const int left_[]  = {27, 91, 68};
    const int up_control_pressed[] = {27, 91, 49, 59, 53, 65};
    const int down_control_pressed[] = {27, 91, 49, 59, 53, 66};
    const int right_control_pressed[] = {27, 91, 49, 59, 53, 67};
    const int left_control_pressed[] = {27, 91, 49, 59, 53, 68};

    switch(inputChar)
    {
        case 13:    {key = ENTER;     break;}
        case 10:    {key = ENTER;     break;}
        case 127:   {key = BACKSPACE; break;}
        case 9:     {key = TAB;       break;}
        default: {
            // for sequences of characters !
            bool identified = false;
            if (buffer_matches(up_, 3, &identified)) {
                key = UP_ARROW;
            }
            else if (buffer_matches(down_, 3, &identified)) {
                key = DOWN_ARROW;
            }
            else if (buffer_matches(right_, 3, &identified)) {
                key = RIGHT_ARROW;
            }
            else if (buffer_matches(left_, 3, &identified)) {
                key = LEFT_ARROW;
            }
            // Control Pressed Arrow Keys !
            else if (buffer_matches(up_control_pressed, 6, &identified)) {
                key = UP_ARROW_CTRL;
            }
            else if (buffer_matches(down_control_pressed, 6, &identified)) {
                key = DOWN_ARROW_CTRL;
            }
            else if (buffer_matches(right_control_pressed, 6, &identified)) {
                key = RIGHT_ARROW_CTRL;
            }
            if (buffer_matches(left_control_pressed, 6, &identified)) {
                key = LEFT_ARROW_CTRL;
            }

            if (identified)
            {
                memset(charbuff, 0, sizeof(int)* (charCounter + 1)); // remember c is an index and indexes start at zero
                charCounter = 0;
                recording = 0;
            }
        }
    }

    if (recording) return RECORDING;
    return key;
}

void render(char *buffer) {
    printf("\x1b[2K");      // clears the current line
    printf("\x1b[1000D");   // moves cursor to left by 1000 characters
    printf("%s", buffer);
}

int main(void)
{
    system("stty raw");
    char buffer[LIMIT];
    int c = 0;

    while(1) {
        char inputchar = getc(stdin);

        if (!inputchar || inputchar == 3) break; // <ctrl-c> ^c

        CONTROLCHAR keyPressed = identify_input(inputchar);

        if (keyPressed == RECORDING) continue;

        else if (keyPressed == NOT_A_CONTROL_CHAR) {
            // add character to buffer !
            buffer[c++] = inputchar;
        }
        else {
            // the input is a control character ! that we need to react to.

            // the first tab is smaller as compared to next tabs
            if (keyPressed == TAB) {
                buffer[c++] = '\t';
            }
            else if (keyPressed == BACKSPACE) {
                if (c == 0) continue;
                buffer[--c] = '\000';
            }
            else if (keyPressed == ENTER) {
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
        // now all the printed control characters get cleared because we rerender the buffer
        // on each input character ! (BUT not when we are recording !)
        render(buffer);
    }
    // set cursor position to left most point on newline when exiting 
    system("stty sane");
    printf("\n");
    return 0;
}

/*
 * https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
 * https://espterm.github.io/docs/VT100%20escape%20codes.html
 * https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#cursor-navigation
 * https://www.includehelp.com/c-programs/gotoxy-clrscr-getch-getche-for-gcc-linux.aspx
 *
 */
