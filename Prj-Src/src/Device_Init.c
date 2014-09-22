#include "include.h"


DMA_InitTypeDef     DMA_InitStructure;	
//======================================================================
// �������ƣ�
// ADC1_configuration(void)
//			 
// ˵����ADC1����
//======================================================================
static void ADC1_configuration(void)
{
  GPIO_InitTypeDef     GPIO_InitStructure;
  ADC_InitTypeDef      ADC_InitStructure;
  NVIC_InitTypeDef     NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  | RCC_APB2Periph_GPIOB |
												 RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
												 
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE);	
	
	//4��ģ���·ѡ����Ƭѡ
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 |
																	GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	//8��ͨ������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 |
																	GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
																		
	
	//ADC_port_configuration
	/* Configure PC.0 (ADC ADC_Channel_10) as ADC_INPUT1 -------------------------*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AIN;     
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PC.4 (ADC ADC_Channel_14) as ADC_INPUT2 -------------------------*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AIN;     
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PB.0 (ADC ADC_Channel_8) as ADC_INPUT3 -------------------------*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AIN;     
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure PB.1 (ADC ADC_Channel_9) as ADC_INPUT4 -------------------------*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AIN;     
	GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* DMA channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr 			= (u32)(&(ADC1->DR));
  DMA_InitStructure.DMA_MemoryBaseAddr 					= (u32)(&ADC_buffer);
  DMA_InitStructure.DMA_DIR 										= DMA_DIR_PeripheralSRC;		
  DMA_InitStructure.DMA_BufferSize 							= 16;					
  DMA_InitStructure.DMA_PeripheralInc 					= DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc 							= DMA_MemoryInc_Enable; 
  DMA_InitStructure.DMA_PeripheralDataSize 			= DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize 					= DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode 										= DMA_Mode_Normal;	 
  //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	  
  DMA_InitStructure.DMA_Priority 								= DMA_Priority_High;
  DMA_InitStructure.DMA_M2M 										= DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //DMA ͨ��1 ��������ж�
// //   /* Enable DMA1 channel1 */
// //   DMA_Cmd(DMA1_Channel1, ENABLE);
   


  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode 								= ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode 				= ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign 					= ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel 				= 16;   //  4��ͨ����ÿ��ͨ��ת��4��
  ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular ADC_Channel_10(ADC_INPUT1) configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 4,  ADC1_SampleTime_cicr);	
	
	/* ADC1 regular ADC_Channel_14(ADC_INPUT2) configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 6,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 8,  ADC1_SampleTime_cicr);

	/* ADC1 regular ADC_Channel_8 (ADC_INPUT3) configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  9,  ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  10, ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  11, ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8,  12, ADC1_SampleTime_cicr);
	
	/* ADC1 regular ADC_Channel_9 (ADC_INPUT4) configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  13, ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  14, ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  15, ADC1_SampleTime_cicr);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9,  16, ADC1_SampleTime_cicr);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
// //   /* Start ADC1 Software Conversion */ 
// //   ADC_SoftwareStartConvCmd(ADC1, ENABLE);		

	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*======================================================================
// �������ƣ�
// ADC1_configuration(void)
//			 
// ˵����ADC1����
//======================================================================*/
static void ADC2_configuration(void)
{
		ADC_InitTypeDef adc_init;
		GPIO_InitTypeDef GPIO_init;  
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC2 |
                   RCC_APB2Periph_AFIO, ENABLE);                
	
		//������ADͨ��     
		GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_init.GPIO_Pin   = GPIO_Pin_4;
		GPIO_init.GPIO_Mode  = GPIO_Mode_AIN;     
		GPIO_Init(GPIOA, &GPIO_init); 
    
    ADC_DeInit(ADC2);

    adc_init.ADC_Mode = ADC_Mode_Independent;                  //��ADC����
    adc_init.ADC_NbrOfChannel = 1;                             //ͨ������
    adc_init.ADC_DataAlign = ADC_DataAlign_Right;              //���ݶ���
    adc_init.ADC_ScanConvMode = ENABLE;                        //ɨ��ģʽ
    adc_init.ADC_ContinuousConvMode = DISABLE;                 //������ʽ
    adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //���������ʽ����(swstart��)

    ADC_Init(ADC2, &adc_init);
    
    ADC_ExternalTrigConvCmd(ADC2, ENABLE);                     //�ⲿ����    
    
    //ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);                  //�ж��趨

    //ͨ���趨
    ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5); //INPUT1

    ADC_Cmd(ADC2, ENABLE);                                      //����
    
    ADC_ResetCalibration(ADC2);                                 //��λ
    while (ADC_GetResetCalibrationStatus(ADC2) == SET);
    
    ADC_StartCalibration(ADC2);                                 //У׼
    while (ADC_GetCalibrationStatus(ADC2) == SET);

    //DMAʹ��
    //ADC_DMACmd(ADC2, ENABLE);

    //�����������
    //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*******************************************************************************
* Function Name  : DMAReConfig
* Description    : ��������DMA,
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DMAReConfig(void)
{
	DMA_DeInit(DMA1_Channel1);
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);

  ADC_Cmd(ADC1, ENABLE);
}
/*******************************************************************************
* Function Name  : USART_configuration
* Description    : ����2���ã�����λ��ͨѶ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void USART2_configuration()
{
		USART_InitTypeDef usart2_init;
		GPIO_InitTypeDef 	GPIO_init;    
		NVIC_InitTypeDef  nvic_init;  
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,  ENABLE);                
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
		//USART2TX2
		GPIO_init.GPIO_Pin = GPIO_Pin_5;
		GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_init.GPIO_Mode = GPIO_Mode_AF_PP;     
		GPIO_Init(GPIOD, &GPIO_init);
		
		//USART2RX2
		GPIO_init.GPIO_Pin = GPIO_Pin_6;
		GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;     
		GPIO_Init(GPIOD, &GPIO_init);  
		GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
	
		/* Configure the NVIC Preemption Priority Bits */  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		//USART2�ж�
    nvic_init.NVIC_IRQChannel = USART2_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_init); 
	
    //��ʼ��
    USART_DeInit(USART2);

    usart2_init.USART_BaudRate = 115200;                       	//������
    usart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;     //���ա�����
    usart2_init.USART_WordLength = USART_WordLength_8b;         //����λ
    usart2_init.USART_StopBits = USART_StopBits_1;              //ֹͣλ
    usart2_init.USART_Parity = USART_Parity_No;                 //żУ��λ
    usart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ������
     
    USART_Init(USART2, &usart2_init); 

    USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);             //�ж�����    
	
    USART_Cmd(USART2, ENABLE);                                  //������
    
    USART_ClearFlag(USART2, USART_FLAG_TC);                     //�������λTCĬ��Ϊ1  

}
void EXTI_configuration()
{
		//��ʼ����������
	NVIC_InitTypeDef nvic_init;  
	//�ⲿ�жϱ�������   
	EXTI_InitTypeDef exti_init;  
	
	GPIO_InitTypeDef GPIO_init;       
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	//��ͨ��ѧϰ��ť
	GPIO_init.GPIO_Pin   = GPIO_Pin_7;
	GPIO_init.GPIO_Mode  = GPIO_Mode_IPU;     
	GPIO_Init(GPIOD, &GPIO_init);  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE ,ENABLE);       
	

// 	//ѡ��������λ�ã����ڱ������ڶ�����Լ��������ļ��У��Ա��л����ģʽ
// 	#ifdef  VECT_TAB_RAM        
// 			 NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
// 	#else        
// 		 // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x9000);
// 		 NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
// 	#endif       

	//ѧϰ��ť
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource7);
	exti_init.EXTI_Line = EXTI_Line7;    
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
	exti_init.EXTI_LineCmd = ENABLE;  
	exti_init = exti_init;
	EXTI_Init(&exti_init);   
	
	 //���ȼ�ѡ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

//    //EXTI�ж�
//     nvic_init.NVIC_IRQChannel = EXTI15_10_IRQChannel;
//     nvic_init.NVIC_IRQChannelCmd = ENABLE;
//     nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
//     nvic_init.NVIC_IRQChannelSubPriority = 2;
//     NVIC_Init(&nvic_init); 

   //EXTI�ж�
    nvic_init.NVIC_IRQChannel = EXTI9_5_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_init);     
}

void power_detect_configuration()
{
	GPIO_InitTypeDef GPIO_init;   
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//������ADͨ��
	GPIO_init.GPIO_Pin   = GPIO_Pin_4;
	GPIO_init.GPIO_Mode  = GPIO_Mode_AIN;     
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_init);     
	
	//======================================================================
	//220v��ԴƵ��
	GPIO_init.GPIO_Pin   = GPIO_Pin_9;
	GPIO_init.GPIO_Mode  = GPIO_Mode_IPU;     
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_init);    
	
	//////POWER-3G////////    
	/*  IO-OUT(CPU2_RESET) pin configuration------------------------------------------------- */
	/* Configure IO-OUT(CPU2_RESET) (PB14) as input */
	GPIO_init.GPIO_Pin = GPIO_Pin_14;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_init.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_init);  
	
}

/*======================================================================
// �������ƣ�
// TIM1_configuration
//			 
// ˵�����ж�����
//======================================================================*/
void TIM1_configuration(u16 count)
{    
    TIM_TimeBaseInitTypeDef time_init;

    //��ʼ��
    TIM_DeInit(TIM1);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   
	
    time_init.TIM_CounterMode = TIM_CounterMode_Up;         //��������
    time_init.TIM_Period = count-1;                         //��������,�����ؼ�������0��ʼ
    time_init.TIM_Prescaler = 0;                            //Ԥ��Ƶ,����ʱPSC=N+1
    time_init.TIM_ClockDivision = TIM_CKD_DIV1;             //��������

    TIM_TimeBaseInit(TIM1, &time_init);

    TIM_TIxExternalClockConfig(TIM1, 
                               TIM_TIxExternalCLK1Source_TI2, 
                               TIM_ICPolarity_Rising, 
                               0x00);
    
    //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    
    //ֱ�Ӹ��¼Ĵ���
    TIM_ARRPreloadConfig(TIM1, DISABLE);

    //�ж��趨
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    //�ض�ʱ��
    TIM_Cmd(TIM1, DISABLE);
}

/*======================================================================
// �������ƣ�
// TIM2_configuration
//			 
// ˵������ʱ���ã�0.5��ı���
//======================================================================*/
void TIM2_configuration(u8 haf_second)
{
    TIM_TimeBaseInitTypeDef time_init;
		//��ʼ����������
		NVIC_InitTypeDef nvic_init;  
	  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
		/* Configure the NVIC Preemption Priority Bits */  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		//TIM2�ж�
    nvic_init.NVIC_IRQChannel = TIM2_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_init);

    //��ʼ��
    TIM_DeInit(TIM2);

    time_init.TIM_CounterMode = TIM_CounterMode_Up;         //��������
    time_init.TIM_Period = haf_second * 1000;               //��������
    time_init.TIM_Prescaler = 35999;                        //Ԥ��Ƶ,����ʱPSC=N+1
    time_init.TIM_ClockDivision = TIM_CKD_DIV4;             //��������

    TIM_TimeBaseInit(TIM2, &time_init);
    
    //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    
    //ֱ�Ӹ��¼Ĵ���
    TIM_ARRPreloadConfig(TIM2, DISABLE);

    //�ж��趨
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    

    //�ض�ʱ��
    TIM_Cmd(TIM2, DISABLE);
}
/*======================================================================
// �������ƣ�
// TIM3_configuration
//			 
// ˵������ʱ���ã�10ms��ı���
//======================================================================*/
void TIM3_configuration(u8 haf_second)
{
    TIM_TimeBaseInitTypeDef time_init;
		//��ʼ����������
		NVIC_InitTypeDef nvic_init;  
	  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
		/* Configure the NVIC Preemption Priority Bits */  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
		//TIM3�ж�
    nvic_init.NVIC_IRQChannel = TIM3_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_init);

    //��ʼ��
    TIM_DeInit(TIM3);

    time_init.TIM_CounterMode = TIM_CounterMode_Up;         //��������
    time_init.TIM_Period = haf_second * 20;               //��������
    time_init.TIM_Prescaler = 35999;                        //Ԥ��Ƶ,����ʱPSC=N+1
    time_init.TIM_ClockDivision = TIM_CKD_DIV4;             //��������

    TIM_TimeBaseInit(TIM3, &time_init);
    
    //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    
    //ֱ�Ӹ��¼Ĵ���
    TIM_ARRPreloadConfig(TIM3, DISABLE);

    //�ж��趨
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);    

    //�ض�ʱ��
    TIM_Cmd(TIM3, DISABLE);
}
//======================================================================
// �������ƣ�
// TIM5_configuration
//			 
// ˵����USART���ڽ��ս�����ʱ�趨10ms
//======================================================================
void TIM5_configuration(u8 ms)
{
    TIM_TimeBaseInitTypeDef time_init;
		NVIC_InitTypeDef nvic_init; 
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
		/* Configure the NVIC Preemption Priority Bits */  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

    //TIM5�ж�
    nvic_init.NVIC_IRQChannel = TIM5_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_init);  
	
    //��ʼ��
    TIM_DeInit(TIM5);

    time_init.TIM_CounterMode = TIM_CounterMode_Up;         //��������
    time_init.TIM_Period = ms * 20;                         //��������
    time_init.TIM_Prescaler = 35999;                        //Ԥ��Ƶ,����ʱPSC=N+1
    time_init.TIM_ClockDivision = TIM_CKD_DIV4;             //��������

    TIM_TimeBaseInit(TIM5, &time_init);
    
    //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    
    //ֱ�Ӹ��¼Ĵ���
    TIM_ARRPreloadConfig(TIM5, DISABLE);

    //�ж��趨
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);    

    //�ض�ʱ��
    TIM_Cmd(TIM5, DISABLE);
}
void I2C_configuration()
{
	GPIO_InitTypeDef GPIO_init;       
	//��ʼ����������
	NVIC_InitTypeDef nvic_init;  
	//�ⲿ�жϱ�������   
	EXTI_InitTypeDef exti_init;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//I2C
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_init.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_OD;     
	GPIO_Init(GPIOB, &GPIO_init);      
	
	//ALARM INTERUPT
	/***********************EXTI**************************/	
	GPIO_init.GPIO_Pin = GPIO_Pin_9;
	GPIO_init.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_init);
			
  //EXTI�ж�
	nvic_init.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic_init);    
	
	//NAOZHONG
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	exti_init.EXTI_Line = EXTI_Line9;    
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;  
	exti_init = exti_init;
	EXTI_Init(&exti_init);   
 	
}
//======================================================================
// �������ƣ�
// SPI1_configuration(void)
//			 
// ˵����SPI1����
//======================================================================
static void SPI1_configuration(void)
{
	
	GPIO_InitTypeDef    GPIO_InitStructure;
	SPI_InitTypeDef 		spi1_init;
	
	

  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
  /*!< Configure  : SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure  : MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure : MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /*!< Configure  :   CS pin */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	

	SPI_I2S_DeInit(SPI1);

	spi1_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //˫��ȫ˫�� 
	spi1_init.SPI_Mode = SPI_Mode_Master;                        //��ģʽ
	spi1_init.SPI_NSS = SPI_NSS_Soft;                            //Ӳ������
	spi1_init.SPI_FirstBit = SPI_FirstBit_MSB;                   //��λ����
	spi1_init.SPI_DataSize = SPI_DataSize_8b;                    //�ֽڷ���
	spi1_init.SPI_CPOL = SPI_CPOL_Low;                           //�͵�ƽ����
	spi1_init.SPI_CPHA = SPI_CPHA_1Edge;                         //��һ���ز���
	spi1_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 		//Ԥ��Ƶ
	spi1_init.SPI_CRCPolynomial = 7;                             //����ʽ����

	SPI_Init(SPI1, &spi1_init);                                  //��ʼ��

	SPI_SSOutputCmd(SPI1, ENABLE);                               //NSS�������ã�����I/O�� 

	GPIO_SetBits(GPIOA, GPIO_Pin_11);         

	SPI_Cmd(SPI1, ENABLE);                                       //ʹ��

// //   /*!< SPI configuration */
// //   spi1_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
// //   spi1_init.SPI_Mode = SPI_Mode_Master;
// //   spi1_init.SPI_DataSize = SPI_DataSize_8b;
// //   spi1_init.SPI_CPOL = SPI_CPOL_High;
// //   spi1_init.SPI_CPHA = SPI_CPHA_2Edge;
// //   spi1_init.SPI_NSS = SPI_NSS_Soft;
// // #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
// //   spi1_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
// // #else
// //   spi1_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
// // #endif

// //   spi1_init.SPI_FirstBit = SPI_FirstBit_MSB;
// //   spi1_init.SPI_CRCPolynomial = 7;
// //   SPI_Init(SPI1, &spi1_init);

// //   /*!< Enable the sFLASH_SPI  */
// //   SPI_Cmd(SPI1, ENABLE);
}
//======================================================================
// �������ƣ�
// WatchDog_init
//			 
// ˵�����������Ź���ʼ��
//======================================================================

void WatchDog_init(void)
{
#define WATCH_DOG
#ifdef WATCH_DOG	
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64);           //25us*64=1.6ms
    IWDG_SetReload(0x9C4);                          //4000/1.6=0xEA6
//     IWDG_SetReload(1000);                          //1.6s 1600/1.6=1000--lxb_debug
    IWDG_ReloadCounter();
    IWDG_Enable();
#endif    
}

//***********************************************************************
// �������ƣ�
// void Delay_ms(u32 time)
//			 
//˵����ms����ʱ
//***********************************************************************
void Delay_ms(u32 time)
{
    os_dly_wait(time * 10);
}


//======================================================================
//                           GLOBAL DATA INIT
//======================================================================
void Global_Data_Init()
{
	/*  ����EEPROM�����е����� */
// 	read_param(START_CHARGE_TIME,	(uint8_t *)&Charge_Para.Start_Time,sizeof(Charge_Para.Start_Time));
	
// 	EEP_erase_ALL();  
// 	save_default_parameter();
	
// 	save_param(START_CHARGE_TIME,	(uint8_t *)&Charge_Para.Start_Time,sizeof(Charge_Para.Start_Time));
	
	read_all_parameter();
	
	if (turn_num_lxb > 10000)
	{
			turn_num_lxb = 1650;
	}
	if (Alarm_Charge > 600)
	{
			Alarm_Charge = 20;
	}
	if (Alarm_LampmA > 200)
	{
			Alarm_LampmA = 30;
	}
	if ((LampmA_ReportTime > 300)||(LampmA_ReportTime < 30))
	{
			LampmA_ReportTime = 30;
	}    
	
}
void part_GPIO_configuration()
{
	GPIO_InitTypeDef GPIO_init;       
	GPIO_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
  
	//======================================================================
	//LED�ӿ���ʾ
	//LED1   ѧϰ+�������״̬��ʾ
	GPIO_init.GPIO_Pin   = GPIO_Pin_3;
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOD, &GPIO_init);     

	//LED7   USART_IAP״̬��ʾ
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_init.GPIO_Pin   = GPIO_Pin_15;
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOA, &GPIO_init);  

	//LED8   INTERNET_IAP״̬��ʾ
	GPIO_init.GPIO_Pin   = GPIO_Pin_11;
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOE, &GPIO_init);  
	//======================================================================
	//�˷Ű���
	GPIO_init.GPIO_Pin   = GPIO_Pin_0 |
	               GPIO_Pin_1;
	GPIO_init.GPIO_Mode  = GPIO_Mode_IPD;     
	GPIO_Init(GPIOD, &GPIO_init);     
	
	//======================================================================
	//�źŻ���λ
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_PP;    
	GPIO_init.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOB, &GPIO_init);  
	//======================================================================
	//���Ӹ�λ
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_PP;    
	GPIO_init.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOB, &GPIO_init);
	//======================================================================
	//��طŵ���ƣ�Ĭ�ϲ��ŵ�
	GPIO_init.GPIO_Mode  = GPIO_Mode_Out_PP;    
	GPIO_init.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOD, &GPIO_init);   

	GPIO_init.GPIO_Mode  = GPIO_Mode_IPU;    
	GPIO_init.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init(GPIOD, &GPIO_init);  
}

void BKP_configuration()
{
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);	
}

//======================================================================
// �������ƣ�
// Device_init(void)
//			 
// ˵����ϵͳ���á���ʼ��
//======================================================================
void Device_init(void)
{  
	part_GPIO_configuration();
	
	/*�տ����ɶ�ʱ��ι��*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	TIM3_configuration(1);      	//10ms�ж�
	TIM_Cmd(TIM3, ENABLE);
	
	BKP_configuration();
	ADC1_configuration();
	ADC2_configuration();
	
	//EXTI_configuration();

	USART2_configuration();
	
	TIM1_configuration(10000);  	//������Ƶ��
	TIM2_configuration(1);      	//0.5���ж�
	TIM5_configuration(5);      //USART���ս���
	
	power_detect_configuration();
		
	I2C_configuration();
	
	SPI1_configuration();
	
	Init_key_port();
	
	Global_Data_Init();
	WatchDog_init();
// 	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	LED_NET_OFF();//NET����
}




