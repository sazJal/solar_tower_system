/*
 * info_operation.h
 *
 *  Created on: Jun 26, 2020
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_INFOTAINMENT_INFO_OPERATION_H_
#define DEVICEOBJECT_INFOTAINMENT_INFO_OPERATION_H_

#include "info_operation_conf.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

#define SRAMRTCC	(uint8_t)(0xDE)
#define RTCPROM		(uint8_t)(0xAE)
#define RTCSECOND	(uint8_t)(0x00)
#define RTCMINUTE	(uint8_t)(0x01)
#define RTCHOUR		(uint8_t)(0x02)
#define RTCWKDAY	(uint8_t)(0x03)
#define RTCDATE		(uint8_t)(0x04)
#define RTCMONTH	(uint8_t)(0x05)
#define RTCYEAR		(uint8_t)(0x06)
#define MFPCONTROL	(uint8_t)(0x07)

#define AT24ADDR	(uint8_t)(0xA0)

typedef enum LED_NAME
{
	LED0,
	LED1,
	LED2
} LED_NAME_t;

typedef enum GPIO_NAME
{
	GPIO0,
	GPIO1,
	GPIO2,
	GPIO3
} GPIO_NAME_t;

typedef enum GPIO_STATUS
{
	GPIO_LOW,
	GPIO_HIGH,
} GPIO_STATUS_t;

typedef enum LED_STATUS
{
	LED_OFF,
	LED_ON
} LED_STATUS_t;

typedef struct RTC_TIME_PARAM
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} RTC_TIME_PARAM_t;

typedef struct RTC_DATE_PARAM
{
  uint16_t year;
  uint8_t  month;
  uint8_t  date;
} RTC_DATE_PARAM_t;

/* Initialization */
void INFO_StartButtonInit(void);
void INFO_ResetButtonInit(void);
void INFO_LEDInit(void);
void INFO_ModeSelectorInit(void);
void INFO_GPIOInit(void);
void INFO_RTCEEPROMInit(void);

/* LED Method */
void INFO_LEDSwitch(LED_NAME_t led_name, LED_STATUS_t status);

/* Mode Selector Methods */
uint8_t INFO_ModeSelectorGetStatus(void);

/* GPIO Methods */
void INFO_GPIOSet(GPIO_NAME_t gpio_name, GPIO_STATUS_t status);

/* RTC Methods */
uint8_t INFO_RTCOscillatorEnable(void);
void INFO_RTCOscillatorDisable(void);
void INFO_RTCTimeInit(RTC_TIME_PARAM_t rtc_time);
void INFO_RTCDateInit(RTC_DATE_PARAM_t rtc_date);
RTC_DATE_PARAM_t INFO_RTCGetDate(void);
RTC_TIME_PARAM_t INFO_RTCGetTime(void);
void INFO_RTCVBatEnable(void);
void INFO_RTCMFPEnable(void);
void INFO_RTCMFPDisable(void);

/* EEPROM Methods */
void INFO_EEPROM_Write(uint16_t address, uint8_t data);
uint8_t INFO_EEPROM_Read(uint16_t address);



#endif /* DEVICEOBJECT_INFOTAINMENT_INFO_OPERATION_H_ */
