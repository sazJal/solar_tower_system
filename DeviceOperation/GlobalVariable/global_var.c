/*
 * global_var.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "global_var.h"

char buffer[300];

/* Main Operation */
bool ErrorBeingHandled 		= false;
bool IsPerformingPrecharge 	= false;
bool flag_standby 			= false;
bool recEspCommIsHalt 		= true;
bool esp32IsHandShake 		= false;
bool recbmsIsHandShake 		= false;

bool sysTickFlag;

/* RS232 variable */
bool system_start = false;

/* Miscellaneous */
MISC_HANDSHAKE_PARAM_t pvHShake;
MISC_HANDSHAKE_PARAM_t esp32HShake;
MISC_HANDSHAKE_PARAM_t buttonHShake;
MISC_CTRL_FLAG_t	   controlFlag;
bool flag_button	= false;
bool flag_misc		= false;

/* PCS global variable */
//bool flag_pcs;
//bool flag_pcsBlockIsStandby;
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

/* REC BMS global variable */
bool flag_geenix = false;

/* RS232 */
bool flag_rs232=false;

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
bool isPeriphMode=false;
bool genix_send= false;
bool isStartHShake = false;
bool isHShakeValid = false;
