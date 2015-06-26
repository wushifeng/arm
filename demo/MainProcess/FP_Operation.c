#include "main.h"
extern int iFPErrCount;
bool FP_Match(bool* bFPMatch)
{
    bool ret = true;

    char ch = getch();
    if(ch == 'y')
    {
        ret = true;
        DEBUG_LOG("FP_Match true");
    }
    else
    {
        ret = false;
        DEBUG_LOG("FP_Match false");
    }
    *bFPMatch = ret;
    return ret;
}

void FP_ErrorClear()
{
    iFPErrCount = 0;
    DEBUG_LOG("iFPErrCount clear");
}

void FP_ErrorAlarm(WINDOW* win)
{
    iFPErrCount ++;
    if(iFPErrCount <= 3)
    {
        PlayRecord("FPMatch Error");
        DisplayChineseLCD(win, "FPMatch Error");
    }
    else
    {
        // 扬声器报警
        DEBUG_LOG("ALARM!!!!FPMatch Error Too Much Times!!!");
    }
}
