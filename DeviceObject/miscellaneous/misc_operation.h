/*
 * misc_operation.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_MISCELLANEOUS_MISC_OPERATION_H_
#define DEVICEOBJECT_MISCELLANEOUS_MISC_OPERATION_H_

#include "misc_operation_conf.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

typedef struct MISC_HANDSHAKE_PARAM
{
	uint16_t cycle;
	bool	 isEnabled;
	bool	 isTimeOut;
	bool 	 isExecuted;
	bool 	 isStarted;
	bool	 isValid;
} MISC_HANDSHAKE_PARAM_t;

typedef struct MISC_CTRL_FLAG
{
	bool isPerforming_Precharge;
	bool isStandby;
	bool isAuto_mode;
	bool isBypass_power_mode;
	bool isForced_shutting_down;
	bool isChecking_peripheral;
} MISC_CTRL_FLAG_t;

typedef struct MISC_SYSTICK_PARAM
{
	bool		enable;
	uint16_t	count;
	bool		isTimeOut;
} MISC_SYSTICK_PARAM_t;

MISC_SYSTICK_PARAM_t sysTickHandle;

void MISC_DelayUs(uint16_t delay);
void MISC_TimerInit(void);
//void MISC_Delay(uint16_t us);

#endif /* DEVICEOBJECT_MISCELLANEOUS_MISC_OPERATION_H_ */
