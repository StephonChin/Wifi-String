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
#include "Type_Private.h"


//----------------------------------------------------------------
// Data type redefine
//----------------------------------------------------------------
typedef 		unsigned char					_Uint8,_Flag;
typedef 		unsigned short				_Uint16;
typedef			unsigned int					_Uint32;
typedef 		signed char						_Sint8;
typedef			signed short					_Sint16;
typedef			signed int						_Sint32;


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
#if AL91 | AL92
	#define		LED_KM5020					(1)

#elif AL94
	#define		LED_UCS1903BC				(1)

#elif AL95
	#define		LED_CUSTOM					(1)

#elif AL96
	#define		LED_PWM							(1)
#endif






//----------------------------------------------------------------
// Color Theme selected
//----------------------------------------------------------------
#if AL96
	#define		COLOR_RGBW					(1)
#else
	#define		COLOR_RGB						(1)
#endif





//----------------------------------------------------------------
// Horse Enable or Disalbe selected
//----------------------------------------------------------------
#if AL91 | AL94
	#define		HORSE_ENABLE				(1)

#else
	#define		HORSE_DISABLE				(1)
#endif




//----------------------------------------------------------------
// Led total selected
//----------------------------------------------------------------
#if AL91
	#define		LED_TOTAL						24
	#define		COLOR_TOTAL					72

#elif _AL92A_
	#define		LED_TOTAL						24
	#define		COLOR_TOTAL					72
	#define 	GRP_MAX							6
	#define 	LED_MAX							4

#elif _AL92B_ | _AL92C_
	#define		LED_TOTAL						21
	#define		COLOR_TOTAL					63
	#define 	GRP_MAX							3
	#define 	LED_MAX							7

#elif AL94
	#define		LED_TOTAL						80
	#define		COLOR_TOTAL					240

#elif AL95
	#define		LED_TOTAL						3
	#define		COLOR_TOTAL					9
	#define 	GRP_MAX							3
	#define 	LED_MAX							1

#elif AL99
	#define			GRP_ONE							0
	#define			GRP_TWO							1
	#define			GRP_THREE						2
	#define			GRP_FOUR						3
	#define			GRP_FIVE						4
	#define			GRP_SIX							5
	#define			GRP_SEVEN						6
	#define			GRP_MAX							7
	#define			GRP_LED							38
	#define			LED_TOTAL						38

#endif





//----------------------------------------------------------------
// Mode parameters const value selected
//----------------------------------------------------------------
#define		RAND_END_TOTAL			80
#define		RAND_TOTAL					LED_TOTAL

#if AL91
	#define		SECTION_END_TOTAL		8
	#define		SECTION_TOTAL				8
	#define		SECTION_LEVEL				3

#elif AL94
	#define		SECTION_END_TOTAL		8
	#define		SECTION_TOTAL				10
	#define		SECTION_LEVEL				8

#elif AL99
		#define			SECTION_END_TOTAL		8
#endif



//----------------------------------------------------------------
// Device Type Redefine
//----------------------------------------------------------------
#define DEVICE_NAME_ONE		'L'
#define DEVICE_NAME_TWO		'A'


/* AL93 - Kaleidoscope*/
#ifdef	_AL67D_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'1'
	#define HORSE_SYN_CNT		1
#endif

/* AL95 - 3 projectors*/
#ifdef	_AL95_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'2'
	#define HORSE_SYN_CNT		2
#endif

/* AL91A - Icicle*/
#ifdef	_AL91A_
	#if _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'3'
	#define HORSE_SYN_CNT		3
#endif

/* AL92B - Snow*/
#ifdef	_AL92B_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'4'
	#define HORSE_SYN_CNT		4
#endif

/* AL92C - Christmas Tree*/
#ifdef	_AL92C_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'5'
	#define HORSE_SYN_CNT		5
#endif

/* AL92A - Candle Cane*/
#ifdef	_AL92A_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'6'
	#define HORSE_SYN_CNT		6
#endif

/* AL94 - Light String*/
#ifdef	_AL94_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'7'
 	#define HORSE_SYN_CNT		7
#endif

/* AL91B - C9*/
#ifdef	_AL91B_
	#if _AL91A_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'8'
	#define HORSE_SYN_CNT		8
#endif

/* AL91D - C6*/
#ifdef	_AL91D_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'9'
	#define HORSE_SYN_CNT		9
#endif

/* AL91C - G40*/
#ifdef	_AL91C_
	#if _AL91A_ | _AL91B_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'A'
	#define HORSE_SYN_CNT		10
#endif

/* AL96 - Bubble*/
#ifdef	_AL96_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL99B_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'B'
	#define HORSE_SYN_CNT		11
#endif

/* AL99B - TREE*/
#ifdef	_AL99B_
	#if _AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_
		#error "Device Type Redefine Repeat Error !"
	#endif

	#define	DEVICE_NUM			'C'
	#define HORSE_SYN_CNT		12
#endif

/* Check the device error */
#if !(_AL91A_ | _AL91B_ | _AL91C_ | _AL91D_ | _AL92A_ | _AL92B_ | _AL92C_ | _AL67D_ | _AL94_ | _AL95_ | _AL96_ | _AL99B_)
	#error "No device type redefine Error !"
#endif




#endif
