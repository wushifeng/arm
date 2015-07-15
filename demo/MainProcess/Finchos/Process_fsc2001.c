#include "fp_comm.h"

#if defined (SENSOR_FSC2001)

#include "menu.h"//显示提示信息的相关函数



/****************************************************************************
*函数：FP_IsLeave
*入参: 无
*返回: 状态码
*功能: 检测手指是否离开传感器(仅用于FSC2001)
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
FPRESULT FP_IsLeave(void){
	
	fsc.dir = DIR_IMAGE_SAMPLE;
	fsc.arg = ARG_FP_LEAVE;
	fsc.len = 0;
	
	return FP_ComFun(DIR_IMAGE_SAMPLE);	
}


/****************************************************************************
*函数：FP_Process
*入参: IDs-起始指纹ID,终止指纹ID,*arg-注册成功后返回对应的指纹ID
*返回: 状态码
*功能: 指纹注册(适用于FSC2001-按压式小面阵传感器)(采图+注册+手指离开检测)
*作者：YangGuo
*时间: 2015-3-11
****************************************************************************/
FPRESULT  FP_Process(uint16_t IDs,uint16_t IDe,uint16_t *arg){
	uint8_t  step=0;
	uint32_t err=0;
	FPRESULT res;
	
	while(1){
		/*采集图像+生成模版(可选)*/
		_PressFinger(0);
		if((res=FP_GetImage(0xA000))!=FPR_OK){//ARG_IMAGE_TEMP
			FP_PowerOff();
			return res;
		}
		
		/*注册模板*/
		
		/*****************************************************************
		根据注册模式分为几种:
		A.光学/大面阵类:
			1.分三步注册：
			2.分两步注册:
		B.刮擦/小面阵类:
			1.3-5步:刮擦
			2.5-10步:小面阵
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
		if(res==FPR_OK){//成功
				*arg = (fsc.pext[0]<<8)+fsc.pext[1];//返回注册的指纹ID
				FP_PowerOff();
				return res;
		}
		else if(res==FPR_FAIL)
		{
				/***************************************************************
				1001和2001 返回8302则继续操作并计数,返回其他的则本次操作不成功,
				计数器不自加,2001-8到10次方能成功,1001-3到5次方能成功,最多错误
				多少次由用户控制,传感器自身并不做错误次数计数,错误时仅返回状态
				信息.
				***************************************************************/
				if((fsc.arg&0x0FFF) !=0x302 ){
					FP_PowerOff();
					return FPR_FAIL;
				}
				else{//请继续注册
					if(step++>9){//超过允许的次数
						FP_PowerOff();
						return FPR_FAIL;
					}
					else{
						_Continue(step+1,0);
					}
				}
		}
		
		/*检测手指是否离开*/
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
