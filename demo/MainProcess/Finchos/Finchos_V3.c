#include "Finchos_V3.h"
#include "fp_port.h"


FSC_TYPE fsc;
uint8_t FPCache[FP_CACHE_SIZE];//���ڻ�����
extern int fd;
unsigned char cmd_query2[] = { 0x3a, 0x01, 0x00, 0x20, 0x00, 0x04, 0x1f, 0x00,
		0x00, 0x00, 0x63, 0x1e };

/********************************Э�鴦����*******************************/

/****************************************************************************
*������FP_GetAck
*���: ��
*����: ״̬��
*����: ��ȡӦ������
*���ߣ�YangGuo
*ʱ��: 2015-3-5
****************************************************************************/
static FPRESULT FP_GetAck(void){
	
	uint8_t i,XOR=0,SUM=0;
	uint8_t *p;
	int j;
	
	if(devif.flag.rxc){//���ݽ������
		devif.flag.rxc = 0;
		p = (uint8_t *)fsc.pbuf;
		/*����У��*/
		for(i=0;i<6;i++){
			XOR ^= p[i];
			SUM += p[i];
		}
		
		if(XOR != p[i]){
			return FPR_CHECK_BAD;//���У�����
		}
		else{
			fsc.dir = p[1];
			fsc.arg = p[2]<<8|p[3];
			fsc.len = p[4]<<8|p[5];
			fsc.XOR = p[6];
			/*����У��*/
			for(;i<fsc.len+7;i++){
				SUM +=  p[i];
			} 
			SUM = ~SUM;
			fsc.SUM = p[i];
			if(fsc.SUM!=SUM){
				return FPR_CHECK_BAD;
			}
			else{
				return FPR_OK;//Ӧ������У����ȷ
			}	
		}
	}
	else{
		return FPR_NO_ACK;//δ��⵽Ӧ����Ϣ
	}
}

/****************************************************************************
*������FP_ProtocolAnalysis
*���: dir-�û����͵�ָ����
*����: ״̬��
*����: Э�����
*���ߣ�YangGuo
*ʱ��: 2015-3-5
****************************************************************************/
static FPRESULT FP_ProtocolAnalysis(uint8_t dir){
	
	if(fsc.dir!=dir)
		return FPR_FAIL;
	if(fsc.arg&0x8000){
		return FPR_FAIL;
	}
	else{
		return FPR_OK;//�ɹ�
	}
}

/****************************************************************************
*������FP_Write
*���: ��
*����: ��
*����: ������дд��ӿ��豸
*���ߣ�YangGuo
*ʱ��: 2015-3-5
****************************************************************************/
static void FP_Write(void){
	uint8_t *p;
	uint8_t i,temp;
	
	fsc.XOR = 0;
	fsc.SUM = 0;
	p = (uint8_t *)fsc.pbuf;
	
	/*�������뻺����*/
	*p++ = 0x3a;
	*p++ = fsc.dir;
	*p++ = fsc.arg>>8;
	*p++ = fsc.arg&0x00FF;
	*p++ = fsc.len>>8;
	*p = fsc.len&0x00FF;
	
	/*�������ֵ*/
	p = (uint8_t *)fsc.pbuf;

	for(i=0;i<6;i++){
		fsc.XOR ^= p[i];
		fsc.SUM += p[i];
	}
	p[i] = fsc.XOR;
	/*�����У��ֵ*/
	temp = fsc.len+7;
	for(;i<temp;i++)
		fsc.SUM += p[i];
	
	p[i] = ~fsc.SUM;
	
	/*������ر���*/
	fsc.cnt = 0;
	devif.flag.mode =0;
	
	/*�������ݰ�*/

	for(i=0;i<temp+1;i++){
		//SendByte(p[i]);
		// printf("%02x ", p[i]);
	}
	p[i]=0;
	// printf("\n");
//	USART1_SentBytes(cmd_query2);
	write(fd, p, temp+1);
	usleep(3000);

}

/****************************************************************************
*������FP_ISR_handler
*���: ��
*����: ��
*����: �ú����������жϷ���������,���մ�������
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
void FP_ISR_handler(void){
	
	int i = 0;
	int res;
	unsigned char  buf[256];
	memset(&buf,0,sizeof(buf));
	if(devif.flag.mode&&fsc.pdata){
			while (1) {
				res = read(fd, fsc.pdata, 255);
				if (res <= 0)
					continue;
				fsc.pdata[res] = 0;
				break;
			}
			fsc.cnt = res;
			//fsc.pdata[fsc.cnt++] = (uint8_t)ReciveByte();	//������
			if(fsc.cnt>=fsc.size){//���ݾ����ָ��ȷ��(256/512/ͼ��bytes)
				devif.flag.mode = 0;//�л���ָ��ģʽ
			}
	}
	else{
		while(1)
			{
				res = read(fd, fsc.pbuf, 255);
				if(res<=0)
					continue;
				fsc.pbuf[res]=0;

				// printf("Read ... \n");
				for(i=0;i<res;i++){
					// printf("%02x ", fsc.pbuf[i]);
				}
				// printf("\nbreak 1\n");
				break;
			}
//			fsc.pbuf[i++] = ReciveByte();	//������
			if(fsc.pbuf[0]!=0X3A){	//У���ͷ
				res = 0;
			}
			/*������Э�鴦��ģʽ*/
			if(res==6){
				fsc.len = (fsc.pbuf[4]<<8) | fsc.pbuf[5];	//ȡ����չ��ĳ���
			}
			if(res>6 /*&& (res==(fsc.len+8))*/){	//���ݽ������
				devif.flag.mode = 1;
				devif.flag.rxc = 1;	//������ɱ�־��λ
				res=0;
			}
	}
}


/********************************Ӧ�ýӿں���*******************************/

/****************************************************************************
*������FP_WaiteAck
*���: dir-ָ����
*����: ״̬��
*����: �ȴ���������Ӧ��ָ��
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_WaiteAck(uint8_t dir){

	StartTimer(0);
	usleep(5000);
	while(1){
		FP_ISR_handler();
		if(devif.flag.tmr){//��ʱ����
			CloseTimer();
			FP_Terminate();//�жϴ���������
			return FPR_TIME_OUT;
		}
		if(Userhook(NULL)){
			CloseTimer();
			FP_Terminate();
			return FPR_USER_INTR;
		}
		if(FP_GetAck()==FPR_OK){
			CloseTimer();
			if(FP_ProtocolAnalysis(dir)==FPR_OK)
				return FPR_OK;
			else
				return FPR_FAIL;
		}
	}

}


/****************************************************************************
*������FP_ComFun
*���: dir-ָ����
*����: ״̬��
*����: ���ú���
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_ComFun(uint8_t dir){
	
	FPRESULT res;
	
	//FP_PowerOn();	//�򿪴�������Դ,ȷ����Դ�ȶ�
	FP_Write();		//��������
	res = FP_WaiteAck(dir);//�ȴ�Ӧ��
	return res;
}


/****************************************************************************
*������FP_GetImage
*���: arg-ָ�����
*����: ״̬��
*����: �ɼ�ͼ��
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT  FP_GetImage(uint16_t arg){
	
	fsc.dir = DIR_IMAGE_SAMPLE;
	fsc.arg = arg;
	fsc.len = 0;
	
	return FP_ComFun(DIR_IMAGE_SAMPLE);
}


/****************************************************************************
*������FP_BuildTemplate
*���: ��
*����: ״̬��
*����: ����ָ��ģ��
*���ߣ�YangGuo
*ʱ��: 2015-4-3
****************************************************************************/
FPRESULT  FP_BuildTemplate(uint16_t arg)
{
	
		fsc.dir = DIR_IMAGE_SAMPLE;
		fsc.arg = arg;
		fsc.len = 0;
		return FP_ComFun(DIR_IMAGE_SAMPLE);
}


/****************************************************************************
*������FP_Match
*���: IDs-��ʼָ��ID,��ָֹ��ID,*arg-�ɹ��󷵻ض�Ӧ��ָ��ID
*����: ״̬��
*����: ��ָ������ƥ��ָ��
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_Match(uint16_t IDs, uint16_t IDe, uint16_t *arg){
	
	FPRESULT res;
	
	/*ͼ��ɼ�*/
	res=FP_GetImage(ARG_IMAGE_SAMPLE);
	if(res!=FPR_OK){
//		FP_PowerOff();
		return res;
	}
//	NY3P_Play(V001,0);//��ʾ��Ϣ:����"��"
	
	/************************************
	*ͼ����+ָ��ƥ��
	*(�˴���������Ϊһ��������)
	************************************/
	fsc.dir = DIR_MATCH_FP;
	fsc.arg = ARG_FP_MATCH2;//�˴���ͼ�����ƥ��϶�Ϊһ
	fsc.len = 4;
	fsc.pext[0] = IDs>>8;
	fsc.pext[1] = IDs&0x00FF;
	fsc.pext[2] = IDe>>8;
	fsc.pext[3] = IDe&0x00FF;
	
	res = FP_ComFun(DIR_MATCH_FP);
	if(res==FPR_OK)
	{
		*arg = (fsc.pext[0]<<8)+fsc.pext[1];
	}
//	FP_PowerOff();
	return res;		
}

/****************************************************************************
*������FP_Delete
*���: IDs-��ʼָ��ID,IDe-��ָֹ��ID
*����: ״̬��
*����: ɾ��ָ����Χ�ڵ���ע��ָ��
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_Delete(uint16_t IDs, uint16_t IDe){
	
	FPRESULT res;
	
	fsc.dir = DIR_DELETE_FP;
	fsc.arg = ARG_DELETE_FP;
	fsc.len = 4;
	fsc.pext[0] = IDs>>8;
	fsc.pext[1] = IDs&0x00FF;
	fsc.pext[2] = IDe>>8;
	fsc.pext[3] = IDe&0x00FF;
	
	res = FP_ComFun(DIR_DELETE_FP);
//	FP_PowerOff();
	return res;
}


/****************************************************************************
*������FP_GetQuatity
*���: IDs-��ʼָ��ID,��ָֹ��ID,*arg-�ɹ��󷵻���ע��ָ������
*����: ״̬��
*����: ��ѯ��ע��ָ������
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_GetQuatity(uint16_t IDs,uint16_t IDe,uint16_t *arg){
	
	FPRESULT res;
	
	fsc.dir = DIR_SYS_CONFIG;
	fsc.arg = ARG_FP_QUANTITY;
	fsc.len = 4;
	fsc.pext[0] = (uint8_t)(IDs>>8);
	fsc.pext[1] = (uint8_t)(IDs&0x00FF);
	fsc.pext[2] = (uint8_t)(IDe>>8);
	fsc.pext[3] = (uint8_t)(IDe&0x00FF);
	
	res = FP_ComFun(DIR_SYS_CONFIG);
	if(res==FPR_OK)
	{
		*arg = (fsc.pext[0]<<8)+fsc.pext[1];
	}
//	FP_PowerOff();
	return res;
}

/****************************************************************************
*������FP_GetUnregMinID
*���: IDs-��ʼָ��ID,��ָֹ��ID,*arg-�ɹ��󷵻�δע�����Сָ��ID
*����: ״̬��
*����: ��ѯ��ע��ָ������
*���ߣ�YangGuo
*ʱ��: 2015-4-9
****************************************************************************/
FPRESULT FP_GetUnregMinID(uint16_t IDs,uint16_t IDe,uint16_t *arg)
{
	FPRESULT res;
	
	fsc.dir = DIR_SYS_CONFIG;
	fsc.arg = ARG_UNREG_MINID;
	fsc.len = 4;
	fsc.pext[0] = (uint8_t)(IDs>>8);
	fsc.pext[1] = (uint8_t)(IDs&0x00FF);
	fsc.pext[2] = (uint8_t)(IDe>>8);
	fsc.pext[3] = (uint8_t)(IDe&0x00FF);
	
	res = FP_ComFun(DIR_SYS_CONFIG);
	if(res==FPR_OK)
	{
		*arg = (fsc.pext[0]<<8)+fsc.pext[1];
	}
	FP_PowerOff();
	return res;
}
/****************************************************************************
*������FP_GetSensorInf
*���: *arg-ָ�����,*ptr-�������ݵ�ָ��,br-�������ݵĳ���ָ��
*����: ״̬��
*����: ��ѯָ�ƴ���������Ϣ
*���ߣ�YangGuo
*ʱ��: 2015-4-9
****************************************************************************/
FPRESULT FP_GetSensorInf(uint16_t arg,uint8_t *br, void *ptr)
{
	FPRESULT res;
	
//		0:  //����������
//		1:	 //�̼�����
//		2:  //��ȫ�ȼ�
//		3:  //оƬΨһID	 
//		4:  //����汾��
//		5:  //ע��ģʽ

	fsc.dir = DIR_SYS_CONFIG;
	fsc.arg = arg;
	fsc.len = 0;

	res = FP_ComFun(DIR_SYS_CONFIG);
	if(res==FPR_OK)
	{
		ptr = (void*)fsc.pext;
		*br  = (uint8_t)fsc.len;
	}
	FP_PowerOff();
	return res;
}

/****************************************************************************
*������FP_BreakOff
*���: ��
*����: ״̬��
*����: �жϴ������ĵ�ǰ����
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_Terminate(void)
{
	FPRESULT res;
	
	fsc.dir = DIR_TERMINATE;
	fsc.arg = ARG_TERMINATE;
	fsc.len = 0;
	
	res = FP_ComFun(DIR_TERMINATE);
	FP_PowerOff();	
	return res;
}

/****************************************************************************
*������FP_ReadTemplate
*���: ID-����ģ���ID��,Sn-����ģ��ı��,Num-ģ�������
*����: ״̬��
*����: ��ָ�ƴ������ж�ȡָ��ģ��
*���ߣ�YangGuo
*ʱ��: 2015-3-19
****************************************************************************/
FPRESULT FP_ReadTemplate(uint16_t ID,uint8_t Sn,uint8_t Num,uint8_t *arg){

	FPRESULT res;
	
	fsc.dir = DIR_TEMPLATE_LOAD;
	fsc.arg = ARG_READ_TEMPLATE;
	fsc.len = 4;
	fsc.pext[0] = (uint8_t)(ID>>8);
	fsc.pext[1] = (uint8_t)(ID&0x00FF);
	fsc.pext[2] = (uint8_t)Sn;
	fsc.pext[3] = (uint8_t)Num;
	
	fsc.pdata = arg;
	fsc.size = 256;

	if((res = FP_ComFun(DIR_TEMPLATE_LOAD))==FPR_OK)//Ӧ��ɹ�
	{
		res = FP_WaiteAck(DIR_TEMPLATE_LOAD);//�ȴ�����У������(��Ҫ��Ӧָ���֧�ֲ���)
		FP_PowerOff();
		return res;
	}
	else{
		FP_PowerOff();
		return res;
	}
}

/****************************************************************************
*������FP_WriteTemplate
*���: ID-ģ���ID��,Sn-ģ��ı��,Num-ģ�������,arg-ģ������ָ��
*����: ״̬��
*����: ��ָ��ģ������д�봫����
*���ߣ�YangGuo
*ʱ��: 2015-3-19
****************************************************************************/
FPRESULT FP_WriteTemplate(uint16_t ID,uint8_t Sn,uint8_t Num,uint8_t *arg){
	
	FPRESULT res;
	uint16_t i;
	
	fsc.dir = DIR_TEMPLATE_LOAD;
	fsc.arg = ARG_WRITE_TEMPLATE;
	fsc.len = 4;
	fsc.pext[0] = (uint8_t)(ID>>8);
	fsc.pext[1] = (uint8_t)(ID&0x00FF);
	fsc.pext[2] = (uint8_t)Sn;
	fsc.pext[3] = (uint8_t)Num;
	
	fsc.pdata = arg;
	fsc.size = 256;
	
	if((res = FP_ComFun(DIR_TEMPLATE_LOAD))!=FPR_OK)//Ӧ��ɹ�
	{
		FP_PowerOff();
		return res;
	}
	else{
		devif.flag.mode=0;
		
		for(i=0;i<256;i++){
			SendByte(fsc.pdata[i]);//�򴫸���д����
		}
		res = FP_WaiteAck(DIR_TEMPLATE_LOAD);//�ȴ�����У������(��Ҫ��Ӧָ���֧�ֲ���)
		FP_PowerOff();		
		return res;
	}
}

/****************************************************************************
*������FP_ReadImage
*���: send-ת��������ָ��,arg-ģ������ָ��
*����: ״̬��
*����: �ɼ�ָ��ͼ��
*���ߣ�YangGuo
*ʱ��: 2015-3-19
****************************************************************************/
FPRESULT FP_ReadImage(void(*send)(uint8_t),uint8_t *arg){
	
	FPRESULT res;
	uint16_t i;
	uint32_t imagesize;
	uint16_t packet,remain,cnt;
	
	fsc.pdata = arg;
	
	
	res = FP_GetImage(ARG_IMAGE_SAMPLE);
	if(res!=FPR_OK)
	{
		FP_PowerOff();
		return res;
	}
	/*����ͼ���С*/
	imagesize = (fsc.pext[0]<<8)|fsc.pext[1];
	imagesize *= ((fsc.pext[2]<<8)|fsc.pext[3]);
	
	packet = imagesize/256;//������
	remain = imagesize%256;//���һ����������
	
	packet = remain!=0 ? packet+1:packet;
	
	/*������*/
	for(cnt=0;cnt<packet;cnt++){
		
		fsc.size = (packet-cnt)==1 ?(remain!=0?remain:256):256;//�������һ�����ݳ���

		fsc.dir = DIR_TEMPLATE_LOAD;
		fsc.arg = ARG_UPLOAD_IMAGE;
		fsc.len = 4;
		fsc.pext[0] = (uint8_t)(cnt>>8);
		fsc.pext[1] = (uint8_t)(cnt&0x00FF);
		fsc.pext[2] = (uint8_t)(fsc.size>>8);
		fsc.pext[3] = (uint8_t)(fsc.size&0x00FF);
		
		fsc.pdata = arg;

		if((res = FP_ComFun(DIR_TEMPLATE_LOAD))!=FPR_OK)
		{
			FP_PowerOff();
			return res;
		}

		if((res=FP_WaiteAck(DIR_TEMPLATE_LOAD))!=FPR_OK)
		{
			//�������
			FP_PowerOff();
			return res;
		}
		
		/*ת������*/
		if(send!=NULL){
			
			if(!cnt){
//				OLED_Clear_Sel(3,3);
//				Disp_String12(3,4,"�ϴ���   ������",POSITIVE);
			}
//			Disp_Num8x16_3(3,52,cnt+1,POSITIVE);
			
			for(i=0;i<fsc.size;i++)
				send(fsc.pdata[i]);
		}
	}
	FP_PowerOff();
	return res;
}
/****************************************************************************
*������SensorInit
*���: ��
*����: ��
*����: ע��ӿں���,��ʼ������.�û���Ҫ�ڳ�ʼ��ʱע����صĽӿں���,���˵�Դ
*			 ���غ����ɸ�����Ҫע����,����ӿں������Ǳ��躯��,������ע��.
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
void SensorInit(void){
	
	/*��ʼ���������,��ֹ�û��޸�*/
	fsc.pbuf = &FPCache[0]; //ע�ᴮ�ڻ���
	fsc.pext = fsc.pbuf+7;  //��ʾ��������չ���봮�ڻ���ͬ������������,fsc.pext�����fsc.buffƫ��7����λ
	devif.flag.mode= 0;
	devif.flag.rxc = 0;
	devif.flag.tmr = 0;			//�ñ�־����Ҫ�û��ڶ�ʱ���жϷ���������1
}
