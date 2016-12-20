/*
 * LMX2326.c
 *
 *  Created on: 19/12/2016
 *      Author: Mariano
 */

#include "LMX2326.h"
#include "spi.h"
#include "hard.h"
#include "stm32f0xx.h"




void LMX2326_Init (void)
{
	unsigned char i = 0;
	//		 //LMX2326 Inicializacion con R counter
	//		 LE_OFF;
	//		 LED_ON;
	//		 Send_SPI_Multiple(0x00);
	//		 Send_SPI_Multiple(0x00);		//0x00
	//		 //Send_SPI_Multiple(0xC3);		//0xC3    11000011; C1 C2, FoLD, Phse pos
	//		 Send_SPI_Multiple(0x43);		//0x43    01000011; C1 C2, FoLD, Phse neg
	//		 Wait_SPI_Busy ();
	//		 LED_OFF;
	//		 LE_ON;
	//		 for (i = 0; i < 255; i++)
	//		 {
	//			 asm (	"nop \n\t"
	//					 "nop \n\t"
	//					 "nop \n\t" );
	//		 }
	//		 LE_OFF;

	//LMX2326 Inicializacion con N counter
	LE_OFF;
	LED_ON;
	Send_SPI_Multiple(0x00);
	Send_SPI_Multiple(0x00);		//0x00
	//Send_SPI_Multiple(0xC3);		//0xC3    10100011; C1 C2, FoLD, Phse pos
	Send_SPI_Multiple(0x23);		//0x43    00100011; C1 C2, FoLD, Phse neg
	Wait_SPI_Busy ();
	LED_OFF;
	LE_ON;
	for (i = 0; i < 255; i++)
	{
		asm (	"nop \n\t"
				"nop \n\t"
				"nop \n\t" );
	}
	LE_OFF;

	//demora intermedia
	for (i = 0; i < 255; i++)
	{
		asm (	"nop \n\t"
				"nop \n\t"
				"nop \n\t" );
	}

}

void LMX2326_SetR (void)
{
	unsigned char i = 0;
	//R counter / 1000
	LED_ON;
	Send_SPI_Multiple(0x00);		//0x00
	Send_SPI_Multiple(0x0F);		//0x00
	//Send_SPI_Multiple(0x10);		//0x20    00100000; C1=0 C2=0, R counter / 4
	Send_SPI_Multiple(0xA0);		//0x00
	Wait_SPI_Busy ();
	LED_OFF;
	LE_ON;
	for (i = 0; i < 255; i++)
	{
		asm (	"nop \n\t"
				"nop \n\t"
				"nop \n\t" );
	}
	LE_OFF;
}

void LMX2326_SetN (void)
{
	unsigned char i = 0;
	//tiene preescaler P simepre /8
	//N counter / 1000
	LED_ON;
	//		 Send_SPI_Multiple(0x01);		//0x00
	//		 Send_SPI_Multiple(0xF4);		//0x00
	//		 //Send_SPI_Multiple(0x10);		//0x20    00100000; C1=1 C2=0, N counter / 1000
	//		 Send_SPI_Multiple(0x01);		//0x01

//	Send_SPI_Multiple(0x00);		//0x00
//	Send_SPI_Multiple(0x3E);		//0x00
//	Send_SPI_Multiple(0x81);		//0x01		00100000; C1=1 C2=0, N counter / 125

	Send_SPI_Multiple(0x02);		//0x00
	Send_SPI_Multiple(0x71);		//0x00
	Send_SPI_Multiple(0x01);		//0x01		00100000; C1=1 C2=0, N counter / 1250

	Wait_SPI_Busy ();
	LED_OFF;
	LE_ON;
	for (i = 0; i < 255; i++)
	{
		 asm (	"nop \n\t"
				 "nop \n\t"
				 "nop \n\t" );
	}
	LE_OFF;
}

