/*******************************************************************************
	*
	* FILENAME
	*						TypeDef.h
	*
	* BRIEF
	*						Data type redefine
	*						MCU header file
	*           Const value redefine
	*						Device type define
*******************************************************************************/
#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

//----------------------------------------------------------------
// Include Files
//----------------------------------------------------------------
#include "Mini58Series.h"


//----------------------------------------------------------------
// Data type redefine
//----------------------------------------------------------------
typedef 		unsigned char					_Uint8,_Flag,_uint8;
typedef 		unsigned short				_Uint16,_uint16;
typedef			unsigned int					_Uint32,_uint32;
typedef 		signed char						_Sint8,_sint8;
typedef			signed short					_Sint16,_sint16;
typedef			signed int						_Sint32,_sint32;


//----------------------------------------------------------------
// Version Information
//----------------------------------------------------------------
#define		SOFT_VER					0x12



//----------------------------------------------------------------
// Const Value Redefine
//----------------------------------------------------------------
#ifndef RESET
	#define		RESET						(0)
	#define		SET							(1)
#endif

#ifndef TRUE
	#define 	TRUE 						(1)
	#define		FALSE 					(0)
#endif

#ifndef ENABLE
	#define 	ENABLE 					(1)
	#define 	DISABLE 				(0)
#endif

#ifndef NULL
	#define		NULL						(0)
#endif






//----------------------------------------------------------------
// Country Define
//----------------------------------------------------------------
#define _USA_		(1)
//#define	_CAN_		(1)

#if _USA_ & _CAN_
	#error "Country Redefined Error !!!"
#elif !(_USA_ | _CAN_)
	#error "No Country defined Error !!!"
#endif






//----------------------------------------------------------------
// Led drive IC selected
//----------------------------------------------------------------
#define		LED_UCS1903BC				(1)


//----------------------------------------------------------------
// Color Theme selected
//----------------------------------------------------------------
#define		COLOR_RGB						(1)


//----------------------------------------------------------------
// Led total selected
//----------------------------------------------------------------
#define		LED_TOTAL						48
#define		COLOR_TOTAL					144
#define		RAND_END_TOTAL			50
#define		RAND_TOTAL					LED_TOTAL
#define		SECTION_END_TOTAL		8
#define		SECTION_TOTAL				6
#define		SECTION_LEVEL				8




#endif
