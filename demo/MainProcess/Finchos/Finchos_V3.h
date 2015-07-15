#ifndef __FINCHOS_V3_H
#define __FINCHOS_V3_H

#include "Types.h"

#define EXTENSION_FIELD				12	//��չ��ĳ���
#define FP_CACHE_SIZE					(EXTENSION_FIELD+8)//���ڻ���ĳ���

/********************V3Э�����ݽṹ��***********************/
typedef struct{
	uint8_t 	dir;	//ָ��
	uint8_t   XOR;	//���У��
	uint8_t   SUM;	//��У��

	uint16_t  arg;	//����
	uint16_t  len;	//����
	uint16_t  cnt;	//������
	uint16_t  size; //�������ݳ���

	uint8_t *pext;	//��չ���ָ��
	uint8_t *pbuf;	//֡����ָ��
	uint8_t *pdata; //ָ���û�������ָ��(�����շ�ģ������)
}FSC_TYPE;


/*****************������ָ��*****************/
enum{
	DIR_TERMINATE     = 0X00,//��ָֹ��
	DIR_SYS_CONFIG    = 0X01,//ϵͳ����/��Ѱָ������ָ��
	DIR_IMAGE_SAMPLE	=	0X02,//ͼ��ɼ�
	DIR_PROCESS_A     = 0X03,//ָ��ע��A��
	DIR_MATCH_FP 			= 0X04,//ָ�Ʊȶ�
	DIR_DELETE_FP			= 0X05,//ָ��ɾ��
	DIR_TEMPLATE_LOAD = 0X06,//ģ��/ͼ����
	DIR_PROCESS_B			= 0X13,//ָ��ע��B��
};

/**********************�û��Զ���״̬��*************************/
typedef enum{
	FPR_OK = 0,  		//�ɹ�
	FPR_FAIL,    		//ʧ��
	FPR_TIME_OUT,		//�ȴ���ʱ
	FPR_NO_ACK,     //��Ӧ��
	FPR_CHECK_BAD,  //����У�����
	FPR_USER_INTR,	//�û��ж�(�����������ǿ��������ǰѭ��)
	
	FPR_IMG_SHORT=0XF0,	//ָ��ͼ��̫��(FSC1001B)
	FPR_IMG_TILT,				//ָ��ͼ��̫б(FSC1001B)
	FPR_IMG_SHIFT,			//ָ��ͼ��̫ƫ(FSC1001B)
	
	FPR_NULL = 0XFF,
}FPRESULT;

/**************************ָ�����***************************/
#define ARG_TERMINATE     			0X0000			//��ָֹ�����

#define ARG_ENROOL_MODE					0X0100			//��ȡע��ģʽ
#define ARG_SW_VERSION					0X0080			//��ȡ����汾��
#define ARG_CHIP_UID						0X0040      //��ȡоƬΨһID
#define ARG_FP_QUANTITY       	0X0020			//��ȡע��ָ������
#define ARG_SECURITY_CLASS			0X0010			//��ѯ��ȫ�ȼ�
#define ARG_SW_DATE							0X0008			//��ѯ�̼�����
#define ARG_UNREG_MINID					0X0004			//��ѯָ������δע����СID
#define ARG_SENSOR_TYPE					0X0002			//��ѯ����������

#define ARG_IMAGE_SAMPLE				0XAA00			//��ͼ
#define ARG_1001B_IMAGE_SAMPLE 	0XAB00			//��ͼ(�β�ע��ʱ��ͼ)
#define ARG_3001_IMAGE_SAMPLE   0XA200      //��ͼ(��ѧ/������ע��ʱ��ͼ)
#define ARG_3001_IMAGE_CHECK		0XB200			//����ָ�뿪���Ĳ�ͼ(��ѧ/������)

#define ARG_BUILD_TEMPLATE			0X0600			//����ģ��(�β�)
#define ARG_FP_LEAVE						0x9200			//��ָ�뿪���
#define ARG_FP_MATCH1						0XA6E0			//ָ�Ʊȶ�(��ͼ+����ģ��+�ȶ�)
#define ARG_FP_MATCH2						0x06E0			//ָ�Ʊȶ�(����ģ��+�ȶ�)
#define ARG_DELETE_FP						0XE000			//ָ��ɾ��

#define ARG_1001B_PROCESS 			0X00BC			//ע��ģ��(�β�)
#define ARG_3001_1ST_PROCESS		0X06A0			//��һ��ע��(��ѧ/������)
#define ARG_3001_2ND_PROCESS		0X0660			//�ڶ���ע��(��ѧ/������)
#define ARG_3001_3RD_PROCESS		0X061C			//������ע��(��ѧ/������)



#define ARG_UPLOAD_IMAGE				0X8000			//�ϴ�ͼ������
#define ARG_READ_TEMPLATE				0X00C0			//��ȡģ��
#define ARG_WRITE_TEMPLATE			0X0030			//д��ģ��	

/************************������Ӧ����****************************/
#define ERR_SUCCESS							0X000				 //�ɹ�
#define ERR_FAIL								0XFFF				 //ʧ��
#define ERR_XOR									0XF01				 //���У�����
#define ERR_SUM									0XF02				 //��У�����
#define ERR_DIR									0XF03        //ָ�����
#define ERR_ARG									0XF04				 //��������
#define ERR_NO_FS								0XF06				 //���ļ�ϵͳ
#define ERR_SYS									0XF07				 //ϵͳ����
#define	ERR_EX_FIELD_ARG				0XF08				 //��չ���������
#define ERR_COMS_INIT	  				0x101				 //��������ʼ��ʧ��
#define ERR_COMS_CORRECT   			0X102				 //��ѧ������У��ʧ��
#define ERR_TIME_OUT						0X201				 //��ָ��ⳬʱ
#define ERR_IMAGE_SAMPLE				0X202				 //��ͼʧ��
#define ERR_TEMPLATE_CREATE 		0X203        //ͼ��������
#define ERR_FP_LEAVED						0X205				 //��ָ�뿪
#define ERR_REGISTERED					0X301				 //ָ����ע��
#define ERR_COMPARE_TEMP1				0X302				 //��ģ��һƥ��ʧ��
#define ERR_PROCESS_CONTINUE		0X302			   //�����ע��(FSC1001B/FSC2001)
#define ERR_COMPARE_TEMP2				0X303				 //��ģ���ƥ��ʧ��
#define ERR_FP_FULL							0X304				 //������ָ������
#define ERR_MV_OVER							0X311				 //���������ƶ���ָ
#define	ERR_NO_MOVE							0X312				 //����΢�ƶ���ָ
#define ERR_FP_EMPTY						0X401				 //��������ָ��		
#define ERR_FP_COMPARE					0X402				 //ָ�Ʊȶ�ʧ��
#define ERR_FP_DELETE						0X501				 //ָ��ɾ��ʧ��
#define	ERR_INVALID_TEMPLATE 		0X601				 //ָ��ģ����Ч


/************************�ⲿ��������*************************/
extern FSC_TYPE fsc;

/**********************�ӿں�������***************************/

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

