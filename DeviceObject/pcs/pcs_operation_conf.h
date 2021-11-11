/*
 * pcs_operation_conf.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_PCS_PCS_OPERATION_CONF_H_
#define DEVICEOBJECT_PCS_PCS_OPERATION_CONF_H_

#include "xmc_can.h"
#include "xmc_gpio.h"

#define CAN_FREQ	11000000UL
#define CAN_BAUD	500000UL

#define CAN0_TXD	P0_5
#define CAN0_RXD	P0_4

XMC_GPIO_CONFIG_t pcs_button_config;
XMC_GPIO_CONFIG_t CAN0_txd_config;
XMC_GPIO_CONFIG_t CAN0_rxd_config;
XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t can_baud;
XMC_CAN_MO_t msgObjToPCS;
XMC_CAN_MO_t msgObjFromPCS;

#endif /* DEVICEOBJECT_PCS_PCS_OPERATION_CONF_H_ */
