#include "main.h"
extern int iFPErrCount;
bool FP_Match(bool* bFPMatch, char** retUserID)
{
    bool ret = true;
    char *ID = "admin";
    int len = 0;

    char ch = getch();
    if(ch == 'y')
    {
        ret = true;
        DEBUG_LOG("FP_Match true");

        len = sizeof(ID) + 20;
        *retUserID = (char*) malloc(len);
        memset(*retUserID, 0, len);
        strcpy(*retUserID, ID);
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
