#include "spi.h"
#include "stm32f0xx.h"






//-------------------------- EXTERNAL VARIABLES ------------------------
extern volatile unsigned char TxBuffer_SPI [];
extern volatile unsigned char RxBuffer_SPI [];
extern volatile unsigned char *pspi_tx;
extern volatile unsigned char *pspi_rx;
extern volatile unsigned char spi_bytes_left;
//extern SPI_InitTypeDef  SPI_InitStructure;

//-------------------------- GLOBAL VARIABLES --------------------------
//SPI_InitTypeDef  SPI_InitStructure;

//--------------------------- FUNCTIONS --------------------------------

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
void SPI_Config(void)
{

	//Habilitar Clk
	if (!RCC_SPI1_CLK)
		RCC_SPI1_CLK_ON;
//
//
  //Configuracion SPI
  //clk / 256; master; CPOL High; CPHA second clock
  //SPIx->CR1 = SPI_CR1_MSTR | SPI_CR1_BR;
  SPIx->CR1 = 0;
  SPIx->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSM | SPI_CR1_SSI;
  //SPIx->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA;
  //SPIx->CR1 |= SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSM;

  //SPIx->CR2 = 0;
  //thresh 8 bits; data 8 bits;
  SPIx->CR2 |= SPI_CR2_FRXTH | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;

  //SPIx->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SMOD);
  SPIx->CR1 |= SPI_CR1_SPE;		//habilito periferico


//
//	  //Configuracion SPI
//	  SPIx->CR1 &= ~SPI_CR1_SPE;		//deshabilito periferico
//	  //clk / 256; master; CPOL High; CPHA second clock
//	  //SPIx->CR1 = 0;	//TODO: CR1 es el que tiene un error respecto de la config de arriba (verificar que puede ser) AHORRO MUCHA MEMORIA
//	  SPIx->CR2 = 0;
//	  //SPIx->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSM;
//	  SPIx->CR1 |= SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_SSM;
//	  //thresh 8 bits; data 8 bits;
//	  SPIx->CR2 |= SPI_CR2_FRXTH | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
//
//	//  SPIx->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SMOD);
//	  SPIx->CR1 |= SPI_CR1_SPE;		//habilito periferico
//
//	  SPIx->CR1 &= ~SPI_CR1_SPE;		//deshabilito periferico

}

//void Send_SPI (unsigned char * p, unsigned char bytes)
//{
//	/* Waiting until TX FIFO is empty */
//    while (SPI_GetTransmissionFIFOStatus(SPIx) != SPI_TransmissionFIFOStatus_Empty);
//
//	pspi_tx = p;
//	spi_bytes_left = bytes;
//
//    SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
//}

unsigned char Send_Receive_SPI (unsigned char a)
{
	unsigned char rx;
	unsigned char j;

	//primero limpio buffer rx spi
	while ((SPIx->SR & SPI_SR_RXNE) == 1)
	{
		rx = SPIx->DR & 0x0F;
	}

	//espero que haya lugar en el buffer
	while ((SPIx->SR & SPI_TXE) == 0);

	SPIx->DR = a;

    //espero tener el dato en RX
    for (j = 0; j < 150; j++)
    {
    	asm("nop");
    }

    return (SPIx->DR & 0x0F);
}

void Wait_SPI_Busy (void)
{
	//espero que se transfiera el dato
	while ((SPIx->SR & SPI_BSY) != 0);
}
void Send_SPI_Multiple (unsigned char a)
{
	//espero que haya lugar en el buffer
	while ((SPIx->SR & SPI_TXE) == 0);

	//*(__IO uint8_t *) SPIx->DR = a;
	SPIx->DR = a;

}

void Send_SPI_Single (unsigned char a)
{
	//espero que se libere el buffer
	while ((SPIx->SR & SPI_TXE) == 0);

	//tengo espacio
	//SPIx->DR = a;
	SPIx->DR = a;

	//espero que se transfiera el dato
	while ((SPIx->SR & SPI_BSY) != 0);
}

unsigned char Receive_SPI_Single (void)
{
	unsigned char dummy;

	//espero que se libere el buffer
	while (((SPIx->SR & SPI_TXE) == 0) || ((SPIx->SR & SPI_BSY) != 0));

	//limpio buffer RxFIFO
	while ((SPIx->SR & SPI_RXNE) != 0)
		dummy = SPIx->DR;

	SPIx->DR = 0xff;

	//espero que se transfiera el dato
	while ((SPIx->SR & SPI_BSY) != 0);

	dummy = (unsigned char) SPIx->DR;
	return dummy;
}
