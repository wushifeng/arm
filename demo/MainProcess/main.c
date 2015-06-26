#include"main.h"

void Controlinital(WINDOW **win);
void DEBUG_LOG(char* pLog);

WINDOW *debug_win;

// 指纹验证错误总数
int iFPErrCount;
int mainA()
{
/*
	WINDOW* win;
	Ncurses_init();
	win = Ncurses_CreateWindow();
	Ncurses_DisplayPage(win,0,true);
	getch();
	Ncurses_CloseWindow();
*/
Cam_Init();
Cam_CatchPicture();
Cam_Close();
}
int main()
{
     // Ncurses 的窗口对象
    WINDOW *win;

    // 当前页面编号
    int iCurrentPageIdx=STATUS_PAGE_ONE;

    // 页面总数，如果完成指纹匹配，则为 STATUS_PAGE_MAX，否则为 STATUS_PAGE_MAX-1
    int iStatusPageMax;
    
    // 输入事件编号
    int iInputEventIdx;

    // 是否已经完成指纹匹配
    bool bFPMatch = false;
    
    // 设备初始化
    Controlinital(&win);


    // LCD上显示首页
    DisplayLCD(win,STATUS_PAGE_ONE);
    
    while(1)
    {

         // 等待输入事件
        iInputEventIdx = WaitForInputEvent();

        
        
        if(bFPMatch)
        {
            iStatusPageMax = STATUS_PAGE_MAX;
        }
        else
        {
            iStatusPageMax = STATUS_PAGE_MAX - 1;
        }
        switch(iInputEventIdx)
        {
            case INPUT_EVENT_KEY_UP:
                    // 显示上一页
                    if(iCurrentPageIdx > 0)
                    {
                        iCurrentPageIdx = (iCurrentPageIdx + iStatusPageMax - 1) % iStatusPageMax;
                    }
                    DisplayLCD(win, iCurrentPageIdx, bFPMatch);
                    break;
            case INPUT_EVENT_KEY_DOWN:
                    // 显示下一页
                    if(iCurrentPageIdx < iStatusPageMax-1)
                    {
                        iCurrentPageIdx = (iCurrentPageIdx + 1) % iStatusPageMax;
                    }
                    DisplayLCD(win, iCurrentPageIdx);
                    break;
            case INPUT_EVENT_KEY_BACK:
                    // 显示首页
                    iCurrentPageIdx = STATUS_PAGE_ONE;
                    DisplayLCD(win, iCurrentPageIdx);
                    break;
            case INPUT_EVENT_FP:
                    // 处理录入指纹之后的操作，主要流程
                    FP_Process(win, &bFPMatch);
		    if(bFPMatch){
			DEBUG_LOG("指纹 == true");
		    }else{
			DEBUG_LOG("指纹 == false");
		    }
                    break;
            case INPUT_EVENT_NONE:
                    // 显示首页
                    iCurrentPageIdx = STATUS_PAGE_ONE;
                    DisplayLCD(win, iCurrentPageIdx);
                    break;
        }
        
    }
}

void Controlinital(WINDOW **win)
{
     /*变量初始化*/
    iFPErrCount = 0;
    
    /*  Ncurses init 	*/
    Ncurses_init();
    *win = Ncurses_CreateWindow();

    // cam init
	Cam_Init();

    DEBUG_INIT();
    DEBUG_LOG("Controlinital();");
}

void DEBUG_LOG(char* pLog)
{
   /* int len_log = sizeof(pLog);
    int len_space = 18 - len_log;

    char *log = (char*)malloc(18*sizeof(char));
    memset(log,0,18);
    strcat(log,pLog);

    int i;
    for(i=0; i<len_space; i++)
    {
        strcat(log," ");
    }
    
    */
	mvwprintw(debug_win,1,1,pLog);
    wrefresh(debug_win);
    LOG(pLog);
    //getch();
}

void DEBUG_INIT()
{
	int current_page_num;
	int startx, starty, width, height;
	int c;
	
	height = 3;
	width = 50;

	starty = 0;
	startx = 0;
	
	refresh();

	debug_win = newwin(height, width, starty, startx);
	box(debug_win,0,0);
	wrefresh(debug_win);

	refresh();
}


