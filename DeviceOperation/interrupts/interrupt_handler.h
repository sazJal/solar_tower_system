/*
 * interrupt_handler.h
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#ifndef DEVICEOPERATION_INTERRUPTS_INTERRUPT_HANDLER_H_
#define DEVICEOPERATION_INTERRUPTS_INTERRUPT_HANDLER_H_

#define rs232_intrpt_handler 		IRQ25_Handler
#define one_sec_intrpt_handler 		IRQ6_Handler
#define adc_intrpt_handler  		IRQ19_Handler
#define pcsCAN_intrpt_handler  		IRQ7_Handler
#define reset_intrpt_handler  		IRQ11_Handler
#define one_msec_intrpt_handler		IRQ23_Handler
#define hundred_msec_intrpt_handler IRQ24_Handler
#define genixrs485_intrpt_handler 	IRQ12_Handler
#define esp32uart_intrpt_handler 	IRQ18_Handler


#endif /* DEVICEOPERATION_INTERRUPTS_INTERRUPT_HANDLER_H_ */
