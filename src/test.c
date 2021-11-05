/*
// print charbuff
// for (int i = 0; i < c; i++)
// {
//     printf("%d\t", charbuff[i]);
// }
*/

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

/** void move(int steps, DIRECTION dir) { */
/**     // move(20, right); works ! */
/**     char dirchar; */
/**     switch (dir) { */
/**         case up: */
/**             dirchar = 'A'; */
/**         case down: */
/**             dirchar = 'B'; */
/**         case right: */
/**             dirchar = 'C'; */
/**         case left: */
/**             dirchar = 'D'; */
/**         default: */
/**             break; */
/**     } */
/**     printf("\x1b[%d%c", steps, dirchar); */
/** } */

// lets make it such that if it identifies the input and returns successfully
// it must always be a control character ! or any other char that need interpretation
int identify_input(char inputChar, char* str, int *is_control_char) {
    //use time to clear the buffer
    // all multi chars input start with \x1b ? 27, 79, 77 -> shift + <enter>
    // all multi chars input start with \x1b ? 27, 91, 65 -> up arrow
    // support up, down, left, right arrows !!!
    /*  ctrl + right arrow
        Key is 27
        Key is 91
        Key is 49
        Key is 59
        Key is 53
        Key is 67
        */
    //static time_t seconds;
    //time(&seconds);
    //printf("<%ld seconds>", seconds);
    // or maybe match until you find a match from the right sifde of the array
    // tab is 9


    static int charbuff[LIMIT] = {};
    static int c = 0;

    // code to check time interval and clear cache if interval is more than 1 seconds.
    // TODO: iknow make it smaller
    static struct timeval cache, thistime;
    gettimeofday(&thistime, NULL); // is seconds 
    if (thistime.tv_sec - cache.tv_sec >= 1) {
        //printf("CLEARING CACHE");
        memset(charbuff, 0, sizeof(int) * c);
    }
    cache.tv_sec = thistime.tv_sec;

    static int recording = 0;
    // finding out which char has been inputed // for control characters
    if (inputChar == 27 && !recording) {
        // start recording on ESCAPE (coz thats when escape sequences start
        // stop when a sequence has been completed or too much time has passed idk ? maybe 2 secs
        // add them to a complete history 
        //charbuff[c++] = (int)inputChar;
        recording = 1;
    }

    if (recording) {
        charbuff[c++] = (int)inputChar;
    }

    int up_[]  = {27, 91, 65};
    int down_[]  = {27, 91, 66};
    int right_[]  = {27, 91, 67};
    int left_[]  = {27, 91, 68};
    // for inputs that dont occupy more than one character.
    switch(inputChar) {
        case 13:// ^M
            strcpy(str, "enter");
        default:
            {
                //ReverseArray(charbuff, c);
                if (memcmp(charbuff, up_, sizeof(int) * 3) == 0) {
                    memset(charbuff, 0, sizeof(int)*c);
                    c = 0;
                    strcpy(str, "UP_ARROW");
                    recording = 0;
                    *is_control_char = 1;
                }
                if (memcmp(charbuff, down_, sizeof(int) * 3) == 0) {
                    memset(charbuff, 0, sizeof(int)*c);
                    c = 0;
                    strcpy(str, "DOWN_ARROW");
                    recording = 0;
                    *is_control_char = 1;
                }
                if (memcmp(charbuff, right_, sizeof(int) * 3) == 0) {
                    memset(charbuff, 0, sizeof(int)*c);
                    c = 0;
                    strcpy(str, "RIGHT_ARROW");
                    recording = 0;
                    *is_control_char = 1;
                }
                if (memcmp(charbuff, left_, sizeof(int) * 3) == 0) {
                    memset(charbuff, 0, sizeof(int)*c);
                    c = 0;
                    strcpy(str, "LEFT_ARROW");
                    recording = 0;
                    *is_control_char = 1;
                }
            }
    }

    if (!recording) memset(charbuff, 0, sizeof(int) * c);
    return recording;
}

void render(char *buffer) {
    resetCursor();
    printf("%s", buffer);
    //move(strlen(buffer), right); no need that happens automatically when we print the string in buffer.
}

int main(void)
{
    //fgets(str, 80, stdin);
    system("stty raw");
    //char a = getc(stdin);
    char buffer[LIMIT];
    int c = 0;

    while(1) {
        char inputChar = getc(stdin);
        int is_control_char = 0;
        //get cursor postion and clear the places written by the control characters
        // <ctrl-c> ^C
        if (!inputChar || inputChar == 3) break;

        char str[100] = "";
        int recording = identify_input(inputChar, str, &is_control_char);
        //printf("is_control_char => %d", is_control_char);
        // ok so the input sink in not a continuous sink .... this code executes
        // before the next character ex: 97 after 27 is read from stdin.
        // return watching too !!!
        if (recording) continue;
        if (is_control_char) {
            // backspace, arrow keys etc ! (enter is not a control chracter but still needs to be cleared  how about we just clear it all and rewrite anyways ?)
            // the input is a control character ! that we need to React to.
            // clear the length of chars written by the control character
            if (!strncmp(str, "enter", 5)) {
                // evaluate the result and move to a newline for further evaluation
                printf("\n");
                printf("\x1b[1000D");
                continue;
            }
            //else if (str == "backspace") {

            //}
        }
        else {
            // add character to buffer !
            buffer[c++] = inputChar;
        }
        // now all the printed control characters get cleared because we rerender the buffer
        // on each input character !
        render(buffer);
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

