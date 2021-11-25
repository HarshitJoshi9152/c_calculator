#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>

#include "./headers/inputline.h"
#include "./headers/calc.h"

int main(int argc, char **args)
{
    introduction();
    system("stty raw");
    render(); // to get >>> easily

    char input[LIMIT];
    // store history
    struct CALCULATION {
        char question[1000];
        int answer;
    };
    unsigned char history_counter = 0; // what chars are signed too ?
    struct CALCULATION history[1000];

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
                render(); // if we dont rerender the buffer once before moving to next line
                                // the ugly control character mark will be left ! (^M)
                // evaluate the result and move to a newline for further evaluation
                printf("\n");         // comment this and the cursor wont go down !
                printf("\x1b[1000D"); // comment this and the cursor wont go left ! (before the next render !)

                // OKAY ITS STARTING
                copyBuffer(input);
                strip(input, strlen(input));
                if (match(input, "exit") || match(input, "quit")) {
                    break;
                }
                else if(match(input, "history")) {
                    for (int i = 0; i < history_counter; i++) {
                        printf("%s", history[i].question);
                        newline();
                        printf("%d", history[i].answer);
                        newline();
                    }
                    // continue -> clear buffer -> rerender !
                    clearBuffer();
                    goto last_section; // but goto is bad should i use a switch ?
                }

                // bytecode generation and evaluation process starts

                unsigned int len = strlen(input);

                float bytecode[1000];
                int byte_code_len;
                Gen_Error e_code = genByteCode(input, len, bytecode, &byte_code_len);

                // dont execute or add the calculation to history if bytecode generation is bad !
                if (e_code == E_SUCCESSFUL) {
                    if (argc > 1) printByteCode(bytecode, byte_code_len);

                    float res = reduce_bytecode(bytecode, byte_code_len);

                    strcpy(history[history_counter].question, input);
                    history[history_counter++].answer = res;

                    // print the result
                    printf("%f", res);
                }
                else handleError(e_code);

                //  move to next line to take next input
                newline();

                // clearing the buffer as we wont have to edit the line anymore !
                clearBuffer();
            }

            else if (keyPressed == LEFT_ARROW) {
                // this is the fix for tailing control characters
                // on 0 position left key press, idk why tho
                // maybe not rendering each time makes the buffer bad ?
                if (cursorPointer != 0) {
                    cursorPointer -= 1;
                }
            }
            else if (keyPressed == RIGHT_ARROW) {
                if (cursorPointer != c - 1) {
                    cursorPointer += 1;
                }
            }
            else if (keyPressed == HOME) {
                if (cursorPointer != 0) {
                    cursorPointer = 0;
                }
            }
            else if (keyPressed == END) {
                if (cursorPointer != c) {
                    cursorPointer = c;
                }
            }
            else if (keyPressed == LEFT_ARROW_CTRL) {
                moveBackByWord();
            }
            else if (keyPressed == RIGHT_ARROW_CTRL) {
                moveForwardByWord();
            }
        }
        // now all the printed control characters get cleared because we rerender the buffer
        // on each input character ! (BUT not when we are recording !)
        last_section:
            render();
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
