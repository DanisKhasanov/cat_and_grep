all: main clean

main: s21_cat.o 
	gcc  s21_cat.o   -o s21_cat

valgrind: 
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT.txt ./s21_cat 
	cat RESULT.txt
	rm -rf RESULT.txt

s21_cat.o: s21_cat.c
	clang-format -style=Google -i s21_cat.c
	gcc -Wall -Werror -Wextra -std=c11 -c s21_cat.c


clean:
	rm -rf s21_cat.o