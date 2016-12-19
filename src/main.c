/**
  ******************************************************************************
  * @file    Template_2/main.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Use this template for new projects with stm32f0xx family.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0x_gpio.h"
#include "stm32f0x_tim.h"
#include "stm32f0xx_it.h"
#include "hard.h"
#include "spi.h"

#include "core_cm0.h"
#include "adc.h"
#include "main_menu.h"
#include "lcd.h"
#include "LMX2326.h"


//#include <stdio.h>
//#include <string.h>




//--- VARIABLES EXTERNAS ---//
volatile unsigned char timer_1seg = 0;

volatile unsigned short timer_led_comm = 0;
volatile unsigned short timer_for_cat_switch = 0;
volatile unsigned short timer_for_cat_display = 0;
volatile unsigned char buffrx_ready = 0;
volatile unsigned char *pbuffrx;
volatile unsigned short wait_ms_var = 0;

volatile unsigned char TxBuffer_SPI [TXBUFFERSIZE];
volatile unsigned char RxBuffer_SPI [RXBUFFERSIZE];
volatile unsigned char *pspi_tx;
volatile unsigned char *pspi_rx;
volatile unsigned char spi_bytes_left = 0;

// ------- Externals de los timers -------
volatile unsigned short show_select_timer = 0;

volatile unsigned short scroll1_timer = 0;
volatile unsigned short scroll2_timer = 0;

volatile unsigned short lcd_backlight_timer = 0;

volatile unsigned char new_t = 0;
volatile unsigned short freq_tim = 0;

// ------- Externals del display LCD -------
const char s_blank_line [] = {"                "};

// ------- Externals del DMX -------
volatile unsigned char Packet_Detected_Flag;
volatile unsigned char dmx_receive_flag = 0;
volatile unsigned char DMX_channel_received = 0;
volatile unsigned char DMX_channel_selected = 1;
volatile unsigned char DMX_channel_quantity = 4;

volatile unsigned char data1[512];
//static unsigned char data_back[10];
volatile unsigned char data[10];

volatile unsigned short prog_timer = 0;

//--- VARIABLES GLOBALES ---//


// ------- de los timers -------
volatile unsigned short timer_standby;
volatile unsigned short timer_dmx_display_show;
volatile unsigned char display_timer;
volatile unsigned char switches_timer;
volatile unsigned char filter_timer;


volatile unsigned char door_filter;
volatile unsigned char take_sample;
volatile unsigned char move_relay;

volatile unsigned char secs = 0;
volatile unsigned short minutes = 0;


// ------- de los filtros DMX -------
#define LARGO_F		32
#define DIVISOR_F	5
unsigned char vd0 [LARGO_F + 1];
unsigned char vd1 [LARGO_F + 1];
unsigned char vd2 [LARGO_F + 1];
unsigned char vd3 [LARGO_F + 1];
unsigned char vd4 [LARGO_F + 1];

float fcalc = 1.0;



//--- FUNCIONES DEL MODULO ---//
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

unsigned char EvaluateFreq (unsigned int, unsigned int);



// ------- de los switches -------
void UpdateSwitches (void);
unsigned char CheckS1 (void);
unsigned char CheckS2 (void);
#define TIMER_FOR_CAT_SW	200



//--- FIN DEFINICIONES DE FILTRO ---//


//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned char i;
	unsigned char main_state = 0;
	unsigned short frequency, freq1, freq2, freq3, freq4;
	unsigned short freq_int = 0;
	unsigned short freq_dec = 0;
	char str [20];
	unsigned char n = 0;

	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();

	//ACTIVAR SYSTICK TIMER
	if (SysTick_Config(48000))
	{
		while (1)	/* Capture error */
		{
			if (LED)
				LED_OFF;
			else
				LED_ON;

			for (i = 0; i < 255; i++)
			{
				asm (	"nop \n\t"
						"nop \n\t"
						"nop \n\t" );
			}
		}
	}

	//TIM Configuration.
//	TIM_3_Init();
//	TIM_14_Init();			//lo uso para detectar el break en el DMX
	//TIM_16_Init();		//para OneShoot() cuando funciona en modo master
	//TIM_17_Init();		//lo uso para el ADC de Igrid

		//--- PRUEBA DISPLAY LCD ---
//		EXTIOff ();
		LCDInit();
		LED_ON;

		//--- Welcome code ---//
		Lcd_Command(CLEAR);
		Wait_ms(100);
		Lcd_Command(CURSOR_OFF);
		Wait_ms(100);
		Lcd_Command(BLINK_OFF);
		Wait_ms(100);

		while (FuncShowBlink ((const char *) "Kirno Technology", (const char *) "  Freq Counter  ", 2, BLINK_NO) != RESP_FINISH);
		LED_OFF;
		while (FuncShowBlink ((const char *) "Hardware: V1.0  ", (const char *) "Software: V1.0  ", 1, BLINK_CROSS) != RESP_FINISH);


	 //PRUEBA LED Y OE

		/* SPI configuration ------------------------------------------------------*/
		SPI_Config();

		//--- COMIENZO PROGRAMA DE PRODUCCION

	 LMX2326_Init ();
	 //LMX2326_SetR ();
	 LMX2326_SetN ();

	 //--- Main loop ---//
	 while (1)
	 {
		 switch (main_state)
		 {
			 case TAKE_FREQ:
				 if (new_t)
				 {
					 new_t = 0;
					 //tengo un periodo
					 switch (n)
					 {
						 case 0:
							 n = 1;
							 freq1 = freq_tim;
							 break;

						 case 1:
							 freq2 = freq_tim;
							 if (EvaluateFreq(freq1, freq2))
								 n++;
							 else
								 n = 0;

							 break;

						 case 2:
							 freq3 = freq_tim;
							 if (EvaluateFreq(freq2, freq3))
								 n++;
							 else
								 n = 0;

							 break;

						 case 3:
							 freq4 = freq_tim;
							 if (EvaluateFreq(freq3, freq4))
							 {
								 main_state = CALC_FREQ;
								 frequency = freq1 + freq2 + freq3 + freq4;
								 frequency >>= 2;
							 }
							 else
								 n = 0;

							 break;

					 }
				 }
				 break;

			 case CALC_FREQ:
				 //corrijo el defasaje
				 //supongo dividido 10000; 43KHz
				 fcalc = 48e6 / frequency;
				 freq_int = (short) (fcalc / 100);
				 fcalc = fcalc - freq_int * 100;
				 freq_dec = (short) fcalc;

				 main_state = SHOW_FREQ;

				 break;

			 case SHOW_FREQ:
				 //muestro resultados
				 //LED1_ON;		//muestra en aprox 120ms
				 LCD_1ER_RENGLON;
				 sprintf(str, "FREQ= %3d.%03d    ", freq_int, freq_dec);
				 LCDTransmitStr(str);

				 LCD_2DO_RENGLON;
				 LCDTransmitStr((char *) s_blank_line);

				 main_state = WAITING_SHOW;

				 break;

			 case WAITING_SHOW:
				 Wait_ms(300);
				 main_state = TAKE_FREQ;
				 break;

			 default:
				 main_state = TAKE_FREQ;
				 break;
		 }

	 }	//termina while(1)

	 //inicio cuestiones particulares
	 //iniciar variables de usao del programa segun funcion de memoria


#ifdef WITH_HARDWARE_WATCHDOG
		KickWatchdog();
#endif

	return 0;
}


//--- End of Main ---//
void Update_PWM (unsigned short pwm)
{
	Update_TIM3_CH1 (pwm);
	Update_TIM3_CH2 (4095 - pwm);
}

//Recibe dos freq (timers) y revisa cercania +/- 10%
//contesta 1 ok 0 nok
unsigned char EvaluateFreq (unsigned int f1, unsigned int f2)
{
	unsigned int percent = 0;

	percent = f1 / 10;

	if (f1 > f2)
	{
		f1 -= f2;
		if (f1 < percent)
			return 1;
	}
	else
	{
		f2 -= f1;
		if (f2 < percent)
			return 1;
	}
	return 0;
}




unsigned char MAFilter (unsigned char new_sample, unsigned char * vsample)
{
	unsigned short total_ma;
	unsigned char j;

	//Kernel mejorado ver 2
	//si el vector es de 0 a 7 (+1) sumo todas las posiciones entre 1 y 8, acomodo el nuevo vector entre 0 y 7
	total_ma = 0;
	*(vsample + LARGO_F) = new_sample;

    for (j = 0; j < (LARGO_F); j++)
    {
    	total_ma += *(vsample + j + 1);
    	*(vsample + j) = *(vsample + j + 1);
    }

    return total_ma >> DIVISOR_F;
}

unsigned short MAFilter16 (unsigned char new_sample, unsigned char * vsample)
{
	unsigned short total_ma;
	unsigned char j;

	//Kernel mejorado ver 2
	//si el vector es de 0 a 7 (+1) sumo todas las posiciones entre 1 y 8, acomodo el nuevo vector entre 0 y 7
	total_ma = 0;
	*(vsample + LARGO_F) = new_sample;

    for (j = 0; j < (LARGO_F); j++)
    {
    	total_ma += *(vsample + j + 1);
    	*(vsample + j) = *(vsample + j + 1);
    }

    return total_ma >> DIVISOR_F;
}


void TimingDelay_Decrement(void)
{
	if (wait_ms_var)
		wait_ms_var--;

	if (display_timer)
		display_timer--;

	if (timer_standby)
		timer_standby--;

	if (switches_timer)
		switches_timer--;

	//-------- Timers para funciones de seleccion ---------//
	if (show_select_timer)
		show_select_timer--;

	if (scroll1_timer)
		scroll1_timer--;

	if (scroll2_timer)
		scroll2_timer--;


}




