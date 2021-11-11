/*
 * esp32_operation.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "esp32_operation.h"

void ESP32_ConnectionInit(void)
{
    XMC_GPIO_Init(P2_0, &esp32_tx_config);
    XMC_GPIO_Init(P2_1, &esp32_rx_config);

   	XMC_USIC_CH_Enable(XMC_USIC0_CH0);
	XMC_UART_CH_Init(XMC_USIC0_CH0, &esp32_config);
	XMC_UART_CH_SetInputSource(XMC_USIC0_CH0, XMC_UART_CH_INPUT_RXD, USIC0_C0_DX0_P2_1);

	XMC_UART_CH_EnableEvent(XMC_USIC0_CH0,XMC_UART_CH_EVENT_STANDARD_RECEIVE); // PSR.RFF
	XMC_UART_CH_SelectInterruptNodePointer(XMC_USIC0_CH0, XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE, 3);
	XMC_SCU_SetInterruptControl(IRQ18_IRQn, XMC_SCU_IRQCTRL_USIC0_SR3_IRQ18);
	NVIC_SetPriority(IRQ18_IRQn, 10U);
	NVIC_EnableIRQ(IRQ18_IRQn);

	XMC_UART_CH_Start(XMC_USIC0_CH0);
}

void BT_prepareData(char *buffer, uint8_t header, uint8_t length, BLUETOOTH_DATA *btData, uint8_t footer)
{
		static uint16_t bt_crc;

		buffer[0] = header;
		buffer[1] = length;

		/* PCS Parameter */
		buffer[2] = (char)(btData->bt_pcs_version >> 8);
		buffer[3] = (char)(btData->bt_pcs_version & 0xFF);
		buffer[4] = (char)(btData->bt_pcs_temp >> 8);
		buffer[5] = (char)(btData->bt_pcs_temp & 0xFF);
		buffer[6] = (char)(btData->bt_pcs_status);

		/* Voltage Parameter */
		buffer[7]  = (char)(btData->bt_voltage_batt >> 8);
		buffer[8]  = (char)(btData->bt_voltage_batt & 0xFF);
		buffer[9] = (char)(btData->bt_voltage_pv >> 8);
		buffer[10] = (char)(btData->bt_voltage_pv & 0xFF);
		buffer[11] = (char)(btData->bt_voltage_grid >> 8);
		buffer[12] = (char)(btData->bt_voltage_grid & 0xFF);
		buffer[13] = (char)(btData->bt_voltage_inv >> 8);
		buffer[14] = (char)(btData->bt_voltage_inv & 0xFF);

		/* Current Parameter */
		buffer[15] 	= (char)(btData->bt_current_batt >> 8);
		buffer[16] 	= (char)(btData->bt_current_batt & 0xFF);
		buffer[17] 	= (char)(btData->bt_current_pv >> 8);
		buffer[18] 	= (char)(btData->bt_current_pv & 0xFF);
		buffer[19] 	= (char)(btData->bt_current_grid >> 8);
		buffer[20] 	= (char)(btData->bt_current_grid & 0xFF);
		buffer[21] 	= (char)(btData->bt_current_inv >> 8);
		buffer[22] 	= (char)(btData->bt_current_inv & 0xFF);

		/* Power bt_Parameter */
		buffer[23] 	= (char)(btData->bt_power_batt >> 8);
		buffer[24] 	= (char)(btData->bt_power_batt & 0xFF);
		buffer[25] 	= (char)(btData->bt_power_pv >> 8);
		buffer[26] 	= (char)(btData->bt_power_pv & 0xFF);
		buffer[27] 	= (char)(btData->bt_power_grid >> 8);
		buffer[28] 	= (char)(btData->bt_power_grid & 0xFF);
		buffer[29] 	= (char)(btData->bt_power_inv >> 8);
		buffer[30] 	= (char)(btData->bt_power_inv & 0xFF);

		/* Module Status */
		buffer[31] 	= (char)(btData->bt_grid_status);
		buffer[32] 	= (char)(btData->bt_inv_status);
		buffer[33] 	= (char)(btData->bt_pv_status);
		buffer[34] 	= (char)(btData->bt_batt_status);

		/* Miscellaneous */
		buffer[35] 	= (char)(btData->bt_inv_frequency >> 8);
		buffer[36] 	= (char)(btData->bt_inv_frequency & 0xFF);

		/* Battery Parameter */
		buffer[37] 	= (char)(btData->bt_bat_pack_soc);
		buffer[38] 	= (char)(btData->bt_bat_pack_volt >> 8);
		buffer[39] 	= (char)(btData->bt_bat_pack_volt & 0xFF);
		buffer[40] 	= (char)(btData->bt_bat_pack_max_temp);
		buffer[41] 	= (char)(btData->bt_bat_pack_min_temp);
		buffer[42] 	= (char)(btData->bt_bat_pack_status);

		/* do CRC */
		bt_crc		= MF_CRC_16((unsigned char *) &buffer[0], length+2);

		buffer[43]  = (char) (bt_crc >> 8);
		buffer[44]  = (char) (bt_crc & 0xFF);

		buffer[45]	= (char) (footer);
}

void BT_sendStream(char *buffer, uint8_t length)
{
	uint8_t count = 0;
	while(count < length)
	{
		while(XMC_USIC_CH_GetTransmitBufferStatus(XMC_USIC0_CH0) == XMC_USIC_CH_TBUF_STATUS_BUSY){}
		XMC_UART_CH_ClearStatusFlag(XMC_USIC0_CH0, (uint32_t)XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

		XMC_USIC0_CH0->TBUF[0U] = buffer[count];

		count++;
	 }
}
