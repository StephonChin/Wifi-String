/*******************************************************************************
	*
	* FILE NAME
	*						Led_Display.h
	*
*******************************************************************************/
#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__

/*<-------------------- Include Files --------------------------------> */
#include "Type_Def.h"
#include "Led_Class.h"
#include "Data_Process.h"
#include "stdlib.h"

/*<--------------------- Const Value Define --------------------------> */
#define	FADE_LEVEL				50
#define	RAND()						RndSeed += 17;\
													srand(RndSeed);

													

/*<-------------------- Type Redefine --------------------------------> */
typedef struct _DISPLAY_TYPE
{
	_Uint8	Mode;
	_Uint8	ModeBuf;
	_Uint8	ColorValue;
	_Flag		InitFlag;
}_Type_Display;




/*<-------------------- Exported Functions ---------------------------> */
void	Led_Display(void);

#if AL96
void All_Color(_Uint8 BufR, _Uint8 BufG, _Uint8 BufB, _Uint8 BufW);
#else
void All_Color(_Uint8 BufR, _Uint8 BufG, _Uint8 BufB);
#endif

void 		Blue_Flash(void);
void 		Green_Flash(void);
void		Red_Flash(void);

#if AL91 | AL94
void 		Horse_Whole_Test(void);
void 		Direct_Set(void);
#endif

void 		All_Off(void);
void 		Steady(void);
void 		Blink(void);
void 		Sparkle(void);
void 		Instead(void);
void 		Sea_Wave(void);
void 		Roll(void);
void 		Ripple(void);
void 		Flower_Bloom(void);
void 		Polar_Light(void);
void 		Color_Show(void);

void		Show_1(void);
void 		Show_1_0(void);
void 		Show_1_1(void);
void 		Show_1_2(void);
void 		Show_1_3(void);


void 		Show_2(void);
void 		Show_2_0(void);
void 		Show_2_1(void);
void 		Show_2_2(void);
void 		Show_2_3(void);
void 		Show_2_4(void);


void 		Show_3(void);
void 		Show_3_0(void);
void 		Show_3_1(void);
void 		Show_3_2(void);
void 		Show_3_3(void);
void 		Show_3_4(void);




/*<-------------------- Exported Parameters --------------------------> */
extern _Type_Display		DisplayData;

#if AL96
extern _Uint8					ColorData[16][4];
extern _Uint8					TempW;
extern _Uint8					FadeW;
#else
extern _Uint8					ColorData[16][3];
#endif

extern _Uint8					ColorNum;
extern _Uint8					TempCnt;
extern _Uint8					TempLed;
extern _Uint8					LedCnt;
extern _Uint8					TempR;
extern _Uint8					TempG;
extern _Uint8					TempB;
extern _Uint8					TempDuty;
extern _Uint8					FadeR;
extern _Uint8					FadeG;
extern _Uint8					FadeB;
extern _Uint8					TempColor;
extern _Uint8					ModeTime[50];
extern _Uint8					ModeStep[50];
extern _Uint8					HoldTime[50];
extern _Uint8					ModeColor[50];
extern _Uint8					ModeLed[50];
extern _Uint8					TempGrp;
extern _Uint8					SpeedTime;
extern _Uint8					KeepTime;
extern _Uint16				RndSeed;
extern _Uint8					ShowMode;
extern _Flag					ShowInit;
extern _Uint8					HorseCnt;
extern _Uint8					EndCnt;
extern _Uint8					RndCnt;
extern _Uint8					SteadyTime;
extern _Uint16				TimeLine;
extern _Uint8					TempStep;



extern _Uint8					TempGrp;
extern _Uint8					SteadyTime;
extern _Uint8					StepCnt;

extern _Uint16				HorseWaitCnt;
extern _Uint16				HorseWaitTime;
extern _Uint16				HorseStep;
extern _Flag					ModeSynFlag;

#if AL99
extern _Uint8					LedNum[6][GRP_LED];
#endif




#endif




