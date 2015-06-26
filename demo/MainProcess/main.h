#include<stdio.h>
#include<ncurses/menu.h>
#include<locale.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>



#define STATUS_PAGE_MAX           3   // 总共3页内容

#define STATUS_PAGE_ONE         0   // 首页
#define STATUS_PAGE_TWO         1   // 下一页
#define STATUS_PAGE_THR         2   // 上一页
#define STATUS_PAGE_CHOISE      3   // 功能选择页面


// 输入事件编号
#define INPUT_EVENT_NONE          0   // 无操作
#define INPUT_EVENT_KEY_UP        1   // 按上一页键
#define INPUT_EVENT_KEY_DOWN      2   // 按下一页键
#define INPUT_EVENT_KEY_BACK      3   // 按返回键
#define INPUT_EVENT_FP            4   // 指纹录入

#define DB_TABLE_NAME_CONFIG          "ywsetting" // 业务设置表
#define CONFIG_NAME_APPROVAL_MODEL    "zxsp" // 监管模式
#define CONFIG_NAME_FP_MODEL          "yzms"

#define CONFIG_FP_MODEL_ONE     "1"
#define CONFIG_FP_MODEL_TWO     "2"
#define CONFIG_FP_MODEL_THR     "3"

#define CONFIG_APPROVAL_MODEL_ON    "on"
#define CONFIG_APPROVAL_MODEL_OFF   "off"


#define USER_OPERATION_STATUS_NONE  1   // 用户操作状态为无
#define USER_OPERATION_STATUS_WAIT  2   // 用户操作状态为等待审批
#define USER_OPERATION_STATUS_PASS  3   // 用户操作状态为通过审批

#define USER_PERMISSION_OPEN        1   // 开柜权限
#define USER_PERMISSION_LOG         2   // 日志审计权限

#define LOG_TYPE_BUSINESS           1   // 业务日志类别
#define LOG_TYPE_ALARM              2   // 报警日志类别
#define LOG_TYPE_MANAGER            3   // 管理日志类别

#define LOG_TABLE_BUSINESS          "ywlog"
#define LOG_TABLE_ALARM             "bjlog"
#define LOG_TABLE_MANAGER           "gllog"
