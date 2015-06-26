#include"main.h"
#include<stdbool.h>


void FP_Process(WINDOW *win, bool *bFPMatch)
{
    bool bFPMatchResult;
    bool bValidateApprovalTime;
    int  iUserOperationStatus;
    int  iInputEventIdx;
    bFPMatchResult = FP_Match(bFPMatch);

    // 如果匹配成功
    if(bFPMatchResult)
    {
        // 将指纹输入错误次数清零
        FP_ErrorClear();
        
        // 首先获取改用户操作状态
        iUserOperationStatus = DB_GetUserOperationStatus();
        
        // 如果已经通过远程审批，并且在有效期内，直接开柜，语音提示
        // 如果等待审批，提示等待审批
        switch(iUserOperationStatus)
        {
            case USER_OPERATION_STATUS_PASS:

                    // 检查审批结果是否过期
                    bValidateApprovalTime = DB_CheckApprovalAvailable();

                    // 如果仍然有效，直接开门
                    if(bValidateApprovalTime)
                    {
        				//调语音提示：验证通过直接开柜
        				PlayRecord("验证通过直接开柜");
                        
                        // LCD显示:  开柜成功
                        DisplayChineseLCD(win,"开柜成功");
                        
        				//写入操作日志
        				DB_LogWrite(LOG_TYPE_BUSINESS, "开柜成功开柜成功");

                        // 开柜
                        Door_Open();
                    }

                    // 如果失效
                    else
                    {
        				//调语音提示：验证通过直接开柜
        				PlayRecord("验证失效");
                        
                        // LCD显示:  验证失效
                        DisplayChineseLCD(win,"验证失效");

                        // 等待n秒后返回首页或者点击返回按钮返回首页
                        WaitAndBack();
                    }
                    break;
            case USER_OPERATION_STATUS_WAIT:
                
        			//调语音提示：等待审批
                    PlayRecord("等待审批");

                    // LCD显示:  等待审批
                    DisplayChineseLCD(win,"等待审批");

                    // 等待n秒后返回首页或者点击返回按钮返回首页
                    WaitAndBack();
                    
                    break;
                    
            case USER_OPERATION_STATUS_NONE:
                    // 显示功能选择页面    
                    DisplayChineseLCD(win,"功能选择");
                    
                    // 获取用户权限
                    DB_GetUserPermission();

                    // 等待输入事件
                    iInputEventIdx = WaitForInputEvent();
                                
                    switch(iInputEventIdx)
                    {
                        case INPUT_EVENT_KEY_UP:
                                // 选择开柜功能
                                DEBUG_LOG("选择开柜功能");
                                Door_Process(win);
                                break;
                        case INPUT_EVENT_KEY_DOWN:
                                // 选择审计功能
                                //DisplayChineseLCD(win,"choice shenji");
                                break;
                        case INPUT_EVENT_KEY_BACK:
                                // 返回状态首页
                                DisplayLCD(win,STATUS_PAGE_ONE,bFPMatchResult);
                                
                                break;
                        case INPUT_EVENT_FP:
                                // 继续输入指纹
                                
                                break;
                        case INPUT_EVENT_NONE:
                                // 返回状态首页
                                DisplayLCD(win,STATUS_PAGE_ONE,bFPMatchResult);
                                break;
                    }
                    
                    break;
        }
    }
    // 如果指纹匹配失败
    else
    {
        // 指纹匹配错误报警，小于等于3次，语音LCD提示，大于三次扬声器报警
        FP_ErrorAlarm(win);
    }
}

