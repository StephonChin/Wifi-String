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
	*									Include KM5020's LED driver protocol
 	*									Include UCS1903BC's Led driver protocol
	*									Include MCU PWM driver
	*									Include custom protocol
	*
***************************************************************************************/
void Led_Update(void)
{
//UCS1903BC---------------------------------AL94--------------------------------
	_Uint16	TempCnt 		= 0;
	_Uint8	TempColor		= 0;
  _Uint8  TempBit  		= 0;
	_Uint8	*DataBufAdd	= 0;
	_Uint8  DataBuf 		= 0;
	
	/* run from the header to tail */																			
  for (TempCnt = 0; TempCnt < LED_TOTAL; TempCnt++)			/* Circle count caculated													*/
  {
    DataBufAdd = &LedData.Buffer[TempCnt].DutyR;
    
    for (TempColor = 0; TempColor < 3; TempColor++)
    {
      DataBuf = *DataBufAdd++;
      DataBuf /= 2;
      /* Address increase																*/
      for (TempBit = 0; TempBit < 8; TempBit++)							/* Send the bit data one bye one									*/
      {
        if ((DataBuf & 0x80) == 0x80)												/* Bit 1 data send																*/
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
        else																								/* Bit0 data send																	*/
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
        DataBuf <<= 1;																			/* move one bit to the left 											*/					
      }
    }
  }
}

