
test: test.c types.h test.h calc.h
	gcc test.c types.h test.h calc.h -Wall -o test.out


run: test.c types.h test.h calc.h
	gcc test.c types.h test.h calc.h -Wall -o test.out && ./test.out

debug: test.c types.h test.h calc.h
	gcc test.c types.h test.h calc.h -Wall -g -o debug.out