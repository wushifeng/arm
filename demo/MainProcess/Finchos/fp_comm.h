#ifndef __FP_COMM_H
#define __FP_COMM_H

#define SENSOR_BLH3001  //BLH3001��TCS2��ѹʽ���������ڸ���
//#define SENSOR_FSC1001B	//FSC1001B�β����������ڸ���
//#define SENSOR_FSC2001	//FSC2001��ѹʽС���󴫸������ڸ���

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

