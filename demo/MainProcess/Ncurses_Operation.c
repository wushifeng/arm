#include "main.h"
#include<string.h>

void Ncurses_DisplayPage(WINDOW *local_win,int page_num)
{
	Ncurses_ClearWindow(local_win);
	switch(page_num){
		case STATUS_PAGE_ONE:	
			mvwprintw(local_win,1,1,"   2015-03-16   ");
			mvwprintw(local_win,2,1,"    15:25:25    ");
			mvwprintw(local_win,3,1,"                ");
			mvwprintw(local_win,4,1,"       0        ");
			break;
		case STATUS_PAGE_TWO:	
			mvwprintw(local_win,1,1,"192.168.0.1S    ");
			mvwprintw(local_win,2,1,"192.168.0.2L    ");
			mvwprintw(local_win,3,1,"                ");
			mvwprintw(local_win,4,1,"       1        ");
			break;
		case STATUS_PAGE_THR:
			mvwprintw(local_win,1,1,"业务报警: 0     ");
			mvwprintw(local_win,2,1,"故障报警: 0     ");
			mvwprintw(local_win,3,1,"事件报警: 0     ");
			mvwprintw(local_win,4,1,"       2        ");
			break;
	}
	wrefresh(local_win);	
}


void Ncurses_DisplayChinese(WINDOW *local_win,char* str)
{
	Ncurses_ClearWindow(local_win);
	mvwprintw(local_win,1,1,"                ");
	mvwprintw(local_win,2,1,"%s",str);
	mvwprintw(local_win,3,1,"                ");
	mvwprintw(local_win,4,1,"       2        ");
			
	wrefresh(local_win);	
}

void Ncurses_init()
{
 
	setlocale(LC_ALL,"");
	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
}

WINDOW * Ncurses_CreateWindow()
{
	WINDOW *my_win;
	int current_page_num;
	int startx, starty, width, height;
	int c;
	
	height = 7;
	width = 18;
	current_page_num = 0;

	starty = (LINES-height)/2;
	startx = (COLS-width)/2;
	
	refresh();

	my_win = newwin(height, width, starty, startx);
	box(my_win,0,0);
	wrefresh(my_win);

	refresh();

    return my_win;

}

void Ncurses_CloseWindow()
{
	endwin();
}

void Ncurses_ClearWindow(WINDOW *local_win)
{
	mvwprintw(local_win,1,1,"                ");
	mvwprintw(local_win,2,1,"                ");
	mvwprintw(local_win,3,1,"                ");
	mvwprintw(local_win,4,1,"                ");
			
	wrefresh(local_win);	
}


