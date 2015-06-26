#include"main.h"

void DisplayLCD(WINDOW *win, int iCurrentPageIdx, bool bFPMatch)
{
    //DEBUG_LOG("DisplayLCD(int iOpt);");

    Ncurses_DisplayPage(win,iCurrentPageIdx, bFPMatch);
}

void PlayRecord()
{
    
}

// 屏幕显示内容
void DisplayChineseLCD(WINDOW *win, char* str)
{
   Ncurses_DisplayChinese(win, str);
}

