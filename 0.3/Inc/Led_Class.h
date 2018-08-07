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
typedef union _LED_DATA_TYPE
{
	_Uint8	SndHeader;
	struct _COLOR_TYPE
	{
		_Uint8	DutyR;
		_Uint8	DutyG;
		_Uint8	DutyB;
	}Buffer[LED_TOTAL];
} _Type_Led;

/* < -------------Exported Parameters ------------------> */
extern _Type_Led 			LedData;


/*<---------------- Exported Function ------------------> */
void Led_Update(void);



#endif
