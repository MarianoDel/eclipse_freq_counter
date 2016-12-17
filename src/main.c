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



//--- FUNCIONES DEL MODULO ---//
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);




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

	 while (1)
	 {
		 if (LED)
		 {
			 LED_OFF;
			 LE_OFF;
		 }
		 else
		 {
			 LED_ON;
			 LE_ON;
		 }

		 Wait_ms(150);
	 }

#ifdef WITH_HARDWARE_WATCHDOG
		KickWatchdog();
#endif

		/* SPI configuration ------------------------------------------------------*/
		//	 SPI_Config();

	 while (1)
	 {
		 LED_ON;
		 Wait_ms(300);
		 LED_OFF;
		 Wait_ms(1000);
	 }

	 //FIN PRUEBA LED Y OE

	 /* SPI configuration ------------------------------------------------------*/
//	 SPI_Config();


	 //DMX_Disa();

	 //PRUEBA DISPLAY
	 /*
	 PWR_DS1_OFF;
	 PWR_DS2_OFF;
	 PWR_DS3_ON;

	 main_state = TranslateNumber(DISPLAY_LINE);
	 SendSegment(DISPLAY_DS3, main_state);
	 while(1);
	 */
	 /*
	 //ShowNumbers(0);
	 //ShowNumbers(876);
	 ShowNumbers(666);
	 //ShowNumbers(111);
	 while(1)
	 {
		 UpdateDisplay ();
	 }
	*/
	 //--- PRUEBA TIM14 DMX
	 /*
	 ShowNumbers(0);
	 while (1)
	 {
		 TIM14->CNT = 0;
		 TIM14->CR1 |= 0x0001;

		 while ((TIM14->CNT) < 2000)
		 {
		 }
		 TIM14->CR1 &= ~0x0001;
		 if (LED)
			 LED_OFF;
		 else
			 LED_ON;

		 UpdateDisplay ();
	 }
	 */
	 //--- FIN PRUEBA TIM14 DMX

	 //--- PRUEBA USART
	 /*
	 EXTIOff();
	 USART_Config();
	 ShowNumbers(0);
	 while (1)
	 {
		 DMX_channel_received = 0;
		 data1[0] = 0;
		 data1[1] = 0;
		 data1[2] = 0;
		 USARTSend('M');
		 USARTSend('E');
		 USARTSend('D');
		 Wait_ms(1);
		 if ((data1[0] == 'M') && (data1[1] == 'E') && (data1[2] == 'D'))
			 LED_ON;
		 else
			 LED_OFF;
		 Wait_ms(200);
		 UpdateDisplay ();
	 }
	*/
	 //--- FIN PRUEBA USART

	 //--- PRUEBA EXTI PA8 con DMX
	 /*
	 ShowNumbers(0);
	 while (1)
	 {
		 //cuando tiene DMX mueve el LED
		 EXTIOn();
		 Wait_ms(200);
		 EXTIOff();
		 Wait_ms(200);
	 }
	 */
	 //--- FIN PRUEBA EXTI PA8 con DMX

	 //--- PRUEBA ADC
	 /*
		if (ADC_Conf() == 0)
		{
			while (1)
			{
				if (LED)
					LED_OFF;
				else
					LED_ON;

				Wait_ms(150);
			}
		}
		while (1)
		{
			i = ReadADC1(ADC_CH0);
			if (i > 2048)
				LED_ON;
			else
				LED_OFF;
			Wait_ms(50);
			i = i >> 4;
			 ShowNumbers (i);
		}
		*/
	//--- FIN ADC

		 //--- PRUEBA FAN
	 /*
			while (1)
			{
				LED_ON;
				CTRL_FAN_ON;
				Wait_ms(300);
				LED_OFF;
				CTRL_FAN_OFF;
				Wait_ms(300);
			}
			*/
		//--- FIN FAN

	//--- PRUEBA CH0 DMX con switch de display	inicializo mas arriba USART y variables
	 /*
	 while (1)
	 {
		 if (CheckS1() > S_NO)
			 sw_state = 1;
		 else if (CheckS2() > S_NO)
			 sw_state = 0;

		 if (sw_state)		//si tengo que estar prendido
		 {
			 if (Packet_Detected_Flag)
			 {
				 //llego un paquete DMX
				 Packet_Detected_Flag = 0;
				 //en data tengo la info
				 ShowNumbers (data[0]);

				 Update_TIM3_CH1 (data[0]);
				 Update_TIM3_CH2 (data[1]);
				 Update_TIM3_CH3 (data[2]);
				 Update_TIM3_CH4 (data[3]);

			 }
		 }
		 else	//apago los numeros
		 {
			 ds1_number = 0;
			 ds2_number = 0;
			 ds3_number = 0;
		 }

		 UpdateDisplay ();
		 UpdateSwitches ();

	 }
	*/
	//--- FIN PRUEBA CH0 DMX

	//--- PRUEBA blinking de display	inicializo mas arriba USART y variables
	 /*
	ds1_number = 1;
	ds2_number = 2;
	ds3_number = 3;
		 while (1)
		 {
			 if (CheckS1() > S_NO)
			 {
				 //muevo el display en blinking
				 display_blinking <<= 1;
				 if (!(display_blinking & 0x07))
					 display_blinking = DISPLAY_DS1;
			 }
			 else if (CheckS2() > S_NO)
			 {
				 //prendo o apago el blinking
				 if (sw_state)
				 {
					 display_blinking = 0;
					 sw_state = 0;
				 }
				 else
				 {
					 display_blinking = DISPLAY_DS1;
					 sw_state = 1;
				 }
			 }

			 UpdateDisplay ();
			 UpdateSwitches ();

		 }

	  */
	//--- FIN PRUEBA blinking display con switches

	//--- PRUEBA CHANNELS PWM
	/*
	while (1)
	{
		for (i = 0; i < 255; i++)
		{
			Update_TIM3_CH1 (i);
			Update_TIM3_CH2 (i);
			Update_TIM3_CH3 (i);
			Update_TIM3_CH4 (i);

			Wait_ms(100);
		}
	}
	*/
	//--- FIN PRUEBA CHANNELS PWM

	 //--- PRUEBA SWITCHES
	 /*
	 i = 0;
	 while (1)
	 {
			 switch (swi)
			 {
				 case 0:
					 s_local = CheckS1();
					 if (s_local > S_NO)
						 swi++;
					 else
					 {
						 s_local = CheckS2();
						 if (s_local > S_NO)
							 swi = 10;
					 }

					 break;

				 case 1:
					 i += s_local;
					 LED_ON;
					 swi++;
					 break;

				 case 2:
					 //espero que se libere
					 s_local = CheckS1();
					 if (s_local == S_NO)
						 swi = 0;

					 break;

				 case 10:
					 LED_OFF;
					 if (i)
						 i--;
					swi++;
					break;

				 case 11:
					 s_local = CheckS2();
					 if (s_local == S_NO)
						 swi = 0;
					break;

				 default:
					 swi = 0;
					 break;

		 }


		 ShowNumbers(i);
		 UpdateDisplay ();
		 UpdateSwitches ();
	 }
	 */
	 //--- FIN PRUEBA SWITCHES


	 //--- COMIENZO PROGRAMA DE PRODUCCION

	 //inicio cuestiones particulares
	 //iniciar variables de usao del programa segun funcion de memoria







	//--- Main loop ---//
	while(1)
	{
		//PROGRAMA DE PRODUCCION

		//prueba DMX_ena() DMX_dis()
		/*
		if (!timer_standby)
		{
			timer_standby = 3000;
			if (swi)
			{
				swi = 0;
				DMX_Ena();
			}
			else
			{
				DMX_Disa();
				swi = 1;
			}
		}
		*/

		switch (main_state)
		{

			default:
				main_state = MAIN_INIT;
				break;

		}

	}	//termina while(1)

	return 0;
}


//--- End of Main ---//
void Update_PWM (unsigned short pwm)
{
	Update_TIM3_CH1 (pwm);
	Update_TIM3_CH2 (4095 - pwm);
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




