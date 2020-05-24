/*
********************************************************************************
* @file    dwt_delay.h
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dwt_delay.h
* @note    ���ټ�������ʱͷ�ļ� 
* @attention
********************************************************************************
*/

#ifndef	_DWT_DELAY_H_
#define	_DWT_DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif
 
#include "stm32f1xx_hal.h"
 
/**
* @brief ��ʼ��DWT��ʱ�Ӽ���
* @param None
* @note None
* @retval ����1��ʾʱ�Ӽ���û�п�ʼ������0��ʾ��������
*/
uint32_t DWT_InitDelay(void);
 
 /**
* @brief ΢�����ӳٴ���
* @param[in] microseconds�ӳٵ�΢����
* @note None
* @retval None
*/
__STATIC_INLINE void DWT_DelayUs(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
 
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}
 
#ifdef __cplusplus
}
#endif
 
#endif
