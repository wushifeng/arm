#include "fp_comm.h"

#if defined (SENSOR_BLH3001)

/****************************************************************************
*函数：FP_Process
*入参: IDs-起始指纹ID,终止指纹ID,*arg-注册成功后返回对应的指纹ID
*返回: 状态码
*功能: 指纹注册(适用于BLH3001-光学和TCS2按压式大面阵传感器)(采图+注册)
*作者：YangGuo
*时间: 2015-3-11
****************************************************************************/
FPRESULT  FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg){

	uint8_t step=0;
	FPRESULT res;
	uint16_t ArgImage;
	uint16_t ArgEnroll[]={ARG_3001_1ST_PROCESS,ARG_3001_2ND_PROCESS,ARG_3001_3RD_PROCESS};
		
	
	while(1){
		
		/*采集图像*/
		if(!step){
//			_PressFinger(0);
		}
		ArgImage = ARG_3001_IMAGE_SAMPLE;
		if((res=FP_GetImage(ArgImage))!=FPR_OK){
			//FP_PowerOff();
			return res;
		}		
//		NY3P_Play(V001,0);//提示信息:语音"嘀"
		/*图像处理并生成模板*/
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
