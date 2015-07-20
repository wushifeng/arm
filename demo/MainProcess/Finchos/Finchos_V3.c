#include "Finchos_V3.h"
#include "fp_port.h"


FSC_TYPE fsc;
uint8_t FPCache[FP_CACHE_SIZE];//串口缓冲区
extern int fd;
unsigned char cmd_query2[] = { 0x3a, 0x01, 0x00, 0x20, 0x00, 0x04, 0x1f, 0x00,
		0x00, 0x00, 0x63, 0x1e };

/********************************协议处理函数*******************************/

/****************************************************************************
*函数：FP_GetAck
*入参: 无
*返回: 状态码
*功能: 读取应答数据
*作者：YangGuo
*时间: 2015-3-5
****************************************************************************/
static FPRESULT FP_GetAck(void){
	
	uint8_t i,XOR=0,SUM=0;
	uint8_t *p;
	int j;
	
	if(devif.flag.rxc){//数据接受完毕
		devif.flag.rxc = 0;
		p = (uint8_t *)fsc.pbuf;
		/*数据校验*/
		for(i=0;i<6;i++){
			XOR ^= p[i];
			SUM += p[i];
		}
		
		if(XOR != p[i]){
			return FPR_CHECK_BAD;//异或校验错误
		}
		else{
			fsc.dir = p[1];
			fsc.arg = p[2]<<8|p[3];
			fsc.len = p[4]<<8|p[5];
			fsc.XOR = p[6];
			/*数据校验*/
			for(;i<fsc.len+7;i++){
				SUM +=  p[i];
			} 
			SUM = ~SUM;
			fsc.SUM = p[i];
			if(fsc.SUM!=SUM){
				return FPR_CHECK_BAD;
			}
			else{
				return FPR_OK;//应答数据校验正确
			}	
		}
	}
	else{
		return FPR_NO_ACK;//未检测到应答信息
	}
}

/****************************************************************************
*函数：FP_ProtocolAnalysis
*入参: dir-用户发送的指令码
*返回: 状态码
*功能: 协议解析
*作者：YangGuo
*时间: 2015-3-5
****************************************************************************/
static FPRESULT FP_ProtocolAnalysis(uint8_t dir){
	
	if(fsc.dir!=dir)
		return FPR_FAIL;
	if(fsc.arg&0x8000){
		return FPR_FAIL;
	}
	else{
		return FPR_OK;//成功
	}
}

/****************************************************************************
*函数：FP_Write
*入参: 无
*返回: 无
*功能: 将数据写写入接口设备
*作者：YangGuo
*时间: 2015-3-5
****************************************************************************/
static void FP_Write(void){
	uint8_t *p;
	uint8_t i,temp;
	
	fsc.XOR = 0;
	fsc.SUM = 0;
	p = (uint8_t *)fsc.pbuf;
	
	/*参数填入缓冲区*/
	*p++ = 0x3a;
	*p++ = fsc.dir;
	*p++ = fsc.arg>>8;
	*p++ = fsc.arg&0x00FF;
	*p++ = fsc.len>>8;
	*p = fsc.len&0x00FF;
	
	/*计算异或值*/
	p = (uint8_t *)fsc.pbuf;

	for(i=0;i<6;i++){
		fsc.XOR ^= p[i];
		fsc.SUM += p[i];
	}
	p[i] = fsc.XOR;
	/*计算和校验值*/
	temp = fsc.len+7;
	for(;i<temp;i++)
		fsc.SUM += p[i];
	
	p[i] = ~fsc.SUM;
	
	/*重置相关变量*/
	fsc.cnt = 0;
	devif.flag.mode =0;
	
	/*发送数据包*/

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
*函数：FP_ISR_handler
*入参: 无
*返回: 无
*功能: 该函数被串口中断服务函数调用,接收串口数据
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
void FP_ISR_handler(void){
	
	int i = 0;
	int res;
	unsigned char  buf[256];
	char* log[80];
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
			//fsc.pdata[fsc.cnt++] = (uint8_t)ReciveByte();	//读数据
			if(fsc.cnt>=fsc.size){//根据具体的指令确定(256/512/图像bytes)
				devif.flag.mode = 0;//切换到指令模式
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
//			fsc.pbuf[i++] = ReciveByte();	//读数据
			if(fsc.pbuf[0]!=0X3A){	//校验包头
				res = 0;
			}
			/*第三版协议处理模式*/
			if(res==6){
				fsc.len = (fsc.pbuf[4]<<8) | fsc.pbuf[5];	//取得扩展域的长度
			}
			if(res>6 /*&& (res==(fsc.len+8))*/){	//数据接收完毕
				devif.flag.mode = 1;
				devif.flag.rxc = 1;	//接收完成标志置位
				res=0;
			}
	}
}


/********************************应用接口函数*******************************/

/****************************************************************************
*函数：FP_WaiteAck
*入参: dir-指令码
*返回: 状态码
*功能: 等待传感器的应答指令
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
FPRESULT FP_WaiteAck(uint8_t dir){

	StartTimer(0);
	usleep(5000);
	while(1){
		FP_ISR_handler();
		if(devif.flag.tmr){//超时处理
			CloseTimer();
			FP_Terminate();//中断传感器工作
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
*函数：FP_ComFun
*入参: dir-指令码
*返回: 状态码
*功能: 公用函数
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
FPRESULT FP_ComFun(uint8_t dir){
	
	FPRESULT res;
	
	//FP_PowerOn();	//打开传感器电源,确保电源稳定
	FP_Write();		//发送命令
	res = FP_WaiteAck(dir);//等待应答
	return res;
}


/****************************************************************************
*函数：FP_GetImage
*入参: arg-指令参数
*返回: 状态码
*功能: 采集图像
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
FPRESULT  FP_GetImage(uint16_t arg){
	
	fsc.dir = DIR_IMAGE_SAMPLE;
	fsc.arg = arg;
	fsc.len = 0;
	
	return FP_ComFun(DIR_IMAGE_SAMPLE);
}


/****************************************************************************
*函数：FP_BuildTemplate
*入参: 无
*返回: 状态码
*功能: 生成指纹模板
*作者：YangGuo
*时间: 2015-4-3
****************************************************************************/
FPRESULT  FP_BuildTemplate(uint16_t arg)
{
	
		fsc.dir = DIR_IMAGE_SAMPLE;
		fsc.arg = arg;
		fsc.len = 0;
		return FP_ComFun(DIR_IMAGE_SAMPLE);
}


/****************************************************************************
*函数：FP_Match
*入参: IDs-起始指纹ID,终止指纹ID,*arg-成功后返回对应的指纹ID
*返回: 状态码
*功能: 在指定区间匹配指纹
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
FPRESULT FP_Match(uint16_t IDs, uint16_t IDe, uint16_t *arg){
	
	FPRESULT res;
	
	/*图像采集*/
	res=FP_GetImage(ARG_IMAGE_SAMPLE);
	if(res!=FPR_OK){
//		FP_PowerOff();
		return res;
	}
//	NY3P_Play(V001,0);//提示信息:语音"嘀"
	
	/************************************
	*图像处理+指纹匹配
	*(此处将两步合为一步来操作)
	************************************/
	fsc.dir = DIR_MATCH_FP;
	fsc.arg = ARG_FP_MATCH2;//此处将图像处理和匹配合二为一
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
*函数：FP_Delete
*入参: IDs-起始指纹ID,IDe-终止指纹ID
*返回: 状态码
*功能: 删除指定范围内的已注册指纹
*作者：YangGuo
*时间: 2015-3-6
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
*函数：FP_GetQuatity
*入参: IDs-起始指纹ID,终止指纹ID,*arg-成功后返回已注册指纹数量
*返回: 状态码
*功能: 查询已注册指纹数量
*作者：YangGuo
*时间: 2015-3-6
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
*函数：FP_GetUnregMinID
*入参: IDs-起始指纹ID,终止指纹ID,*arg-成功后返回未注册的最小指纹ID
*返回: 状态码
*功能: 查询已注册指纹数量
*作者：YangGuo
*时间: 2015-4-9
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
*函数：FP_GetSensorInf
*入参: *arg-指令参数,*ptr-返回数据的指针,br-返回数据的长度指针
*返回: 状态码
*功能: 查询指纹传感器的信息
*作者：YangGuo
*时间: 2015-4-9
****************************************************************************/
FPRESULT FP_GetSensorInf(uint16_t arg,uint8_t *br, void *ptr)
{
	FPRESULT res;
	
//		0:  //传感器类型
//		1:	 //固件日期
//		2:  //安全等级
//		3:  //芯片唯一ID	 
//		4:  //软件版本号
//		5:  //注册模式

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
*函数：FP_BreakOff
*入参: 无
*返回: 状态码
*功能: 中断传感器的当前操作
*作者：YangGuo
*时间: 2015-3-6
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
*函数：FP_ReadTemplate
*入参: ID-所读模板的ID号,Sn-所读模板的编号,Num-模板区域号
*返回: 状态码
*功能: 从指纹传感器中读取指定模板
*作者：YangGuo
*时间: 2015-3-19
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

	if((res = FP_ComFun(DIR_TEMPLATE_LOAD))==FPR_OK)//应答成功
	{
		res = FP_WaiteAck(DIR_TEMPLATE_LOAD);//等待返回校验数据(需要相应指令的支持才行)
		FP_PowerOff();
		return res;
	}
	else{
		FP_PowerOff();
		return res;
	}
}

/****************************************************************************
*函数：FP_WriteTemplate
*入参: ID-模板的ID号,Sn-模板的编号,Num-模板区域号,arg-模板数据指针
*返回: 状态码
*功能: 将指纹模板数据写入传感器
*作者：YangGuo
*时间: 2015-3-19
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
	
	if((res = FP_ComFun(DIR_TEMPLATE_LOAD))!=FPR_OK)//应答成功
	{
		FP_PowerOff();
		return res;
	}
	else{
		devif.flag.mode=0;
		
		for(i=0;i<256;i++){
			SendByte(fsc.pdata[i]);//向传感器写数据
		}
		res = FP_WaiteAck(DIR_TEMPLATE_LOAD);//等待返回校验数据(需要相应指令的支持才行)
		FP_PowerOff();		
		return res;
	}
}

/****************************************************************************
*函数：FP_ReadImage
*入参: send-转发函数的指针,arg-模板数据指针
*返回: 状态码
*功能: 采集指纹图像
*作者：YangGuo
*时间: 2015-3-19
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
	/*计算图像大小*/
	imagesize = (fsc.pext[0]<<8)|fsc.pext[1];
	imagesize *= ((fsc.pext[2]<<8)|fsc.pext[3]);
	
	packet = imagesize/256;//整包数
	remain = imagesize%256;//最后一包的数据量
	
	packet = remain!=0 ? packet+1:packet;
	
	/*读数据*/
	for(cnt=0;cnt<packet;cnt++){
		
		fsc.size = (packet-cnt)==1 ?(remain!=0?remain:256):256;//计算最后一包数据长度

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
			//传输错误
			FP_PowerOff();
			return res;
		}
		
		/*转发数据*/
		if(send!=NULL){
			
			if(!cnt){
//				OLED_Clear_Sel(3,3);
//				Disp_String12(3,4,"上传第   包数据",POSITIVE);
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
*函数：SensorInit
*入参: 无
*返回: 无
*功能: 注册接口函数,初始化数据.用户需要在初始化时注册相关的接口函数,除了电源
*			 开关函数可根据需要注册外,其余接口函数都是必需函数,必须先注册.
*作者：YangGuo
*时间: 2015-3-6
****************************************************************************/
void SensorInit(void){
	
	/*初始化相关数据,禁止用户修改*/
	fsc.pbuf = &FPCache[0]; //注册串口缓存
	fsc.pext = fsc.pbuf+7;  //该示例代码扩展域与串口缓存同在已连续区域,fsc.pext相对于fsc.buff偏移7个单位
	devif.flag.mode= 0;
	devif.flag.rxc = 0;
	devif.flag.tmr = 0;			//该标志量需要用户在定时器中断服务函数中置1
}
