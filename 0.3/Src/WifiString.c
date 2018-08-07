/**********************************************************************************
	*
	* FUNCTION NAME				ShowHomeApp.proj
	* AUTHOR							DS.Chin
	* COPYRIGHT						(C) CopyRight  @2017 ~ 2027 Willis Ltd. Co.
	*
************************************************************************************/
#include "WifiString.h"

	
/************************************************************************************
	*
	* FunctionName	main
	*
	* Brief					The Programme entrance
	*
*************************************************************************************/
int main(void)
{
	/* If the MCU reset by WDT,
	 *  then set the flag WdtRstFlag, else reset the flag WdtRstFlag,
	 */
	if ((WDT->CTL & WDT_CTL_RSTF_Msk) == WDT_CTL_RSTF_Msk)
	{
		WdtRstFlag = TRUE;
	}
	else
	{
		WdtRstFlag = FALSE;
	}
	
	/* Initialize the MCU peripheral */
	MCU_Config();

	/* Read the Mode, Coor & Timer after Power On */
	User_Data_Init();

	/* Clear the UART0 receive FIFO */
	Uart_Clear_Buf();

	while (1)
	{
		if (TIMER_GetIntFlag(TIMER0))
		{
			TIMER_ClearIntFlag(TIMER0);
			
			// Clear the WDT count number
			SYS_UnlockReg();
			WDT->CTL &= WDT_CTL_RSTCNT_Msk;			//Clear WDT Cnt
			SYS_LockReg();
			
			Key_Scan();
			
			Data_Process();

			Led_Display();
			
			Led_Update();
			
			Count_Down_Control();	
			
			Flash_Write_Data();
		}
	}
}


