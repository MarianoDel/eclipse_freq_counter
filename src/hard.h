/*
 * hard.h
 *
 *  Created on: 28/11/2013
 *      Author: Mariano
 */

#ifndef HARD_H_
#define HARD_H_


//-- Defines For Configuration -------------------
//---- Configuration for Hardware or Software Watchdog -------
//#define WITH_HARDWARE_WATCHDOG

//---- Configuration for Hardware Versions -------
#define VER_1_0

//---- Configuration for Software Versions -------
#define FOUR_FILTER
//#define SIXTEEN_FILTER

//-- End Of Defines For Configuration ---------------

//GPIOA pin0
//GPIOA pin1
//GPIOA pin2
//GPIOA pin3	lcd d4-d7

//GPIOA pin4
#define LCD_E ((GPIOA->ODR & 0x0010) != 0)
#define LCD_E_ON	GPIOA->BSRR = 0x00000010
#define LCD_E_OFF GPIOA->BSRR = 0x00100000

//GPIOA pin5
#define LCD_RS ((GPIOA->ODR & 0x0020) != 0)
#define LCD_RS_ON	GPIOA->BSRR = 0x00000020
#define LCD_RS_OFF GPIOA->BSRR = 0x00200000

//GPIOA pin6
#define FREQ_PIN ((GPIOA->IDR & 0x0040) != 0)

//GPIOA pin7
//GPIOA pin8
//GPIOA pin9
//GPIOA pin10
//GPIOA pin11
//GPIOA pin12
//GPIOA pin13
//GPIOA pin14
//GPIOA pin15

//GPIOB pin0
//GPIOB pin1

//GPIOB pin3	spi_clk

//GPIOB pin4
#define LE ((GPIOB->ODR & 0x0010) != 0)
#define LE_ON GPIOB->BSRR = 0x00000010
#define LE_OFF GPIOB->BSRR = 0x00100000

//GPIOB pin5	spi_mosi

//GPIOB pin6

//GPIOB pin7
#define LED ((GPIOB->ODR & 0x0080) != 0)
#define LED_ON GPIOB->BSRR = 0x00000080
#define LED_OFF GPIOB->BSRR = 0x00800000



//ESTADOS DEL PROGRAMA PRINCIPAL
enum MainState
{
	TAKE_FREQ = 0,
	CALC_FREQ,
	SHOW_FREQ,
	WAITING_SHOW
};


//---- Temperaturas en el LM335
//37	2,572
//40	2,600
//45	2,650
//50	2,681
//55	2,725
//60	2,765
#define TEMP_IN_30		3226
#define TEMP_IN_35		3279
#define TEMP_IN_50		3434
#define TEMP_IN_65		3591
#define TEMP_DISCONECT		4000

//ESTADOS DEL DISPLAY EN RGB_FOR_CAT
#define SHOW_CHANNELS	0
#define SHOW_NUMBERS	1

//ESTADOS DEL DISPLAY
#define DISPLAY_INIT		0
#define DISPLAY_SENDING		1
#define DISPLAY_SHOWING		2
#define DISPLAY_WAITING		3

#define DISPLAY_ZERO	10
#define DISPLAY_POINT	11
#define DISPLAY_LINE	12
#define DISPLAY_REMOTE	13
#define DISPLAY_PROG	14
#define DISPLAY_E		15
#define DISPLAY_C		16
#define DISPLAY_H		17
#define DISPLAY_1P		18
#define DISPLAY_S		19
#define DISPLAY_A		20
#define DISPLAY_T		21
#define DISPLAY_R		22
#define DISPLAY_G		23
#define DISPLAY_N		24
#define DISPLAY_NONE	0xF0

#define DISPLAY_P	DISPLAY_PROG

#define DISPLAY_DS3		0x04
#define DISPLAY_DS2		0x02
#define DISPLAY_DS1		0x01

#define DISPLAY_TIMER_RELOAD	6		//166Hz / 3
#define SWITCHES_TIMER_RELOAD	10
#define BLINKING_UPDATE 	12			//son 18 ms de update * num elegido

#define SWITCHES_THRESHOLD_FULL	300		//3 segundos
#define SWITCHES_THRESHOLD_HALF	100		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//50 ms

#define TTIMER_FOR_CAT_DISPLAY			2000	//tiempo entre que dice canal y el numero
#define TIMER_STANDBY_TIMEOUT_REDUCED	2000	//reduced 2 segs
#define TIMER_STANDBY_TIMEOUT			6000	//6 segundos
#define DMX_DISPLAY_SHOW_TIMEOUT		30000	//30 segundos

//----- Definiciones para timers ------
//#define TTIMER_FOR_CAT_DISPLAY			2000	//tiempo entre que dice canal y el numero
//#define TIMER_STANDBY_TIMEOUT_REDUCED	2000	//reduced 2 segs
//#define TIMER_STANDBY_TIMEOUT			6000	//6 segundos
//#define DMX_DISPLAY_SHOW_TIMEOUT		30000	//30 segundos
//#define TT_MENU_ENABLED					30000	//30 segundos
#define TT_LCD_BACKLIGHT					30000	//30 segundos
#define TT_UPDATE_BUTTON 80
#define TT_UPDATE_BUTTON_SPEED 10



#define S_FULL		10
#define S_HALF		3
#define S_MIN		1
#define S_NO		0

#define FUNCTION_DMX	1
#define FUNCTION_MAN	2
#define FUNCTION_CAT	FUNCTION_MAN

#endif /* HARD_H_ */
