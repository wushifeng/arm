#include "main.h"
extern int iFPErrCount;
bool bFPOpen;

void FP_Init()
{
    int ret ;
    ret = Finchos_Open();
    if(ret < 0)
    {
        bFPOpen = false;
        DEBUG_LOG("指纹仪打开失败！");
        return ;
    }
    bFPOpen = true;
    DEBUG_LOG("指纹仪打开成功！");
}


bool FP_Match_OP(bool* bFPMatch, char** retUserID)
{
    char log[20];
    bool ret = true;
    char *ID = NULL;
    int len = 0;
    int fpID ;
    
    fpID = VerifyUser(); // fpID = 22;
    sprintf(log,"FP ID = %d",fpID);
    DEBUG_LOG(log);

    // 指纹匹配失败
    if( fpID < 0 )
    {
        ret = false;
        DEBUG_LOG("FP_Match false");
    }
    else
    {

        // 根据指纹id获取对应的用户id
        if( DB_GetUserIDByFPID(fpID, &ID) )
        {
            ret = true;
            DEBUG_LOG("FP_Match true");

            sprintf(log,"User ID = %s",ID);
            DEBUG_LOG(log);

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
