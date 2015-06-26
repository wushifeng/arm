#include"main.h"
void Door_Open(WINDOW* win)
{
    DisplayChineseLCD(win, "开门成功        ");
    DEBUG_LOG("开门成功     ");
    //写入操作日志־
    DB_LogWrite(LOG_TYPE_BUSINESS, "开门成功");
	
    Cam_CatchPicture();
}
