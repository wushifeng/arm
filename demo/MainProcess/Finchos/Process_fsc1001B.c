#include "fp_comm.h"
#if defined (SENSOR_FSC1001B)


/****************************************************************************
*������FP_Process
*���: arg-Ԥ������
*����: ״̬��
*����: �ж����ɼ�����ָ��ͼ���Ʒ��(������FSC1001B-�β�ʽ������)
*���ߣ�YangGuo
*ʱ��: 2015-4-17
****************************************************************************/
FPRESULT FP_IsGoodImg(void *arg){
	uint16_t imgstat=0;
	
	if(fsc.len<6){//�������		
		return FPR_OK;//�������Ϣ,����ͼ��Ʒ�ʼ�� 
	}
	
	imgstat = (fsc.pext[4]<<8)|fsc.pext[5];//��ȡͼ����������
	if(!imgstat){
		return FPR_OK;//ͼ�����쳣
	}
	/*�����ָ������ʽ,(��BITλ����)*/
	if(imgstat&(1<4)){
		/*��ָ����̫��*/
		_fsc1001B_short(0);
		return FPR_IMG_SHORT;
	}
	else if(imgstat&(1<<5))
	{
		/*��ָ��б����*/
		_fsc1001B_tilt(0);
		return FPR_IMG_TILT;
	}
	/*���ˮƽ������ָ����Ч��Ϣ*/
	else	if(imgstat&(1<7)){
		/*��ָˮƽ����ƫ�����*/
		_fsc1001B_shift(0);
		return FPR_IMG_SHIFT;
	}
	else{
		return FPR_OK;
	}
}



/****************************************************************************
*������FP_Process
*���: IDs-��ʼָ��ID,��ָֹ��ID,*arg-ע��ɹ��󷵻ض�Ӧ��ָ��ID
*����: ״̬��
*����: ָ��ע��(������FSC1001B-�β�ʽ������)
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT  FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg){

	FPRESULT res;
	uint8_t  step=0,fail=0,img_t;
	

	while(1){
		img_sample:		
		/*��ָ���*/
		if(!(step||fail)){_PressFinger(0);}//��ʾ��Ϣ
		
		if((res=FP_GetImage(ARG_1001B_IMAGE_SAMPLE))!=FPR_OK){
			FP_PowerOff();
			return res;
		}
		
		NY3P_Play(V001,0);//��ʾ��Ϣ:����"��"
		
		/***ͼ��Ʒ���ж�(��ѡ)***/
		
		res = FP_IsGoodImg(NULL);
		if(res!=FPR_OK){
				if(++img_t>=3){
					return FPR_FAIL;
				}
				goto img_sample;
		}
		img_t = 0;
		/*ͼ����*/
		if((res=FP_BuildTemplate(ARG_BUILD_TEMPLATE))!=FPR_OK){
			FP_PowerOff();
			return res;
		}
		
		/*ָ��ע��*/
		fsc.dir = DIR_PROCESS_B;
		fsc.arg = ARG_1001B_PROCESS;
		fsc.len = 5;
		fsc.pext[0] = IDs>>8;
		fsc.pext[1] = IDs&0x00FF;//��ʼID
		fsc.pext[2] = IDe>>8;
		fsc.pext[3] = IDe&0x00FF;//��ֹID
		fsc.pext[4] = step;			 //����
		
		
		res = FP_ComFun(DIR_PROCESS_B);
		if(res==FPR_OK){//�ɹ�
				*arg = (fsc.pext[0]<<8)+fsc.pext[1];//����ע���ָ��ID
				FP_PowerOff();
				_Continue(step+1,20);//��ʾ��Ϣ
				return res;
		}
		else if(res==FPR_FAIL){
				if((fsc.arg&0x0FFF) !=0x302 ){
						if(++fail>=3){//���������޶�
							FP_PowerOff();
							return FPR_FAIL;
						}else{
							_tryagain(fsc.arg&0x0FFF,0);//��ʾ��Ϣ
						}
				}
				else{//�����ע��
					fail = 0;
					step++;
					_Continue(step,20);//��ʾ��Ϣ
					if(step>5){//��������Ĵ���
						FP_PowerOff();
						return FPR_FAIL;
					}
					else{
						
					}
				}
		}
	}
}




#endif

