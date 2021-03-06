#include"main.h"
extern bool bCamOpen;
void Cam_Init()
{
	if(pzInit(0,NULL)<0)
	{
		bCamOpen = false;
		DEBUG_LOG("摄像头初始化失败");
		return;
	}
	bCamOpen = true;
	DEBUG_LOG("摄像头初始化成功");
}

void Cam_Close()
{
	pzClose();
	DEBUG_LOG("摄像头关闭");
}

void Cam_CatchPicture()
{
	if(!bCamOpen)
	{
		return;
	}
	char* pic_name = NULL;
	pz(&pic_name);
	DEBUG_LOG("抓拍图像");
	DEBUG_LOG(pic_name);

	if(pic_name)
	{
		free(pic_name);
		pic_name=NULL;
	}
}

void Cam_CatchPicture_BMP()
{
	// if(!bCamOpen)
	// {
	// 	return;
	// }
	// char* pic_name = NULL;
	// pz(&pic_name);
	// DEBUG_LOG("抓拍图像");
	// DEBUG_LOG(pic_name);

	// if(pic_name)
	// {
	// 	free(pic_name);
	// }
	pz_bmp();
}