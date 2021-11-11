/*
 * geenix_operation.c
 *
 *  Created on: Jan 14, 2020
 *      Author: lenovo
 */

#include "geenix_operation.h"
#include "misc_operation.h"

void GEENIX_Connection_Init(void)
{
	XMC_GPIO_Init(P0_6, &geenix_sel_config);
    XMC_GPIO_Init(P0_2, &geenix_tx_config);
    XMC_GPIO_Init(P0_3, &geenix_rx_config);

   	XMC_USIC_CH_Enable(XMC_USIC1_CH0);
	XMC_UART_CH_Init(XMC_USIC1_CH0, &geenix_config);
	XMC_UART_CH_SetInputSource(XMC_USIC1_CH0, XMC_UART_CH_INPUT_RXD, USIC1_C0_DX0_P0_3);

	XMC_UART_CH_EnableEvent(XMC_USIC1_CH0,XMC_UART_CH_EVENT_STANDARD_RECEIVE); // PSR.RFF
	XMC_UART_CH_SelectInterruptNodePointer(XMC_USIC1_CH0, XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE, 3);
	XMC_SCU_SetInterruptControl(IRQ12_IRQn, XMC_SCU_IRQCTRL_USIC1_SR3_IRQ12);
	NVIC_SetPriority(IRQ12_IRQn, 10U);
	NVIC_EnableIRQ(IRQ12_IRQn);

	XMC_UART_CH_Start(XMC_USIC1_CH0); // Sets the USIC channel operation mode to UART mode
}

void GEENIX_Async_Delay(uint8_t byte_sent)
{
	uint8_t  byte_count;
	uint16_t delay_count;
	for(byte_count = 0; byte_count<=byte_sent-1;byte_count++)
	{
		for(delay_count=0; delay_count < 700; delay_count++)
		{
			__NOP();
		}
	}

}

void GEENIX_Sel_Mode(GEENIX_SEL_STAT_t mode)
{
	mode?(XMC_GPIO_SetOutputLevel(P0_6, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
		 (XMC_GPIO_SetOutputLevel(P0_6, XMC_GPIO_OUTPUT_LEVEL_LOW));
}

void GEENIX_AskData(GEENIX_COMMAND_TYPE_t command)
{
	unsigned char ecc;
	unsigned char buffer[10];

	buffer[0] = 0xCC;
	buffer[1] = 0xDD;
	buffer[2] = 0x0A;
	buffer[3] = command;
	buffer[4] = 0x00;
	buffer[5] = 0x00;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	buffer[8] = 0x00;
	buffer[9] = 0x00;

	ecc = 0x00;
	for(uint8_t i = 0; i<10; i++)
	{
		ecc = ecc+buffer[i];
	}

	ecc = 0x1D;

	for(uint8_t i=0; i<10;i++)
	{
		XMC_UART_CH_Transmit(XMC_USIC1_CH0, buffer[i]);
	}
	XMC_UART_CH_Transmit(XMC_USIC1_CH0, ecc);
	XMC_UART_CH_Transmit(XMC_USIC1_CH0, 0x0D);
	XMC_UART_CH_Transmit(XMC_USIC1_CH0, 0x0A);

	GEENIX_Async_Delay(13);
//	MISC_DelayUs(14); // take 14 ms
}
