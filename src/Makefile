OS = $(shell uname -s)

GNU = linux-gnu
MACOS = Darwin

CC = gcc
CFLAGS = -Werror -Wextra -Wall -std=c11
LDFLAGS = -fprofile-arcs -ftest-coverage -lgcov
TEST = unittest
TEST_CHECK = $(TEST)
LIBS = -lcheck
MAIN_FILE = s21_decimal

ifeq ($(OS), $(MACOS))
CC += -D MACOS
MEMORY_TEST = leaks -atExit --
else
CC += -D LINUX
LIBS += -lsubunit -lrt -lm -lpthread
MEMORY_TEST = valgrind --trace-children=yes --leak-check=yes --track-origins=yes
endif

all: clean $(MAIN_FILE).a test gcov_report

decimal:
	$(CC) $(CFLAGS) -o $(MAIN_FILE) $(MAIN_FILE).c common/*.c $(LIBS)
	./$(MAIN_FILE)

common.o: common/*.c common/common.h
	$(CC) $(CFLAGS) -c common/*.c 

$(MAIN_FILE).o: $(MAIN_FILE).c $(MAIN_FILE).h 
	$(CC) $(CFLAGS) -c $(MAIN_FILE).c 

$(MAIN_FILE).a: $(MAIN_FILE).o common.o
	ar rc $(MAIN_FILE).a $(MAIN_FILE).o *.o
	ranlib $(MAIN_FILE).a
	rm -rf *.o

gcov_report: 
	gcc -o $(TEST) $(TEST).c common/*.c $(MAIN_FILE).c $(LIBS) $(LDFLAGS)
	./$(TEST)
	lcov --capture --directory . --output-file s21_coverage.info
	genhtml s21_coverage.info --output-directory s21_lcov_report
	open ./s21_lcov_report/index.html

test: clean $(MAIN_FILE).a
	$(CC) $(CFLAGS) -o $(TEST) $(TEST).c $(MAIN_FILE).a $(LIBS)
	rm -rf *.o
	./$(TEST)

check: test
	clang-format --style=Google -n *.c *.h common/*
	cppcheck --check-config --enable=all --suppress=missingIncludeSystem *.c *.h
	$(MEMORY_TEST) ./$(TEST) 

clean:
	rm -rf $(MAIN_FILE)
	rm -rf *.gch *.a *.out *.gcda *.gcno *.o
	rm -rf valgrind_log*
	rm -rf test test.c $(TEST)
	rm -rf s21_lcov_report
	rm -rf *.info

valgrind:
	valgrind --trace-children=yes --track-fds=yes --leak-check=yes --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_log.txt ./$(TEST) 

autotest: $(MAIN_FILE).a
	checkmk $(TEST_CHECK).check > $(TEST_CHECK).c
	clang-format --style=Google -i *.c *.h

format:
	clang-format --style=Google -i *.c *.h common/*
