/*************************************************************
	*
	* @FileName			Timer_Ctrl.c
	* @Date					2017.03.17
	* @Author				DS.Chin
	* @Description	Timer through 24 hours
	*
**************************************************************/
#include "Timer_Ctrl.h"


/* @Brief Gloable Variables */
_TypeStructTime		TimerData;

//_Flag	SecondFlag;



///*****************************************************
// *
// * FunctionName		TMR1_IRQHandler
// * Brief					Timer1 interrupt every 1 second
// *
//******************************************************/
//void TMR1_IRQHandler(void)
//{
//	// Clear interrupt flag
//	TIMER_ClearIntFlag(TIMER1);
//	
//	if (TimerData.CntDwnHour != 0)
//	{
//		SecondFlag = TRUE;
//	}
//}


/*****************************************************
	*
	* @FunctionName		Count_Down_Control
	*
*****/
void Count_Down_Control(void)
{
	_Uint32	TimerRegBuf = 0;
	static _Uint16 TimerChk;
	
	/* if count down hour equals 0, then exit this function */
	if (TimerData.CntDwnHour == 0)			
	{
		TimerRegBuf = TIMER1->CTL;
		if ((TimerRegBuf & (1 << 25)) == (1 << 25))
		{
			TIMER1->CTL &= ~(1 << 26);			//Reset timer1
			TIMER_ClearIntFlag(TIMER1);		//Clear flag
		}
		return;
	}
	
	
	/* if count down hour unequals 0, then start the time */
	// Initial the real time
	if (TimerData.InitFlag == TRUE)
	{
		TimerData.InitFlag = FALSE;
		TimerData.RealTime.MicroSec		= 0;
		TimerData.RealTime.Second			= 0;
		TimerData.RealTime.Minute			= 0;
		TimerData.RealTime.Hour				= 0;
		TimerChk = 0;
		TIMER_Start(TIMER1);
		TIMER_ClearIntFlag(TIMER1);		//Clear flag
	}
	
	
	/* Real Time */
	if (TIMER_GetIntFlag(TIMER1))
	{
		TIMER_ClearIntFlag(TIMER1);
		TimerChk++;
		if (TimerChk >= 15000)
		{
			TimerChk = 0;
			return;
		}
		TimerData.RealTime.Second++;
		
		if (TimerData.RealTime.Second >= 60)		// 1minute
		{
			TimerData.RealTime.Second		= 0;
			TimerData.RealTime.Minute ++;
			if (TimerData.RealTime.Minute >= 60)	// 1hour
			{
				TimerData.RealTime.Minute	= 0;
				TimerData.RealTime.Hour	++;
				if (TimerData.RealTime.Hour >= 24)	// 1day
				//if (TimerData.RealTime.Hour >= 12)	// 1day
				{
					TimerData.RealTime.Hour	= 0;
				}
			}
		}

		
		
		/* Get the timer hour status */
		if (TimerData.RealTime.MicroSec		== 0
			&&TimerData.RealTime.Second			== 0
			&&TimerData.RealTime.Minute			== 0
			&&TimerData.RealTime.Hour				== 0)
		{
			TimerData.Status		= TIM_OPEN;
			TimerData.OnOffFlag	= TRUE;
		}
		
		else
		if (TimerData.RealTime.MicroSec		== 0
			&&TimerData.RealTime.Second			== 0
			&&TimerData.RealTime.Minute			== 0
			&&TimerData.RealTime.Hour				== TimerData.CntDwnHour)
		{
			TimerData.Status		= TIM_CLOSE;
			TimerData.OnOffFlag = FALSE;
		}
	}
}










