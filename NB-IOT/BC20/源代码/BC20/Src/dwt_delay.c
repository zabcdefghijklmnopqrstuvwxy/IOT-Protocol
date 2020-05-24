/**
********************************************************************************
* @file    dwt_delay.c
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dwt_delay.c
* @note    ʹ�ø��ټĴ���������ʱ����
* @attention
********************************************************************************
*/

#include "dwt_delay.h"

/**
* @brief ��ʼ��DWT��ʱ�Ӽ���
* @param None
* @note None
* @retval ����1��ʾʱ�Ӽ���û�п�ʼ������0��ʾ��������
*/
uint32_t DWT_InitDelay(void)
{
	/* Disable TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
	/* Enable TRC */
	CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

	/* Disable clock cycle counter */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
	/* Enable  clock cycle counter */
	DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

	/* Reset the clock cycle counter value */
	DWT->CYCCNT = 0;

	/* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");

	/* Check if clock cycle counter has started */
	if(DWT->CYCCNT)
	{
		return 0; /*clock cycle counter started*/
	}
	else
	{
		return 1; /*clock cycle counter not started*/
	}
}
