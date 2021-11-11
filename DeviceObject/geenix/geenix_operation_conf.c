/*
 * geenix_operation_conf.c
 *
 *  Created on: Jan 14, 2020
 *      Author: lenovo
 */

#include "geenix_operation_conf.h"

XMC_GPIO_CONFIG_t geenix_sel_config =
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
};

XMC_GPIO_CONFIG_t geenix_tx_config 	=
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT9,
};

XMC_GPIO_CONFIG_t geenix_rx_config	=
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};

XMC_UART_CH_CONFIG_t geenix_config =
{
		.baudrate	= 9600,
		.data_bits	= 8,
		.stop_bits	= 1
};
