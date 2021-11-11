/*
 * geenix_operation_conf.h
 *
 *  Created on: Jan 14, 2020
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_GEENIX_GEENIX_OPERATION_CONF_H_
#define DEVICEOBJECT_GEENIX_GEENIX_OPERATION_CONF_H_

#include "xmc_uart.h"
#include "xmc_gpio.h"

XMC_GPIO_CONFIG_t geenix_sel_config;
XMC_GPIO_CONFIG_t geenix_tx_config;
XMC_GPIO_CONFIG_t geenix_rx_config;

XMC_UART_CH_CONFIG_t geenix_config;

#endif /* DEVICEOBJECT_GEENIX_GEENIX_OPERATION_CONF_H_ */
