/*
 * rs232_operation_conf.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_RS232_RS232_OPERATION_CONF_H_
#define DEVICEOBJECT_RS232_RS232_OPERATION_CONF_H_

#include "xmc_scu.h"
#include "xmc1_scu.h"
#include "xmc_uart.h"
#include "xmc_gpio.h"


XMC_GPIO_CONFIG_t rs232_tx_config;
XMC_GPIO_CONFIG_t rs232_rx_config;
XMC_UART_CH_CONFIG_t rs232_config;

#endif /* DEVICEOBJECT_RS232_RS232_OPERATION_CONF_H_ */
