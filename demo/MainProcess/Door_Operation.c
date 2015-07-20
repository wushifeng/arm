#include"main.h"
void Door_Open(WINDOW* win, char* pUserID)
{
	
    char* pLogContent = NULL;
    DisplayChineseLCD(win, "开门成功        ");
    DEBUG_LOG("开门成功     ");
    
    // 生成操作日志
    Log_GenerateYW(pUserID, LOG_ACTION_TYPE_OPEN_DOOR, LOG_RESULT_TYPE_SUCCESS, &pLogContent);
    
    //写入操作日志־
    DB_LogWrite(LOG_TYPE_BUSINESS, pLogContent);
	
    // Cam_CatchPicture();
    Cam_CatchPicture_BMP();
}
