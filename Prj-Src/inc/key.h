#ifndef __KEY_LM_H
#define __KEY_LM_H

/* ---------------------------------------- */
#define KEY_MAN_PORT              GPIOE	   
#define KEY_STUDY_PORT            GPIOD

#define KEY_MAN_GPIO_PIN           GPIO_Pin_8  
#define KEY_STUDY_GPIO_Pin         GPIO_Pin_7


#define RCC_APB2Periph_KEY_MAN 	  RCC_APB2Periph_GPIOE
#define RCC_APB2Periph_KEY_STUDY  RCC_APB2Periph_GPIOD

/* ---------------------------------------- */
#define GET_MAN_KEY()    				(!GPIO_ReadInputDataBit(KEY_MAN_PORT,KEY_MAN_GPIO_PIN))
#define GET_STUDY_KEY()   			(GPIO_ReadInputDataBit(KEY_STUDY_PORT,KEY_STUDY_GPIO_Pin))


#define NO_KEY 	       	0
#define MAN_KEY 	   		1
#define STUDY_KEY 	   	2
#define RESET_ALL_KEY 	3
#define DELAY_STUDY_KEY 	4

#define RESET_ALL_TIME 		(10000/10)   	//10s定时，10ms时基
#define DELAY_STUDY_TIME 	(5000/10)   	//5s定时， 10ms时基

uint8_t Scan_Key(void);
void Init_key_port(void);
#endif
