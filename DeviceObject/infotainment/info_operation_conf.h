/*
 * info_operation_conf.h
 *
 *  Created on: Jun 26, 2020
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_INFOTAINMENT_INFO_OPERATION_CONF_H_
#define DEVICEOBJECT_INFOTAINMENT_INFO_OPERATION_CONF_H_

#include "xmc_ccu4.h"
#include "xmc_uart.h"
#include "xmc_gpio.h"
#include "xmc_i2c.h"
#include "xmc_eru.h"
#include "xmc_scu.h"

/* LED, Button, and GPIO Configurations */
XMC_GPIO_CONFIG_t 	 button_config;
XMC_GPIO_CONFIG_t 	 led_config;
XMC_GPIO_CONFIG_t 	 dipswitch_config;
XMC_GPIO_CONFIG_t 	 gpio_config;

/* RTC and EEPROM Configurations */
XMC_I2C_CH_CONFIG_t  iic_config;

/* External interrupt for reset and Clock Configuration */
XMC_ERU_ETL_CONFIG_t eru1_etl2_config;
XMC_ERU_OGU_CONFIG_t eru1_ogu3_config;
XMC_ERU_ETL_CONFIG_t eru0_etl2_config;
XMC_ERU_OGU_CONFIG_t eru0_ogu2_config;

/* IO for Peripheral Configuration */
XMC_GPIO_CONFIG_t 	 rtc_sda_config;
XMC_GPIO_CONFIG_t 	 rtc_scl_config;
XMC_GPIO_CONFIG_t 	 eru_io_config;
XMC_GPIO_CONFIG_t 	 eru_mfp_config;

#endif /* DEVICEOBJECT_INFOTAINMENT_INFO_OPERATION_CONF_H_ */
