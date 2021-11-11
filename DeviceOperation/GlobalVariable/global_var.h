/*
 * global_var.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOPERATION_GLOBALVARIABLE_GLOBAL_VAR_H_
#define DEVICEOPERATION_GLOBALVARIABLE_GLOBAL_VAR_H_

#include "pcs_operation.h"
#include "esp32_operation.h"
#include "misc_operation.h"
#include "info_operation.h"
#include "pdu_operation.h"
#include "geenix_operation.h"
#include "rs232_operation.h"
#include "mathfunc_operation.h"

char buffer[300];

/* Main Operation */
bool ErrorBeingHandled;
bool flag_standby;
bool IsPerformingPrecharge;
bool recEspCommIsHalt;
bool esp32IsHandShake;
bool recbmsIsHandShake;

bool sysTickFlag;

/* RS232 variable */
bool system_start;

/* Miscellaneous */
MISC_HANDSHAKE_PARAM_t pvHShake;
MISC_HANDSHAKE_PARAM_t esp32HShake;
MISC_HANDSHAKE_PARAM_t buttonHShake;
MISC_CTRL_FLAG_t	   controlFlag;

/* PCS global variable */
bool flag_pcs;
bool flag_pcsBlockIsStandby;
PCS_BMS_STATUS_t pcsBmsStatus;
BMS_PARAMETER_t	 bmsSensor;

PCS_VOLTAGE_PARAM_t	pcsVoltages;
PCS_CURRENT_PARAM_t	pcsCurrents;
PCS_POWER_PARAM_t 	pcsPowers;
PCS_MISC_PARAM_t 	pcsMiscs;

PCS_STATUS_t 		pcsSystemStatus;
DCGRID_STATUS_t 	pcsDCgridStatus;
ACINV_STATUS_t 		pcsACinvStatus;
PVCONV_STATUS_t 	pcsPVconvStatus;
BATCONV_STATUS_t 	pcsBatconvStatus;

/* PDU global variable */
bool flag_pdu;
bool flag_pdu_adc;
PDU_SENSOR_RAW_t pduSensorRaw;
PDU_SENSOR_AVG_t pduSensorAvg;
PDU_SENSOR_t 	 pduSensor;

/* MISC */
bool flag_button;
bool flag_error;
bool flag_misc;

/* RS232 */
bool flag_rs232;

/* RTC */
RTC_DATE_PARAM_t rtcDate;
RTC_TIME_PARAM_t rtcTime;
bool flag_rtc;

/* ESP32 */
bool flag_esp32;
bool enable_bt;
BLUETOOTH_DATA btData;
ESP32_DATA_PACK_t esp32DataPack;

/* GEENIX */
GEENIX_DATA batPackData;
uint8_t temporaryStatus;

/* math of battery */
float soc_f;
float ocv;
bool genix_send;
bool isPeriphMode;
bool isStartHShake;
bool isHShakeValid;
char NTPdata[200];

#endif /* DEVICEOPERATION_GLOBALVARIABLE_GLOBAL_VAR_H_ */
