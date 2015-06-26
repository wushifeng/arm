#include"main.h"

void Cam_Init()
{
	pzInit(0,NULL);
	DEBUG_LOG("摄像头初始化");
}

void Cam_Close()
{
	pzClose();
	DEBUG_LOG("摄像头关闭");
}

void Cam_CatchPicture()
{
	char* pic_name = NULL;
	pz(&pic_name);
	DEBUG_LOG("抓拍图像");
	DEBUG_LOG(pic_name);

	if(pic_name)
	{
		free(pic_name);
	}
}
