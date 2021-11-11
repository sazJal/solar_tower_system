/*
 * rs232_operation_conf.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "rs232_operation_conf.h"

XMC_GPIO_CONFIG_t rs232_tx_config 	=
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7,
};

XMC_GPIO_CONFIG_t rs232_rx_config	=
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};

XMC_UART_CH_CONFIG_t rs232_config =
{
		.baudrate	= 115200,
		.data_bits	= 8,
		.stop_bits	= 1
};
