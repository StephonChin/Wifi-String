/*************************************************************************
 *
 * File Name  USART.c
 *
 * Brief
 *						Communicate withe the Wifi module
 *            9600 + 1bit stop + no checksum
 *
 * 						TOCNTEN = 1, TOIC = 40
 *
*****************************************************************************/
#include "USART.h"

//Gloable Variables
_Flag						RcvFlag;
_Type_ST_USART	USART_Buffer;


/**
 	* UART_Clear_Buf()
	*		> Clear the buffer after Initialize the usart peripheral
	*		> Input none
	* 	> Output none
 	*/
void Uart_Clear_Buf(void)
{
	_Uint8 			cnt   = 0;
  volatile _Uint8      temp;

	for (cnt = 0; cnt < RCV_BYTE_MAX; cnt++)
	{
		temp = UART0->DAT;
		if (UART0->FIFOSTS & UART_FIFOSTS_RXEMPTY_Msk)  break;
	}
}


/**
	* UART0_IRQHandler
 	* 	> Serve for USART0 interrupt
 	*/
void UART0_IRQHandler(void)
{
	RcvFlag = USART0_Rcv_Byte();
}



/**
	* USART0_Rcv_Byte
	*
****/
static _Flag USART0_Rcv_Byte(void)
{
	_Uint8 	cnt = 0;
	_Uint8 	* rcvP = (_Uint8 *)&USART_Buffer;


	//Get the data from buffer
	for (cnt = 0; cnt < RCV_BYTE_MAX; cnt++){
		*rcvP = UART0->DAT;
		if (UART0->FIFOSTS & UART_FIFOSTS_RXEMPTY_Msk)  break;
		rcvP++;
	}

	//The data have been overflow the USART0 FIFO upper limit
	if (cnt == RCV_BYTE_MAX){
		return FALSE;
	}

	return TRUE;
}


#if SND_FUNCTION == ENABLE
/**
	* USART_Snd_Data
	* 	> USART0 send data
	*
*****/
void USART0_Snd_Data(void)
{

}



/**
	* USART_Snd_Byte
	*
****/
static void Csr1010_Snd_Byte(_Uint8 SndBuf)
{
	UART_WRITE(UART0, SndBuf);
}
#endif
