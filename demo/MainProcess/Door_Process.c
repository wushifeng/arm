#include"main.h"
#include<stdbool.h>

void Door_Process(WINDOW* win, char* pUserID, unsigned char cUserPermission)
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
            TwoFPModelProcess(win, pUserID, cUserPermission);
    }
    else if(strcmp(pFPModel,CONFIG_FP_MODEL_THR) == 0)
    {
            DEBUG_LOG("FPModel == 三指纹模式  ");
            ThrFPModelProcess(win, pUserID, cUserPermission);
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

void TwoFPModelProcess(WINDOW* win, char* pUserID, unsigned char cUserPermission)
{
    char log[80] = "";
    char* pSuperviseModel = ""; 
    char* pApprovalModel = "";
    char* pUser2ID ;
    unsigned char cUser2Permission ;
    bool bFPMatchResult ;

    // 首先获取配置信息  监管开柜模式
    DB_GetConfigInfo(CONFIG_NAME_SUPERVISE_MODEL, &pSuperviseModel);

    // 首先获取配置信息  远程审批模式
    DB_GetConfigInfo(CONFIG_NAME_APPROVAL_MODEL, &pApprovalModel);

RE_FP_MATCH:
    bFPMatchResult = false;
    pUser2ID = NULL;
    cUser2Permission = PERMISSION_NULL;
    // 首先验证第二个人的指纹
    bFPMatchResult = FP_Match_OP(NULL, &pUser2ID);

    // 如果两次验证指纹的编号相同，即同一个人，要求重新验证
    if(bFPMatchResult && (strcmp(pUser2ID, pUserID) == 0))
    {
        DEBUG_LOG("两次验证为同一用户，重新验证");
        if(FP_ErrorAlarm(win)<0)
            goto END;
        goto RE_FP_MATCH;
    }
    // 指纹验证通过，第二人的编号存入 pUser2ID 中
    else if(bFPMatchResult)
    {

        // 指纹错误清零
        FP_ErrorClear();
        DEBUG_LOG("第二个用户指纹匹配成功");
        memset(log,0,sizeof(log));
        sprintf(log,"第二个指纹用户为%s；第一个是%s",pUser2ID, pUserID);
        DEBUG_LOG(log);
        // 如果开启了监管开柜模式
        if(strcmp(pSuperviseModel,CONFIG_SUPERVISE_MODEL_ON) == 0)
        {

            DEBUG_LOG("开启了监管开柜模式");
            // 首先获取第二人的权限, 获取的权限存入 cUser2Permission 中
            DB_GetUserPermission(pUser2ID, &cUser2Permission);

            // 如果两人中包含至少一个拥有监管开柜权限
            if( (cUserPermission & PERMISSION_SUPERVISE_OPEN) || (cUser2Permission & PERMISSION_SUPERVISE_OPEN) )
            {
                DEBUG_LOG("监管开柜模式校验成功");
            }

            // 如果两人都没有监管开柜权限，则开柜失败
            else
            {

                DisplayMoreLCD(win, "      ","  开柜失败    ", "无监管开柜权限","     ");
                DEBUG_LOG("监管开柜模式校验失败");
                return;
            }
        }

        // 如果没有开启监管开柜模式
        else
        {

            DEBUG_LOG("没有开启监管开柜模式");
        }

        // 如果开启了远程审批模式
        if(strcmp(pApprovalModel,CONFIG_APPROVAL_MODEL_ON) == 0)
        {

            DEBUG_LOG("远程审批模式 == on");
        }
        // 如果没有开启远程审批模式，直接开柜
        else
        {

            DEBUG_LOG("远程审批模式 == off");
            Door_Open(win, pUserID);
        }


    }

    // 第二人指纹验证失败， 重新进行验证
    else
    {
        DEBUG_LOG("第二个用户指纹匹配失败");
        if(FP_ErrorAlarm(win)<0)
            goto END;
        FREE(pUser2ID);
        goto RE_FP_MATCH;
    }

END:
    return;

}
void ThrFPModelProcess(WINDOW* win, char* pUserID, unsigned char cUserPermission)
{
    char log[80] = "";
    char* pSuperviseModel = ""; 
    char* pApprovalModel = "";
    char* pUser2ID;
    char* pUser3ID;
    unsigned char cUser2Permission ;
    unsigned char cUser3Permission ;
    bool bFPMatchResult ;
    bool bFPMatchResult3 ;

    // 首先获取配置信息  监管开柜模式
    DB_GetConfigInfo(CONFIG_NAME_SUPERVISE_MODEL, &pSuperviseModel);

    // 首先获取配置信息  远程审批模式
    DB_GetConfigInfo(CONFIG_NAME_APPROVAL_MODEL, &pApprovalModel);

RE_FP_MATCH:
    bFPMatchResult = false;
    pUser2ID = NULL;
    cUser2Permission = PERMISSION_NULL;
    // 首先验证第二个人的指纹
    bFPMatchResult = FP_Match_OP(NULL, &pUser2ID);

    // 如果两次验证指纹的编号相同，即同一个人，要求重新验证
    if(bFPMatchResult && (strcmp(pUser2ID, pUserID) == 0))
    {
        DEBUG_LOG("用户重复验证");
        // 当错误次数超过一定次数，报警并返回
        if(FP_ErrorAlarm(win)<0)
            goto END;
        FREE(pUser2ID);
        goto RE_FP_MATCH;
    }
    // 指纹验证通过，第二人的编号存入 pUser2ID 中
    else if(bFPMatchResult)
    {

        // 指纹错误清零
        FP_ErrorClear();
        DEBUG_LOG("第二个用户指纹匹配成功");
        memset(log,0,sizeof(log));
        sprintf(log,"第二个指纹用户为%s；第一个是%s",pUser2ID, pUserID);
        DEBUG_LOG(log);


RE_FP_MATCH_3:
        bFPMatchResult3 = false;
        pUser3ID = NULL;
        cUser3Permission = PERMISSION_NULL;
        // 首先验证第三个人的指纹
        bFPMatchResult3 = FP_Match_OP(NULL, &pUser3ID);

        // 如果第三个人的编号和前两个中的任意一个相同，要求重新验证第三个人
        if(bFPMatchResult3 && ( (strcmp(pUser3ID, pUserID) == 0) || (strcmp(pUser3ID, pUser2ID) == 0) ))
        {
            DEBUG_LOG("用户重复验证");
            // 当错误次数超过一定次数，报警并返回
            if(FP_ErrorAlarm(win)<0)
                goto END;
            FREE(pUser3ID);
            goto RE_FP_MATCH_3;
        }
        // 指纹验证通过，第三人的编号存入 pUser3ID 中
        else if(bFPMatchResult3)
        {
            // 指纹错误清零
            FP_ErrorClear();
            DEBUG_LOG("第三个用户指纹匹配成功");
            memset(log,0,sizeof(log));
            sprintf(log,"第三个指纹用户为%s；第一个是%s；第二个是%s",pUser3ID, pUserID, pUser2ID);
            DEBUG_LOG(log);

             // 如果开启了监管开柜模式
            if(strcmp(pSuperviseModel,CONFIG_SUPERVISE_MODEL_ON) == 0)
            {

                DEBUG_LOG("开启了监管开柜模式");
                // 首先获取第二人的权限, 获取的权限存入 cUser2Permission 中
                DB_GetUserPermission(pUser2ID, &cUser2Permission);
                // 首先获取第三人的权限, 获取的权限存入 cUser3Permission 中
                DB_GetUserPermission(pUser3ID, &cUser3Permission);

                // 如果三人中包含至少一个拥有监管开柜权限
                if( (cUserPermission & PERMISSION_SUPERVISE_OPEN) || (cUser2Permission & PERMISSION_SUPERVISE_OPEN) || (cUser3Permission & PERMISSION_SUPERVISE_OPEN) )
                {
                    DEBUG_LOG("监管开柜模式校验成功");
                }

                // 如果三人都没有监管开柜权限，则开柜失败
                else
                {

                    DisplayMoreLCD(win, "      ","  开柜失败    ", "无监管开柜权限","     ");
                    DEBUG_LOG("监管开柜模式校验失败");
                    return;
                }
            }

            // 如果没有开启监管开柜模式
            else
            {

                DEBUG_LOG("没有开启监管开柜模式");
            }

            // 如果开启了远程审批模式
            if(strcmp(pApprovalModel,CONFIG_APPROVAL_MODEL_ON) == 0)
            {

                DEBUG_LOG("远程审批模式 == on");
            }
            // 如果没有开启远程审批模式，直接开柜
            else
            {

                DEBUG_LOG("远程审批模式 == off");
                Door_Open(win, pUserID);
            }
        }

        // 第三人指纹验证失败， 重新进行验证
        else
        {

            DEBUG_LOG("第三个用户指纹匹配失败");
            // 当错误次数超过一定次数，报警并返回
            if(FP_ErrorAlarm(win)<0)
                goto END;
            FREE(pUser3ID);
            goto RE_FP_MATCH_3;
        }

       

    }

    // 第二人指纹验证失败， 重新进行验证
    else
    {
        DEBUG_LOG("第二个用户指纹匹配失败");
        // 当错误次数超过一定次数，报警并返回
        if(FP_ErrorAlarm(win)<0)
            goto END;
        FREE(pUser2ID);
        goto RE_FP_MATCH;
    }
END:
    return;
}
