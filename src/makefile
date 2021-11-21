
test: test.c ./headers/types.h ./headers/test.h ./headers/calc.h
	gcc test.c -Wall -o test.out


run: test.c ./headers/types.h ./headers/test.h ./headers/calc.h
	gcc test.c -Wall -o test.out && ./test.out

debug: test.c ./headers/types.h ./headers/test.h ./headers/calc.h
	gcc test.c -Wall -g -o debug.out