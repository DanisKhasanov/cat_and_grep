all: main clean

main: s21_grep.o 
	gcc  s21_grep.o   -o s21_grep

valgrind: 
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT.txt ./s21_grep 
	grep RESULT.txt
	rm -rf RESULT.txt

s21_grep.o: s21_grep.c
	clang-format -style=Google -i s21_grep.c
	gcc -Wall -Werror -Wextra -std=c11 -c s21_grep.c
clean:
	rm -rf s21_grep.o