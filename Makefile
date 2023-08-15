CHECK_VER=0.15.2
MAC_INCLUDE=-I /opt/homebrew/include
MAC_LIB=-L /opt/homebrew/Cellar/check/$(CHECK_VER)/lib
EXTRA_FLAGS=-Wall -Werror -Wextra
COV_FALGS=-fprofile-arcs -ftest-coverage
OS := $(shell uname -s)

all: clean test

test: c_decimal.a test.o
ifeq ($(OS), Darwin)
	gcc test.o c_decimal.a $(MAC_LIB) -lcheck -lm -lpthread -o Test $(EXTRA_FLAGS); \
	./Test
else
	gcc test.o c_decimal.a -lcheck -lsubunit -lm -lrt -lpthread -o Test $(EXTRA_FLAGS)
	./Test
endif

c_decimal.o: c_decimal.c c_decimal.h
	gcc -c c_decimal.c -o c_decimal.o

c_decimal.a: c_decimal.o
	ar rcs c_decimal.a c_decimal.o

test.o: test.c
	gcc $(MAC_INCLUDE) -c test.c -o test.o;

clean:
	rm -f *.out *.exe *.o *.a *.gcda *.gcno *.info *.gcov ./Test
	rm -rf ./report
