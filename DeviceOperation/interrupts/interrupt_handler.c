/*
 * interrupt_handler.c
 *
 *  Created on: Oct 28, 2019
 *      Author: lenovo
 */

#include "global_var.h"
#include "interrupt_handler.h"

void rs232_intrpt_handler(void)
{
    uint8_t cmd;
    cmd = XMC_UART_CH_GetReceivedData(XMC_USIC0_CH1);
    XMC_UART_CH_Transmit(XMC_USIC0_CH1, cmd);

    /* if it is not if checking peripheral, this command can't be accessed */
//    if(!isPeriphMode){cmd = ')';}

    switch(cmd)
    {
    	case 'A' : GEENIX_Sel_Mode(GEENIX_SEL_HIGH);
    			   GEENIX_AskData(0x02);
    			   GEENIX_Sel_Mode(GEENIX_SEL_LOW);
    			   XMC_GPIO_ToggleOutput(P2_11);
    			   break;
    	case 'a' : break;
    	case 'B' : break;
    	case 'C' : break;
    	case 'c' : break;
    	case 'D' : break;
    	case 'd' : break;
    	case 'E' : break;
    	case 'e' : PDU_Switch_Relay(PDU_RELAY_KBAT, PDU_RELAY_OPEN);
    			   break;
    	case 'F' : PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_CLOSE);
    			   break;
    	case 'f' : PDU_Switch_Relay(PDU_RELAY_KMP, PDU_RELAY_OPEN);
    			   break;
    	case 'G' : PDU_Switch_Relay(PDU_RELAY_KMF, PDU_RELAY_CLOSE);
    			   break;
    	case 'g' : PDU_Switch_Relay(PDU_RELAY_KMF, PDU_RELAY_OPEN);
    			   break;
    	case 'H' : PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_CLOSE);
    			   break;
    	case 'h' : PDU_Switch_Relay(PDU_RELAY_KDSG, PDU_RELAY_OPEN);
    			   break;
    	case 'I' : break;
    	case 'J' : genix_send = true;
    			   break;
    	case 'M' : break;
    	case 'm' : break;
    	default  : break;
    }

	XMC_UART_CH_ClearStatusFlag(XMC_USIC0_CH1,XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION);
}

void one_sec_intrpt_handler(void)
{
	rtcDate	= INFO_RTCGetDate();
	rtcTime = INFO_RTCGetTime();
	esp32DataPack.year  = rtcDate.year + 2000;
	esp32DataPack.month = rtcDate.month;
	esp32DataPack.date  = rtcDate.date;

	esp32DataPack.hour  	= rtcTime.hour;
	esp32DataPack.minutes	= rtcTime.minute;
	esp32DataPack.seconds  = rtcTime.second;

	XMC_ERU_ETL_ClearStatusFlag(ERU1_ETL3);
}

void adc_intrpt_handler(void)
{
	XMC_VADC_GROUP_ClearResultEvent(VADC_G1,2); // Clear Result 2 event flag
	pduSensorRaw.rawPVPT = (XMC_VADC_GROUP_GetResult(VADC_G1,0));
	pduSensorRaw.rawBTPT = (XMC_VADC_GROUP_GetResult(VADC_G1,1));
	pduSensorRaw.rawDCCT = (XMC_VADC_GROUP_GetResult(VADC_G1,2));
	pduSensorRaw.rawHVPT = (XMC_VADC_GROUP_GetResult(VADC_G1,3));

	/* Do moving Average Filter */
	MF_MovingAverage(&pduSensorAvg, &pduSensorRaw);
}

void pcsCAN_intrpt_handler(void)
{
	// takes 30 us
	XMC_CAN_MO_Receive(&msgObjFromPCS);

	switch(msgObjFromPCS.can_identifier)
	{
		case 0x200 : // PCS System Properties
			pcsSystemStatus = PCS_GetPCSStatus(&msgObjFromPCS);

			btData.bt_pcs_version 	= 0;
			temporaryStatus			= pcsSystemStatus.pcs_system_status;
			temporaryStatus			= (temporaryStatus << 2) | pcsSystemStatus.pcs_system_warning;
			temporaryStatus			= (temporaryStatus << 3) | pcsSystemStatus.pcs_system_fault;
			btData.bt_pcs_status	= temporaryStatus;
			break;

		case 0x201 : // DC Grid Converter Properties
			pcsDCgridStatus = PCS_GetDCGridStatus(&msgObjFromPCS);

			temporaryStatus = pcsDCgridStatus.dcgrid_system_status;
			temporaryStatus = (temporaryStatus << 2) | pcsDCgridStatus.dcgrid_system_warning;
			temporaryStatus = (temporaryStatus << 3) | pcsDCgridStatus.dcgrid_system_fault;
			btData.bt_grid_status	= temporaryStatus;
			break;

		case 0x202 : // AC Inverter Properties
			pcsACinvStatus = PCS_GetACInvStatus(&msgObjFromPCS);

			temporaryStatus = pcsACinvStatus.acinv_system_status;
			temporaryStatus = (temporaryStatus << 2) | pcsACinvStatus.acinv_system_warning;
			temporaryStatus = (temporaryStatus << 3) | pcsACinvStatus.acinv_system_fault;
			btData.bt_inv_status	= temporaryStatus;
			break;

		case 0x203 : // PV Converter Properties
			pcsPVconvStatus = PCS_GetPVConvStatus(&msgObjFromPCS);

			temporaryStatus = pcsPVconvStatus.pvconv_system_status;
			temporaryStatus = (temporaryStatus << 2) | pcsPVconvStatus.pvconv_system_warning;
			temporaryStatus = (temporaryStatus << 3) | pcsPVconvStatus.pvconv_system_fault;
			btData.bt_pv_status	= temporaryStatus;
			break;

		case 0x204 : // Battery Converter Properties
			pcsBatconvStatus = PCS_GetBatConvStatus(&msgObjFromPCS);

			temporaryStatus = pcsBatconvStatus.batconv_system_status;
			temporaryStatus = (temporaryStatus << 2) | pcsBatconvStatus.batconv_system_warning;
			temporaryStatus = (temporaryStatus << 3) | pcsBatconvStatus.batconv_system_fault;
			btData.bt_batt_status	= temporaryStatus;
			break;

		case 0x205 : // Voltage Properties
			pcsVoltages = PCS_GetVoltParam(&msgObjFromPCS);

			btData.bt_voltage_batt	 = pcsVoltages.vbat;
			btData.bt_voltage_pv	 = pcsVoltages.vPV;
			btData.bt_voltage_grid	 = pcsVoltages.vdcGrid;
			btData.bt_voltage_inv	 = pcsVoltages.vacOut;
			break;

		case 0x206 : // Current Properties
			pcsCurrents = PCS_GetAmpParam(&msgObjFromPCS);

			btData.bt_current_batt	 = pcsCurrents.ibat;
			btData.bt_current_pv	 = pcsCurrents.iPV;
			btData.bt_current_grid	 = pcsCurrents.idcGrid;
			btData.bt_current_inv	 = pcsCurrents.iacOut;
			break;

		case 0x207 : // Power Properties
			pcsPowers = PCS_GetPowerParam(&msgObjFromPCS);

			btData.bt_power_batt	 = pcsPowers.pbat;
			btData.bt_power_pv		 = pcsPowers.pPV;
			btData.bt_power_grid	 = pcsPowers.pdcGrid;
			btData.bt_power_inv		 = pcsPowers.pacOut;
			break;

		case 0x208 : // Frequency and Temperature Properties
			pcsMiscs = PCS_GetMiscParam(&msgObjFromPCS);

			btData.bt_inv_frequency = pcsMiscs.frequency;
			btData.bt_pcs_temp		= pcsMiscs.tempPCS;
			break;

		default    : // others
			break;
	}

	flag_pcs = true;
	NVIC_ClearPendingIRQ(IRQ7_IRQn);
}

void reset_intrpt_handler(void)
{
	XMC_ERU_ETL_ClearStatusFlag(ERU0_ETL2);
	__NVIC_SystemReset();
}

void one_msec_intrpt_handler(void)
{
	static uint16_t counting[3];
	static uint8_t  countingADC;
	XMC_CCU4_SLICE_ClearEvent(CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

	/* PV Handshaking */
	if(pvHShake.isEnabled)
	{
		if(counting[0] == pvHShake.cycle-1)
		{
			pvHShake.isEnabled 		= false;
			pvHShake.isTimeOut 		= true;
			counting[0]				= 0;
		}
		else counting[0]++;
	}
	else {counting[0]=0;}

	/* ESP32 Handshaking */
	if(esp32HShake.isEnabled)
	{
		if(counting[1] == esp32HShake.cycle-1)
		{
			esp32HShake.isEnabled 	= false;
			esp32HShake.isTimeOut 	= true;
			counting[1]				= 0;
		}
		else counting[1]++;
	}
	else {counting[1]=0;}

	/* Button Handshaking */
	if(buttonHShake.isEnabled)
	{
		if(counting[2] == buttonHShake.cycle-1)
		{
			buttonHShake.isEnabled 	= false;
			buttonHShake.isTimeOut 	= true;
			counting[2]				= 0;
		}
		else counting[2]++;
	}
	else {counting[2]=0;}

	/* ADC is processed every 3 ms */
	if(countingADC<2)
	{
		countingADC++;
	}
	else
	{
		PDU_ADC_Start();
	}
}

void hundred_msec_intrpt_handler(void)
{
	static uint8_t cycle_100ms=0;
	static const uint8_t esp32_cycle   = 15;  // 1.5 s
	static const uint8_t rs232_cycle   = 25;  // 2.5 s
	static const uint8_t max_cycle     = 30;  // 3 s

	XMC_CCU4_SLICE_ClearEvent(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

	/* Update ADC Readings */
//	if(pduSensorAvg.isValid)
//	{
		/*	Convert to true value	*/
//		pduSensorAvgSend.sumAvgBTPT = pduSensorAvg.sumAvgBTPT;
//		pduSensorAvgSend.sumAvgPVPT = pduSensorAvg.sumAvgPVPT;
//		pduSensorAvgSend.sumAvgHVPT = pduSensorAvg.sumAvgHVPT;
//		pduSensorAvgSend.sumAvgDCCT = pduSensorAvg.sumAvgDCCT;

//		pduSensor.mPVPT = 0.0998*pduSensorRaw.rawPVPT - 0.856;
//		pduSensor.mBTPT = 0.0990*pduSensorRaw.rawBTPT - 0.869;
//		pduSensor.mHVPT = 0.0983*pduSensorRaw.rawHVPT - 0.842;
//		pduSensor.mDCCT = 0.0291*pduSensorRaw.rawDCCT - 59.824;

//		ocv = pduSensor.mBTPT + pduSensor.mDCCT*0.1;
//
//		/* piece-wise OCV-SoC transformation */
//
//		if 		((ocv > 191.97) && (ocv <= 212.51)){soc_f = 0.1352*ocv - 25.962;}
//		else if ((ocv > 212.51) && (ocv <= 247.67)){soc_f = 0.3006*ocv - 61.374;}
//		else if ((ocv > 247.67) && (ocv <= 258.33)){soc_f = 2.3441*ocv - 566.67;}
//		else if ((ocv > 258.33) && (ocv <= 264.25)){soc_f = 9.1982*ocv - 2338;}
//		else if ((ocv > 264.25) && (ocv <= 267.02)){soc_f = 1.0046*ocv - 168.25;}
//		else if (ocv  > 267.02){soc_f = 100.0;}
//		else if (ocv <= 191.97){soc_f = 0.0;}
//
//		bmsSensor.soc = (uint16_t)(soc_f*10);
//		bmsSensor.ibat = (pduSensor.mDCCT*10);
//		bmsSensor.vbat = (pduSensor.mBTPT*10);
//		bmsSensor.tempbat = 230;
//
//		esp32DataPack.pcs_soc = bmsSensor.soc;


	/* Are Communication Devices attached? */
	if(!recEspCommIsHalt) {cycle_100ms++;}

	/* send PCS status */
	PCS_BMSTransmitToPCS(0x100, &pcsBmsStatus, &bmsSensor);
	PCS_BMSTransmitToPCS(0x101, &pcsBmsStatus, &bmsSensor);

	/* Handshake Phase */
	if(isStartHShake)
	{
		XMC_UART_CH_Transmit(XMC_USIC0_CH0, ';');
		isStartHShake = false;
	}

	/* Activate Communication Order */
	if (cycle_100ms == esp32_cycle-1)
	{
		flag_esp32 = true;
	}
	if (cycle_100ms == rs232_cycle-1)
	{
		flag_rs232 = true;
	}
	if (cycle_100ms == max_cycle-1)
	{
		cycle_100ms = 0;
		if(controlFlag.isChecking_peripheral)
		{
//			sprintf(buffer, "1. raw sensor BTPT = %lu \r\n", pduSensorAvg.sumAvgBTPT);
//			RS232_SendString(XMC_USIC0_CH1, buffer, strlen(buffer));
//			sprintf(buffer, "2. raw sensor PVPT = %lu \r\n", pduSensorAvg.sumAvgPVPT);
//			RS232_SendString(XMC_USIC0_CH1, buffer, strlen(buffer));
//			sprintf(buffer, "3. raw sensor HVPT = %lu \r\n", pduSensorAvg.sumAvgHVPT);
//			RS232_SendString(XMC_USIC0_CH1, buffer, strlen(buffer));
//			sprintf(buffer, "4. raw sensor DCCT = %lu \r\n", pduSensorAvg.sumAvgDCCT);
//			RS232_SendString(XMC_USIC0_CH1, buffer, strlen(buffer));
		}
	}

	if(enable_bt && flag_esp32)
	{
		/* PCS Battery Parameter */
		btData.bt_bat_pack_soc 		= batPackData.pack_soc;
		btData.bt_bat_pack_volt		= batPackData.pack_voltage;
		btData.bt_bat_pack_max_temp	= batPackData.pack_max_temp;
		btData.bt_bat_pack_min_temp	= batPackData.pack_min_temp;
		btData.bt_bat_pack_status	= batPackData.pack_status;
		BT_prepareData(buffer, 0x05, 41, &btData, 0xAA);
		BT_sendStream(buffer, 46);
	}
}

void genixrs485_intrpt_handler(void)
{
	static GEENIX_RCV_STATE_t rcvState = HEADER_CHECK;
	static uint16_t head;
	static uint16_t tail;
	static uint8_t 	count, retry=0;
	static uint8_t 	length;
	static char 	buffer[32];
	uint8_t data;

	XMC_UART_CH_ClearStatusFlag(XMC_USIC1_CH0,
								XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION);

	data = XMC_UART_CH_GetReceivedData(XMC_USIC1_CH0);

	switch(rcvState)
	{
	case HEADER_CHECK	:
		rcvState = HEADER_CHECK;
		head = (head << 8) | data;
		if(head == 0xAABB)
		{
			rcvState = HEADER_RECEIVE;
			head	 = 0x0000;
			count	 = 0;
			retry	 = 0;
			/* let first a maximum length */
			length 	 = 40;
		}
		else
		{
			retry++;
			if(retry==2)
			{
				head  = 0x0000;
				retry = 0;
			}
		}

		break;
	case HEADER_RECEIVE	:
		rcvState = HEADER_RECEIVE;
		if(count==0)
		{
			length = data;
			count++;
		}
		else if(count == (length - 2) - 1) // count starts after header
		{
			buffer[count-1] = (char)data;
			rcvState = HEADER_PARSE;
			count++;
		}
		else
		{
			buffer[count-1] = (char)data;
			count++;
		}
		break;
	case HEADER_PARSE	:
		buffer[count-1] = (char)data;
		/* do a parsing */
		// PARSE //
		/* restart counter */
		count	 = 0;
		rcvState = HEADER_FINISH;
		break;
	case HEADER_FINISH	:
		rcvState = HEADER_FINISH;

		tail = (tail << 8) | data;
		if(tail == 0x0D0A)
		{
			rcvState = HEADER_CHECK;
			tail	 = 0x0000;
			RS232_SendString(XMC_USIC0_CH1, buffer, 30);
			XMC_UART_CH_Transmit(XMC_USIC0_CH1, 'F');
			XMC_UART_CH_Transmit(XMC_USIC0_CH1, 0x0D);
			XMC_UART_CH_Transmit(XMC_USIC0_CH1, 0x0A);
		}
		break;
	default				:break;
	}

//	XMC_UART_CH_Transmit(XMC_USIC0_CH1, data);
}

void esp32uart_intrpt_handler(void)
{
	static uint8_t n=0;
	static char NTPdata[200];

	NTPdata[n] = XMC_UART_CH_GetReceivedData(XMC_USIC0_CH0);

	if(NTPdata[n] =='#')
	{
		n=0;
	}
	else if(NTPdata[n] =='K')
	{
	  esp32HShake.isValid 	= true;
	  rtcDate.year  	= (NTPdata[0]-48)*1000 + (NTPdata[1]-48)*100 + (NTPdata[2]-48)*10 + (NTPdata[3]-48);
	  rtcDate.month 	= (NTPdata[4]-48)*10 + (NTPdata[5]-48);
	  rtcDate.date  	= (NTPdata[6]-48)*10 + (NTPdata[7]-48);
	  rtcTime.hour  	= (NTPdata[8]-48)*10 + (NTPdata[9]-48);
	  rtcTime.minute  	= (NTPdata[10]-48)*10 + (NTPdata[11]-48);
	  rtcTime.second	= (NTPdata[12]-48)*10 + (NTPdata[13]-48);
	  n=0;
	}
	else {n++;}
	XMC_UART_CH_ClearStatusFlag(XMC_USIC0_CH0,XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION);
}

void SysTick_Handler(void)
{
	if(sysTickHandle.enable)
	{
		sysTickHandle.count--;
		if(sysTickHandle.count == 0)
		{
			sysTickHandle.enable  = false;
			sysTickHandle.isTimeOut = true;
		}
	}
}
