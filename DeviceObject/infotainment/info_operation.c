/*
 * info_operation.c
 *
 *  Created on: Jun 26, 2020
 *      Author: lenovo
 */

#include "info_operation.h"

/* Initialization */
void INFO_StartButtonInit(void)
{
	XMC_GPIO_Init(P2_2,&button_config);
}
void INFO_ResetButtonInit(void)
{
	XMC_GPIO_Init(P2_10, &eru_io_config);

	/* ETL and OGU Configuration */
	XMC_ERU_ETL_Init(ERU0_ETL2, &eru0_etl2_config);
	XMC_ERU_OGU_Init(ERU0_OGU2, &eru0_ogu2_config);

	XMC_SCU_SetInterruptControl(IRQ11_IRQn, XMC_SCU_IRQCTRL_ERU0_SR2_IRQ11);
	NVIC_SetPriority(IRQ11_IRQn, 4U);
	NVIC_EnableIRQ(IRQ11_IRQn);
}
void INFO_LEDInit(void)
{
	XMC_GPIO_Init(P2_11,&led_config);
	XMC_GPIO_Init(P2_12,&led_config);
	XMC_GPIO_Init(P2_13,&led_config);

	XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_HIGH);
	XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_HIGH);
}
void INFO_GPIOInit(void)
{
	XMC_GPIO_Init(P4_8,&gpio_config);
	XMC_GPIO_Init(P4_9,&gpio_config);
	XMC_GPIO_Init(P4_10,&gpio_config);
	XMC_GPIO_Init(P4_11,&gpio_config);

	XMC_GPIO_SetOutputLevel(P4_11, XMC_GPIO_OUTPUT_LEVEL_LOW);
}
void INFO_ModeSelectorInit(void)
{
	XMC_GPIO_Init(P4_2,&dipswitch_config);
	XMC_GPIO_Init(P4_3,&dipswitch_config);
	XMC_GPIO_Init(P4_4,&dipswitch_config);
	XMC_GPIO_Init(P4_5,&dipswitch_config);
}
void INFO_RTCEEPROMInit(void)
{
	XMC_GPIO_Init(P3_2, &eru_io_config);

	/* ETL and OGU Configuration for RTC MFP Configuration */
	XMC_ERU_ETL_Init(ERU1_ETL2, &eru1_etl2_config);
	XMC_ERU_OGU_Init(ERU1_OGU3, &eru1_ogu3_config);

	XMC_SCU_SetInterruptControl(IRQ6_IRQn, XMC_SCU_IRQCTRL_ERU1_SR3_IRQ6);
	NVIC_SetPriority(IRQ6_IRQn, 8U);
	NVIC_EnableIRQ(IRQ6_IRQn);

	/* Configure IIC channel */
	XMC_I2C_CH_Init(XMC_USIC1_CH1, &iic_config);
	/* Configure input multiplexors */
	XMC_I2C_CH_SetInputSource(XMC_USIC1_CH1, XMC_I2C_CH_INPUT_SDA, USIC1_C1_DX0_P0_0);
	XMC_I2C_CH_SetInputSource(XMC_USIC1_CH1, XMC_I2C_CH_INPUT_SCL, USIC1_C1_DX1_P0_1);

	/* Initialize GPIO */
	XMC_GPIO_Init(P0_0, &rtc_sda_config);
	XMC_GPIO_Init(P0_1, &rtc_scl_config);

	/* Start IIC channel */
	XMC_I2C_CH_Start(XMC_USIC1_CH1);
}

/* LED Method */
void INFO_LEDSwitch(LED_NAME_t led_name, LED_STATUS_t status)
{
	// active low IO
	switch(led_name)
	{
		case LED0   : status?(XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_LOW)):\
							 (XMC_GPIO_SetOutputLevel(P2_11, XMC_GPIO_OUTPUT_LEVEL_HIGH));
						  	  break;
		case LED1 	: status?(XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_LOW)):\
							 (XMC_GPIO_SetOutputLevel(P2_12, XMC_GPIO_OUTPUT_LEVEL_HIGH));
							  break;
		case LED2 	: status?(XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_LOW)):\
							 (XMC_GPIO_SetOutputLevel(P2_13, XMC_GPIO_OUTPUT_LEVEL_HIGH));
							  break;
		default				: break;
	}
}

/* Mode Selector Methods */
uint8_t INFO_ModeSelectorGetStatus(void)
{
	uint8_t status;
	status = XMC_GPIO_GetInput(P4_5);
	status = (status << 1) | XMC_GPIO_GetInput(P4_4);
	status = (status << 1) | XMC_GPIO_GetInput(P4_3);
	status = (status << 1) | XMC_GPIO_GetInput(P4_2);

	return status;
}

/* GPIO Methods */
void INFO_GPIOSet(GPIO_NAME_t gpio_name, GPIO_STATUS_t status)
{
	// active high IO
	switch(gpio_name)
	{
		case GPIO0   : status?(XMC_GPIO_SetOutputLevel(P4_8, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
							  (XMC_GPIO_SetOutputLevel(P4_8, XMC_GPIO_OUTPUT_LEVEL_LOW));
						  	  break;
		case GPIO1   : status?(XMC_GPIO_SetOutputLevel(P4_9, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
							  (XMC_GPIO_SetOutputLevel(P4_9, XMC_GPIO_OUTPUT_LEVEL_LOW));
						  	  break;
		case GPIO2   : status?(XMC_GPIO_SetOutputLevel(P4_10, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
							  (XMC_GPIO_SetOutputLevel(P4_10, XMC_GPIO_OUTPUT_LEVEL_LOW));
						  	  break;
		case GPIO3   : status?(XMC_GPIO_SetOutputLevel(P4_11, XMC_GPIO_OUTPUT_LEVEL_HIGH)):\
							  (XMC_GPIO_SetOutputLevel(P4_11, XMC_GPIO_OUTPUT_LEVEL_LOW));
						  	  break;
		default				: break;
	}
}

/* RTC Methods */
uint8_t INFO_RTCOscillatorEnable(void)
{
	uint8_t data;

	/*    						Accessing RTCC Control Byte w/ WR					*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Second Address Byte   							*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCSECOND);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again w/ RD				*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*							Send NACK											*/
    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                              XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                  XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Load Register Information  								*/
    data     = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    /* 							Enable ST Bit 										*/
    data	= data | 0x80;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Second Address Byte  					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCSECOND);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write value at Second Address Byte 					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, data);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

	return data;

}
void INFO_RTCOscillatorDisable(void)
{
	uint8_t data;

	/*    						Accessing RTCC Control Byte w/ WR					*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Second Address Byte   							*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCSECOND);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again w/ RD				*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*							Send NACK											*/
    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                              XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                  XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Load Register Information  								*/
    data     = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    /* 							Disable ST Bit 										*/
    data	= data & 0x7F;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Second Address Byte  					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCSECOND);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write value at Second Address Byte 					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, data);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}

void INFO_RTCTimeInit(RTC_TIME_PARAM_t rtc_time)
{
	static uint8_t tens;
	static uint8_t ones;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

  	/*    						Write Seconds Address Byte   					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCSECOND);
 	ones	= rtc_time.second % 10;				// preparing 'seconds' value while bus is transmitting RTCSEC address value
	tens	= rtc_time.second / 10;
	rtc_time.second = (tens << 4) | (0x0F & ones);

	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write Data into Seconds Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, rtc_time.second | 0x80);	// '1' in MSB is used to keep ST bit active
	ones 		= rtc_time.minute % 10;		// preparing 'minutes' value while bus is transmitting 'seconds' value
	tens 		= rtc_time.minute / 10;
	rtc_time.minute = (tens << 4) | (0x0F & ones);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write Data into Minutes Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, rtc_time.minute);
	ones 		= rtc_time.hour % 10;				// preparing 'hour' value while bus is transmitting 'minutes' value
	tens 		= rtc_time.hour / 10;
	rtc_time.hour = (tens << 4) | (0x0F & ones);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 						Write Data into Hour Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, rtc_time.hour & ~0x40);		// is used for 24H mode

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

  	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}
void INFO_RTCDateInit(RTC_DATE_PARAM_t rtc_date)
{
	static uint8_t tens;
	static uint8_t ones;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

  	/*    						Write Date Address Byte   					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCDATE);
 	ones	= rtc_date.date % 10;
	tens	= rtc_date.date / 10;
	rtc_date.date = (tens << 4) | (0x0F & ones);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write Data into Date Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, rtc_date.date);
	ones 		= rtc_date.month % 10;
	tens 		= rtc_date.month / 10;
	rtc_date.month = (tens << 4) | (0x0F & ones);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write Data into Month Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, rtc_date.month);
	ones 		= rtc_date.year % 10;
	tens 		= rtc_date.year / 10;
	rtc_date.year = (tens << 4) | (0x0F & ones);

	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 						Write Data into Year Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, rtc_date.year);

//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

  	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}

RTC_DATE_PARAM_t INFO_RTCGetDate(void)
{
	RTC_DATE_PARAM_t DataReceived;
	uint8_t tens;
	uint8_t ones;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write Date Address Byte   					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCDATE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again  					*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    XMC_I2C_CH_MasterReceiveAck(XMC_USIC1_CH1);

//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                              XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                             XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Read Date Information  					*/
    DataReceived.date     = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    DataReceived.date     = DataReceived.date & 0x7F;

    XMC_I2C_CH_MasterReceiveAck(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                             XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                           XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Read Month Information  					*/
    DataReceived.month    = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    DataReceived.month	  = DataReceived.month & 0x1F;

    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//                                XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                           XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Read Year Information  					*/
    DataReceived.year      = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    tens = 10*(DataReceived.date >> 4);
    ones = DataReceived.date & 0x0F;
    DataReceived.date = tens + ones;

    tens = 10*(DataReceived.month >> 4);
    ones = DataReceived.month & 0x0F;
    DataReceived.month = tens + ones;

    tens = 10*(DataReceived.year >> 4);
    ones = DataReceived.year & 0x0F;
    DataReceived.year = tens + ones;

    return DataReceived;
}

RTC_TIME_PARAM_t INFO_RTCGetTime(void)
{
	RTC_TIME_PARAM_t DataReceived;
	uint8_t tens;
	uint8_t ones;
	XMC_GPIO_SetOutputHigh(P2_11);

	/*    						Accessing RTCC Control Byte 					*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

  	/*    						Write Seconds Address Byte   					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCSECOND);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/* 							Start Read Sequence								*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/* 							Read first sequence 							*/
    XMC_I2C_CH_MasterReceiveAck(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//                                   XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                   XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    DataReceived.second   = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    DataReceived.second   = DataReceived.second & 0x7F;

    /*							Read Second Sequence							*/
    XMC_I2C_CH_MasterReceiveAck(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//                                   XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                   XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    DataReceived.minute    = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    DataReceived.minute	 = DataReceived.minute & 0x7F;

    /*							Read Third Sequence then Dismiss				*/
    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//                                   XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                    XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    DataReceived.hour      = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    DataReceived.hour	     = DataReceived.hour & 0x3F;

    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    tens = 10*(DataReceived.hour >> 4);
    ones = DataReceived.hour & 0x0F;
    DataReceived.hour = tens + ones;

    tens = 10*(DataReceived.minute >> 4);
    ones = DataReceived.minute & 0x0F;
    DataReceived.minute = tens + ones;

    tens = 10*(DataReceived.second >> 4);
    ones = DataReceived.second & 0x0F;
    DataReceived.second = tens + ones;

    return DataReceived;
}

void INFO_RTCVBatEnable(void)
{
	uint8_t data;

	/*    						Accessing RTCC Control Byte w/ WR					*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write Date Address Byte   							*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCWKDAY);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again w/ RD				*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*							Send NACK											*/
    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                              XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                  XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Load Register Information  								*/
    data     = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    /* 							Enable VBATEN bit										*/
    data	= data & 0x07; // enable VBATEN bit
    data	= data | 0x08; // enable VBATEN bit

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write Control Address Byte  					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, RTCWKDAY);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write value at Control Register 					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, data);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}

void INFO_RTCMFPEnable(void)
{
	uint8_t data;

	/*    						Accessing RTCC Control Byte w/ WR					*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Second Address Byte   							*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, MFPCONTROL);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again w/ RD				*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*							Send NACK											*/
    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                              XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                  XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Load Register Information  								*/
    data     = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    /* 							Enable SQWEN Bit SQWFS1 and SQWFS0 depend on MFP_FREQ value, others '0'									*/
    data	= data | 0x40;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Control Address Byte  					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, MFPCONTROL);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write value at Control Address Byte 					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, data);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}
void INFO_RTCMFPDisable(void)
{
	uint8_t data;

	/*    						Accessing RTCC Control Byte w/ WR					*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Second Address Byte   							*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, MFPCONTROL);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again w/ RD				*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_READ);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*							Send NACK											*/
    XMC_I2C_CH_MasterReceiveNack(XMC_USIC1_CH1);
//    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
//	                              XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                  XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Load Register Information  								*/
    data     = XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1);
    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

    /* 							Disable SQWEN Bit SQWFS1 and SQWFS0 depend on MFP_FREQ value, others '0'									*/
    data	= data & 0x3F;

	/*    						Accessing RTCC Control Byte 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, SRAMRTCC, XMC_I2C_CH_CMD_WRITE);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Write RTC Control Address Byte  					*/
    XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, MFPCONTROL);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
    for(uint16_t count=0; count < 500; count++){}
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write value at Control Address Byte 					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, data);
//	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U){/* wait for ACK */}
	for(uint16_t count=0; count < 500; count++){}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}

/* EEPROM Methods */
void INFO_EEPROM_Write(uint16_t address, uint8_t data)
{
	uint8_t address_buff;
	/*    						Accessing Device Address 						*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, AT24ADDR, XMC_I2C_CH_CMD_WRITE);
	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*    						Write MSB Address Byte   					*/
	address_buff = (address >> 8);
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, address_buff);

	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write LSB Address Byte						*/
	address_buff = address & 0xFF;
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, (address_buff));
	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write Data into the AT24C64						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, data);

	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);
}
uint8_t INFO_EEPROM_Read(uint16_t address)
{
	uint8_t data=0;

	/*    						Accessing Device Address 				*/
	XMC_I2C_CH_MasterStart(XMC_USIC1_CH1, AT24ADDR, XMC_I2C_CH_CMD_WRITE);

	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*    						Write MSB Address Byte   					*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, (uint8_t)(address>>8));
	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

	/*	 					   Write LSB Address Byte						*/
	XMC_I2C_CH_MasterTransmit(XMC_USIC1_CH1, (uint8_t)(address));
	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */

	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    /*    						Accessing RTCC Control Byte again  					*/
    XMC_I2C_CH_MasterRepeatedStart(XMC_USIC1_CH1, AT24ADDR, XMC_I2C_CH_CMD_READ);
	while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0U)
	{
		/* wait for ACK */
	}
	XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    XMC_I2C_CH_MasterReceiveAck(XMC_USIC1_CH1);

    while((XMC_I2C_CH_GetStatusFlag(XMC_USIC1_CH1) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
	                                                 XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) == 0U)
    {
    	/* wait for ACK */
    }
    XMC_I2C_CH_ClearStatusFlag(XMC_USIC1_CH1, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
                                           XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    /*    						Read Month Information  					*/
    data    = (uint8_t)(XMC_I2C_CH_GetReceivedData(XMC_USIC1_CH1));

    XMC_I2C_CH_MasterStop(XMC_USIC1_CH1);

	return data;
}




