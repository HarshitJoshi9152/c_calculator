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
    DELETE,
    ENTER,
    HOME,
    END
} CONTROLCHAR;

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

bool buffer_matches(const int *sequence, int seq_len) {
    // change buffer to be a global variable
    // is it bad to multiply $seq_len with sizeof(int) implicitly ?????
    // what if we change the type of the array items to soemthing else
    // like a `short int` or `long long` ;
    // *identifies = (!memcmp(charbuff, sequence, sizeof(int) * seq_len));
    // return *identifies;
    return (!memcmp(charbuff, sequence, sizeof(int) * seq_len));
}

char buffer[LIMIT];
int c = 0;
int cursorPointer = 0;

void clearBuffer() {
    memset(buffer, '\000', c + 1);
    c = 0;
    cursorPointer = 0;
}
void addToBuffer(int val) {
    for (int i = c; i > cursorPointer; i--) {
        buffer[i] = buffer[i - 1];
    }
    buffer[cursorPointer++] = val;
    c += 1;
}
void PopBuffer() {
    cursorPointer -= 1;
    for (int i = cursorPointer; i < c; i++) {
        buffer[i] = buffer[i + 1];
    }
    buffer[--c] = '\x00';
}
void RevPopBuffer() {
    for (int i = cursorPointer; i < c; i++) {
        buffer[i] = buffer[i + 1];
    }
    buffer[--c] = '\x00';
}

void render(char *buffer) {
    printf("\x1b[2K");      // clears the current line
    printf("\x1b[1000D");   // moves cursor to left by 1000 characters
    printf("%s", buffer);
}

CONTROLCHAR identify_input(char inputChar) {
    printf("<%d>", (int)inputChar);

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
    const int home_[] = {27, 91, 72};
    const int end_[] = {27, 91, 70};
    const int delete_[] = {27, 91, 51, 126};

    switch(inputChar)
    {
        case 13:    {key = ENTER;     break;}
        case 10:    {key = ENTER;     break;}
        case 127:   {key = BACKSPACE; break;}
        case 8:     {key = BACKSPACE; break;}
        default: {
            if (charCounter < 3) break;     // optimisation [minimum len of sequence is `3`]
            // for sequences of characters !
            if (buffer_matches(home_, 3))                       key = HOME;
            else if (buffer_matches(end_, 3))                   key = END;
            else if (buffer_matches(up_, 3))                    key = UP_ARROW;
            else if (buffer_matches(down_, 3))                  key = DOWN_ARROW;
            else if (buffer_matches(right_, 3))                 key = RIGHT_ARROW;
            else if (buffer_matches(left_, 3))                  key = LEFT_ARROW;
            else if (buffer_matches(up_control_pressed, 6))     key = UP_ARROW_CTRL;
            else if (buffer_matches(down_control_pressed, 6))   key = DOWN_ARROW_CTRL;
            else if (buffer_matches(right_control_pressed, 6))  key = RIGHT_ARROW_CTRL;
            else if (buffer_matches(left_control_pressed, 6))   key = LEFT_ARROW_CTRL;
            else if (buffer_matches(delete_, 4))                key = DELETE;

            // it has been identified
            if (key != NOT_A_CONTROL_CHAR && key != RECORDING)
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

int main(void)
{
    system("stty raw");

    while(1) {
        char inputchar = getc(stdin);

        if (!inputchar || inputchar == 3) break; // <ctrl-c> ^c

        CONTROLCHAR keyPressed = identify_input(inputchar);

        if (keyPressed == RECORDING) continue;

        else if (keyPressed == NOT_A_CONTROL_CHAR) {
            // add character to buffer !
            addToBuffer(inputchar);
        }
        else {
            // the input is a control character ! that we need to react to.
            // the first tab is smaller as compared to next tabs

            if (keyPressed == BACKSPACE) {
                if (cursorPointer != 0) PopBuffer();
            }
            else if (keyPressed == DELETE) {
                if (cursorPointer != c) RevPopBuffer();
            }
            else if (keyPressed == ENTER) {
                render(buffer); // if we dont rerender the buffer once before moving to next line
                                // the ugly control character mark will be left ! (^M)
                // evaluate the result and move to a newline for further evaluation
                printf("\n");         // comment this and the cursor wont go down !
                printf("\x1b[1000D"); // comment this and the cursor wont go left ! (before the next render !)

                // clearing the buffer as we wont have to edit the line anymore !
                clearBuffer();
                continue;
            }
            else if (keyPressed == LEFT_ARROW) {
                if (cursorPointer == 0) continue;
                cursorPointer -= 1;
            }
            else if (keyPressed == RIGHT_ARROW) {
                if (cursorPointer == c - 1) continue;
                cursorPointer += 1;
            }
            else if (keyPressed == HOME) {
                if (cursorPointer == 0) continue;
                cursorPointer = 0;
            }
            else if (keyPressed == END) {
                if (cursorPointer == c) continue;
                cursorPointer = c;
            }
        }
        // now all the printed control characters get cleared because we rerender the buffer
        // on each input character ! (BUT not when we are recording !)
        render(buffer);
        // move to the current position of cursor
        if (c - cursorPointer) move(c - cursorPointer, left);
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
