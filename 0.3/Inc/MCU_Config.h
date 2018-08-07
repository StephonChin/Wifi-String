/*******************************************************************************
	*
	* FILE NAME
	*						MCU_Config.h
	*
*******************************************************************************/
#ifndef __MCU_CONFIG_H__
#define __MCU_CONFIG_H__

/*<------------------Include Files--------------------> */
#include "Type_Def.h"

/*<-----------------Define-----------------------> */
#define		PLL_CLOCK					48000000

#define		PORT_RX						P12
#define		PORT_TX						P13

#define		PORT_KEY_MODE			P15
#define   PORT_KEY_COLOR    P32
#define		PORT_COMM					P24

#define   PORT_2H           P04
#define   PORT_4H           P05
#define   PORT_6H           P06
#define   PORT_8H           P07



/*<-----------------File Functions--------------------> */
static void CLK_Init(void);
static void WDT_Init(void);
static void GPIO_Init(void);
static void UART_Init(void);
static void TIM_Init(void);
static void PWM_Init(void);


/*<-----------------Exported Function-----------------> */
void MCU_Config(void);


/*---------------------<Exported Paramters>----------------------*/
extern _Uint8	HseStatus;


#endif
