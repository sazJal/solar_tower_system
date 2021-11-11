/*
 * rs232_operation.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "rs232_operation.h"

void RS232_Connection_Init(void)
{
    XMC_GPIO_Init(P1_2, &rs232_tx_config);
    XMC_GPIO_Init(P1_3, &rs232_rx_config);

   	XMC_USIC_CH_Enable(XMC_USIC0_CH1);
	XMC_UART_CH_Init(XMC_USIC0_CH1, &rs232_config);
	XMC_UART_CH_SetInputSource(XMC_USIC0_CH1, XMC_UART_CH_INPUT_RXD, USIC0_C1_DX0_P1_3);
	XMC_UART_CH_EnableEvent(XMC_USIC0_CH1,XMC_UART_CH_EVENT_STANDARD_RECEIVE); // PSR.RFF
	XMC_UART_CH_SelectInterruptNodePointer(XMC_USIC0_CH1, XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE, 4);
	XMC_SCU_SetInterruptControl(IRQ25_IRQn, XMC_SCU_IRQCTRL_USIC0_SR4_IRQ25);
	NVIC_SetPriority(IRQ25_IRQn, 10U);
	NVIC_EnableIRQ(IRQ25_IRQn);

	XMC_UART_CH_Start(XMC_USIC0_CH1); // Sets the USIC channel operation mode to UART mode
}

void RS232_SendString(XMC_USIC_CH_t *const channel, char* DataPtr, uint32_t count)
{
	while(count)
	{
	    /* Wait till the Transmit Buffer is free for transmission */
	    while(XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	    {
	    }

	    /* Clear the Transmit Buffer indication flag */
	    XMC_UART_CH_ClearStatusFlag(channel, (uint32_t)XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

	    /*Transmit data */
	    channel->TBUF[0U] = *DataPtr;

	    /* Shifts Pointer */
	    count--;
	    DataPtr++;
	 }
}

