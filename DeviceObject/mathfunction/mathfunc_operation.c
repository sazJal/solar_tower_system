/*
 * mathfunc_operation.c
 *
 *  Created on: Feb 14, 2020
 *      Author: lenovo
 */

#include "mathfunc_operation.h"

static bool      crc_tab16_init = false;
static uint16_t  crc_tab16[256];

uint16_t MF_CRC_16(const unsigned char *input_str, unsigned char num_bytes)
{
	uint16_t crc;
	uint16_t tmp;
	uint16_t short_c;
	const unsigned char *ptr;
	unsigned char a;

	if (!crc_tab16_init) MF_Init_CRC16_Tab();

	crc = CRC_START_16;
	ptr = input_str;

	if ( ptr != NULL ) for (a=0; a<num_bytes; a++) {

		short_c = 0x00ff & (uint16_t) *ptr;
		tmp     =  crc       ^ short_c;
		crc     = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];

		ptr++;
	}

	return crc;

}  /* crc_16 */

void MF_Init_CRC16_Tab(void)
{

	uint16_t i;
	uint16_t j;
	uint16_t crc;
	uint16_t c;

	for (i=0; i<256; i++) {

		crc = 0;
		c   = i;

		for (j=0; j<8; j++) {

			if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC_POLY_16;
			else                      crc =   crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = true;

}  /* init_crc16_tab */

void MF_MovingAverage(PDU_SENSOR_AVG_t *sensorAvg, PDU_SENSOR_RAW_t *sensorRaw)
{
	static PDU_SENSOR_AVG_t sensorAvgBuff;
	static uint8_t count = 0;
	sensorAvgBuff.sumAvgBTPT = sensorAvgBuff.sumAvgBTPT + sensorRaw->rawBTPT;
	sensorAvgBuff.sumAvgPVPT = sensorAvgBuff.sumAvgPVPT + sensorRaw->rawPVPT;
	sensorAvgBuff.sumAvgHVPT = sensorAvgBuff.sumAvgHVPT + sensorRaw->rawHVPT;
	sensorAvgBuff.sumAvgDCCT = sensorAvgBuff.sumAvgDCCT + sensorRaw->rawDCCT;

	if(count == 31)
	{
		sensorAvg->sumAvgPVPT = sensorAvgBuff.sumAvgPVPT >> 5;
		sensorAvg->sumAvgHVPT = sensorAvgBuff.sumAvgHVPT >> 5;
		sensorAvg->sumAvgBTPT = sensorAvgBuff.sumAvgBTPT >> 5;
		sensorAvg->sumAvgDCCT = sensorAvgBuff.sumAvgDCCT >> 5;
		sensorAvgBuff.sumAvgBTPT = 0;
		sensorAvgBuff.sumAvgPVPT = 0;
		sensorAvgBuff.sumAvgHVPT = 0;
		sensorAvgBuff.sumAvgDCCT = 0;

		count = 0;
	}
	else
	{
		count++;
	}
}


