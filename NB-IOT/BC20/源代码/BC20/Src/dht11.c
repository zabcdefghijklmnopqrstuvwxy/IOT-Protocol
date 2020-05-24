/**
********************************************************************************
* @file    dht11.c
* @author  Sai
* @version 01V01
* @date    2020-05-24
* @brief   dht11.c
* @note    ��ʪ�����ݴ���Դ�ļ�
* @attention
********************************************************************************
*/

#include "dht11.h"
#include "main.h"
#include "cmsis_os.h"
#include "dwt_delay.h"


#define		DHT11_DQ_SET 		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET)     /**< IO��ƽ���� */
#define		DHT11_DQ_RST 		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET)   /**< IO��ƽ���� */
#define		DHT11_DQ_READ		HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)					  /**< IO��ƽ��ȡ */


void DHT11_DelayUs(uint32_t us)
{		
	HAL_RCC_GetHCLKFreq();	 
}


/**
 * @brief ��ʪ�ȴ�����IO���ó��������
 * @param None
 * @note 
 * @retval None
 */
void DHT11_IOOut(void)	
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_DeInit(DHT11_GPIO_Port,DHT11_Pin);	
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief ��ʪ�����ݼ������
 * @param[in] argument �������
 * @note ʵʱ�ɼ���ʪ������
 * @retval ��
 */
void DHT11_IOIn(void)	
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_DeInit(DHT11_GPIO_Port,DHT11_Pin);	
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief ��ʪ�Ȳɼ����ݿ�ʼ
 * @param None
 * @note ��������һ��20~40us�ĵ�����
 * @retval ��
 */
void DHT11_Rst(void)	   
{                 
	DHT11_IOOut(); 		//SET OUTPUT
    DHT11_DQ_RST; 		//����DQ
    osDelay(20);    	//��������18ms
    DHT11_DQ_SET; 		//DQ=1 
	DWT_DelayUs(30);    //��������20~40us
}

/**
 * @brief Ӧ����
 * @param None
 * @note �����ȴ��ӻ���Ӧ��
 * @retval ����1δ��⵽DHT11�Ĵ��ڣ�����0��ʾ��⵽DHT11�Ĵ���
 */
uint8_t DHT11_Check(void) 	   
{   
	uint8_t ucRetry=0;
	DHT11_IOIn();//SET INPUT	 
    while (DHT11_DQ_READ&&ucRetry<100)  //DHT11������40~80us
	{
		ucRetry++;
		DWT_DelayUs(1);
	}	
	
	if(ucRetry>=100)
	{
		return 1;
	}
	else 
	{
		ucRetry=0;
	}
	
    while (!DHT11_DQ_READ&&ucRetry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		ucRetry++;
		DWT_DelayUs(1);
	}
	
	if(ucRetry>=100)
	{
		return 1;	    
	}
	
	return 0;
}

/**
 * @brief ��ȡһλ����
 * @param None
 * @note None
 * @retval ����1δ��⵽DHT11�Ĵ��ڣ�����0��ʾ��⵽DHT11�Ĵ���
 */
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t ucRetry=0;
	while(DHT11_DQ_READ&&ucRetry<100)//�ȴ���Ϊ�͵�ƽ
	{
		ucRetry++;
		DWT_DelayUs(1);
	}
	
	ucRetry=0;
	while(!DHT11_DQ_READ&&ucRetry<100)//�ȴ���ߵ�ƽ
	{
		ucRetry++;
		DWT_DelayUs(1);
	}
	DWT_DelayUs(40);//�ȴ�40us
	if(DHT11_DQ_READ)
	{
		return 1;
	}
	else 
	{
		return 0;
	}	
}

/**
 * @brief ��DHT11�ж�ȡһ���ֽڵ�����
 * @param None
 * @note None
 * @retval ���ض�ȡ������
 */
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,ucData;
    ucData=0;
	for (i=0;i<8;i++) 
	{
   		ucData<<=1; 
	    ucData|=DHT11_Read_Bit();
    }						    
    return ucData;
}

/**
 * @brief ��DHT11����ʪ������
 * @param[in] temp�¶�����ָ�룬humiʪ������ָ��
 * @note None
 * @retval ����0��ʾ����������1��ʾ��ȡʧ��
 */
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else
	{
		return 1;
	}
	
	return 0;	    
}

/**
 * @brief DHT11��ʼ��
 * @param None
 * @note ���DHT11�豸�Ƿ����
 * @retval ����0��ʾ����������1��ʾ������
 */   	 
uint8_t DHT11_Init(void)
{   
	DHT11_Rst();
	return DHT11_Check();
}

/**
 * @brief ��ʪ�����ݼ������
 * @param[in] argument �������
 * @note ʵʱ�ɼ���ʪ������
 * @retval ��
 */
void DHT11_MonitorTask(void const * argument)
{
	DHT11_Init();
	uint8_t ucTemp = 0;
	uint8_t ucHumi = 0;
	while(1)
	{
		DHT11_Read_Data(&ucTemp,&ucHumi);
		osDelay(10);
	}
}
