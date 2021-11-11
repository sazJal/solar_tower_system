/*
 * esp32_operation_conf.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "esp32_operation_conf.h"

XMC_GPIO_CONFIG_t esp32_tx_config 	=
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6,
};

XMC_GPIO_CONFIG_t esp32_rx_config	=
{
	.mode = XMC_GPIO_MODE_INPUT_PULL_UP,
};

XMC_UART_CH_CONFIG_t esp32_config =
{
		.baudrate	= 9600,
		.data_bits	= 8,
		.stop_bits	= 1
};
