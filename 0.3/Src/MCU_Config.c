/*******************************************************************************
	*
	* FILE NAME
	*						MCU_Config.c
	*
*******************************************************************************/
#include "MCU_Config.h"


/* Gloable Parameters */
_Uint8	HseStatus;


/**
	* FunctionName		MCU_Config
	*/
void MCU_Config(void)
{
	CLK_Init();
	WDT_Init();
	GPIO_Init();
	UART_Init();
	TIM_Init();
}

/**
	* FunctionName		CLK_Init
	*/
static void CLK_Init(void)
{
	/*---------------------------------------------------------------------------------------------------------*/
	/* Init System Clock                                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	/* Unlock protected registers */
	SYS_UnlockReg();

	/* Set P5 multi-function pins for crystal output/input */
	SYS->P5_MFP &= ~(SYS_MFP_P50_Msk | SYS_MFP_P51_Msk);
	SYS->P5_MFP |= (SYS_MFP_P50_XT1_IN | SYS_MFP_P51_XT1_OUT);

	/* Enable HXT clock (external XTAL 12MHz) */
	CLK_EnableXtalRC(CLK_PWRCTL_XTLEN_HXT);

	/* Wait for HXT clock ready */
	HseStatus = (_Uint8)CLK_WaitClockReady(CLK_STATUS_XTLSTB_Msk);

	if (HseStatus == 0) // Change the HCLK to HIRC if HXT clock doesn't work
	{
		/* Enable HIRC clock (Internal RC 22.1184MHz) */
		CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
		/* Wait for HIRC clock ready */
		CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
		/* Select HCLK clock source as HIRC and and HCLK source divider as 1 */
		CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV_HCLK(1));
	}

	/* Set core clock as PLL_CLOCK from PLL */
	CLK_SetCoreClock(PLL_CLOCK);


	// Enable LIRC for WDT
	CLK->PWRCTL = CLK_PWRCTL_LIRCEN_Msk;
	// Wait for LIRC
	CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);
	// Enable WDT IP clock
	CLK->APBCLK = CLK_APBCLK_WDTCKEN_Msk;
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_WDTSEL_Msk) | CLK_CLKSEL1_WDTSEL_IRC10K;


	/* Reset UART clock */
  CLK->APBCLK &= ~CLK_APBCLK_UART0CKEN_Msk;
	CLK->APBCLK |= CLK_APBCLK_UART0CKEN_Msk;
	/* Select UART clock source from external crystal*/
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UARTSEL_Msk) | CLK_CLKSEL1_UARTSEL_XTAL;
	/* Set P1 multi-function pins for UART1 RXD and TXD  */
	SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
	SYS->P1_MFP |= (SYS_MFP_P12_UART0_RXD | SYS_MFP_P13_UART0_TXD);


	/* Enable Timer 0 clock */
  CLK->APBCLK |= CLK_APBCLK_TMR0CKEN_Msk;
	/* Select TIMER0 clock source from external crystal*/
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_TMR0SEL_Msk) | CLK_CLKSEL1_TMR0SEL_XTAL;


	/* Enable Timer 1 clock */
  CLK->APBCLK |= CLK_APBCLK_TMR1CKEN_Msk;
	/* Select TIMER0 clock source from LIRC*/
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_TMR1SEL_Msk) | CLK_CLKSEL1_TMR1SEL_XTAL;


	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CycylesPerUs automatically. */
	SystemCoreClockUpdate();

	/* Lock protected registers */
  SYS_LockReg();
}


/**
	* FunctionName		WDT_Init
	*/
static void WDT_Init(void)
{
	SYS_UnlockReg();

	WDT->CTL = 0x00000300;		// 2^10 = 1024 = 102.4ms
	WDT->CTL |= WDT_CTL_RSTEN_Msk;
	WDT->ALTCTL = 0x00000001;	// Reset after 130 * WDT_CLK = 13ms
	WDT->CTL |= WDT_CTL_WDTEN_Msk;		//Enable

	SYS_LockReg();
}




/**
	* Function Name		TIM_Init
	*/
static void TIM_Init(void)
{
	// Set timer 0 working 50Hz in periodic mode
	TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 50);
	TIMER_Start(TIMER0);

	// Timer1 for count down hour
	TIMER1->CTL = TIMER_PERIODIC_MODE | 0xff;		//12M/256
  TIMER1->CMP = 46875;


	TIMER_Start(TIMER1);
}

/**
  * Function Name: GPIO_Init
	*/
static void GPIO_Init(void)
{
	// PIO11_T-P07,VCC_CE-P06,COMM-P24,KEY-P15,P32
  GPIO_SetMode(P0, GP_PIN_PIN6_Msk, GPIO_MODE_OUTPUT);
	GPIO_SetMode(P1, GP_PIN_PIN5_Msk, GPIO_MODE_QUASI);
  GPIO_SetMode(P3, GP_PIN_PIN2_Msk, GPIO_MODE_QUASI);
	GPIO_SetMode(P2, GP_PIN_PIN4_Msk, GPIO_MODE_OUTPUT);
	PORT_COMM 	= RESET;
  
  //P04-P07
  GPIO_SetMode(P0, GP_PIN_PIN4_Msk | GP_PIN_PIN5_Msk | GP_PIN_PIN6_Msk | GP_PIN_PIN7_Msk, GPIO_MODE_OUTPUT);
  PORT_2H = 0;
  PORT_4H = 0;
  PORT_6H = 0;
  PORT_8H = 0;
}


/**
 	* FunctionName: UART_Init
 	*/
static void UART_Init(void)
{
	/* Enable Interrupt and install the call back function */
	UART_SetTimeoutCnt(UART0, 30);
	UART_ENABLE_INT(UART0, UART_INTEN_RXTOIEN_Msk);
  NVIC_EnableIRQ(UART0_IRQn);
	UART_Open(UART0, 9600);
}
