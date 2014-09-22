/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "include.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

extern OS_TID t_ADC_task;                        /* assigned task id of task: ADC_task */
/*	4��ADC_INPUT,һ��������ÿ��INPUTת��TRANSFORM_TIMES�Ρ�	*/
extern __IO uint16_t 	ADC_groups_data_once[4*TRANSFORM_TIMES];

extern __IO uint16_t  ADC_buffer[4*4];     						 //DMA����

void DMA1_Channel1_IRQHandler(void)	   
{
	static uint32_t count	=0;
	uint32_t 	i	=0;
	
	if(DMA_GetITStatus(DMA1_IT_TC1))			//ͨ��1 ��������ж�
	{
		DMA_ClearITPendingBit(DMA1_IT_GL1); //���ȫ���жϱ�־
		
		for(i=0;i<16;i++)
		{
			ADC_groups_data_once[count+i]=ADC_buffer[i];
		}
		count+=16;

		if(count < ((TRANSFORM_TIMES<<2)-1)) //4��INPUT��ÿ��INPUTת������ΪTRANSFORM_TIMES
		{
			DMAReConfig();	
		}
		else 
		{
			count=0;
			isr_evt_set (0x0100, t_ADC_task);
		}

	}
}


/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
    //ѧϰ��ť
    if (EXTI_GetITStatus(EXTI_Line7))
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
		//����
		if (EXTI_GetITStatus(EXTI_Line9))
    {
       EXTI_ClearITPendingBit(EXTI_Line9);
			 I2C_Clear_Alarm_Register();
			 if(test_batton_flag == 0)
			 {
				 Batton_Break_flag = 1;//�����
				 printf("�����\n");
			 }
			 printf("��ؼ��one-day\n");
			 CHARGE_OFF();//�ŵ����
			 test_batton_flag = 0;			
    }
    EXTI_ClearITPendingBit(EXTI9_5_IRQn);
}
/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
//     u8 data = 0;
    
    if (USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);

        if ((USART_ControlBlk.rx_count < USART_RX_BUFSIZE) )//&& (Flag_Communication != 1)
        {
// //             data = USART2->DR;                   //��ֹ���뾯��                           
// //             USART_ControlBlk.usart_rx[USART_ControlBlk.rx_count++] = data;
// //         
					
					/* Read one Frame from the  USART2  receive   */
						usart2_rx_buff[usart2_rx_counter] =USART_ReceiveData(USART2);	
						usart2_rx_counter++;
			
            if (USART_ControlBlk.rx_count == 1)
            {
                TIM_Cmd(TIM5, ENABLE);
            }

            TIM5->CNT = 0;        
        }
    }
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern os_mbx_declare(err_report_mbox,20);						//�ϱ�����
void TIM2_IRQHandler(void)
{
    static u32 timetemp=0;
		u8 i = 0;
	
		static U32 hour_count = 0;
		u8 *msg;
		static u8 kk = 0;
	
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)) 
    {
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			
			hour_count++;
			if(hour_count >= (2*2*60*60))
			{
				hour_count = 0;
				msg = &kk;
				isr_mbx_send(err_report_mbox,(void*) msg);			
			}
			timetemp++;
			if(timetemp == 100)
			{
				timetemp=0;
			}
			//ѧϰ״̬����˸
			if ((timetemp % 2) == 0)
			{
				if (start_learning_flag == 1)
				{
					GPIO_PinReverse(GPIOD, GPIO_Pin_3);
				}
			}

			if(ledlamp_err_count>0)
			{
				ledlamp_err_count--;
				GPIO_PinReverse(GPIOA, GPIO_Pin_15);
			}
			else
			{
				LED_COM_ON();
			}
			
			if(eth_connect_count>0)//��̫��ͨѶ��LED8��NET�ƣ���˸
			{
				eth_connect_count--;
				GPIO_PinReverse(GPIOE, GPIO_Pin_11);
			}
			else 
			{
				LED_NET_OFF();
			}
			
			if(eth_restart_count>0)//��̫���ж�ͨѶ���Сʱ��������̫��
			{
				eth_restart_count--;
			}
			else 
			{
				net_state_flag = 1;
			}
			
   }
    TIM_ClearITPendingBit(TIM2, TIM2_IRQn);
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	static u16 tim_count=0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)) 
	{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		if(tim_count<1000)
		{
			tim_count++;
			if(tim_count%2==0)//20ms
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_15);
			}
			else if(tim_count%3 ==0)
			{
				GPIO_ResetBits(GPIOB, GPIO_Pin_15);
			}
		}
		else
		{
			TIM_Cmd(TIM3, DISABLE);/*�رն�ʱ��ι��*/
		}
 }
	TIM_ClearITPendingBit(TIM3, TIM3_IRQn);
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern os_mbx_declare(Communication_Mbox,20);						//��̫������ͨѶ����
void TIM5_IRQHandler(void)
{
		u8 *msg;
		static u8 kk = 2;
	
    if (TIM_GetITStatus(TIM5, TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update); 
        TIM_Cmd(TIM5, DISABLE);                     //�ض�ʱ��

        TIM5->CNT = 0;
			
        Flag_UsartProtocol = 1;
        Flag_Communication = 1;
			
				msg = &kk;
				isr_mbx_send(Communication_Mbox,(void*) msg);
    }
}


/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
