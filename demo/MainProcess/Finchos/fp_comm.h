#ifndef __FP_COMM_H
#define __FP_COMM_H

#define SENSOR_BLH3001  //BLH3001和TCS2按压式大面阵属于该类
//#define SENSOR_FSC1001B	//FSC1001B刮擦传感器属于该类
//#define SENSOR_FSC2001	//FSC2001按压式小面阵传感器属于该类

#include "fp_port.h"

#if defined (SENSOR_BLH3001) &&!defined(SENSOR_FSC1001B) && !defined(SENSOR_FSC2001)

	#include "Finchos_V3.h"
	#include "Process_blh3001.h"
	
#elif defined (SENSOR_FSC1001B) &&!defined(SENSOR_BLH3001) && !defined(SENSOR_FSC2001)

	#include "Finchos_V3.h"
	#include "Process_fsc1001B.h"
	
#elif defined (SENSOR_FSC2001) &&!defined(SENSOR_FSC1001B) && !defined(SENSOR_BLH3001)

	#include "Finchos_V3.h"
	#include "Process_fsc2001.h"
#else

	#error "Please define the type of sensor"

#endif

#endif

