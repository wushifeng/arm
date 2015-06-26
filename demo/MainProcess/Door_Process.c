#include"main.h"
#include<stdbool.h>

void Door_Process(WINDOW* win, char* pUserID)
{
    char* pFPModel="";
    // 首先获取配置信息  指纹模式
    DB_GetConfigInfo(CONFIG_NAME_FP_MODEL, &pFPModel);
    if(strcmp(pFPModel,CONFIG_FP_MODEL_ONE) == 0)
    {
            DEBUG_LOG("FPModel == 单指纹模式  ");
            OneFPModelProcess(win, pUserID);
    }
    else if(strcmp(pFPModel,CONFIG_FP_MODEL_TWO) == 0)
    {
            DEBUG_LOG("FPModel == 双指纹模式  ");
            TwoFPModelProcess();
    }
    else if(strcmp(pFPModel,CONFIG_FP_MODEL_THR) == 0)
    {
            DEBUG_LOG("FPModel == 三指纹模式  ");
            ThrFPModelProcess();
    }
    else
    {
        
            DEBUG_LOG("FPModel == error    ");
            DEBUG_LOG(pFPModel);
            printf("++%d=+=+=%d++", strlen(pFPModel),strlen(CONFIG_FP_MODEL_ONE));
            getch();
    }
}

void OneFPModelProcess(WINDOW* win, char* pUserID)
{
    char* pApprovalModel = "";
    int iInputEventIdx;
    bool bFPMatchResult;
    // 首先获取配置信息  远程审批模式
    DB_GetConfigInfo(CONFIG_NAME_APPROVAL_MODEL, &pApprovalModel);

    // 如果开启了远程审批模式
    if(strcmp(pApprovalModel,CONFIG_APPROVAL_MODEL_ON) == 0)
    {
        DEBUG_LOG("远程审批模式 == on");
    }
    
    // 如果没开启远程审批模式
    else
    {
        DEBUG_LOG("远程审批模式 == off");
        /*
        // 等待指纹输入
        iInputEventIdx = WaitForInputEvent();
        switch(iInputEventIdx)
        {
            case INPUT_EVENT_FP:
                    // 输入指纹匹配
                    bFPMatchResult = FP_Match();
                    if(bFPMatchResult)
                    {
                        Door_Open(win);
                    }
                    else
                    {
                        // 匹配错误提示
                        FP_ErrorAlarm(win);
                    }
                    break;
            default:
                    // 返回状态首页
                    DisplayLCD(win, STATUS_PAGE_ONE);
                    break;
        }
        */

        
        // 直接开柜
        Door_Open(win, pUserID);
    }
}

void TwoFPModelProcess()
{
}
void ThrFPModelProcess()
{
}
