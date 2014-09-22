
#ifndef __ADC_H
#define __ADC_H

//220V��ѹ��ֵ����
#define FREQ_VALVE_COUNT	35

#define RED_ALL_ON              (0x80)//ȫ��
#define YELLOW_ALL_ON           (0x40)//����    
#define POWER3G_ERROR         	(0X20)//3G����
#define LEARN_FAIL              (0x10)//ѧϰ���ʧ��
#define PASSAGE_EXCEPTION       (0x08)//ͨ���쳣   
#define POWER_ERROR             (0x04)//���������
#define CHARGE_ERROR            (0x02)//��ŵ����   
#define MACHINE_START           (0x01)//����

#define PASSAGE_COLLISION       (0x18) //ͨ���쳣-ͨ����ͻ
#define PHASE_COLLISION       	(0x28) //ͨ���쳣-��λ��ͻ
#define LAMP_BROKEN             (0x38) //ͨ���쳣-�ƹ���
#define LAMP_PHY_BROKEN         (0x48) //ͨ���쳣-���������


// // // #define AD_Channel0()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0000))
// // // #define AD_Channel1()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0010))
// // // #define AD_Channel2()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0020))
// // // #define AD_Channel3()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0030))
// // // #define AD_Channel4()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0040))
// // // #define AD_Channel5()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0050))
// // // #define AD_Channel6()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0060))
// // // #define AD_Channel7()    (GPIO_Write(GPIOE,GPIO_ReadInputData(GPIOE)&0xFF8F|0x0070))



#define HARDWARE_BOARD_A()  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0))
#define HARDWARE_BOARD_B()  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1))

// // // #define AD_CS1_ON()      (GPIO_SetBits(GPIOE, GPIO_Pin_0))    
// // // #define AD_CS1_OFF()     (GPIO_ResetBits(GPIOE, GPIO_Pin_0))    

// // // #define AD_CS2_ON()      (GPIO_SetBits(GPIOE, GPIO_Pin_1))    
// // // #define AD_CS2_OFF()     (GPIO_ResetBits(GPIOE, GPIO_Pin_1))    

// // // #define AD_CS3_ON()      (GPIO_SetBits(GPIOE, GPIO_Pin_2))    
// // // #define AD_CS3_OFF()     (GPIO_ResetBits(GPIOE, GPIO_Pin_2))    

// // // #define AD_CS4_ON()      (GPIO_SetBits(GPIOE, GPIO_Pin_3))    
// // // #define AD_CS4_OFF()     (GPIO_ResetBits(GPIOE, GPIO_Pin_3))  




#define AD_Channel0() {	\
												GPIOE->BRR =  GPIO_Pin_4; \
												GPIOE->BRR =  GPIO_Pin_5; \
												GPIOE->BRR =  GPIO_Pin_6; \
											}
#define AD_Channel1() {	\
												GPIOE->BSRR =  GPIO_Pin_4; \
												GPIOE->BRR 	=  GPIO_Pin_5; \
												GPIOE->BRR 	=  GPIO_Pin_6; \
											}
#define AD_Channel2() {	\
												GPIOE->BRR 	=  GPIO_Pin_4; \
												GPIOE->BSRR =  GPIO_Pin_5; \
												GPIOE->BRR 	=  GPIO_Pin_6; \
											}
#define AD_Channel3() {	\
												GPIOE->BSRR =  GPIO_Pin_4; \
												GPIOE->BSRR =  GPIO_Pin_5; \
												GPIOE->BRR 	=  GPIO_Pin_6; \
											}
#define AD_Channel4() {	\
												GPIOE->BRR 	=  GPIO_Pin_4; \
												GPIOE->BRR 	=  GPIO_Pin_5; \
												GPIOE->BSRR =  GPIO_Pin_6; \
											}
#define AD_Channel5() {	\
												GPIOE->BSRR =  GPIO_Pin_4; \
												GPIOE->BRR 	=  GPIO_Pin_5; \
												GPIOE->BSRR =  GPIO_Pin_6; \
											}
#define AD_Channel6() {	\
												GPIOE->BRR 	=  GPIO_Pin_4; \
												GPIOE->BSRR =  GPIO_Pin_5; \
												GPIOE->BSRR =  GPIO_Pin_6; \
											}
#define AD_Channel7() {	\
												GPIOE->BSRR =  GPIO_Pin_4; \
												GPIOE->BSRR =  GPIO_Pin_5; \
												GPIOE->BSRR =  GPIO_Pin_6; \
											}




#define AD_CS_ALL_ON(){	\
												GPIOE->BSRR =  GPIO_Pin_0; \
												GPIOE->BSRR =  GPIO_Pin_1; \
												GPIOE->BSRR =  GPIO_Pin_2; \
												GPIOE->BSRR =  GPIO_Pin_3; \
											}
#define AD_CS_ALL_OFF(){	\
												GPIOE->BRR =  GPIO_Pin_0; \
												GPIOE->BRR =  GPIO_Pin_1; \
												GPIOE->BRR =  GPIO_Pin_2; \
												GPIOE->BRR =  GPIO_Pin_3; \
											}
#define AD_CS1_ON()    (GPIOE->BSRR =  GPIO_Pin_0)    
#define AD_CS1_OFF()   (GPIOE->BRR 	=  GPIO_Pin_0)  
#define AD_CS2_ON()    (GPIOE->BSRR =  GPIO_Pin_1)    
#define AD_CS2_OFF()   (GPIOE->BRR 	=  GPIO_Pin_1)  
#define AD_CS3_ON()    (GPIOE->BSRR =  GPIO_Pin_2)    
#define AD_CS3_OFF()   (GPIOE->BRR 	=  GPIO_Pin_2)  
#define AD_CS4_ON()    (GPIOE->BSRR =  GPIO_Pin_3)    
#define AD_CS4_OFF()   (GPIOE->BRR 	=  GPIO_Pin_3)  


#define AD_LEARN()       (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7)) 

#define MACHINE_ON()     (GPIO_SetBits(GPIOB, GPIO_Pin_9)) 
#define MACHINE_OFF()    (GPIO_ResetBits(GPIOB, GPIO_Pin_9)) 

#define CHARGE_ON()      (GPIO_SetBits(GPIOD, GPIO_Pin_4))                  //��طŵ�
#define CHARGE_OFF()     (GPIO_ResetBits(GPIOD, GPIO_Pin_4)) 

#define CHARGE_OVER()    (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13))        //����طŵ��Ƿ����






//���ֵ����ֵ
#define ABS(x, y)        ((x)>(y)?(x-y):(y-x))


void  ADC_Calculate(u32 *status_temp, u8 index, u32 voltage_level[]);
void  HexToChar(u8 hex, u8 chr[]);
void  DecToChar(u16 dec, u8 chr[]);
void  Serial_Communcation(u8 type, u32 status, u8 time);
void  Error_Communcation(u8 error_type, u32 error, u32 error_time, u8 index_status);
void  ADC_PhyBroken_Level(u8 index, u32 voltage[]);
void  Error_Recovery_Detect(u32 status_temp, u8 index);
void  ADC_Hardware_Detect(void);
void  Error_LampPhyLevel_Detect(u32 status_temp);
void RegularError_Communcation(void);

////POWER-3G//////
u8 Power3G_Detect(void);
////POWER-3G//////



u32 GlideFilterAD(u8 Channel_i , u32 Ad_val);
u32 Filter(u32 New_Phase);


__task void LampStates_Collect_task(void);//400msִ��һ��
__task void Calculate_Task(void);
__task void  Learn_Task(void);


#endif
