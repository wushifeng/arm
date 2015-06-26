all: main

main: DB_Operation.o Door_Operation.o Door_Process.o FP_Operation.o FP_Process.o LCD_Operation.o main.o Wait_Operation.o Ncurses_Operation.o Sqlite_Operation.o sqlite3.o Debug_Operation.o 
	gcc DB_Operation.o Door_Operation.o Door_Process.o FP_Operation.o FP_Process.o LCD_Operation.o main.o Wait_Operation.o Ncurses_Operation.o Sqlite_Operation.o sqlite3.o Debug_Operation.o -o main -lncurses -ldl -lpthread

DB_Operation.o: DB_Operation.c
	gcc -c DB_Operation.c
Door_Operation.o: Door_Operation.c
	gcc -c Door_Operation.c
Door_Process.o:  Door_Process.c main.h
	gcc -c Door_Process.c
FP_Operation.o: FP_Operation.c
	gcc -c FP_Operation.c
FP_Process.o: FP_Process.c
	gcc -c  FP_Process.c
LCD_Operation.o: LCD_Operation.c
	gcc -c LCD_Operation.c
main.o: main.c
	gcc -c main.c
Wait_Operation.o: Wait_Operation.c
	gcc -c Wait_Operation.c
Ncurses_Operation.o: Ncurses_Operation.c
	gcc -c Ncurses_Operation.c
Sqlite_Operation.o: Sqlite_Operation.c 
	gcc -c Sqlite_Operation.c
sqlite3.o: sqlite3.c
	gcc -c sqlite3.c
Debug_Operation.o: Debug_Operation.c
	gcc -c Debug_Operation.c

clean:
	rm -rf *.o 
	rm -rf main
