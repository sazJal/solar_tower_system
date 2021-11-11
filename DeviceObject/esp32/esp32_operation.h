/*
 * esp32_operation.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_ESP32_ESP32_OPERATION_H_
#define DEVICEOBJECT_ESP32_ESP32_OPERATION_H_

#include "esp32_operation_conf.h"
#include "misc_operation.h"
#include "mathfunc_operation.h"
#include "stdio.h"

typedef struct _ESP32_DATA_PACK
{
	uint16_t pcs_soc;				// 1
	uint16_t pcs_vbat;				// 2
	uint16_t pcs_vpv;				// 3
	uint16_t pcs_vgrid;				// 4
	uint16_t pcs_vacout;			// 5
	int16_t  pcs_ibat;				// 6
	uint16_t pcs_ipv;				// 7
	int16_t  pcs_igrid;				// 8
	uint16_t pcs_iacout;			// 9
	int16_t  pcs_pbat;				// 10
	uint16_t pcs_ppv;				// 11
	int16_t  pcs_pgrid;				// 12
	uint16_t pcs_pacout;			// 13
	uint16_t pcs_freq;				// 14
	uint16_t pcs_tempPCS;			// 15
	uint16_t pcs_tempbat1;			// 16
	uint16_t pcs_tempbat2;			// 17
	uint16_t pcs_tempbat3;			// 18
	uint16_t pcs_tempbat4;			// 19
	uint16_t pcs_tempbat5;			// 20
	uint16_t pcs_system_status;		// 21
	uint16_t pcs_system_warning;	// 22
	uint16_t pcs_system_fault;		// 23
	uint16_t pv_conv_status;		// 24
	uint16_t pv_conv_warning;		// 25
	uint16_t pv_conv_fault;			// 26
	uint16_t ac_inv_status;			// 27
	uint16_t ac_inv_warning;		// 28
	uint16_t ac_inv_fault;			// 29
	uint16_t bat_conv_status;		// 30
	uint16_t bat_conv_warning;		// 31
	uint16_t bat_conv_fault;		// 32
	uint16_t dc_grid_status;		// 33
	uint16_t dc_grid_warning;		// 34
	uint16_t dc_grid_fault;			// 35
	uint16_t bat_status;			// 36
	uint16_t temp_pack1_status;		// 37
	uint16_t temp_pack2_status;		// 38
	uint16_t temp_pack3_status;		// 39
	uint16_t temp_pack4_status;		// 40
	uint16_t temp_pack5_status;		// 41
	uint16_t cell_status;			// 42
	uint16_t year;
	uint8_t  month;
	uint8_t  date;
	uint8_t  hour;
	uint8_t  minutes;
	uint8_t  seconds;
} ESP32_DATA_PACK_t;

typedef struct _BLUETOOTH_DATA
{
	/* PCS Parameter */
	uint16_t bt_pcs_version;
	uint16_t bt_pcs_temp;
	uint8_t	 bt_pcs_status;

	/* Voltage Parameter */
	uint16_t bt_voltage_batt;
	uint16_t bt_voltage_pv;
	uint16_t bt_voltage_grid;
	uint16_t bt_voltage_inv;
	/* Current Parameter */
	uint16_t bt_current_batt;
	uint16_t bt_current_pv;
	uint16_t bt_current_grid;
	uint16_t bt_current_inv;
	/* Power bt_Parameter */
	uint16_t bt_power_batt;
	uint16_t bt_power_pv;
	uint16_t bt_power_grid;
	uint16_t bt_power_inv;
	/* Miscellaneous */
	uint16_t bt_inv_frequency;

	/* Module Status */
	uint8_t  bt_grid_status;
	uint8_t  bt_inv_status;
	uint8_t  bt_pv_status;
	uint8_t  bt_batt_status;

	/* Battery Status */
	uint8_t  bt_bat_pack_soc;
	uint16_t bt_bat_pack_volt;
	uint8_t  bt_bat_pack_max_temp;
	uint8_t  bt_bat_pack_min_temp;
	uint8_t  bt_bat_pack_status;
} BLUETOOTH_DATA;

void ESP32_ConnectionInit(void);
void BT_prepareData(char *buffer, uint8_t header, uint8_t length, BLUETOOTH_DATA *btData, uint8_t footer);
void BT_sendStream(char *buffer, uint8_t length);

#endif /* DEVICEOBJECT_ESP32_ESP32_OPERATION_H_ */
