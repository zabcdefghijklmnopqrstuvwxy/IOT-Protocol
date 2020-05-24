/**
********************************************************************************
* @file    dht11.h
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dht11.h
* @note    ��ʪ������ͷ�ļ� 
* @attention
********************************************************************************
*/

#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f1xx_hal.h"


/**
 * @brief ��ʪ�����ݼ������
 * @param[in] argument �������
 * @note ʵʱ�ɼ���ʪ������
 * @retval ��
 */
void DHT11_MonitorTask(void const * argument);


#endif

