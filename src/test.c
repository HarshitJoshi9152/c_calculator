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
    printf("\n");
    printf("\x1b[1000D");
}

/* 
    Up: \u001b[{n}A
    Down: \u001b[{n}B
    Right: \u001b[{n}C
    Left: \u001b[{n}D
*/

typedef enum directions {UP, DOWN, RIGHT, LEFT} DIRECTION;
// http://www.robelle.com/library/smugbook/ascii.html
typedef enum ControlChars {NUL, _, _1, ETX, EOT} CONTROLCHARS;

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
    // move(20, RIGHT); works !
    char dirchar;
    switch (dir) {
        case UP:
            dirchar = 'A';
        case DOWN:
            dirchar = 'B';
        case RIGHT:
            dirchar = 'C';
        case LEFT:
            dirchar = 'D';
        default:
            break;
    }
    printf("\x1b[%d%c", steps, dirchar);
}

int identify_input(char inputChar, char* str) {
    //use time to clear the buffer
    // all multi chars input start with \x1b ? 27, 79, 77 -> shift + <enter>
    // all multi chars input start with \x1b ? 27, 91, 65 -> UP arrow
    // support UP, DOWN, LEFT, RIGHT arrows !!!
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
    if (inputChar == 27) {
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
            }
            if (memcmp(charbuff, down_, sizeof(int) * 3) == 0) {
                memset(charbuff, 0, sizeof(int)*c);
                c = 0;
                strcpy(str, "DOWN_ARROW");
            }
            if (memcmp(charbuff, right_, sizeof(int) * 3) == 0) {
                memset(charbuff, 0, sizeof(int)*c);
                c = 0;
                strcpy(str, "RIGHT_ARROW");
            }
            if (memcmp(charbuff, left_, sizeof(int) * 3) == 0) {
                memset(charbuff, 0, sizeof(int)*c);
                c = 0;
                strcpy(str, "LEFT_ARROW");
            }
        }
    }

    //if () {
    //    printf("this is the UP arrow !");
    //    c = 0;
    //    // clear the screen uptil the no of characters in buffer !
    //    // we dont want to see control characters on the screen
    //}

    //printf("Key is %d", (int)inputChar);
    //printf("str is %s", str);
    
    if (strcmp(str ,"") == 0) {
        // can we clear cache here too ?? yes ig
        return 0;
    }
    recording = 0;
    memset(charbuff, 0, sizeof(int) * c);
    return 1;
}

int main(void)
{
    //fgets(str, 80, stdin);
    system("stty raw");
    //char a = getc(stdin);
    char buffer[LIMIT];
    while(1) {
        char inputChar = getc(stdin);
        //get cursor postion and clear the places written by the control characters
        if (!inputChar) {
            break;
        }
        // <ctrl-c> ^C
        if (inputChar == 3) break;
        char str[LIMIT] = "";
        int modified = identify_input(inputChar, str);
        //printf("modified => %d", modified);
        printf("%s", str);
        resetCursor();
    }       
    // set cursor position to left most point on newline
    resetCursor();
    system("stty sane");
    return 0;
}

/*
 * https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
 * https://espterm.github.io/docs/VT100%20escape%20codes.html
 * https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#cursor-navigation
 */

