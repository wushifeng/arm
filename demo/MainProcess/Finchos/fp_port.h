#ifndef __FP_PORT_H
#define __FP_PORT_H

#include "Types.h"

typedef struct{
	struct{
		unsigned char tmr:1;	//定时器溢出标志位
		unsigned char rxc:1;	//串口指令接收完成标志位
		unsigned char mode:1; //0-指令传送模式/1-批量数据传送模式
		unsigned char pwr:1;	//电源状态, 0-关闭,1-开启
		unsigned char res :5;	//保留
	}flag;									//标志组
}DevifTypeDef;


extern DevifTypeDef devif;



/*******************函数声明*****************/
void SendByte(uint8_t ch);
uint8_t ReciveByte(void);
void StartTimer(uint16_t t);
void CloseTimer(void);
void FP_PowerOn(void);
void FP_PowerOff(void);
uint8_t Userhook(void *arg);


#endif

