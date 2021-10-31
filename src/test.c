#include <stdio.h>
#include <stdlib.h>

#define LIMIT 10000

int main(void)
{
    //fgets(str, 80, stdin);
    system("stty raw");
    //char a = getc(stdin);
    char buffer[LIMIT];
    while(1) {
        char inputChar = getc(stdin);
        if (!inputChar) {
            break;
        }
        printf("%c\n", inputChar);
    }
    system("stty sane");
    return 0;
}
/*
 * https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
 * https://espterm.github.io/docs/VT100%20escape%20codes.html
 * https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#cursor-navigation
 */

