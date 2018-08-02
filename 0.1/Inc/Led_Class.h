/*******************************************************************************
	*
	* FILE NAME
	*						 	LedClass.h
	* BRIEF
	*							Communicate with LED
	*
*******************************************************************************/
#ifndef __LED_CLASS_H__
#define __LED_CLASS_H__

/* < -----------------Include Files --------------------> */
#include "Type_Def.h"
#include "MCU_Config.h"

/* < ---------------Data Type Redefine -----------------> */
#if AL94
	typedef union _LED_DATA_TYPE
	{
		_Uint8	SndByte[COLOR_TOTAL];
		struct _COLOR_TYPE
		{
			_Uint8	DutyR;
			_Uint8	DutyG;
			_Uint8	DutyB;
		}Buffer[LED_TOTAL];
	} _Type_Led;
#elif AL95
	typedef union _LED_DATA_TYPE
	{
		_Uint8	SndByte[COLOR_TOTAL];
		struct _COLOR_TYPE
		{
			_Uint8	DutyR;
			_Uint8	DutyG;
			_Uint8	DutyB;
		}Buffer[LED_TOTAL];
	} _Type_Led;

#elif AL91 | AL92
	typedef union _LED_DATA_TYPE
	{
		_Uint8	SndByte[COLOR_TOTAL];
		struct _COLOR_TYPE
		{
			_Uint8	DutyB;
			_Uint8	DutyG;
			_Uint8	DutyR;
		}Buffer[LED_TOTAL];
	} _Type_Led;
	
#elif AL99
	#define				GROUP_TOTAL					(_Uint8)7					/* GROUP数量		*/
	#define				GROUP_CNT						(_Uint8)38				/* GROUP灯数量	*/
	#define				GRP_LED_TOTAL				(_Uint8)228				/* LED总数量 		= GROUP_TOTAL * GROUP_CNT*/
	#define				COLOR_CNT						(_Uint8)3					/* 每灯颜色总数 */
	#define				GROUP_COLOR_TOTAL		(_Uint8)114				/* 每组颜色总数 = GROUP_CNT * COLOR_CNT*/

	#define				LIGHT_LEVEL						3

	#define				BIT0_H								1
	#define				BIT0_L								3
	#define				BIT1_H								3
	#define				BIT1_L								1

	/* Type Redefine */
	typedef 	struct		LED_TYPE
	{
		_Uint8		DutyG;
		_Uint8		DutyR;
		_Uint8		DutyB;
	} _TypeLed_ST;

#endif

/* < -------------Exported Parameters ------------------> */
extern _Flag					DirectFlag;
	
#if AL91 | AL92 | AL94 | AL95
	extern _Type_Led 			LedData;

#elif AL96
	extern	_Uint8				DutyR;
	extern 	_Uint8				DutyG;
	extern  _Uint8				DutyB;
	extern 	_Uint8				DutyW;

#elif AL99
	extern _TypeLed_ST			ColorBuffer[GROUP_TOTAL][GROUP_CNT];

#endif

/*<---------------- Exported Function ------------------> */
void Led_Update(void);



#endif
