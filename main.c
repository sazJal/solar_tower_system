/*
 * main.c
 *
 *  Created on: 2020 Jun 11 15:07:03
 *  Author: lenovo
 */



#include <xmc_common.h>
#include "op_config.h"

int main(void)
{
  /* Create 1 kHz tick */
  SysTick_Config(SystemCoreClock / 1000);

  /* Initialization for PC Communication */
  RS232_Connection_Init();

  /* Initialization for ESP32 Communication */
  ESP32_ConnectionInit();

  /* Initialization for PCS Communication */
  PCS_Connection_Init();

  /* Initialization for Battery Communication */
  GEENIX_Connection_Init();

  /* Initialization for Power Distribution Control Peripheral */
  PDU_Relay_Init();
  PDU_SensorADC_Init();
  pduSensorAvg.count 	= 0;
  pduSensorAvg.isValid 			= false;
  pduSensorAvg.isBeingProcessed = false;

  /* Setup for infotainment Functions */
  INFO_LEDInit();
  INFO_GPIOInit();
  INFO_StartButtonInit();
  INFO_ModeSelectorInit();
  INFO_ResetButtonInit();
  INFO_RTCEEPROMInit();

  /* Setup Timer */
  MISC_TimerInit();

  /* Initiate RTC Modes */
  INFO_RTCVBatEnable();
  INFO_RTCOscillatorEnable();
  INFO_RTCMFPEnable();

  /* PCS Command Initialization */
  mcuState = OP_SETUP;
  recEspCommIsHalt = false;

  while(1U)
  {
	  /* Continuously Checking Mode and Error */
	  OP_CheckingError(&controlFlag, &pcsSystemStatus, &pcsDCgridStatus,
			  	  	   &pcsACinvStatus, &pcsPVconvStatus, &pcsBatconvStatus);
	  OP_CheckingMode(&controlFlag);
	  if(controlFlag.isChecking_peripheral){
		  mcuState = OP_PERIPH;
	  }

	  /* Perform an action the according to the state */
	  switch(mcuState)
	  {
	  case OP_SETUP 	:
		  mcuState	= OP_SETUP;

		  /* Set LED Indicator for this state */
		  INFO_LEDSwitch(LED0, LED_ON);
		  INFO_LEDSwitch(LED1, LED_OFF);
		  INFO_LEDSwitch(LED2, LED_OFF);

		  /* Perform actions for this state */
		  OP_SetupConnection(&mcuState, &esp32HShake, &pvHShake, &controlFlag,
				  	  	  	 pduSensor.mPVPT, &pcsBmsStatus, &pcsSystemStatus,
							 &rtcTime, &rtcDate);
		  break;
	  case OP_TURN_ON 	:
		  mcuState	= OP_TURN_ON;

		  /* Set LED Indicator for this state */
		  INFO_LEDSwitch(LED0, LED_OFF);
		  INFO_LEDSwitch(LED1, LED_ON);
		  INFO_LEDSwitch(LED2, LED_OFF);

		  /* Perform actions for this state */
		  OP_TurnOnPCS(&mcuState, &buttonHShake, &controlFlag, &pcsBmsStatus);
		  break;
	  case OP_RUNNING 	:
		  mcuState	= OP_RUNNING;

		  /* Set LED Indicator for this state */
		  INFO_LEDSwitch(LED0, LED_ON);
		  INFO_LEDSwitch(LED1, LED_ON);
		  INFO_LEDSwitch(LED2, LED_OFF);

		  /* Perform actions for this state */
		  OP_CheckingPCSStatus(&mcuState, &pcsSystemStatus, &buttonHShake,
				  	  	  	   &controlFlag);
		  break;
	  case OP_STANDBY	:
		  mcuState	= OP_STANDBY;

		  /* Set LED Indicator for this state */
		  INFO_LEDSwitch(LED0, LED_OFF);
		  INFO_LEDSwitch(LED1, LED_OFF);
		  INFO_LEDSwitch(LED2, LED_ON);

		  /* Perform actions for this state */
		  OP_PutSystemStandby(&mcuState, &pcsSystemStatus, &buttonHShake,
				  	  	  	  &controlFlag, &pcsBmsStatus);
		  break;
	  case OP_ERROR		:
		  mcuState	= OP_ERROR;

		  /* Set LED Indicator for this state */
		  INFO_LEDSwitch(LED0, LED_ON);
		  INFO_LEDSwitch(LED1, LED_OFF);
		  INFO_LEDSwitch(LED2, LED_ON);

		  /* Perform actions for this state */
		  OP_HandlingPCSWarning(&mcuState, &pcsSystemStatus, &buttonHShake,
				  	  	  	  	&controlFlag, &bmsSensor, &pcsBmsStatus);
		  break;
	  case OP_SHUTDOWN	:
		  mcuState 	= OP_SHUTDOWN;

		  /* Set LED Indicator for this state */
		  INFO_LEDSwitch(LED0, LED_ON);
		  INFO_LEDSwitch(LED1, LED_ON);
		  INFO_LEDSwitch(LED2, LED_ON);

		  /* Perform actions for this state */
		  OP_PutSystemShutDown(&mcuState, &buttonHShake);
		  break;
	  case OP_PERIPH	:
		  mcuState	= OP_PERIPH;

		  /* Set LED Indicator for this state */
//		  INFO_LEDSwitch(LED0, LED_ON);
//		  INFO_LEDSwitch(LED1, LED_ON);
//		  INFO_LEDSwitch(LED2, LED_ON);

		  /* Perform actions for this state */
		  if(!controlFlag.isChecking_peripheral)
		  {
			  mcuState = OP_SETUP;
		  }
		  break;
	  default			: break;
	  }
  }
}
