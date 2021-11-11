/*
 * geenix_operation.h
 *
 *  Created on: Jan 14, 2020
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_GEENIX_GEENIX_OPERATION_H_
#define DEVICEOBJECT_GEENIX_GEENIX_OPERATION_H_

#include "geenix_operation_conf.h"
#include "stdio.h"
#include "string.h"
#include "xmc_scu.h"
#include "xmc1_scu.h"

typedef struct _GEENIX_DATA
{
	uint8_t 	pack_soc;
	uint16_t	pack_voltage;
	uint8_t		pack_max_temp;
	uint8_t		pack_min_temp;
	uint8_t		pack_status;
} GEENIX_DATA;

typedef enum GEENIX_RCV_STATE
{
	HEADER_CHECK,
	HEADER_RECEIVE,
	HEADER_PARSE,
	HEADER_FINISH
} GEENIX_RCV_STATE_t;

typedef enum GEENIX_SEL_STAT
{
	GEENIX_SEL_LOW,
	GEENIX_SEL_HIGH
} GEENIX_SEL_STAT_t;

typedef enum GEENIX_COMMAND_TYPE
{
	GEENIX_SUMMARY 	= 0x02,		// Summary information acquisition request
	GEENIX_ALARM   	= 0x03,		// Alarm information
	GEENIX_INPUT	= 0x04,		// Drive input state
	GEENIX_VOLTAGE	= 0x10,		// Cell voltage
	GEENIX_TEMP		= 0x30,		// Temperatures
} GEENIX_COMMAND_TYPE_t;

void GEENIX_Connection_Init(void);
void GEENIX_Async_Delay(uint8_t byte_sent);
void GEENIX_Sel_Mode(GEENIX_SEL_STAT_t mode);
void GEENIX_AskData(GEENIX_COMMAND_TYPE_t command);

#endif /* DEVICEOBJECT_GEENIX_GEENIX_OPERATION_H_ */
