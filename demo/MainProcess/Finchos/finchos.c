#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "Finchos_V3.h"


#define BAUDRATE        B115200
#define UART_DEVICE     "/dev/ttyUSB0"

#define FALSE  -1
#define TRUE   0
#define uint8_t unsigned char

#define IO_CTL_MAGIC  'Z'
#define SET_TSC_PWRON _IOW(IO_CTL_MAGIC,  1,int)
#define SET_TSC_PWROFF _IOW(IO_CTL_MAGIC,  2,int)

typedef struct{
	uint16_t vol; //传感器指纹容量
	uint16_t fp;	//已注册指纹数目
}sensor_t;

sensor_t SensorInf;

/* 指纹模块指令*/
unsigned char cmd_gather[] = { 0x3a, 0x02, 0xaa, 0x00, 0x00, 0x00, 0x92, 0x87 };
unsigned char cmd_query[] = { 0x3a, 0x01, 0x00, 0x20, 0x00, 0x04, 0x1f, 0x00,
		0x00, 0x00, 0x63, 0x1e };
unsigned char cmd_get_first_pkt[] = { 0x3a, 0x06, 0x80, 0x00, 0x00, 0x04, 0xb8,
		0x00, 0x00, 0x01, 0x00, 0x82 };
unsigned char cmd_irq[] = { 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x8b };
unsigned char cmd_match[]={0x3a,0x04,0xa6,0x80,0x00,0x04,0x1c,0x00,0x00,0x00,0x63,0x18};
//unsigned char request_buf[]={'d','u','h','o','n','g','y','u',' ','.','.','.','?'};

int speed_arr[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
		B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = { 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300, 115200,
		38400, 19200, 9600, 4800, 2400, 1200, 300, };

int fd;

/*************************************************************
 *@brief  设置串口通信速率
 *@param  fd     类型 int  打开串口的文件句柄
 *@param  speed  类型 int  串口速度
 *@return  void
 **************************************************************/
void set_speed(int fd, int speed) {
	int i;
	int status;
	struct termios Opt;
	tcgetattr(fd, &Opt);
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (speed == name_arr[i]) {
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if (status != 0) {
				perror("tcsetattr fd1");
				return;
			}
			tcflush(fd, TCIOFLUSH);
		}
	}
}

/*************************************************************
 *@brief   设置串口数据位，停止位和效验位
 *@param  fd     类型  int  打开的串口文件句柄
 *@param  databits 类型  int 数据位   取值 为 7 或者8
 *@param  stopbits 类型  int 停止位   取值为 1 或者2
 *@param  parity  类型  int  效验类型 取值为N,E,O,,S
 **************************************************************/
int set_Parity(int fd, int databits, int stopbits, int parity) {
	struct termios options;
	if (tcgetattr(fd, &options) != 0) {
		perror("SetupSerial 1");
		return (FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "Unsupported data size\n");
		return (FALSE);
	}
	switch (parity) {
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB; /* Clear parity enable */
		options.c_iflag &= ~INPCK; /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB; /* Enable parity */
		options.c_cflag &= ~PARODD; /* 转换为偶效验*/
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 'S':
	case 's': /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported parity\n");
		return (FALSE);
	}
	/* 设置停止位*/
	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported stop bits\n");
		return (FALSE);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	if (tcsetattr(fd, TCSANOW, &options) != 0) {
		perror("SetupSerial 3");
		return (FALSE);
	}
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
	options.c_oflag &= ~OPOST; /*Output*/
	return (TRUE);
}

/*************************************************************
 *@brief   指纹模块电源开，给指纹模块上电
 **************************************************************/
/*
void PowerOn(void) {
	int fd;
	fd = open("/dev/io_ctl", O_RDWR);
	if (fd < 0) {
		printf("open device /dev/io_ctl error!\n");
		return;
	}

	if (ioctl(fd, SET_TSC_PWRON, 0) < 0) {
		perror("ioctl SET_TSC_PWRON error");
	}

	close(fd);
	return;
}
*/
/*************************************************************
 *@brief   指纹模块电源关，给指纹模块下电
 **************************************************************/
/*
void PowerOff(void) {
	int fd;
	fd = open("/dev/io_ctl", O_RDWR);
	if (fd < 0) {
		printf("open device /dev/io_ctl error!\n");
		return;
	}

	if (ioctl(fd, SET_TSC_PWROFF, 0) < 0) {
		perror("ioctl SET_TSC_PWROFF error");
	}
	close(fd);
	return;
}
*/
/*************************************************************
 *@brief   设置UART1口波特率
 **************************************************************/
int USART1_Config(void) {
	//printf("Start...\n");
	fd = open(UART_DEVICE, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror(UART_DEVICE);
		sleep(1);
		//exit(1);
		DEBUG_LOG("FP open fail");
		return -1;
	}

	DEBUG_LOG("FP open success");
	//printf("Open...\n");
	set_speed(fd, 115200);
	if (set_Parity(fd, 8, 1, 'N') == FALSE) {
		DEBUG_LOG("Set Parity Error");
		//exit(0);
		return -1;
	}
	return 0;
}


/************************************************
函数名称 ： Disp_String12
功    能 ： 显示字符串(8x16、12x16)
参    数 ： Page ----- 页
            Col ------ 列
            pHZ ------ 字符指针
            Invert --- 反面
返 回 值 ： 无
作    者 ： Huang Fugui
*************************************************/
void Disp_String12(uint8_t Page, uint8_t Col, uint8_t *pStr)
{
	DEBUG_LOG(pStr);
}

void GetSensorInf()
{
	char log[20];
	FPRESULT res;
	res = FP_GetQuatity(0,99,&SensorInf.fp);
	sprintf(log,"fp count = %d\n", SensorInf.fp);
	DEBUG_LOG(log);
}
/****************************************************************************
*函数：VerifyUser
*入参: 无
*返回: 无
*功能: 指纹验证
*作者：YangGuo
*时间: 2015-3-16
****************************************************************************/
int VerifyUser(void)
{
	char log[20];
	uint16_t fp;
	FPRESULT res;
	Disp_String12(6,8,"验证指纹");
	res=FP_Match(0,99,&fp);
	if(res==FPR_OK)
	{
		Disp_String12(6,8,"指纹ID:");
		sprintf(log,"%d\n", fp);
		DEBUG_LOG(log);
		return (int)fp;
	}
	else if(res==FPR_USER_INTR)
	{
		Disp_String12(3,32,"终止操作");
		return -1;
	}
	else if(res==FPR_TIME_OUT){
		Disp_String12(3,48,"超时");
		return -1;
	}
	else{

		DEBUG_LOG("fp match failed\n");
		return -1;
	}
//	Mainmenu();
}


/****************************************************************************
*函数：EnrollUser
*入参: 无
*返回: 无
*功能: 用户注册
*作者：YangGuo
*时间: 2015-3-16
****************************************************************************/
void EnrollUser(void){
		uint16_t fp;
		FPRESULT res;

		Disp_String12(6,8,"注册指纹");
		res=FP_Process(0+1,99,&fp);
		if(res==FPR_OK)
		{
			SensorInf.fp++;
			Disp_String12(6,8,"注册ID:");
		}
		else if(res==FPR_USER_INTR)
		{
			Disp_String12(3,24,"已终止操作");
		}
		else if(res==FPR_TIME_OUT){
			Disp_String12(3,48,"超时");
		}
		else{

		}
//		Mainmenu();
}


/****************************************************************************
*函数：DeleteUser
*入参: 无
*返回: 无
*功能: 删除功能选择
*作者：YangGuo
*时间: 2015-3-16
****************************************************************************/
void DeleteUser(void){
	int res;
	if((res=FP_Delete(0,99))==FPR_OK){
				SensorInf.fp=0;
				Disp_String12(3,24,"指纹已清空");
	}
}


/****************************************************************************
*函数：ReadImage
*入参: 无
*返回: 无
*功能: 获取指纹传感器信息
*作者：YangGuo
*时间: 2015-3-16
****************************************************************************/
void ReadImage(void){
	FPRESULT res;
	uint8_t buff[272];


	Disp_String12(3,40,"......");
	res=FP_ReadImage(NULL,buff);
	if(res==FPR_OK){
		Disp_String12(3,24,"采图成功");
	}
	else if(res==FPR_USER_INTR)
	{
		Disp_String12(3,32,"终止操作");
	}
	else if(res==FPR_TIME_OUT){
		Disp_String12(3,48,"超时");
	}
	else{

	}
}

/**
 * 打开指纹仪
 */
int Finchos_Open()
{
	int ret ;
	ret = USART1_Config();
	if(ret < 0)
	{
		return -1;
	}

	SensorInit();
	return 0;
}

/**
 * 关闭指纹仪
 */
void Finchos_Close()
{
	close(fd);
}

int finchos() {
	

	// GetSensorInf();
//	EnrollUser();//注册
//	ret = VerifyUser();//验证
//	DeleteUser();//删除
//	ReadImage();
//	printf("close\n");

//	return ret;
	return 0;
}
