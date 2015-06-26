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
   Ncurses_DisplayChinese(win, str);
}

