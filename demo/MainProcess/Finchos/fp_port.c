/************************************************************************************************
文件:fp_port.c
描述:
		该文件为指纹传感器通信协议的移植接口函数,用户在将该协议移植到字节的目标板上时,需要完成本文件
中的接口函数的编写.具体功能参考对应函数的注释,其中函数Userhook用户可根据自己的需求决定是否实现.

*************************************************************************************************/


/*******************************************头文件***********************************************/
#include "fp_port.h"


/*****************************************全局变量申明*******************************************/
DevifTypeDef devif;


/****************************************************************************
*函数:SendByte
*入参: ch-需发送的数据
*返回: 无
*功能: 通过通信接口发送一个字节的数据
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
void SendByte(uint8_t ch)
{
	//USART1_SentByte(ch);
}

/****************************************************************************
*函数:ReciveByte
*入参: 无
*返回: 收到的数据
*功能: 通过通信接口接收一个字节的数据
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
//uint8_t ReciveByte(void)
//{
////	return (uint8_t)(USART1->DR);
//	return 0;
//}

/****************************************************************************
*函数:StartTimer
*入参: 定时时间(非必要参数),预留给用户可能需要传递参数时使用
*返回: 无
*功能: 启动定时器
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
void StartTimer(uint16_t t)
{
	devif.flag.tmr=0;
//	TIM2_Open();
}

/****************************************************************************
*函数:CloseTimer
*入参: 无
*返回: 无
*功能: 关闭定时器
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
void CloseTimer(void)
{
	devif.flag.tmr=0;
//	TIM2_Close();
}


/****************************************************************************
*函数:FP_PowerOn
*入参: 无
*返回: 无
*功能: 使能传感器电源(3.3V),传感器电源稳定需要不少于20ms的时间为宜
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
void FP_PowerOn(void)
{ 
	
	if(!devif.flag.pwr){
//		PowerOn();
		devif.flag.pwr = 1;
//		Delay(2);//20ms 延时以使传感器稳定工作
	}
}

/****************************************************************************
*函数:FP_PowerOff
*入参: 无
*返回: 无
*功能: 关闭传感器电源(3.3V)
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
void FP_PowerOff(void)
{
	//PowerOff();
	devif.flag.pwr = 0; 
}

/****************************************************************************
*函数:Userhook
*入参: arg-参数参数指针,预留给用户可能需要传递参数时使用
*返回: 0x01-用户中断,0-无操作,
*功能: 钩子函数,用户可在该函数中添加功能,以实现某种目的,示例代码中该函数通过
*			 检测对应按键来中断当前操作,实现从while(1){}中退出之目的
*作者:Yangguo
*时间: 2015-3-24
*****************************************************************************/
uint8_t Userhook(void *arg){
//	return UserKey();
}


/*********************************************end***********************************************/
