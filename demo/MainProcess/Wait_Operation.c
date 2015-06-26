#include"main.h"

// 等待输入事件
int WaitForInputEvent()
{
   int ret = 0;
   int ch = getch();
   switch(ch)
   {
        case KEY_DOWN:
			ret = INPUT_EVENT_KEY_DOWN;
            DEBUG_LOG("KEY_DOWN          ");
			break;
		case KEY_UP:
			ret = INPUT_EVENT_KEY_UP;
            DEBUG_LOG("KEY_UP            ");
			break;
		case KEY_LEFT:
			ret = INPUT_EVENT_KEY_BACK;
            DEBUG_LOG("KEY_LEFT          ");
			break;
        case KEY_RIGHT:
            ret = INPUT_EVENT_FP;
            DEBUG_LOG("KEY_RIGHT         ");
            break;
   }
   return ret;
}

void WaitAndBack()
{
    
}
