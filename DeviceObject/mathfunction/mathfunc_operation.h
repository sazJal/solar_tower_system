/*
 * mathfunc_operation.h
 *
 *  Created on: Feb 14, 2020
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_MATHFUNCTION_MATHFUNC_OPERATION_H_
#define DEVICEOBJECT_MATHFUNCTION_MATHFUNC_OPERATION_H_

#include "xmc_scu.h"
#include "pdu_operation.h"

#define	CRC_POLY_16			0xA001
#define	CRC_START_16		0x0000

void MF_MovingAverage(PDU_SENSOR_AVG_t *sensorAvg, PDU_SENSOR_RAW_t *sensorRaw);

void MF_Init_CRC16_Tab(void);
uint16_t MF_CRC_16(const unsigned char *input_str, unsigned char num_bytes);

#endif /* DEVICEOBJECT_MATHFUNCTION_MATHFUNC_OPERATION_H_ */
