#include "include.h"
#include "key.h"
uint8_t Scan_Key(void)
{	
	static uint8_t key_state = 0,key_value=NO_KEY;
	static uint16_t reset_all_count = 0;
	static uint16_t delay_study_count = 0;
	
	uint8_t key_return = NO_KEY;

	switch (key_state)
	{
		case 0:
			if(GET_MAN_KEY())
			{
				key_value= MAN_KEY; 
				key_state++;
				break;  		
			}
			else if(GET_STUDY_KEY())
			{
				key_value= STUDY_KEY; 
				key_state++;
				break;  		
			}
			else break;
	
		case 1:
			 if (GET_MAN_KEY()&&(key_value==MAN_KEY))	     	// 再次读电平
				{
					//key_return = MAN_KEY;
					key_state++;
					break;
				}
			 else if (GET_STUDY_KEY()&&(key_value==STUDY_KEY))		// 再次读电平
				{
					//key_return = STUDY_KEY;
					key_state++;
					break;
				}
			 else 	
			 {
				  key_state--;		   // 两次电平不同,返回状态0，（消抖处理）							
					key_value =NO_KEY;	
					break;	
			 }
			 break;	
		case 2:									             
				if (key_value	==	MAN_KEY)		// 再再次读电平
				{
					if(GET_MAN_KEY())
					{
							if(reset_all_count >= RESET_ALL_TIME)
							{
								key_return =RESET_ALL_KEY;		
								key_state++;
							}	
							else	reset_all_count	++;	
					}	
					else
					{		
							key_return =MAN_KEY;
							reset_all_count=0;	
							key_value=NO_KEY;							 
							key_state=0;
					}		
					break;		
				}				
				else if (key_value==STUDY_KEY)		
				{
					if(GET_STUDY_KEY())
					{
							if(delay_study_count >= DELAY_STUDY_TIME)
							{
								key_return =DELAY_STUDY_KEY;		
								key_state++;
							}	
							else	delay_study_count	++;	
					}	
					else
					{		
							key_return =STUDY_KEY;
							delay_study_count=0;	
							key_value=NO_KEY;							 
							key_state=0;
					}		
				}
				break;
			case 3:	
				if(key_value	==	RESET_ALL_KEY)
				{				
					if(GET_MAN_KEY()!=1) 
						{						 			
								reset_all_count=0;	
								key_value=NO_KEY;							 
								key_state=0;
						}					 
				}	
				else if(key_value	==	DELAY_STUDY_KEY)
				{					
					if(GET_STUDY_KEY()!=1) 
						{
								delay_study_count=0;	
								key_value=NO_KEY;									 
								key_state=0;
						}	
				}						 
				break;
			default:	break;			
	}
	return key_return;
}

void Init_key_port(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
		
	/* Enable KEY_MAN_PORT,KEY_STUDY_PORT    Pclocks */	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_KEY_MAN | RCC_APB2Periph_KEY_STUDY, ENABLE);
	
   	/* Set KEY_MAN_GPIO_PIN  as input function push pull */	
	GPIO_InitStructure.GPIO_Pin = KEY_MAN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	      
	GPIO_Init(KEY_MAN_PORT, &GPIO_InitStructure);

   	/* Set KEY_STUDY_GPIO_Pin  as input function push pull */	
	GPIO_InitStructure.GPIO_Pin = KEY_STUDY_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			 
	GPIO_Init(KEY_STUDY_PORT, &GPIO_InitStructure);
}
