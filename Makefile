## Setting CC this way instead straight up calling gcc is fine on ubuntu 18.04,
## since it symlinks to gcc.
## The good part about this is the ability to specify the AFL compiler instead
CC?=gcc

default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check lcov

dictionary.o: dictionary.c
	$(CC) -Wall -c dictionary.c dictionary.h

run_spell: clean prog
	./spell_check

spell.o: spell.c
	$(CC) -Wall -c -fprofile-arcs -ftest-coverage spell.c

test.o: clean test_main.c
	$(CC) -Wall -c test_main.c

main.o: main.c
	$(CC) -Wall -c -g main.c

test: dictionary.o spell.o test_main.o
	$(CC) -Wall  -fprofile-arcs -ftest-coverage -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

coverage: test
	gcov spell.c
	lcov -c --directory . --output-file spell_coverage.info
	genhtml spell_coverage.info --output-directory .cover/

prog: dictionary.o spell.o main.o
	$(CC) -Wall -g -o spell_check dictionary.o spell.o main.o

clean:
	-rm dictionary.o spell.o main.o test_main.o

cleanall:clean
	rm spell_check

afl:
	CC=`which afl-gcc` $(MAKE) prog
	afl-fuzz -i aflinput/ -o out/ -- ./spell_check '@@' wordlist.txt


##################
# Docker targets #
##################
DOCKER_RUN=docker run -it --mount src=`pwd`,target=/AppSecAssignment1,type=bind appsec1-test

dbuild:
	docker build . -t appsec1-test

dtest: dbuild
	$(DOCKER_RUN)

dcover: dbuild
	$(DOCKER_RUN) make coverage
