#include "fp_comm.h"

#if defined (SENSOR_BLH3001)

/****************************************************************************
*������FP_Process
*���: IDs-��ʼָ��ID,��ָֹ��ID,*arg-ע��ɹ��󷵻ض�Ӧ��ָ��ID
*����: ״̬��
*����: ָ��ע��(������BLH3001-��ѧ��TCS2��ѹʽ�����󴫸���)(��ͼ+ע��)
*���ߣ�YangGuo
*ʱ��: 2015-3-11
****************************************************************************/
FPRESULT  FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg){

	uint8_t step=0;
	FPRESULT res;
	uint16_t ArgImage;
	uint16_t ArgEnroll[]={ARG_3001_1ST_PROCESS,ARG_3001_2ND_PROCESS,ARG_3001_3RD_PROCESS};
		
	
	while(1){
		
		/*�ɼ�ͼ��*/
		if(!step){
//			_PressFinger(0);
		}
		ArgImage = ARG_3001_IMAGE_SAMPLE;
		if((res=FP_GetImage(ArgImage))!=FPR_OK){
			//FP_PowerOff();
			return res;
		}		
//		NY3P_Play(V001,0);//��ʾ��Ϣ:����"��"
		/*ͼ��������ģ��*/
		fsc.dir = DIR_PROCESS_A;
		fsc.arg = ArgEnroll[step];
		fsc.len = 4;
		fsc.pext[0] = IDs>>8;
		fsc.pext[1] = IDs&0x00FF;
		fsc.pext[2] = IDe>>8;
		fsc.pext[3] = IDe&0x00FF;
		
		res = FP_ComFun(DIR_PROCESS_A);
		if(res!=FPR_OK){
			FP_PowerOff();
			return res;
		}
		else{
			step++;
//			_Continue(step,20);
			if(step>=3){
				*arg=(fsc.pext[0]<<8)|fsc.pext[1];
				FP_PowerOff();
				return res;
			}
		}
	}
}




#endif
