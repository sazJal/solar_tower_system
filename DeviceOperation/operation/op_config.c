/*
 * op_config.c
 *
 *  Created on: Jul 3, 2020
 *      Author: lenovo
 */

#include "op_config.h"
#include "stdio.h"

void OP_SetupConnection(OP_STATE_t *state, MISC_HANDSHAKE_PARAM_t *espHandShake,
						MISC_HANDSHAKE_PARAM_t *pvHandShake,
						MISC_CTRL_FLAG_t *ctrlFlag, float pvVoltage,
						PCS_BMS_STATUS_t *pcsBmsStatus, PCS_STATUS_t *pcsStatus,
						RTC_TIME_PARAM_t *rtcTime, RTC_DATE_PARAM_t *rtcDate)
{
	/* 1. Handshaking for ESP32 */
	if(espHandShake->isExecuted)
	{
		/* Activate Time Out Parameter */
		espHandShake->isTimeOut = false;
		espHandShake->cycle		= 7000;
		espHandShake->isEnabled = true;

		/* Activate Handshaking Process */
		espHandShake->isStarted	= true;
		espHandShake->isValid	= true;

		/* 2. Ensure all relay at initial state */
		/* Initiate relay status */
		PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_OPEN);
		PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_OPEN);
		PDU_Switch_Relay(PDU_RELAY_KMP,  PDU_RELAY_OPEN);
		PDU_Switch_Relay(PDU_RELAY_KMF,  PDU_RELAY_OPEN);

		/* CAN Status Setup */
		pcsBmsStatus->bms_main_relay		= RELAY_OPEN;
		pcsBmsStatus->bms_precharge_relay	= RELAY_OPEN;
		pcsBmsStatus->bms_status			= BMU_PRECHARGE;
		pcsBmsStatus->bms_pcs_command		= BMU_PCS_STOP;

		/* Notify conditional state */
		espHandShake->isExecuted	= true;
	}

	/* 2. Checking PV Condition */
	if(!pvHandShake->isExecuted)
	{
		if(pvVoltage > 150.0 || ctrlFlag->isBypass_power_mode)
		{
			pvHandShake->isTimeOut	= false;
			pvHandShake->isEnabled	= true;
			pvHandShake->cycle		= 10000;
			pvHandShake->isExecuted	= true;
		}
	}

	/* Evaluate 1) and 2) status */
	if((pvVoltage > 150.0 || ctrlFlag->isBypass_power_mode)
		&& (pcsStatus->pcs_system_status == PCS_STAT_STANDBY))
	{
		if(pvHandShake->isTimeOut)
		{
			if(espHandShake->isValid)
			{
				rtcDate->year = rtcDate->year - 2000;
				INFO_RTCDateInit(*rtcDate);
				INFO_RTCTimeInit(*rtcTime);
				espHandShake->isValid 	= false;
				espHandShake->isStarted = false;
			}

			PDU_Switch_Relay(PDU_RELAY_KMF,  PDU_RELAY_CLOSE);
			PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_CLOSE);
			pcsBmsStatus->bms_precharge_relay = RELAY_CLOSE;
			*state	= OP_TURN_ON;

			/* restart handshaking variable */
			espHandShake->isEnabled		= false;
			espHandShake->isExecuted	= false;
			pvHandShake->isEnabled		= false;
			pvHandShake->isExecuted		= false;
		}
	}
	else
	{
		pvHandShake->isExecuted	= false;
	}
}

void OP_TurnOnPCS(OP_STATE_t *state, MISC_HANDSHAKE_PARAM_t *CtrlButton,
				  MISC_CTRL_FLAG_t *ctrlFlag, PCS_BMS_STATUS_t *pcsBmsStatus)
{
	static bool isPrechargeExecuted = false;

	if(!CtrlButton->isExecuted)
	{
		/* Turning on Timer if button pressed */
		if(!XMC_GPIO_GetInput(P2_2))
		{
			CtrlButton->isTimeOut 	= false;
			CtrlButton->isEnabled	= true;
			CtrlButton->cycle		= 2000;
			CtrlButton->isExecuted	= true;
		}
	}

	/* Check if it is still being pressed */
	if(!XMC_GPIO_GetInput(P2_2))
	{
		if(CtrlButton->isTimeOut)
		{
			CtrlButton->isTimeOut = false;
			ctrlFlag->isPerforming_Precharge = true;
		}
	}
	else
	{
		/* Clear status if button is released */
		CtrlButton->isExecuted	= false;
		CtrlButton->isEnabled	= false;
	}

	if(ctrlFlag->isPerforming_Precharge)
	{
		if(!isPrechargeExecuted)
		{
			pcsBmsStatus->bms_main_relay 		= RELAY_OPEN;
			pcsBmsStatus->bms_precharge_relay	= RELAY_CLOSE;
			pcsBmsStatus->bms_status			= BMU_PRECHARGE;
			pcsBmsStatus->bms_pcs_command		= BMU_PCS_STOP;
			isPrechargeExecuted					= true;
		}

		if(pcsSystemStatus.pcs_precharge_IsComplete)
		{
			/* CLose Main Relay */
			PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_CLOSE);

			/* Create 10 us Delay */
			sysTickFlag = true;
			while(sysTickFlag){}

			/* Open Predischarge Relay */
			PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_OPEN);

			pcsBmsStatus->bms_main_relay		= RELAY_CLOSE;
			pcsBmsStatus->bms_precharge_relay	= RELAY_OPEN;
			pcsBmsStatus->bms_status			= BMU_RUNNING;
			pcsBmsStatus->bms_pcs_command		= BMU_PCS_START;

			*state = OP_RUNNING;

			/* close battery relay */
			PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_CLOSE);
			isPrechargeExecuted					= false;
			ctrlFlag->isPerforming_Precharge	= false;
		}
	}
}

void OP_CheckingPCSStatus(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						  MISC_HANDSHAKE_PARAM_t *CtrlButton,
						  MISC_CTRL_FLAG_t *ctrlFlag)
{
	static bool isPutToStandbyExecuted = false;

	/* 1. Check PCS Warning Status */
	if(pcsStatus->pcs_system_status == PCS_STAT_STANDBY)
	{
		*state = OP_ERROR;
	}

	/* 2. Check Standby Button */
	if(!CtrlButton->isExecuted && (!ctrlFlag->isAuto_mode))
	{
		if(!XMC_GPIO_GetInput(P2_2))
		{
			CtrlButton->isTimeOut	= false;
			CtrlButton->isEnabled	= true;
			CtrlButton->cycle		= 3000;
			CtrlButton->isExecuted	= true;
		}
	}

	/* Check if it is still being pressed */
	if(!XMC_GPIO_GetInput(P2_2) && (!isPutToStandbyExecuted))
	{
		if(CtrlButton->isTimeOut)
		{
			isPutToStandbyExecuted	= true;
			CtrlButton->isTimeOut	= false;
		}
	}
	else
	{
		/* Clear status if button is released */
		CtrlButton->isExecuted	= false;
		CtrlButton->isEnabled	= false;
	}

	/* Change to STANDBY upon the timer's time-out and button is released */
	if(isPutToStandbyExecuted)
	{
		if(XMC_GPIO_GetInput(P2_2))
		{
			isPutToStandbyExecuted	= false;
			*state					= OP_STANDBY;
			CtrlButton->isEnabled	= false;
			CtrlButton->isExecuted	= false;
		}
	}
}

void OP_PutSystemStandby(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						 MISC_HANDSHAKE_PARAM_t *CtrlButton,
						 MISC_CTRL_FLAG_t *ctrlFlag,
						 PCS_BMS_STATUS_t *pcsBmsStatus)
{
	static bool isStandbyExecuted = false;
	if(pcsStatus->pcs_system_status == PCS_STAT_RUNNING)
	{
		if(!isStandbyExecuted)
		{
			pcsBmsStatus->bms_pcs_command = BMU_PCS_STOP;
			isStandbyExecuted = true;
		}
	}

	if(pcsStatus->pcs_system_status == PCS_STAT_STANDBY)
	{
		/* Open Pre Discharge Relay */
		PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_OPEN);

		/* Create 10 us Delay */
		sysTickFlag = true;
		while(sysTickFlag){}

		/* Open Main Relay */
		PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_OPEN);

		pcsBmsStatus->bms_main_relay		= RELAY_OPEN;
		pcsBmsStatus->bms_precharge_relay	= RELAY_OPEN;
		pcsBmsStatus->bms_status			= BMU_PRECHARGE;

		*state = OP_TURN_ON;
		isStandbyExecuted = false;
	}
}

void OP_HandlingPCSWarning(OP_STATE_t *state, PCS_STATUS_t *pcsStatus,
						   MISC_HANDSHAKE_PARAM_t *CtrlButton,
						   MISC_CTRL_FLAG_t *ctrlFlag, BMS_PARAMETER_t *bmsParam,
						   PCS_BMS_STATUS_t *pcsBmsStatus)
{

	/* 1. if error elapsed, attempt for change PCS to RUN state */
	if(!ctrlFlag->isStandby)
	{
		pcsBmsStatus->bms_pcs_command = BMU_PCS_START;
	}

	/* 1. if PCS acknowledged that already change to RUN, return the state to RUNNING */
	if(pcsStatus->pcs_system_status == PCS_STAT_RUNNING)
	{
		*state = OP_RUNNING;
	}

	/* 2. Check SoC Level */
	/* force shutdown */
	//if(ctrlFlag->isForced_shut_down){bmsParam->soc = 2;}
	if((bmsParam->soc <= 50) && (!ctrlFlag->isBypass_power_mode))
	{
		*state = OP_SHUTDOWN;
	}
}

void OP_PutSystemShutDown(OP_STATE_t *state, MISC_HANDSHAKE_PARAM_t *CtrlButton)
{
	if(!CtrlButton->isExecuted)
	{
		CtrlButton->isEnabled	= true;
		CtrlButton->cycle		= 1000;
		CtrlButton->isExecuted	= true;
		CtrlButton->isTimeOut	= false;
	}

	if(CtrlButton->isTimeOut)
	{
		PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_OPEN);
		CtrlButton->isExecuted = false;
		*state = OP_SETUP;
	}
}
void OP_CheckingMode(MISC_CTRL_FLAG_t *ctrlFlag)
{
	/* Read Dip Switch */
	(XMC_GPIO_GetInput(P4_2) == 1)?(ctrlFlag->isBypass_power_mode = true):
								   (ctrlFlag->isBypass_power_mode = false);
	(XMC_GPIO_GetInput(P4_3) == 1)?(ctrlFlag->isAuto_mode = true):
								   (ctrlFlag->isAuto_mode = false);
	(XMC_GPIO_GetInput(P4_4) == 1)?(ctrlFlag->isChecking_peripheral = true):
								   (ctrlFlag->isChecking_peripheral = false);
	(XMC_GPIO_GetInput(P4_5) == 1)?(ctrlFlag->isForced_shutting_down = true):
								   (ctrlFlag->isForced_shutting_down = false);
}


void OP_CheckingError(MISC_CTRL_FLAG_t *ctrlFlag, PCS_STATUS_t *pcsStatus,
					  DCGRID_STATUS_t *gridStatus, ACINV_STATUS_t *invStatus,
					  PVCONV_STATUS_t *pvStatus, BATCONV_STATUS_t *BatStatus)
{
	  /* Check if any system in Standby mode */
	ctrlFlag->isStandby = pcsStatus->pcs_system_pcsIsStandBy 	 |\
						  gridStatus->dcgrid_system_pcsIsStandBy |\
						  invStatus->acinv_system_pcsIsStandBy	 |\
						  pvStatus->pvconv_system_pcsIsStandBy   |\
						  BatStatus->batconv_system_pcsIsStandBy;
}
