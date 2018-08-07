/****************************************************************************
 *
 * FunctionName		Key.c
 *
 * Brief
 *								Get the key status
 *
*****************************************************************************/
#include "Key.h"


/*------------------------< Gloable Parameters >------------------------*/
_EnKey KeyModeStatus;
_EnKey KeyColorStatus;


/*
 * FunctionName		Key_Scan
 */
void Key_Scan(void)
{
	static _Flag			KeyModeDoneFlag;
	static _Uint8			KeyModeTime;
  
  static _Flag			KeyColorDoneFlag;
	static _Uint8			KeyColorTime;
	
	if (PORT_KEY_MODE == RESET)
	{
		if (KeyModeDoneFlag == FALSE)
		{
			KeyModeTime++;
			if (KeyModeTime > 70)
			{
				KeyModeDoneFlag = TRUE;
				KeyModeStatus = KEY_LONG;
			}
		}
	}
	
	else
	{
		if (KeyModeDoneFlag == FALSE && KeyModeTime > 1)
		{
			KeyModeStatus = KEY_SHORT;
		}
		KeyModeTime = 0;
		KeyModeDoneFlag = FALSE;
	}
  
  
  
  if (PORT_KEY_COLOR == RESET)
	{
		if (KeyColorDoneFlag == FALSE)
		{
			KeyColorTime++;
			if (KeyColorTime > 70)
			{
				KeyColorDoneFlag = TRUE;
				KeyColorStatus = KEY_LONG;
			}
		}
	}
	
	else
	{
		if (KeyColorDoneFlag == FALSE && KeyColorTime > 1)
		{
			KeyColorStatus = KEY_SHORT;
		}
		KeyColorTime = 0;
		KeyColorDoneFlag = FALSE;
	}
}
