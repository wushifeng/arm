#include "fp_comm.h"

#if defined (SENSOR_FSC2001)

#include "menu.h"//��ʾ��ʾ��Ϣ����غ���



/****************************************************************************
*������FP_IsLeave
*���: ��
*����: ״̬��
*����: �����ָ�Ƿ��뿪������(������FSC2001)
*���ߣ�YangGuo
*ʱ��: 2015-3-6
****************************************************************************/
FPRESULT FP_IsLeave(void){
	
	fsc.dir = DIR_IMAGE_SAMPLE;
	fsc.arg = ARG_FP_LEAVE;
	fsc.len = 0;
	
	return FP_ComFun(DIR_IMAGE_SAMPLE);	
}


/****************************************************************************
*������FP_Process
*���: IDs-��ʼָ��ID,��ָֹ��ID,*arg-ע��ɹ��󷵻ض�Ӧ��ָ��ID
*����: ״̬��
*����: ָ��ע��(������FSC2001-��ѹʽС���󴫸���)(��ͼ+ע��+��ָ�뿪���)
*���ߣ�YangGuo
*ʱ��: 2015-3-11
****************************************************************************/
FPRESULT  FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg){
	uint8_t  step=0;
	uint32_t err=0;
	FPRESULT res;
	
	while(1){
		/*�ɼ�ͼ��+����ģ��(��ѡ)*/
		_PressFinger(0);
		if((res=FP_GetImage(0xA000))!=FPR_OK){//ARG_IMAGE_TEMP
			FP_PowerOff();
			return res;
		}
		
		/*ע��ģ��*/
		
		/*****************************************************************
		����ע��ģʽ��Ϊ����:
		A.��ѧ/��������:
			1.������ע�᣺
			2.������ע��:
		B.�β�/С������:
			1.3-5��:�β�
			2.5-10��:С����
		*****************************************************************/
		
		fsc.dir = DIR_PROCESS_B;
		fsc.arg = ARG_1001B_PROCESS_;
		fsc.len = 5;
		fsc.pext[0] = IDs>>8;
		fsc.pext[1] = IDs&0x00FF;
		fsc.pext[2] = IDe>>8;
		fsc.pext[3] = IDe&0x00FF;
		fsc.pext[4] = step;
		
		res = FP_ComFun(DIR_PROCESS_B);
		if(res==FPR_OK){//�ɹ�
				*arg = (fsc.pext[0]<<8)+fsc.pext[1];//����ע���ָ��ID
				FP_PowerOff();
				return res;
		}
		else if(res==FPR_FAIL)
		{
				/***************************************************************
				1001��2001 ����8302���������������,�����������򱾴β������ɹ�,
				���������Լ�,2001-8��10�η��ܳɹ�,1001-3��5�η��ܳɹ�,������
				���ٴ����û�����,�������������������������,����ʱ������״̬
				��Ϣ.
				***************************************************************/
				if((fsc.arg&0x0FFF) !=0x302 ){
					FP_PowerOff();
					return FPR_FAIL;
				}
				else{//�����ע��
					if(step++>9){//��������Ĵ���
						FP_PowerOff();
						return FPR_FAIL;
					}
					else{
						_Continue(step+1,0);
					}
				}
		}
		
		/*�����ָ�Ƿ��뿪*/
		do{
			FP_IsLeave();
			err++;
			if(err>0xFFFFF){
				FP_PowerOff();
				return FPR_TIME_OUT;
			}
			else if(err==0xF){
				OLED_Clear_Sel(6,6);
				Disp_String12(6,16,"leave finger",POSITIVE);
			}
			
		}while(fsc.arg!=ERR_FP_LEAVED);
		err = 0;
	}	
}


#endif
