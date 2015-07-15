#ifndef __FP_PORT_H
#define __FP_PORT_H

#include "Types.h"

typedef struct{
	struct{
		unsigned char tmr:1;	//��ʱ�������־λ
		unsigned char rxc:1;	//����ָ�������ɱ�־λ
		unsigned char mode:1; //0-ָ���ģʽ/1-�������ݴ���ģʽ
		unsigned char pwr:1;	//��Դ״̬, 0-�ر�,1-����
		unsigned char res :5;	//����
	}flag;									//��־��
}DevifTypeDef;


extern DevifTypeDef devif;



/*******************��������*****************/
void SendByte(uint8_t ch);
uint8_t ReciveByte(void);
void StartTimer(uint16_t t);
void CloseTimer(void);
void FP_PowerOn(void);
void FP_PowerOff(void);
uint8_t Userhook(void *arg);


#endif

