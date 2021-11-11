/*
 * op_config.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOPERATION_OPERATION_OP_CONFIG_H_
#define DEVICEOPERATION_OPERATION_OP_CONFIG_H_

#include "global_var.h"
#include "stdbool.h"

/* library from middle ends application */

typedef enum OP_STATE
{
	OP_SETUP,		// Initialize remote device and handshaking
	OP_TURN_ON,
	OP_RUNNING,
	OP_STANDBY,
	OP_ERROR,
	OP_SHUTDOWN,
	OP_PERIPH,
} OP_STATE_t;

typedef enum CONF_STATE
{
	NONE
} CONF_STATE_t;

typedef struct MCU_FSM
{
	OP_STATE_t 		operation;
	CONF_STATE_t 	configuration;
} MCU_FSM_t;

/* operation */
OP_STATE_t mcuState;

void OP_SetupConnection(OP_STATE_t *state, MISC_HANDSHAKE_PARAM_t *espHandShake,
						MISC_HANDSHAKE_PARAM_t *pvHandShake,
						MISC_CTRL_FLAG_t *ctrlFlag, float pvVoltage,
						PCS_BMS_STATUS_t *pcsBmsStatus, PCS_STATUS_t *pcsStatus,
						RTC_TIME_PARAM_t *rtcTime, RTC_DATE_PARAM_t *rtcDate);
void OP_TurnOnPCS(OP_STATE_t *state, MISC_HANDSHAKE_PARAM_t *CtrlButton,
				  MISC_CTRL_FLAG_t *ctrlFlag, PCS_BMS_STATUS_t *pcsBmsStatus);
void OP_CheckingPCSStatus(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						  MISC_HANDSHAKE_PARAM_t *CtrlButton,
						  MISC_CTRL_FLAG_t *ctrlFlag);
void OP_PutSystemStandby(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						 MISC_HANDSHAKE_PARAM_t *CtrlButton,
						 MISC_CTRL_FLAG_t *ctrlFlag,
						 PCS_BMS_STATUS_t *pcsBmsStatus);
void OP_HandlingPCSWarning(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						   MISC_HANDSHAKE_PARAM_t *CtrlButton,
						   MISC_CTRL_FLAG_t *ctrlFlag, BMS_PARAMETER_t *bmsParam,
						   PCS_BMS_STATUS_t *pcsBmsStatus);
void OP_PutSystemShutDown(OP_STATE_t *state,
						  MISC_HANDSHAKE_PARAM_t *CtrlButton);

void OP_CheckingMode(MISC_CTRL_FLAG_t *ctrlFlag);
void OP_CheckingError(MISC_CTRL_FLAG_t *ctrlFlag, PCS_STATUS_t *pcsStatus,
					  DCGRID_STATUS_t *gridStatus, ACINV_STATUS_t *invStatus,
					  PVCONV_STATUS_t *pvStatus, BATCONV_STATUS_t *BatStatus);
#endif /* DEVICEOPERATION_OPERATION_OP_CONFIG_H_ */
