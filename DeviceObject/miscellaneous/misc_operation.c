/*
 * misc_operation.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "misc_operation.h"

MISC_SYSTICK_PARAM_t sysTickHandle;

void MISC_TimerInit(void)
{

	/* Enable clock, enable prescaler block and configure global control */
	XMC_CCU4_Init(CCU40, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);

	/* Start the prescaler and restore clocks to slices */
	XMC_CCU4_StartPrescaler(CCU40);

	/* Start of CCU4 configurations */
	/* Ensure fCCU reaches CCU40 */
	XMC_CCU4_SetModuleClock(CCU40, XMC_CCU4_CLOCK_SCU);

	//Configure Slice(s) Functions, Interrupts and Start-up
	/* Initialize the Slice */
	XMC_CCU4_SLICE_CompareInit(CCU40_CC40, &SLICE0_config);
	XMC_CCU4_SLICE_CompareInit(CCU40_CC41, &SLICE1_config);

	/* Frequency CCU4 = 96MHz */
	/* Freq / Prescaler 32 = 3 MHz */
	/* Program duty cycle[50%] and frequency [1 kHz] */
	XMC_CCU4_SLICE_SetTimerCompareMatch(CCU40_CC40, 1500); // any value less than PWM Period Counter
	XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC40, 3000);

	/* Frequency CCU4 = 96MHz */
	/* Freq / Prescaler 1024 = 93750 Hz */
	/* Program duty cycle[50%] and frequency [10 Hz] */
	XMC_CCU4_SLICE_SetTimerCompareMatch(CCU40_CC41, 4687); // any value less than PWM Period Counter
	XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC41, 9375);

	/* Enable shadow transfer for PWM Slice */
	XMC_CCU4_EnableShadowTransfer(CCU40, \
		(uint32_t)(XMC_CCU4_SHADOW_TRANSFER_SLICE_0|XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0));
	XMC_CCU4_EnableShadowTransfer(CCU40, \
		(uint32_t)(XMC_CCU4_SHADOW_TRANSFER_SLICE_1|XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_1));

	/* Enable events */
	XMC_CCU4_SLICE_EnableEvent(CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
	/* Enable events */
	XMC_CCU4_SLICE_EnableEvent(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

	/* Connect event to SR2 */
	XMC_CCU4_SLICE_SetInterruptNode(CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_2);
	XMC_CCU4_SLICE_SetInterruptNode(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_3);

	/* Set NVIC priority */
	NVIC_SetPriority(IRQ23_IRQn, 3U); // for CCU40_CC40
	NVIC_EnableIRQ(IRQ23_IRQn);

	/* Set NVIC priority */
	NVIC_SetPriority(IRQ24_IRQn, 9U); // for CCU40_CC41
	NVIC_EnableIRQ(IRQ24_IRQn);

	/* Get the slice out of idle mode */
	XMC_CCU4_EnableClock(CCU40, 0U);
	XMC_CCU4_EnableClock(CCU40, 1U);

	/* Start the TImer*/
	XMC_CCU4_SLICE_StartTimer(CCU40_CC40);	// timer for Handshaking
	XMC_CCU4_SLICE_StartTimer(CCU40_CC41); // clocking for BMS and ESP32

}

void MISC_DelayUs(uint16_t delay)
{
	sysTickHandle.count   = delay;
	sysTickHandle.enable  = true;
	sysTickHandle.isTimeOut = false;
	while(!sysTickHandle.isTimeOut){}
}
