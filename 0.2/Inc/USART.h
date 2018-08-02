/********************************************************************
  *
  * FILE NAME      USART.h
  *
*********************************************************************/
#ifndef __USART_H__
#define __USART_H__

//Include File
#include "Type_Def.h"


//Enalbe module
#define 		RCV_FUNCTION			ENABLE
#define 		SND_FUNCTION 			DISABLE



//Define
#define 	RCV_BYTE_MAX	    15							/* the USART0 FIFO buffer maximum 16 */
#define   PROTOCOL_VER      0x55
#define   CRC_VAL           10


// Command type for light strings controlled by WIFI via bluetooth
typedef struct{
    _Uint8 	version;          // protocol version
    //_Uint8  crc8;             // crc checksum
    _Uint8 	cmd;							// command
    _Uint8  idx;		  				// reserved for sensor
    _Uint8  len;              // data length
    _Uint8  payload[11];      // data contents
} _Type_ST_USART;

//Command value define 
#define		CMD_NONE          0x00
#define		CMD_SOCKET_ON			0x30
#define		CMD_SOCKET_OFF		0x31
#define		CMD_THEME_SET     0x32     		// Change color or theme
                                        // len = 1  >>          Value from 0 to 0x1C
                                        //---------------------------------------------------
                                        //                      0x00 - Red
                                        //                      0x01 - Green
                                        //                      0x02 - Blue
                                        //                      0x03 - Orange
                                        //                      0x04 - Pink
                                        //                      0x05 - Spring Green
                                        //                      0x06 - Gold
                                        //                      0x07 - Deep Pink
                                        //                      0x08 - Lawn Green
                                        //                      0x09 - Magenta
                                        //                      0x0A - Cyan
                                        //                      0x0B - Yellow
                                        //                      0x0C - Purple
                                        //                      0x0D - White
                                        //                      0x0E - Cold White
                                        //                      0x0F - Warm White
                                        //                      0x10 - Spring
                                        //                      0x11 - Summer
                                        //                      0x12 - Autumn
                                        //                      0x13 - Winter
                                        //                      0x14 - Christmas Day
                                        //                      0x15 - Valentines' Day
                                        //                      0x16 - Independence Day
                                        //                      0x17 - Thanksgiving Day
                                        //                      0x18 - St. Patrick's Day
                                        //                      0x19 - Halloween
                                        //                      0x1A - Sun
                                        //                      0x1B - Earth
                                        //                      0x1C - Multi Color


#define		CMD_NORMAL_MODE        0x33   // Change mode
                                        // len = 1  >>          Value from 0 to 9
                                        //---------------------------------------------------------
                                        //                      0x00 - Steady
                                        //                      0x01 - Blink
                                        //                      0x02 - Sparkle
                                        //                      0x03 - Instead
                                        //                      0x04 - Wave
                                        //                      0x05 - Roll
                                        //                      0x06 - Fade
                                        //                      0x07 - Fireworks
                                        //                      0x08 - Polar
                                        //                      0x09 - Color Show


#define		CMD_SHOW_MODE           0x34  // Select Show Mode
                                        // len = 1  >>          Value from 0 to 6
                                        // ---------------------------------------------------
                                        //                      0x00 - Christmas Show
                                        //                      0x01 - Valentines Day Show
                                        //                      0x02 - Independence Day Show
                                        //                      0x03 - Thanksgiving Day Show
                                        //                      0x04 - St. Patrick's Day Show
                                        //                      0x05 - Halloween Show
                                        //                      0x06 - MultiColor Show

#define		CMD_TIMER_SET           0x35  // Set timer
                                        // len = 1              Value from 0 to 8 (even number)
                                        //------------------------------------------------------
                                        //                      0 - timer off
                                        //                      2 - set 2h
                                        //                      4 - set 4h
                                        //                      6 - set 6h
                                        //                      8 - set 8h


#define	  CMD_MSC_EN             0x36   // Music mode enable or disable
                                        // len = 1              Value from 0 to 1
                                        //------------------------------------------------------
                                        //                      0 - disable
                                        //                      1 - enable






//Function
void Uart_Clear_Buf(void);
static _Flag USART0_Rcv_Byte(void);

#if SND_FUNCTION == ENABLE
	void USART0_Snd_Data(void);
	static void USART0USART0_Snd_Byte();
#endif



//Exported arguments
extern _Flag				      RcvFlag;
extern _Type_ST_USART			USART_Buffer;


#endif
