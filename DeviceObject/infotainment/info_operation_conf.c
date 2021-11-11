/*
 * info_operation_conf.c
 *
 *  Created on: Jun 26, 2020
 *      Author: lenovo
 */

#include "info_operation_conf.h"

XMC_I2C_CH_CONFIG_t iic_config =
{
	.baudrate		= 100000,
};

XMC_GPIO_CONFIG_t button_config =
{
	.mode = XMC_GPIO_MODE_INPUT_PULL_DOWN,
};

XMC_GPIO_CONFIG_t dipswitch_config =
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};

XMC_GPIO_CONFIG_t led_config =
{
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
};

XMC_GPIO_CONFIG_t gpio_config =
{
	.mode 		  = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
};

XMC_ERU_ETL_CONFIG_t eru1_etl2_config =
{
	.input = XMC_ERU_ETL_INPUT_A1, // P3_2
	.edge_detection = XMC_ERU_ETL_EDGE_DETECTION_FALLING,
	.enable_output_trigger = true,
	.status_flag_mode = XMC_ERU_ETL_STATUS_FLAG_MODE_SWCTRL,
	.output_trigger_channel = XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL3, // ogu channel
	.source = XMC_ERU_ETL_SOURCE_A,
};

XMC_ERU_OGU_CONFIG_t eru1_ogu3_config =
{
	.service_request = XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER,
//	.enable_pattern_detection = true,
//	.pattern_detection_input = XMC_ERU_OGU_PATTERN_DETECTION_INPUT3 // from trigger channel 3
};

XMC_ERU_ETL_CONFIG_t eru0_etl2_config =
{
//	.input = XMC_ERU_ETL_INPUT_B0, // P2_10 //reset
	.input = ERU0_ETL2_INPUTB_P2_10,
	.edge_detection = XMC_ERU_ETL_EDGE_DETECTION_FALLING,
	.enable_output_trigger = true,
	.status_flag_mode = XMC_ERU_ETL_STATUS_FLAG_MODE_SWCTRL,
	.output_trigger_channel = XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL2, // ogu channel
	.source = XMC_ERU_ETL_SOURCE_B,
};

XMC_ERU_OGU_CONFIG_t eru0_ogu2_config =
{
	.service_request = XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER,
//	.enable_pattern_detection = true,
//	.pattern_detection_input = XMC_ERU_OGU_PATTERN_DETECTION_INPUT2 // from trigger channel 2
};

XMC_GPIO_CONFIG_t rtc_sda_config =
{
	.mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT9,
};

XMC_GPIO_CONFIG_t rtc_scl_config =
{
	.mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT8,
};

XMC_GPIO_CONFIG_t eru_io_config =
{
	.mode = XMC_GPIO_MODE_INPUT_TRISTATE,
};

XMC_GPIO_CONFIG_t eru_mfp_config =
{
	.mode = XMC_GPIO_MODE_INPUT_PULL_UP,
};
