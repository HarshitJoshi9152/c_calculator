
calc: calc.c ./headers/types.h ./headers/inputline.h ./headers/calc.h
	gcc calc.c -Wall -lm -o calc.out

run: calc.c ./headers/types.h ./headers/inputline.h ./headers/calc.h
	gcc calc.c -Wall -lm -o calc.out && ./calc.out

debug: calc.c ./headers/types.h ./headers/inputline.h ./headers/calc.h
	gcc calc.c -Wall -lm -g -o debug.out