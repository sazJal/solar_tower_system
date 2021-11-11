/*
 * rs232_operation.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOBJECT_RS232_RS232_OPERATION_H_
#define DEVICEOBJECT_RS232_RS232_OPERATION_H_

#include "rs232_operation_conf.h"
#include "stdio.h"
#include "string.h"

void RS232_Connection_Init(void);
void RS232_SendString(XMC_USIC_CH_t *const channel, char* DataPtr, uint32_t count);

#endif /* DEVICEOBJECT_RS232_RS232_OPERATION_H_ */
