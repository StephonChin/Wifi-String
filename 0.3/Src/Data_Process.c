/*******************************************************************************
	*
	* FILE NAME
	*						Data_Process.c
	*
	* NODE
	*						About horse run mode
	*							if the mode is run mode, ripple mode, flower bloom mode, polar light mode, color show mode,
	*							only AL91 or AL94 have horse function.
	*
	*							if the mode is show1, show2, or show3
	*							all the product have horse function.
	*
*******************************************************************************/
#include "Data_Process.h"

//----------------------------------------------------------------------------
// Gloable Parameters
//----------------------------------------------------------------------------
_Uint8						PowerOnNum;
_Flag							WdtRstFlag;
_Uint8 						NormalColorPre;
_Uint8						NormalModePre;
_Uint8            ShowColorPre;
_Uint8            ShowModePre;
_Uint8            KeyMode;

const _Uint8  KEY_MODE[][2]={
  0x0,0x0,
  0x1,0x0,
  0x1,0x1,
  0x1,0x2,
  0x1,0x3,
  0x1,0x9,
  0x1,0xd,
  0x3,0x14,
  0x6,0x16,
  0x8,0x1c,
  0xb,0x0,
  0xc,0x2,
  0xd,0x6
};






/******************************************************************************
	*
	* COLOR_TABLE
	*
	* DESCRIPTION
	*								Color value combine with R value, G value and B value,
	*								or  with R value, G value , B value and W value
	*
*******************************************************************************/
const _Uint8 COLOR_VECTOR[18][3] =
{
  250,0,0,      		//0:red
  0,250,0,      		//1:green
  0,0,250,      		//2:blue
  250,50,0,     		//3:orange
  250,50,50,   		  //4:pink
  0,250,50,     		//5:spring green
  250,150,0,    		//6:gold
  250,0,50,     		//7:peach
  100,200,0,    		//8:lawn green
  250,0,250,    		//9:Cyan
  0,250,250,    		//a:sky blue
  200,250,0,    		//b:yellow
  100,0,250,     		//c:purple
	150,250,200,
	150,250,250,
	150,200,50,
	0,150,0,			    // dark green
	50,0,250,			    // DARK_RED
};




/************************************************************************************
	*
	* FUNCTION NAME		|	User_Data_Init
	*	INPUT						|	None
	* OUTPUT					|	None
	* BRIEF						|	Initialize the user data after power on, include display
	*										mode, color theme, time count down hour and others
	*
*************************************************************************************/
void User_Data_Init(void)
{
	/* Readt the data from APPROM */
	Flash_Read_Data();

	//-----------------------------------------------------------------
	// if the first time to power on
	//-----------------------------------------------------------------
	if (PowerOnNum != 0xAA)
	{
		DisplayData.ModeBuf 			= 0xA;
		DisplayData.ColorValue		= 0x1C;
		TimerData.CntDwnHour			= 6;

		PowerOnNum								= 0xAA;
		FlashSaveFlag							= TRUE;

		DisplayData.Mode 					= 0xA;
		DisplayData.InitFlag			= TRUE;
		Color_Value_Get(DisplayData.ColorValue);
		return;
	}

	//-----------------------------------------------------------------
	// Get the mode from eprom after power on
	// When display mode equl 0 (means turn off all leds, then set
	// the mode to 0xA (color show), and save it to eprom
	//-----------------------------------------------------------------
	if (DisplayData.ModeBuf == 0 || DisplayData.ModeBuf > 0xD)
	{
		DisplayData.ModeBuf 	= 0xA;
		FlashSaveFlag					= TRUE;
	}
	DisplayData.Mode				= DisplayData.ModeBuf;
	DisplayData.InitFlag		= TRUE;


	//---------------------------------------------------------------------
	// Get the color value from eprom after power on
	// When the color value is more than the COLOR_MAX, set it to COLOR_MAX
	// and save the value to eprom
	//----------------------------------------------------------------------
	if (DisplayData.ColorValue > COLOR_MAX)
	{
		DisplayData.ColorValue 	= COLOR_MAX;
		FlashSaveFlag						= TRUE;
	}
	Color_Value_Get(DisplayData.ColorValue);


	//----------------------------------------------------------------------
	// Get the count down hour from eprom after power on,
	// if valid value is more than 8 , seit it to 6,
	// and save6 to eprom
	// Set the OnOffFlag
	//----------------------------------------------------------------------
	if (TimerData.CntDwnHour > 8 || (TimerData.CntDwnHour % 2 == 1))
	{
		TimerData.CntDwnHour	= 0;
		FlashSaveFlag					= TRUE;
	}
	TimerData.InitFlag	= TRUE;
	TimerData.OnOffFlag = TRUE;
}



/*************************************************************************************
	*
	* FUNCTION NAME		|	Data_Process
	* INPUT						|	None
	* OUTPUT					|	None
	* DESCRIPTION			|	Process all the data here. change the mode , color, timer or
	*										other parameters after received valid data from CSR1010
	*
***************************************************************************************/
void Data_Process(void)
{
	/* Key Process */
	Key_Process();

	/* Timer Process */
	Count_Down_Process();

	/* CSR1010 Process */
	USART_Process();
}


/************************************************************
	*
	* @FunctonName	Key_Process
	* @Brief				Process the key
	* 							Change the color and mode after short key
	*/
static void Key_Process(void)
{
	if (KeyModeStatus == KEY_SHORT){
		KeyModeStatus = KEY_IDLE;

		KeyMode++;
    if (KeyMode > 12)   KeyMode = 0;
    
    DisplayData.Mode = KEY_MODE[KeyMode][0];
    DisplayData.ColorValue = KEY_MODE[KeyMode][1];
    
    
    //ColorValue get
    Color_Value_Get(DisplayData.ColorValue);
    DisplayData.InitFlag = TRUE;
    
    
    if (DisplayData.Mode != 0){
      DisplayData.ModeBuf = DisplayData.Mode;
      FlashSaveFlag = TRUE;
    }
		return;
	}
  
  if (KeyColorStatus == KEY_SHORT){
    KeyColorStatus = KEY_IDLE;
    
  }
  
  
  

	/* Key Long Pressed */
	if (KeyModeStatus == KEY_LONG){
		KeyModeStatus = KEY_IDLE;
    
    TimerData.CntDwnHour += 2;
    if (TimerData.CntDwnHour > 8){
      TimerData.CntDwnHour = 0;
    }
    
    TimerData.InitFlag			= TRUE;
    FlashSaveFlag						= TRUE;

    DisplayData.Mode			  = 0xFB;
    DisplayData.InitFlag	  = TRUE;
	}
}


/************************************************************
	*
	* @FunctonName	Count_Down_Process
	*
*****/
static void Count_Down_Process(void)
{
  //Timer Led display
  switch (TimerData.CntDwnHour){
    default:{
      PORT_2H = RESET;
      PORT_4H = RESET;
      PORT_6H = RESET;
      PORT_8H = RESET;
    } break;
    
    case 2:{
      PORT_2H = SET;
      PORT_4H = RESET;
      PORT_6H = RESET;
      PORT_8H = RESET;
    } break;
    
    case 4:{
      PORT_2H = RESET;
      PORT_4H = SET;
      PORT_6H = RESET;
      PORT_8H = RESET;
    } break;
    
    case 6:{
      PORT_2H = RESET;
      PORT_4H = RESET;
      PORT_6H = SET;
      PORT_8H = RESET;
    } break;
    
    case 8:{
      PORT_2H = RESET;
      PORT_4H = RESET;
      PORT_6H = RESET;
      PORT_8H = SET;
    } break;
  }
  
  
  
  
	/* When TimerData.Status equals TIM_IDLE, exit this function */
	if (TimerData.Status == TIM_IDLE)	return;

	/* Open */
	if (TimerData.Status == TIM_OPEN)
	{
		TimerData.Status			= TIM_IDLE;
		DisplayData.Mode			= DisplayData.ModeBuf;
		DisplayData.InitFlag	= TRUE;
		return;
	}

	/* Close */
	if (TimerData.Status == TIM_CLOSE)
	{
		TimerData.Status			= TIM_IDLE;
		DisplayData.Mode			= 0x0;
		DisplayData.InitFlag	= TRUE;
		return;
	}
}


/****************************************************************************************
	*
	* FUNCTIONNAME		| USART_Process
	*	INPUT						| None
	* OUTPUT					| None
	* DESCRIPTION			| Send data to or received data via USART
	*									| Process the data
	*
*****************************************************************************************/
static void USART_Process(void)
{
	//If there're no any data recieved
	if (RcvFlag == FALSE)		return;
	RcvFlag = FALSE;

	//Verify the USART version
	if(USART_Buffer.version != PROTOCOL_VER)		return;

	switch (USART_Buffer.cmd){
		case CMD_SOCKET_ON:{
			DisplayData.Mode			= 0;
			DisplayData.InitFlag	= TRUE;
		}break;

		case CMD_SOCKET_OFF:{
			DisplayData.Mode 			= DisplayData.ModeBuf;
			DisplayData.InitFlag	= TRUE;
		}break;

		case CMD_THEME_SET:{
			//If the lights have turned off, exit
			if (DisplayData.Mode == 0)		break;

			//If the current mode is show mode,then recovery it by the previos normal mode
			if (DisplayData.Mode > 0xA){
				DisplayData.Mode 			= NormalModePre;
				DisplayData.ModeBuf		= NormalModePre;
			}
      
      if (DisplayData.Mode > 0x8){
        DisplayData.Mode      = 0x1;
        DisplayData.ModeBuf   = 0x1;
        NormalModePre         = 0x1;
      }
      
			DisplayData.ColorValue	= USART_Buffer.payload[0];
			NormalColorPre					= DisplayData.ColorValue;		//Save the normal color
			Color_Value_Get(DisplayData.ColorValue);
			DisplayData.InitFlag		= TRUE;
			FlashSaveFlag						= TRUE;
		}break;

		case CMD_NORMAL_MODE:{
			//If the lights have turned off, exit
			if (DisplayData.Mode == 0)		break;

			//If the current mode is show mode, then recovery the color by previos normal color
			if (DisplayData.Mode > 0xA){
				DisplayData.ColorValue = NormalColorPre;
				Color_Value_Get(DisplayData.ColorValue);
			}
			DisplayData.Mode				= USART_Buffer.payload[0] + 1;
			DisplayData.ModeBuf			= DisplayData.Mode;
      if (DisplayData.Mode < 0x9)			NormalModePre = DisplayData.Mode;				//Save the normal mode except 0x9 0xA
			DisplayData.InitFlag    = TRUE;
      FlashSaveFlag						= TRUE;
		}break;

		case CMD_SHOW_MODE:{
			//If the lights have turned off, exit
			if (DisplayData.Mode == 0)		break;

			RndSeed += 199;
			srand(RndSeed);
			DisplayData.Mode 				= (_Uint8)rand() % 3 + 0xB;
			DisplayData.ModeBuf			= DisplayData.Mode;
			DisplayData.ColorValue	= USART_Buffer.payload[0];
			DisplayData.InitFlag		= TRUE;
			FlashSaveFlag						= TRUE;
      Color_Value_Get(DisplayData.ColorValue);
		}break;

		case CMD_TIMER_SET:{
			//If the lights have turned off, exit
			if (DisplayData.Mode == 0)		break;

			//if (USART_Buffer.payload[0] > 8 || (USART_Buffer.payload[0] % 2 == 1))			break;
			TimerData.CntDwnHour		= USART_Buffer.payload[0];
			TimerData.InitFlag			= TRUE;
			FlashSaveFlag						= TRUE;

			DisplayData.Mode			= 0xFB;
			DisplayData.InitFlag	= TRUE;
		}break;
	}
}


/****************************************************************************
 * Color_Value_Get
 */
void Color_Value_Get(_Uint8 ColorNumBuf)
{
	_Uint8	TempCnt = 0;

	if (DisplayData.Mode > 0xA)
	{
		ColorNum = 12;
		switch (ColorNumBuf)
		{
			case 0x6:
			{
				for (TempCnt = 0; TempCnt < 16; TempCnt++)
				{
					ColorData[TempCnt][0] = COLOR_VECTOR[TempCnt][0];
					ColorData[TempCnt][1] = COLOR_VECTOR[TempCnt][1];
					ColorData[TempCnt][2] = COLOR_VECTOR[TempCnt][2];
				}
			} break;

			/* christmas */
			case 0:
			{
				for (TempCnt = 0; TempCnt < 16; TempCnt++)
				{
					if ( (TempCnt % 5) == 0)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[GREEN][2];
					}
					else
					if ((TempCnt % 5) == 1)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[RED][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[RED][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[RED][2];
					}
					else
					if ((TempCnt % 5) == 2)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[WHITE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[WHITE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[WHITE][2];
					}
					else
					if ((TempCnt % 5) == 3)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[DARK_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[DARK_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[DARK_GREEN][2];
					}
					else
					if ((TempCnt % 5) == 4)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[COLD_WHITE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[COLD_WHITE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[COLD_WHITE][2];
					}
				}
			} break;


			/* Valentines' Day */
			case 1:
			{
				for (TempCnt = 0; TempCnt < 16; TempCnt++)
				{
					if ((TempCnt % 7) == 0)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PINK][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PINK][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PINK][2];
					}
					else if ((TempCnt % 7) == 1)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PEACH][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PEACH][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PEACH][2];
					}
					else if ( (TempCnt % 7) == 2)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PINK][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PINK][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PINK][2];
					}
					else if ((TempCnt % 7) == 3)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[LAWN_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[LAWN_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[LAWN_GREEN][2];
					}
					else if ((TempCnt % 7) == 4)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PEACH][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PEACH][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PEACH][2];
					}
					else if ( (TempCnt % 7) == 5)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PINK][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PINK][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PINK][2];
					}
					else if ((TempCnt % 7) == 6)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PEACH][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PEACH][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PEACH][2];
					}
				}
			} break;


			/* Independence day */
			case 2:
			{
				#ifdef	_USA_
					for (TempCnt = 0; TempCnt < 16; TempCnt++)
					{
						if ( (TempCnt % 4) == 0)
						{
							ColorData[TempCnt][0] = COLOR_VECTOR[RED][0];
							ColorData[TempCnt][1] = COLOR_VECTOR[RED][1];
							ColorData[TempCnt][2] = COLOR_VECTOR[RED][2];
						}
						else
						if ((TempCnt % 4) == 1)
						{
							ColorData[TempCnt][0] = COLOR_VECTOR[BLUE][0];
							ColorData[TempCnt][1] = COLOR_VECTOR[BLUE][1];
							ColorData[TempCnt][2] = COLOR_VECTOR[BLUE][2];
						}
						else
						if ((TempCnt % 4) == 2)
						{
							ColorData[TempCnt][0] = COLOR_VECTOR[COLD_WHITE][0];
							ColorData[TempCnt][1] = COLOR_VECTOR[COLD_WHITE][1];
							ColorData[TempCnt][2] = COLOR_VECTOR[COLD_WHITE][2];
						}
						else
						if ((TempCnt % 4) == 3)
						{
							ColorData[TempCnt][0] = COLOR_VECTOR[BLUE][0];
							ColorData[TempCnt][1] = COLOR_VECTOR[BLUE][1];
							ColorData[TempCnt][2] = COLOR_VECTOR[BLUE][2];
						}
					}
				#endif

				#ifdef	_CAN_
					for (TempCnt = 0; TempCnt < 16; TempCnt++)
					{
						if ( (TempCnt % 2) == 0)
						{
							ColorData[TempCnt][0] = COLOR_VECTOR[RED][0];
							ColorData[TempCnt][1] = COLOR_VECTOR[RED][1];
							ColorData[TempCnt][2] = COLOR_VECTOR[RED][2];
						}
						else
						if ((TempCnt % 2) == 1)
						{
							ColorData[TempCnt][0] = COLOR_VECTOR[COLD_WHITE][0];
							ColorData[TempCnt][1] = COLOR_VECTOR[COLD_WHITE][1];
							ColorData[TempCnt][2] = COLOR_VECTOR[COLD_WHITE][2];
						}
					}
				#endif
			} break;

			/* Thanks giving */
			case 3:
			{
				for (TempCnt = 0; TempCnt < 16; TempCnt++)
				{
					if ( (TempCnt % 5) == 0)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[SPRING_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[SPRING_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[SPRING_GREEN][2];
					}
					else
					if ((TempCnt % 5) == 1)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[ORANGE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[ORANGE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[ORANGE][2];
					}
					else
					if ((TempCnt % 5) == 2)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[LAWN_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[LAWN_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[LAWN_GREEN][2];
					}
					else
					if ((TempCnt % 5) == 3)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[RED][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[RED][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[RED][2];
					}
					else
					if ((TempCnt % 5) == 4)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[GOLD][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[GOLD][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[GOLD][2];
					}
				}
			} break;

			/* st. patrick's day */
			case 4:
			{
				for (TempCnt = 0; TempCnt < 16; TempCnt++)
				{
					if ( (TempCnt % 4) == 0)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[GREEN][2];
					}
					else
					if ((TempCnt % 4) == 1)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[SPRING_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[SPRING_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[SPRING_GREEN][2];
					}
					else
					if ((TempCnt % 4) == 2)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[DARK_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[DARK_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[DARK_GREEN][2];
					}
					else
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[LAWN_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[LAWN_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[LAWN_GREEN][2];
					}
				}
			} break;

			/* Halloween */
			case 5:
			{
				for (TempCnt = 0; TempCnt < 16; TempCnt++)
				{
					if ( (TempCnt % 9) == 0)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[ORANGE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[ORANGE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[ORANGE][2];
					}
					else
					if ((TempCnt % 9) == 1)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PURPLE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PURPLE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PURPLE][2];
					}
					else
					if ((TempCnt % 9) == 2)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[DARK_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[DARK_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[DARK_GREEN][2];
					}
					else
					if ((TempCnt % 9) == 3)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[DARK_RED][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[DARK_RED][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[DARK_RED][2];
					}
					else
					if ((TempCnt % 9) == 4)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PURPLE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PURPLE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PURPLE][2];
					}
					else
					if ((TempCnt % 9) == 5)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[GREEN][2];
					}
					else
					if ((TempCnt % 9) == 6)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[PURPLE][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[PURPLE][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[PURPLE][2];
					}
					else
					if ((TempCnt % 9) == 7)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[DARK_GREEN][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[DARK_GREEN][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[DARK_GREEN][2];
					}
					else
					if ((TempCnt % 9) == 8)
					{
						ColorData[TempCnt][0] = COLOR_VECTOR[DARK_RED][0];
						ColorData[TempCnt][1] = COLOR_VECTOR[DARK_RED][1];
						ColorData[TempCnt][2] = COLOR_VECTOR[DARK_RED][2];
					}
				}
			} break;
		}
	}

	else
	{
		//ColorNumBuf = 0x0d;
		// single color
		if (ColorNumBuf < 0x10)
		{
			ColorData[0][0] = COLOR_VECTOR[ColorNumBuf][0];
			ColorData[0][1] = COLOR_VECTOR[ColorNumBuf][1];
			ColorData[0][2] = COLOR_VECTOR[ColorNumBuf][2];
			ColorNum = 1;
		} // single color

		// many color
		else
		{
				switch (ColorNumBuf)
				{
						/* spring */
						case 0x10:
								ColorData[0][0] = COLOR_VECTOR[SPRING_GREEN][0];
								ColorData[0][1] = COLOR_VECTOR[SPRING_GREEN][1];
								ColorData[0][2] = COLOR_VECTOR[SPRING_GREEN][2];

								ColorData[1][0] = COLOR_VECTOR[LAWN_GREEN][0];
								ColorData[1][1] = COLOR_VECTOR[LAWN_GREEN][1];
								ColorData[1][2] = COLOR_VECTOR[LAWN_GREEN][2];

								ColorData[2][0] = COLOR_VECTOR[PINK][0];
								ColorData[2][1] = COLOR_VECTOR[PINK][1];
								ColorData[2][2] = COLOR_VECTOR[PINK][2];

								ColorNum = 3;
						break;

						/* summer*/
						case 0x11:
								ColorData[0][0] = COLOR_VECTOR[GREEN][0];
								ColorData[0][1] = COLOR_VECTOR[GREEN][1];
								ColorData[0][2] = COLOR_VECTOR[GREEN][2];

								ColorData[1][0] = COLOR_VECTOR[RED][0];
								ColorData[1][1] = COLOR_VECTOR[RED][1];
								ColorData[1][2] = COLOR_VECTOR[RED][2];

								ColorData[2][0] = COLOR_VECTOR[LAWN_GREEN][0];
								ColorData[2][1] = COLOR_VECTOR[LAWN_GREEN][1];
								ColorData[2][2] = COLOR_VECTOR[LAWN_GREEN][2];

								ColorData[3][0] = COLOR_VECTOR[ORANGE][0];
								ColorData[3][1] = COLOR_VECTOR[ORANGE][1];
								ColorData[3][2] = COLOR_VECTOR[ORANGE][2];

								ColorNum = 4;
						break;

						/* autumn*/
						case 0x12:
								ColorData[0][0] = COLOR_VECTOR[ORANGE][0];
								ColorData[0][1] = COLOR_VECTOR[ORANGE][1];
								ColorData[0][2] = COLOR_VECTOR[ORANGE][2];

								ColorData[1][0] = COLOR_VECTOR[GOLD][0];
								ColorData[1][1] = COLOR_VECTOR[GOLD][1];
								ColorData[1][2] = COLOR_VECTOR[GOLD][2];

								ColorData[2][0] = COLOR_VECTOR[ORANGE][0];
								ColorData[2][1] = COLOR_VECTOR[ORANGE][1];
								ColorData[2][2] = COLOR_VECTOR[ORANGE][2];

								ColorData[3][0] = COLOR_VECTOR[CYAN][0];
								ColorData[3][1] = COLOR_VECTOR[CYAN][1];
								ColorData[3][2] = COLOR_VECTOR[CYAN][2];

								ColorNum = 4;

						break;

						/* winter */
						case 0x13:
								ColorData[0][0] = COLOR_VECTOR[COLD_WHITE][0];
								ColorData[0][1] = COLOR_VECTOR[COLD_WHITE][1];
								ColorData[0][2] = COLOR_VECTOR[COLD_WHITE][2];

								ColorData[1][0] = COLOR_VECTOR[SKY_BLUE][0];
								ColorData[1][1] = COLOR_VECTOR[SKY_BLUE][1];
								ColorData[1][2] = COLOR_VECTOR[SKY_BLUE][2];


								ColorData[2][0] = COLOR_VECTOR[BLUE][0];
								ColorData[2][1] = COLOR_VECTOR[BLUE][1];
								ColorData[2][2] = COLOR_VECTOR[BLUE][2];

								ColorNum = 3;
						break;

						/* Christmas' day */
						case 0x14:
								ColorData[0][0] = COLOR_VECTOR[RED][0];
								ColorData[0][1] = COLOR_VECTOR[RED][1];
								ColorData[0][2] = COLOR_VECTOR[RED][2];;

								ColorData[1][0] = COLOR_VECTOR[GREEN][0];
								ColorData[1][1] = COLOR_VECTOR[GREEN][1];
								ColorData[1][2] = COLOR_VECTOR[GREEN][2];

								ColorData[2][0] = COLOR_VECTOR[WHITE][0];
								ColorData[2][1] = COLOR_VECTOR[WHITE][1];
								ColorData[2][2] = COLOR_VECTOR[WHITE][2];

								ColorNum = 3;
						break;

						/* Valentines' day */
						case 0x15:
								for (TempCnt = 0; TempCnt < 16; TempCnt++)
								{
									if ((TempCnt % 7) == 0)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PINK][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PINK][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PINK][2];
									}
									else if ((TempCnt % 7) == 1)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PEACH][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PEACH][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PEACH][2];
									}
									else if ( (TempCnt % 7) == 2)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PINK][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PINK][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PINK][2];
									}
									else if ((TempCnt % 7) == 3)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[LAWN_GREEN][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[LAWN_GREEN][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[LAWN_GREEN][2];
									}
									else if ((TempCnt % 7) == 4)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PEACH][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PEACH][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PEACH][2];
									}
									else if ( (TempCnt % 7) == 5)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PINK][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PINK][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PINK][2];
									}
									else if ((TempCnt % 7) == 6)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PEACH][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PEACH][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PEACH][2];
									}
								}


								ColorNum = 7;
						break;

						/* Independence day */
						case 0x16:
							#ifdef	_USA_
								ColorData[0][0] = COLOR_VECTOR[RED][0];
								ColorData[0][1] = COLOR_VECTOR[RED][1];
								ColorData[0][2] = COLOR_VECTOR[RED][2];

								ColorData[1][0] = COLOR_VECTOR[WHITE][0];
								ColorData[1][1] = COLOR_VECTOR[WHITE][1];
								ColorData[1][2] = COLOR_VECTOR[WHITE][2];

								ColorData[2][0] = COLOR_VECTOR[BLUE][0];
								ColorData[2][1] = COLOR_VECTOR[BLUE][1];
								ColorData[2][2] = COLOR_VECTOR[BLUE][2];

								ColorNum = 3;
							#endif

							#ifdef	_CAN_
								ColorData[0][0] = COLOR_VECTOR[RED][0];
								ColorData[0][1] = COLOR_VECTOR[RED][1];
								ColorData[0][2] = COLOR_VECTOR[RED][2];

								ColorData[1][0] = COLOR_VECTOR[WHITE][0];
								ColorData[1][1] = COLOR_VECTOR[WHITE][1];
								ColorData[1][2] = COLOR_VECTOR[WHITE][2];

								ColorNum = 2;
							#endif


						break;

						/* Thanks giving */
						case 0x17:
								ColorData[0][0] = COLOR_VECTOR[SPRING_GREEN][0];
								ColorData[0][1] = COLOR_VECTOR[SPRING_GREEN][1];
								ColorData[0][2] = COLOR_VECTOR[SPRING_GREEN][2];

								ColorData[1][0] = COLOR_VECTOR[ORANGE][0];
								ColorData[1][1] = COLOR_VECTOR[ORANGE][1];
								ColorData[1][2] = COLOR_VECTOR[ORANGE][2];

								ColorData[2][0] = COLOR_VECTOR[RED][0];
								ColorData[2][1] = COLOR_VECTOR[RED][1];
								ColorData[2][2] = COLOR_VECTOR[RED][2];

								ColorData[3][0] = COLOR_VECTOR[LAWN_GREEN][0];
								ColorData[3][1] = COLOR_VECTOR[LAWN_GREEN][1];
								ColorData[3][2] = COLOR_VECTOR[LAWN_GREEN][2];

								ColorData[4][0] = COLOR_VECTOR[GOLD][0];
								ColorData[4][1] = COLOR_VECTOR[GOLD][1];
								ColorData[4][2] = COLOR_VECTOR[GOLD][2];

								ColorNum = 5;
						break;

						/* st. patrick's day */
						case 0x18:
								ColorData[0][0] = COLOR_VECTOR[GREEN][0];
								ColorData[0][1] = COLOR_VECTOR[GREEN][1];
								ColorData[0][2] = COLOR_VECTOR[GREEN][2];

								ColorData[1][0] = COLOR_VECTOR[SPRING_GREEN][0];
								ColorData[1][1] = COLOR_VECTOR[SPRING_GREEN][1];
								ColorData[1][2] = COLOR_VECTOR[SPRING_GREEN][2];

								ColorData[2][0] = COLOR_VECTOR[LAWN_GREEN][0];
								ColorData[2][1] = COLOR_VECTOR[LAWN_GREEN][1];
								ColorData[2][2] = COLOR_VECTOR[LAWN_GREEN][2];



								ColorNum = 3;
						break;

						/* Halloween */
						case 0x19:
								for (TempCnt = 0; TempCnt < 16; TempCnt++)
								{
									if ( (TempCnt % 9) == 0)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[ORANGE][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[ORANGE][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[ORANGE][2];
									}
									else
									if ((TempCnt % 9) == 1)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PURPLE][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PURPLE][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PURPLE][2];
									}
									else
									if ((TempCnt % 9) == 2)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[DARK_GREEN][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[DARK_GREEN][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[DARK_GREEN][2];
									}
									else
									if ((TempCnt % 9) == 3)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[DARK_RED][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[DARK_RED][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[DARK_RED][2];
									}
									else
									if ((TempCnt % 9) == 4)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PURPLE][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PURPLE][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PURPLE][2];
									}
									else
									if ((TempCnt % 9) == 5)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[GREEN][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[GREEN][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[GREEN][2];
									}
									else
									if ((TempCnt % 9) == 6)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[PURPLE][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[PURPLE][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[PURPLE][2];
									}
									else
									if ((TempCnt % 9) == 7)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[DARK_GREEN][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[DARK_GREEN][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[DARK_GREEN][2];
									}
									else
									if ((TempCnt % 9) == 8)
									{
										ColorData[TempCnt][0] = COLOR_VECTOR[DARK_RED][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[DARK_RED][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[DARK_RED][2];
									}
								}

								ColorNum = 9;
						break;

						/* sun */
						case 0x1A:
								ColorData[0][0] = COLOR_VECTOR[RED][0];
								ColorData[0][1] = COLOR_VECTOR[RED][1];
								ColorData[0][2] = COLOR_VECTOR[RED][2];

								ColorData[1][0] = COLOR_VECTOR[ORANGE][0];
								ColorData[1][1] = COLOR_VECTOR[ORANGE][1];
								ColorData[1][2] = COLOR_VECTOR[ORANGE][2];

								ColorData[2][0] = COLOR_VECTOR[GOLD][0];
								ColorData[2][1] = COLOR_VECTOR[GOLD][1];
								ColorData[2][2] = COLOR_VECTOR[GOLD][2];

								ColorData[3][0] = COLOR_VECTOR[YELLOW][0];
								ColorData[3][1] = COLOR_VECTOR[YELLOW][1];
								ColorData[3][2] = COLOR_VECTOR[YELLOW][2];

								ColorNum = 4;
						break;

						/* earth */
						case 0x1B:
								ColorData[0][0] = COLOR_VECTOR[BLUE][0];
								ColorData[0][1] = COLOR_VECTOR[BLUE][1];
								ColorData[0][2] = COLOR_VECTOR[BLUE][2];

								ColorData[1][0] = COLOR_VECTOR[GREEN][0];
								ColorData[1][1] = COLOR_VECTOR[GREEN][1];
								ColorData[1][2] = COLOR_VECTOR[GREEN][2];

								ColorData[2][0] = COLOR_VECTOR[SKY_BLUE][0];
								ColorData[2][1] = COLOR_VECTOR[SKY_BLUE][1];
								ColorData[2][2] = COLOR_VECTOR[SKY_BLUE][2];


								ColorData[3][0] = COLOR_VECTOR[ORANGE][0];
								ColorData[3][1] = COLOR_VECTOR[ORANGE][1];
								ColorData[3][2] = COLOR_VECTOR[ORANGE][2];

								ColorNum = 4;
						break;

						/* Multi */
						case 0x1C:
							for (TempCnt = 0; TempCnt < 12; TempCnt++)
								{
										ColorData[TempCnt][0] = COLOR_VECTOR[TempCnt][0];
										ColorData[TempCnt][1] = COLOR_VECTOR[TempCnt][1];
										ColorData[TempCnt][2] = COLOR_VECTOR[TempCnt][2];
								}

								ColorNum = 12;
						break;
				}//switch (RcvData[1])
		} // many color
	}
}
