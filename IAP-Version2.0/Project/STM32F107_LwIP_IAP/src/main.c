/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    07/16/2010 
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32_eth.h"
#include "ethernetif.h"
#include "netconf.h"
#include "main.h"
#include "httpserver.h" 
#include "tftpserver.h"
#include "flash_if.h"
#include "stm32f10x_bkp.h"
#include "common.h"

/* Private typedef -----------------------------------------------------------*/
//typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
pFunction Jump_To_Application;
uint32_t JumpAddress;


u8 IAP_NET_FLAG = 0;//以太网下程序标志
u8 IAP_UART_FLAG = 0;//串口下程序标志
u32 COUNT = 0;//进入滴答时钟次数标志


/* Private function prototypes -----------------------------------------------*/
void System_Periodic_Handle(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 

#include "flash.h"

//串口IAP状态灯
#define LED_IAP_USART_ON()  (GPIO_ResetBits(GPIOA, GPIO_Pin_15)) 
#define LED_IAP_USART_OFF() (GPIO_SetBits(GPIOA, GPIO_Pin_15))

//以太网IAP状态灯
#define LED_IAP_NET_OFF() (GPIO_SetBits(GPIOE, GPIO_Pin_11))

#define SysTick_Counter_Disable        ((u32)0xFFFFFFFE)
#define SysTick_Counter_Enable         ((u32)0x00000001)
#define SysTick_Counter_Clear          ((u32)0x00000000)
void SysTick_CounterCmd(u32 SysTick_Counter)
{
  if (SysTick_Counter == SysTick_Counter_Enable)
  {
    SysTick->CTRL |= SysTick_Counter_Enable;
  }
  else if (SysTick_Counter == SysTick_Counter_Disable) 
  {
    
    SysTick->CTRL &= SysTick_Counter_Disable;
  }
  else /* SysTick_Counter == SysTick_Counter_Clear */
  {
    SysTick->VAL = SysTick_Counter_Clear;
  }    
}
int main(void)
{
	u16 tmp;

	FLASH_Unlock();

	System_Setup();
        LED_IAP_NET_OFF();                                  //LED FLAG
        LED_IAP_USART_OFF();
        
	//BKP_WriteBackupRegister(BKP_DR11,1);
	tmp = BKP_ReadBackupRegister(BKP_DR11);


	SerialPutString("\r\nIAP START! \n");

  if(tmp == 0x00)//跳转到应用程序
  {     
   	SysTick_CounterCmd(SysTick_Counter_Disable);        //disable systick interrupt
        LED_IAP_NET_OFF();                                  //LED FLAG
        LED_IAP_USART_OFF();
  
  	SerialPutString("\r\nENTER IN APPLICATION MODE! \n");
    /* Check if valid stack address (RAM address) then jump to user application */
    if (((*(__IO uint32_t*)USER_FLASH_FIRST_PAGE_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
      GPIO_PinReverse(GPIOB, GPIO_Pin_15);
      Jump_To_Application();
    }
    else
    {
      while(1);
    }
  }
	
  /* enter in IAP mode */
  else
  {
        
	SerialPutString("\r\nENTER IN IAP MODE! \n");
        
        if(PORTSELECT == 1)
  	{
	    LED_IAP_USART_ON();
	    IAP_NET_FLAG = 1;
            SerialPutString("\r\nENTER IN IAP-LWIP MODE!\n");
        
        Flash_Init();
        Flash_ReadEthernetPara();
	    LwIP_Init();
	    
#ifdef USE_IAP_HTTP
	    IAP_httpd_init();
#endif
	  
#ifdef USE_IAP_TFTP    
	    IAP_tftpd_init();
#endif    

	    while (1)
	    {
	      if (ETH_GetRxPktSize()!=0)
	      { 
	        if(ETH_GetRxPktSize()==1514)
					{
					   //STM_EVAL_LEDInit(LED3);
					}
					/* process received eth packet */
	        LwIP_Pkt_Handle();
	      }
	      /* Periodic tasks */
	      System_Periodic_Handle();
	    }
  	}
		
        else if(PORTSELECT == 0)
	{
                LED_IAP_USART_ON();
		IAP_UART_FLAG = 1;
		SerialPutString("\r\nENTER IN IAP-UART MODE!\n");
			
		Main_Menu();
	}

  }
  return 0;
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  Handles the periodic tasks of the system
  * @param  None
  * @retval None
  */
void System_Periodic_Handle(void)
{
#ifdef USE_LCD 
  
  /* Update the LCD display and the LEDs status */
  /* Manage the IP address setting */
  Display_Periodic_Handle(LocalTime);
  
#endif
  
  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(LocalTime);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
