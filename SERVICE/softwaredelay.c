/*
 * softwaredelay.c
 *
 * Created: 1/17/2020 6:36:31 PM
 *  Author: MENA
 */ 
#include "softwareDelay.h"



void softwareDelayMs(uint32_t u32_delay_in_ms)
{
	uint32_t volatile x ;
	for (uint32_t count = 0 ; count <u32_delay_in_ms ; count ++)
	{
		for (uint32_t count2 = 0 ; count2 <100 ; count2 ++)
		{
			x++ ;
		}
	}
}
