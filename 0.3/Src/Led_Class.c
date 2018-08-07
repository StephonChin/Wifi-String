/************************************************************************************************************
	*
	* FileName			Led_Class.c
	*
	* Description
	*								The protocol about communicate with LED, such as LED with KM5020, LED with UCS1903BC,
	*								LED drived by PWM, LED drived by general I/O or LED drived by custom protol.
	*
*************************************************************************************************************/
#include "Led_Class.h"

/* Gloable Parameters */
_Type_Led 			LedData;




/**************************************************************************************
	*
	* FunctionName		Led_Update
	*
	*	Brief
 	*									Include UCS1903BC's Led driver protocol
	*
***************************************************************************************/
void Led_Update(void)
{
	//UCS1903BC---------------------------------AL94--------------------------------
	_Uint16	TempCnt 		= 0;
  _Uint8  TempBit  		= 0;
	_Uint8	*DataBufAdd	= 0;
	_Uint8  DataBuf 		= 0;


	DataBufAdd = &LedData.SndHeader;

  for (TempCnt = 0; TempCnt < COLOR_TOTAL; TempCnt++)			//Circle count caculated
  {
    DataBuf = *DataBufAdd++;    // Address increase
    DataBuf = (_Uint8)((_Uint16)DataBuf * 3 / 5);

    
    for (TempBit = 0; TempBit < 8; TempBit++)							//Send the bit data one bye one
    {
      if ((DataBuf & 0x80) == 0x80)												//Bit 1 data send
      {
        PORT_COMM = SET;
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");
        PORT_COMM = RESET;
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
      }
      else																								// Bit0 data send
      {
        PORT_COMM = SET;
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");
        PORT_COMM = RESET;
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
        __asm("nop");__asm("nop");__asm("nop");__asm("nop");
      }
      DataBuf <<= 1;																			// move one bit to the left
    }
  }
}
