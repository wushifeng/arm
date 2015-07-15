#ifndef __TYPES_H_
#define __TYPES_H_

/*********************************************************************************
������keil ��stdint.h���Ѷ���һ����������,�ʴ˴�ֱ�Ӱ�����ͷ�ļ�����,����û�ʹ��
�ı���������δ����,��ʹ�ô˴����������Ͷ���.
**********************************************************************************/

#if(1)
	#include "stdint.h"
#else
typedef unsigned char           uint8_t;       /* unsigned 8 bit integer */
typedef unsigned short          uint16_t;      /* unsigned 16 bit integer */
typedef unsigned long           uint32_t;      /* unsigned 32 bit integer */
typedef signed char             int8_t;        /* signed 8 bit integer */
typedef signed short            int16_t;       /* signed 16 bit integer */
typedef signed long             int32_t;       /* signed 32 bit integer */
#endif


#ifndef NULL
#define NULL	(void*)0
#endif

#ifndef TRUE
#define TRUE 	1
#endif

#ifndef FALSE
#define FALSE	0
#endif


#endif  

