#include "fp_comm.h"
#if defined (SENSOR_FSC1001B)


/****************************************************************************
*函数：FP_Process
*入参: arg-预留参数
*返回: 状态码
*功能: 判断所采集到的指纹图像的品质(适用于FSC1001B-刮擦式传感器)
*作者：YangGuo
*时间: 2015-4-17
****************************************************************************/
FPRESULT FP_IsGoodImg(void *arg){
	uint16_t imgstat=0;
	
	if(fsc.len<6){//参数检查		
		return FPR_OK;//无相关信息,不做图像品质检查 
	}
	
	imgstat = (fsc.pext[4]<<8)|fsc.pext[5];//获取图像质量参数
	if(!imgstat){
		return FPR_OK;//图像无异常
	}
	/*检测手指滑动方式,(低BIT位优先)*/
	if(imgstat&(1<4)){
		/*手指滑动太短*/
		_fsc1001B_short(0);
		return FPR_IMG_SHORT;
	}
	else if(imgstat&(1<<5))
	{
		/*手指倾斜过度*/
		_fsc1001B_tilt(0);
		return FPR_IMG_TILT;
	}
	/*检测水平方向上指纹有效信息*/
	else	if(imgstat&(1<7)){
		/*手指水平方向偏离过度*/
		_fsc1001B_shift(0);
		return FPR_IMG_SHIFT;
	}
	else{
		return FPR_OK;
	}
}



/****************************************************************************
*函数：FP_Process
*入参: IDs-起始指纹ID,终止指纹ID,*arg-注册成功后返回对应的指纹ID
*返回: 状态码
*功能: 指纹注册(适用于FSC1001B-刮擦式传感器)
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
FPRESULT  FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg){

	FPRESULT res;
	uint8_t  step=0,fail=0,img_t;
	

	while(1){
		img_sample:		
		/*手指检测*/
		if(!(step||fail)){_PressFinger(0);}//提示信息
		
		if((res=FP_GetImage(ARG_1001B_IMAGE_SAMPLE))!=FPR_OK){
			FP_PowerOff();
			return res;
		}
		
		NY3P_Play(V001,0);//提示信息:语音"嘀"
		
		/***图像品质判断(可选)***/
		
		res = FP_IsGoodImg(NULL);
		if(res!=FPR_OK){
				if(++img_t>=3){
					return FPR_FAIL;
				}
				goto img_sample;
		}
		img_t = 0;
		/*图像处理*/
		if((res=FP_BuildTemplate(ARG_BUILD_TEMPLATE))!=FPR_OK){
			FP_PowerOff();
			return res;
		}
		
		/*指纹注册*/
		fsc.dir = DIR_PROCESS_B;
		fsc.arg = ARG_1001B_PROCESS;
		fsc.len = 5;
		fsc.pext[0] = IDs>>8;
		fsc.pext[1] = IDs&0x00FF;//起始ID
		fsc.pext[2] = IDe>>8;
		fsc.pext[3] = IDe&0x00FF;//终止ID
		fsc.pext[4] = step;			 //步骤
		
		
		res = FP_ComFun(DIR_PROCESS_B);
		if(res==FPR_OK){//成功
				*arg = (fsc.pext[0]<<8)+fsc.pext[1];//返回注册的指纹ID
				FP_PowerOff();
				_Continue(step+1,20);//提示信息
				return res;
		}
		else if(res==FPR_FAIL){
				if((fsc.arg&0x0FFF) !=0x302 ){
						if(++fail>=3){//连续错误限定
							FP_PowerOff();
							return FPR_FAIL;
						}else{
							_tryagain(fsc.arg&0x0FFF,0);//提示信息
						}
				}
				else{//请继续注册
					fail = 0;
					step++;
					_Continue(step,20);//提示信息
					if(step>5){//超过允许的次数
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

