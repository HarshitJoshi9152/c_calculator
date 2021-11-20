#ifndef TEST_H
#define TEST_H

// https://www.google.com/search?channel=fs&client=ubuntu&q=writing+a+library+in+C
// https://stackoverflow.com/questions/36314426/do-you-define-global-variables-in-a-c-library
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "./types.h"


#define LIMIT 10000

typedef enum directions {up, down, right, left} DIRECTION;  // for the move function.
typedef enum controlChars {
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

static int cha_seq_buff[LIMIT] = {};

bool buffer_matches(const int *sequence, int seq_len) {
    // change buffer to be a global variable
    // is it bad to multiply $seq_len with sizeof(int) implicitly ?????
    // what if we change the type of the array items to soemthing else
    // like a `short int` or `long long` ;
    // *identifies = (!memcmp(cha_seq_buff, sequence, sizeof(int) * seq_len));
    // return *identifies;
    return (!memcmp(cha_seq_buff, sequence, sizeof(int) * seq_len));
}

char buffer[LIMIT];
int c = 0;
int cursorPointer = 0;

void setBuffer(char *str, int len) {
    for (int i = 0; i < len; ++i) {
        buffer[i] = *(str + i);
    }
    cursorPointer = len;
}
void copyBuffer(char *str) {
    for (int i = 0; i < c; ++i) {
        *(str + i) = buffer[i];
    }
    *(str + c) = '\x00';
}
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
void moveBackByWord() {
    for (int i = cursorPointer - 1; i >= 0; i--)
    {
        if (isspace(buffer[i]) || i == 0) {
            cursorPointer = i;
            break;
        }
    }
}
void moveForwardByWord() {
    for (int i = cursorPointer + 1; i <= c; i++)
    {
        if (isspace(buffer[i]) || i == c) {
            cursorPointer = i;
            break;
        }
    }
}

void render() {
    printf("\x1b[2K");      // clears the current line
    printf("\x1b[1000D");   // moves cursor to left by 1000 characters
    printf(">> %s", buffer);
}


// we seriously need to clear this buffer !
CONTROLCHAR identify_input(char inputChar) {
    // printf("<%d>", (int)inputChar);

    // find time and clear
    // memset(cha_seq_buff, 0, sizeof(int)* (charCounter + 1)); // remember c is an index and indexes start at zero
                // charCounter = 0;
                // recording = 0;

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

    if (recording) cha_seq_buff[charCounter++] = (int)inputChar;

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
                memset(cha_seq_buff, 0, sizeof(int)* (charCounter + 1)); // remember c is an index and indexes start at zero
                charCounter = 0;
                recording = 0;
            }
        }
    }

    if (recording) return RECORDING;
    return key;
}

void newline() {
    printf("\n");
    printf("\x1b[1000D");
}

#endif