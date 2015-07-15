#ifndef __FINCHOS_V3_H
#define __FINCHOS_V3_H

#include "Types.h"

#define EXTENSION_FIELD				12	//扩展域的长度
#define FP_CACHE_SIZE					(EXTENSION_FIELD+8)//串口缓存的长度

/********************V3协议数据结构体***********************/
typedef struct{
	uint8_t 	dir;	//指令
	uint8_t   XOR;	//异或校验
	uint8_t   SUM;	//和校验

	uint16_t  arg;	//参数
	uint16_t  len;	//包长
	uint16_t  cnt;	//计数器
	uint16_t  size; //单包数据长度

	uint8_t *pext;	//扩展域的指针
	uint8_t *pbuf;	//帧缓存指针
	uint8_t *pdata; //指向用户缓冲区指针(用于收发模板数据)
}FSC_TYPE;


/*****************传感器指令*****************/
enum{
	DIR_TERMINATE     = 0X00,//终止指令
	DIR_SYS_CONFIG    = 0X01,//系统配置/查寻指纹数量指令
	DIR_IMAGE_SAMPLE	=	0X02,//图像采集
	DIR_PROCESS_A     = 0X03,//指纹注册A类
	DIR_MATCH_FP 			= 0X04,//指纹比对
	DIR_DELETE_FP			= 0X05,//指纹删除
	DIR_TEMPLATE_LOAD = 0X06,//模板/图像传输
	DIR_PROCESS_B			= 0X13,//指纹注册B类
};

/**********************用户自定义状态码*************************/
typedef enum{
	FPR_OK = 0,  		//成功
	FPR_FAIL,    		//失败
	FPR_TIME_OUT,		//等待超时
	FPR_NO_ACK,     //无应答
	FPR_CHECK_BAD,  //数据校验错误
	FPR_USER_INTR,	//用户中断(打断正常流程强制跳出当前循环)
	
	FPR_IMG_SHORT=0XF0,	//指纹图像太短(FSC1001B)
	FPR_IMG_TILT,				//指纹图像太斜(FSC1001B)
	FPR_IMG_SHIFT,			//指纹图像太偏(FSC1001B)
	
	FPR_NULL = 0XFF,
}FPRESULT;

/**************************指令功能码***************************/
#define ARG_TERMINATE     			0X0000			//终止指令功能码

#define ARG_ENROOL_MODE					0X0100			//读取注册模式
#define ARG_SW_VERSION					0X0080			//读取软件版本号
#define ARG_CHIP_UID						0X0040      //读取芯片唯一ID
#define ARG_FP_QUANTITY       	0X0020			//读取注册指纹数量
#define ARG_SECURITY_CLASS			0X0010			//查询安全等级
#define ARG_SW_DATE							0X0008			//查询固件日期
#define ARG_UNREG_MINID					0X0004			//查询指定区间未注册最小ID
#define ARG_SENSOR_TYPE					0X0002			//查询传感器类型

#define ARG_IMAGE_SAMPLE				0XAA00			//采图
#define ARG_1001B_IMAGE_SAMPLE 	0XAB00			//采图(刮擦注册时采图)
#define ARG_3001_IMAGE_SAMPLE   0XA200      //采图(光学/大面阵注册时采图)
#define ARG_3001_IMAGE_CHECK		0XB200			//带手指离开检测的采图(光学/大面阵)

#define ARG_BUILD_TEMPLATE			0X0600			//生成模板(刮擦)
#define ARG_FP_LEAVE						0x9200			//手指离开检测
#define ARG_FP_MATCH1						0XA6E0			//指纹比对(采图+生成模板+比对)
#define ARG_FP_MATCH2						0x06E0			//指纹比对(生成模板+比对)
#define ARG_DELETE_FP						0XE000			//指纹删除

#define ARG_1001B_PROCESS 			0X00BC			//注册模板(刮擦)
#define ARG_3001_1ST_PROCESS		0X06A0			//第一次注册(光学/大面阵)
#define ARG_3001_2ND_PROCESS		0X0660			//第二次注册(光学/大面阵)
#define ARG_3001_3RD_PROCESS		0X061C			//第三次注册(光学/大面阵)



#define ARG_UPLOAD_IMAGE				0X8000			//上传图像数据
#define ARG_READ_TEMPLATE				0X00C0			//读取模板
#define ARG_WRITE_TEMPLATE			0X0030			//写入模板	

/************************传感器应答码****************************/
#define ERR_SUCCESS							0X000				 //成功
#define ERR_FAIL								0XFFF				 //失败
#define ERR_XOR									0XF01				 //异或校验错误
#define ERR_SUM									0XF02				 //和校验错误
#define ERR_DIR									0XF03        //指令错误
#define ERR_ARG									0XF04				 //参数错误
#define ERR_NO_FS								0XF06				 //无文件系统
#define ERR_SYS									0XF07				 //系统错误
#define	ERR_EX_FIELD_ARG				0XF08				 //扩展域参数错误
#define ERR_COMS_INIT	  				0x101				 //传感器初始化失败
#define ERR_COMS_CORRECT   			0X102				 //光学传感器校正失败
#define ERR_TIME_OUT						0X201				 //手指检测超时
#define ERR_IMAGE_SAMPLE				0X202				 //采图失败
#define ERR_TEMPLATE_CREATE 		0X203        //图像质量差
#define ERR_FP_LEAVED						0X205				 //手指离开
#define ERR_REGISTERED					0X301				 //指纹已注册
#define ERR_COMPARE_TEMP1				0X302				 //与模板一匹配失败
#define ERR_PROCESS_CONTINUE		0X302			   //请继续注册(FSC1001B/FSC2001)
#define ERR_COMPARE_TEMP2				0X303				 //与模板二匹配失败
#define ERR_FP_FULL							0X304				 //传感器指纹已满
#define ERR_MV_OVER							0X311				 //请勿大幅度移动手指
#define	ERR_NO_MOVE							0X312				 //请稍微移动手指
#define ERR_FP_EMPTY						0X401				 //传感器无指纹		
#define ERR_FP_COMPARE					0X402				 //指纹比对失败
#define ERR_FP_DELETE						0X501				 //指纹删除失败
#define	ERR_INVALID_TEMPLATE 		0X601				 //指定模板无效


/************************外部变量声明*************************/
extern FSC_TYPE fsc;

/**********************接口函数声明***************************/

FPRESULT FP_WaiteAck(uint8_t dir);
FPRESULT FP_ComFun(uint8_t dir);
FPRESULT FP_GetImage(uint16_t arg);
FPRESULT FP_BuildTemplate(uint16_t arg);
FPRESULT FP_IsLeave(void);


void SensorInit(void);
void FP_ISR_handler(void);

FPRESULT FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg);
FPRESULT FP_Match(uint16_t IDs, uint16_t IDe, uint16_t *arg);
FPRESULT FP_Delete(uint16_t IDs, uint16_t IDe);
FPRESULT FP_GetQuatity(uint16_t IDs,uint16_t IDe,uint16_t *arg);
FPRESULT FP_GetUnregMinID(uint16_t IDs,uint16_t IDe,uint16_t *arg);
FPRESULT FP_Terminate(void);
FPRESULT FP_ReadTemplate(uint16_t ID,uint8_t Sn,uint8_t Num,uint8_t *arg);
FPRESULT FP_WriteTemplate(uint16_t ID,uint8_t Sn,uint8_t Num,uint8_t *arg);
FPRESULT FP_ReadImage(void(*send)(uint8_t),uint8_t *arg);
#endif

