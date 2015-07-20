#include"main.h"

void DisplayLCD(WINDOW *win, int iCurrentPageIdx)
{
    //DEBUG_LOG("DisplayLCD(int iOpt);");

    Ncurses_DisplayPage(win,iCurrentPageIdx);
}

void PlayRecord()
{
    
}

// 屏幕显示内容
void DisplayChineseLCD(WINDOW *win, char* str)
{
   	Ncurses_DisplayChinese(win, str, 2);
}

void DisplayMoreLCD(WINDOW *win, char* str1, char* str2, char* str3, char* str4)
{
	Ncurses_DisplayMore(win, str1, str2, str3, str4);
}

