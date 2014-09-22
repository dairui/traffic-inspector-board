#include "include.h"
#include <stdio.h>

// /*	ad�ɼ����˴���0��С�ڸ�ֵ����0*/
// #define LAMP_CURRENT_MIN				50

// /*	���ι��˴���1�������˻Ƶ�*/
// #define WINDOW_RYG							(720/ADC_CYCLE_TIME)
// /*	���ι��˴���2�����˵��Ƶ�*/
// #define WINDOW_RG								(2400/ADC_CYCLE_TIME)

// /*  �������ڶ����˲�FIFO: ���ι��˴����ܳ��� */
// #define PASSAGE_FILTER_SIZE     (2400/ADC_CYCLE_TIME+10)   
/*  ���ι��˴���Fifo  */
vu32 passage_filter_fifo[PASSAGE_FILTER_SIZE] = {0};
/*  ���ι��˴���д����������ֵ  */
vu8 passage_filter_wr_index,passage_filter_rd_index,passage_filter_counter;

/*==============================================================================================
* �﷨��ʽ��    uint16_t ADC_filter_one(uint16_t *pData,uint8_t len)
* ʵ�ֹ��ܣ�    �ɼ���ĳ�ʼ����
* ����1��         
* ����2��         
* ����ֵ��       
===============================================================================================*/
#define MIN_NUM   4
#define MAX_NUM   4
uint16_t ADC_filter_one(uint16_t *pData,uint8_t len)
{
	uint16_t value_return=0;
	uint16_t temp=0;
	uint16_t sum=0;

	uint32_t i,j;
	
	for(i=0;i<len-1;i++) 
	{
		for(j=0;j<len-i-1;j++) 
		{
			if(*(pData+j) > *(pData+j+1))
			{
				temp	=*(pData+j);
				*(pData+j)=*(pData+j+1);
				*(pData+j+1)=temp;
			}
		}
	}
	
	for(i=MIN_NUM;i<(len-MAX_NUM);i++) 
	{
		 sum +=*(pData+i);
	}
	value_return =sum/(len-MIN_NUM-MAX_NUM);
	if(value_return<LAMP_CURRENT_MIN)
		value_return = 0;
	return (value_return);
}

/*==============================================================================================
* �﷨��ʽ��    u32 first_filtering_binaryzation(u16  filter_one_value[],u8 reset_learn)
* ʵ�ֹ��ܣ�    һ�ι��˶�ֵ�� 
* ����1��         u16  filter_one_value[]����һ�ι��˵õ��ĵ�ǰadֵ
* ����2��         
* ����ֵ��         first_all_passage����ֵ�����ͨ��״̬
===============================================================================================*/
u16	last_filter_ADC[LAMP_NUMBER]	=  
{
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN
};				//��һ�ε�adc����ֵ
u32 first_filtering_binaryzation(u16  filter_one_value_in[],u8 reset_checking)
{
	u32  	first_all_passage_in 		  = 0;						//��ֵ�����ͨ��״̬

// 	static	u16	last_filter_ADC[LAMP_NUMBER]	=  {LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN};				//��һ�ε�adc����ֵ

	u8 		i = 0;
		
	/*	��ǰ״̬д����ι��˴���FIFO	*/
	for(i=0;i<LAMP_NUMBER;i++)
	{		
		if(filter_one_value_in[i] > last_filter_ADC[i]*BINARYZATION_WEIGHT)
		{
			last_filter_ADC[i] = (last_filter_ADC[i] + filter_one_value_in[i])/2;
			first_all_passage_in |= (1<<i);
// 			cur_filter_ADC[i] = last_filter_ADC[i];
		}
// 		printf("%x ", last_filter_ADC[i] );
	}
// 	printf("\n ");
	return first_all_passage_in;
}


/*==============================================================================================
* �﷨��ʽ��    void second_Filtering(u32  first_all_passage)
* ʵ�ֹ��ܣ�    �����˲�
* ����1��          u32  first_all_passage����һ�ι��˵õ��ĵ�ǰͨ��״̬
* ����2��       
* ����ֵ��         ��ǰ����״̬ *cur_RG_passage,��ǰ�����״̬ *cur_RYG_passage��
							��Ч��(��)�̵�״̬,valid_RG_passage��valid_RYG_passage;
===============================================================================================*/
void second_Filtering(u32  first_all_passage,u32  *p_state)
{
	u8 		i = 0, j = 0;
	u8 		rd_index_temp			  = 0;						//�����ָ�����ʱ����
	u8		windows_count_temp = 0;						//������ĳ·ͨ��Ϊ1�ĸ���
	u32      cur_RG_passage		  = 0;						//��ǰ���̵�״̬
	u32 		 cur_RYG_passage	  = 0;						//��ǰ����̵�״̬
// 	u32 status;
	/*	��ǰ״̬д����ι��˴���FIFO	*/
	passage_filter_fifo[passage_filter_wr_index] = first_all_passage;
	passage_filter_rd_index		   =	passage_filter_wr_index;
	if (++passage_filter_wr_index == PASSAGE_FILTER_SIZE) 
		passage_filter_wr_index=0;
	
	rd_index_temp	= 	passage_filter_rd_index;			//�����ʼ��ָ��λ��		
	for(j=0;j < LAMP_NUMBER;j++)								//ͨ��ѭ���ж�
	{
		windows_count_temp = 0;									//ÿ��ͨ��ѭ��ǰ���������ֵ
		rd_index_temp	   =	passage_filter_rd_index;
		for(i = 0;i<WINDOW_RG;i++)			//���˴����ж�
		{
			
			if((passage_filter_fifo[rd_index_temp] & (u32)(1<<j)) == (u32)(1<<j))
			{
				windows_count_temp++;								//������״ֵ̬��ǰ·״̬Ϊ1���������һ
			}
			
			/*�ӵ�ǰ״̬��ǰ��ȡFIFO״ֵ̬*/
			if (rd_index_temp == 0)
			{
				rd_index_temp=PASSAGE_FILTER_SIZE-1;
			}
			else
			{
				rd_index_temp--;							//�ӵ�ǰ״̬��ǰ��ȡ״ֵ̬
			}
			if(i==WINDOW_RYG-1)										//���˴���1����С�������˻Ƶ�								
			{
				if(windows_count_temp == i+1)						//������1�¸�·ͨ����Ϊ1�����õ�ǰ״̬Ϊ1
				{
					cur_RYG_passage	|= (u32)(1<<j);
				}
			}
			if(i==WINDOW_RG-1)  										//���˴���2���ϴ󣬹��˵��Ƶ�		
			{
				if(windows_count_temp == i+1)						//������2�¸�·ͨ����Ϊ1�����õ�ǰ״̬Ϊ1
				{
					cur_RG_passage	|= (u32)(1<<j);
				}
			}
		}
	}
	/*	��ֵָ�룬���ڷ����ĸ�״ֵ̬	*/
	 *p_state 	   = cur_RG_passage;
	 *(++p_state) = cur_RYG_passage;
// // // printf("%s\n ", "11111111111111111111111111111111111111111111111111");
// // // 	for(j=0;j < WINDOW_RG;j++)								//ͨ��ѭ���ж�
// // // 	{	
// // // 		status = passage_filter_fifo[rd_index_temp];
// // // 		/*�ӵ�ǰ״̬��ǰ��ȡFIFO״ֵ̬*/
// // // 		printf("%d\n ", status);
// // // 			if (rd_index_temp == 0)
// // // 			{
// // // 				rd_index_temp=PASSAGE_FILTER_SIZE-1;
// // // 			}
// // // 			else
// // // 			{
// // // 				rd_index_temp--;							//�ӵ�ǰ״̬��ǰ��ȡ״ֵ̬
// // // 			}
// // // 		  for (i=0; i<12; i++)
// // // 			{        
// // // 				if ((status&0x01) == 0x01)
// // // 				{
// // // 					printf("%d", 1);
// // // 				}
// // // 				else
// // // 				{
// // // 					printf("%d", 0);
// // // 				} 

// // // 	//             if (((i-2)%3) == 0)
// // // 	//             {
// // // 	//                 printf("%d	 ", 1);
// // // 	//             }
// // // 				status >>= 1;
// // // 			}
// // // 		printf("\n");			
// // // 		}
}

/*==============================================================================================
* �﷨��ʽ��   u32* passage_RG_learn(u32 valid_RG_passage,u32 passage_get_RG,u8 passage_start)
* ʵ�ֹ��ܣ�    ����졢�̵ƣ��õ����״̬����̵�״̬��
* ����1��       valid_RG_passage����Ч���̵�ͨ��
* ����2��       passage_get_RGΪc0000000����A0000000�����ں��̻ƻ��ߺ�����ж�
* ����3��       passage_start��һ�������ʼλ��
* ����ֵ��      ��Ч���ͨ��passage_red,��Ч�̵�ͨ�� passage_green
===============================================================================================*/
void passage_RG_learn(u32 valid_RG_passage,u32 passage_get_RG,u8 passage_start,u32	*p_state)
{
	u8		i;
	u32		passage_human 	= 0;						//����ͨ��
	u32 		passage_red 			= 0;						//��Ч���ͨ��
	u32		passage_green 	= 0;						//��Ч�̵�ͨ��
	
	for (i=passage_start;i<LAMP_NUMBER-2;i=i+3)													//�����û�������Ч�졢�̵�ͨ��
	{
		/*����ǰ��Ч����ͨ��״̬�ӵ�һ����ƿ�ʼΪ110����101�������Ч�졢�̸�ֵ */
		if((valid_RG_passage & (passage_get_RG>>i)) == (passage_get_RG>>i))		
		{
			passage_red 		|= (0x80000000>>i);
			passage_green |= (~passage_red & (passage_get_RG>>i));
		}
	}
	
	/*�������ͨ�� */
	passage_human = valid_RG_passage & (~(passage_red | passage_green));
	
	/*�������ͨ���ĺ졢�̵�״̬ */
	if(passage_human > 0)
	{
		for(i=0;i<LAMP_NUMBER-2;i++)
		{
			if((passage_human & (0x80000000>>i)) == (0x80000000>>i))
			{
				passage_red 		|= (0x80000000>>i);
				passage_green	|= (0x80000000>>(i+1));				
			}
		}
	}
	/*	��ֵָ�룬���ڷ����ĸ�״ֵ̬	*/
	 *p_state 	   = passage_red;
	 *(p_state+1) = passage_green;
}

/*==============================================================================================
* �﷨��ʽ��    u32* passage_RYG_learn(u32 valid_RG_passage,u32 valid_RYG_passage)
* ʵ�ֹ��ܣ�    ����졢�ơ��̵Ƶ���Чͨ��״̬
* ����1��         ��Ч��(��)�̵�״̬,valid_RG_passage��valid_RYG_passage;
* ����2��       
* ����ֵ��      �������Ч���ͨ��passage_red,��Ч�̵�ͨ�� passage_green����Ч�Ƶ�ͨ��passage_yellow
===============================================================================================*/
void passage_RYG_learn(u32 valid_RG_passage,u32 valid_RYG_passage,u32	*p_state)
{
	u8		i;
	u8		passage_start		=		0;					//��һ�������ʼλ��
	u32		passage_get_RG	=		0;					//Ϊc0000000����A0000000�����ں��̻ƻ��ߺ�����ж�
	
	u32 		passage_red 			= 		0;					//����õ��ĺ��״̬
	u32 		passage_yellow 	= 		0;					//����õ��ĻƵ�״̬
	u32 		passage_green 	= 		0;					//����õ����̵�״̬
	
	passage_yellow	 =	(~valid_RG_passage) & valid_RYG_passage;						 //���ͨ���Ƶ�
	for(i=0;i<LAMP_NUMBER;i++)																					 //��õ�һ�������ʼλ��
	{
		if((valid_RG_passage & (0x80000000>>i)) == (0x80000000>>i) )
		{
			passage_start = i;
			break;
		}
	}
	if ( ((valid_RG_passage & ( 0x80000000 >>(passage_start+1))) ==0x80000000 >>(passage_start+1))	
	&& ((valid_RG_passage & ( 0x80000000 >>(passage_start+2))) ==0)) 			 //˳�򣺺��̻�
	{
		passage_get_RG	=	0xC0000000;
		passage_RG_learn(valid_RG_passage,passage_get_RG,passage_start,p_state); 	 //���ͨ������
		passage_red     = *p_state;
		passage_green = *(p_state+1);
	}
	
	else if (((valid_RG_passage & 0x80000000 >>(passage_start+2)) ==0x80000000 >>(passage_start+2))
		  && (valid_RG_passage & 0x80000000 >>(passage_start+1)) ==0)  	 	 //˳�򣺺����
	{
		passage_get_RG	=	0xA0000000;
		passage_RG_learn(valid_RG_passage,passage_get_RG,passage_start,p_state); 	 //���ͨ������
		passage_red     = *p_state;
		passage_green = *(p_state+1);
	}	
	/*	��ֵָ�룬���ڷ����ĸ�״ֵ̬	*/
	 *p_state 	   = passage_red;
	 *(++p_state) = passage_yellow;
	 *(++p_state) = passage_green;
	
}

/*==============================================================================================
* �﷨��ʽ��    void get_passage_table(u32 passage_red,u32 passage_yellow,u32 passage_green)
* ʵ�ֹ��ܣ�    �ɷ���ĺ��̻���Чͨ���õ�����λ��Чͨ����passage_table[12]	
* ����1��       	u32 passage_red,u32 passage_yellow,u32 passage_green
* ����2��       
* ����ֵ��      	�õ�passage_table[12]
===============================================================================================*/
void get_passage_table(u32 passage_red,u32 passage_yellow,u32 passage_green,u32 passage_table[])
{
	u8		i;
	u8		j	=	0;
// 	u8		k	=	0;
// 	u8		l	=	0;
	u32 	flag_lampuse_temp=0;
	
	for(i=0;i<LAMP_NUMBER;i++)
	{
		if((passage_red & (u32)(1<<i)) ==  (u32)(1<<i) )					//��Ʒ�����Чͨ������
		{
			passage_table[j]	|=		(u32)(1<<i);
			flag_lampuse_temp			|=		(u32)(1<<i);
			j++;
		}
		if((passage_yellow & (u32)(1<<i)) ==  (u32)(1<<i) )			//�ƵƷ�����Чͨ������
		{
			passage_table[j]	|=		(u32)(1<<i);
			flag_lampuse_temp			|=		(u32)(1<<i);
			//k++;
		}
		if((passage_green & (u32)(1<<i)) ==  (u32)(1<<i) )			//�̵Ʒ�����Чͨ������
		{
			passage_table[j]	|=		(u32)(1<<i);
			flag_lampuse_temp			|=		(u32)(1<<i);
			//l++;
		}
	}
	Flag_LampUse = flag_lampuse_temp;
}

/*==============================================================================================
* �﷨��ʽ��    u8 get_phase_clash_table(u32 cur_passage_green)
* ʵ�ֹ��ܣ�    ѧϰ��õ���λ��ͻ��passage_clash_table[CLASH_STATE_NUMBER]
* ����1��       	cur_passage_green��ǰ�̵�״̬
* ����2��       	reset_learn����������ѧϰ���û���
* ����ֵ��      	learning_flag=1��ʾ���ѧϰ��learning_flag=0δ���ѧϰ
===============================================================================================*/
u8 get_phase_clash_table(u32 cur_passage_green,u32 *phase_clash_table,u8 reset_learning)
{
	u8		i,j;
	u8		learning_flag	=	0;										//ѧϰ��־���ɹ�Ϊ1
	static 	u32		last_passage_green	=	0;		    	//��һ�ε��̵�״̬
	static 	u8		stage_number			=	0;				//��Ч�׶���
	static 	u8		checkout_count			=	0;				//ѧϰУ�����
	/*У��ʱ����ǰ�̵�״̬����λ��ͻ���в�ͬ�Ĵ�����
		�����̵�ֵ����λ��ͻ���е�����״̬����ͬ����ѧϰʧ�ܣ�����ѧϰ*/
	u8		differ_clashTb_count			=	0;			
	
	/*	����ѧϰʱ���þ�̬����	*/
	if(reset_learning == _TRUE)
	{
		last_passage_green=0;	
		stage_number=0;	
		checkout_count=0;	
	}
	
	if(	  (cur_passage_green > 0) 
	&& (last_passage_green != cur_passage_green)	)									//���̵Ʒ��н׶�,���Һ�ǰһ�β���ͬ
	{
		last_passage_green = cur_passage_green;  						   				 //���浱ǰ״̬		
		for(i = 0;i<PHASE_CLASH_STATE_NUMBER;i++)
		{
			if((*(phase_clash_table+i)== 0) && (stage_number <= i))					//�µ�״̬������λ��ͻ��
			{
				*(phase_clash_table+i)= (~cur_passage_green); 							//���浱ǰ�׶���λ��ͻ
				stage_number++;																				//����׶���
				return 0;
			}
			else if ((*(phase_clash_table+i)== (~cur_passage_green)) && (stage_number > i))
			{																												//����У��׶�
				if(stage_number == i+1)																	//һ��У����ɣ�����++
				{
					checkout_count++;
				}
				
				if (checkout_count == 2)																	//У�����γɹ�����ѧϰ���
				{
					learning_flag = 1;
					return learning_flag;
				}
				else																										//�������Σ�����У��
				{
					return 0;
				}
			}
			else if((*(phase_clash_table+i) != (~cur_passage_green)) 
				&&(*(phase_clash_table+i)   != 0) && (stage_number > i))
			{
				differ_clashTb_count++;
				if(differ_clashTb_count == stage_number && checkout_count>0)
				{//У��׶Σ�����ǰ�׶���λ�����б������λ��ͬ����ѧϰʧ�ܣ�����ѧϰ
					stage_number = 0;
					checkout_count = 0;
					for(j=0;j<PHASE_CLASH_STATE_NUMBER;j++)
					{
						*(phase_clash_table+j) = 0;
					}
					printf("*************8����ѧϰ*********");
					return 2;
				}
				else
				{//��У��׶Σ���Ϊ��һ��ѧϰ�׶ε�д����λ��ͻ��continue
					continue;	
				}
			}
			else
			{
			}
		}
	}
	return 0;
}
/*==============================================================================================
* �﷨��ʽ��    u32 green_phase_clash_check(u32 cur_passage_green)
* ʵ�ֹ��ܣ�    ��λ��ͻ��⣬����λ��ͻ��passage_clash_table[]��һ������Ϊ0�����޳�ͻ�������ͻ
* ����1��       	cur_passage_green��ǰ�̵�״̬
* ����2��       
* ����ֵ��      	��ͻ�򷵻�cur_passage_green�����򷵻�0
===============================================================================================*/
u32 green_phase_clash_check(u32 cur_passage_green,u32 *phase_clash_table)
{
	u8		i;
	
	if(cur_passage_green >=1)  											//��ǰ״̬���̵Ʒ�����λ
	{
		for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)    //���12����λ������豸ͨ��=��λ
		{		
			if(*(phase_clash_table+i) == 0)									//���Ѿ��ж�����Ч��λ��ͻ��
			{
				break;
			}
			else 
			{
				if( (*(phase_clash_table+i) & cur_passage_green)==0) 
				{
					return 0;																	//û�г�ͻ����0
				}
				else
				{
					continue;
				}
			}
		}
		return cur_passage_green;											//�г�ͻ���س��ر�
	}
	return 0;																	//û�г�ͻ����0
}
/*==============================================================================================
* �﷨��ʽ��    u32 passage_clash_check(u32 cur_RYG_passage)
* ʵ�ֹ��ܣ�    ͨ����ͻ���
* ����1��       	cur_RYG_passage��ǰ��״̬
* ����2��       
* ����ֵ��      	��ͻ�򷵻�passage_clash�����򷵻�0
===============================================================================================*/
u32 passage_clash_check(u32 cur_RYG_passage,u32 *passage_table)
{
	u8		i;
	u8		j;
	u32		passage_state_temp = 0;			//����ͨ��״̬��ʱ����
	u8		count_temp			   	 = 0;
	u32		passage_clash			 = 0;			//ͨ����ͻ��
	
	for(i=0;i<PASSAGE_NUMBER;i++)    														//ͨ��ͬʱ����һ����ɫ�����ͻ
	{		
		count_temp = 0;		
		passage_state_temp = cur_RYG_passage & *(passage_table+i); 		//��ȡһ��ͨ��״̬������ʱ����
		for (j=0;j<LAMP_NUMBER;j++)
		{
			if((passage_state_temp>>j & 1) ==1) 
				count_temp++;
		}
		if(count_temp>1)
		{
			passage_clash |= passage_state_temp;										//����ͨ����ͻ��
		}
	}
	if(passage_clash>0)																					//��ͨ����ͻ
	{
		return passage_clash;
	}
	else
	{
		return 0;
	}
}
/*==============================================================================================
* �﷨��ʽ��    u8 is_period_end(u32 cur_RYG_passage)
* ʵ�ֹ��ܣ�    �Ƿ����ڽ���
* ����1��       	cur_RYG_passage��ǰ��״̬
* ����2��       
* ����ֵ��      	����1��ʾ���ڽ�����0��Ϊδ����
===============================================================================================*/
u8 is_period_end(u32 cur_passage_green,u8 reset_checking)
{
	static u8	 lamp_on_count[LAMP_NUMBER]	=	{0};		//���������Ĵ���
	static u32 last_green_passage = 0;									//��һ�θ�����״̬
	u8		i,j;
	
// 	if(reset_checking == _TRUE)
// 	{
// 		period_green_passage = 0;	
// 	}
// 	period_green_passage |= cur_passage_green;
// 	if(period_green_passage == passage_green && period_green_passage!=0)
// 	{
// 		return (_TRUE);														//���ڽ���
// 	}
// 	else
// 	{
// 		return (_FALSE);													//����û�н���
// 	}

// // 	if((last_green_passage != cur_passage_green) && (cur_passage_green>0))					
// // 	{																								//lamp state��ǰһ�β���ͬ���Ҵ���0
// // 		
// // 	}
	if(reset_checking == _TRUE)
	{
		last_green_passage = 0;	
		for(j = 0;j<LAMP_NUMBER;j++)
		{
			lamp_on_count[j]=0;
		}
	}
	if((last_green_passage != cur_passage_green) && (cur_passage_green>0))					
	{																								//lamp state��ǰһ�β���ͬ���Ҵ���0
		for(i = 0;i<LAMP_NUMBER;i++)
		{
			if(((cur_passage_green>>i& 1) == 1) && ((last_green_passage>>i& 1) != 1))	
			{																						//�µ�״̬������λ��ͻ��
				lamp_on_count[i]++;
				if(lamp_on_count[i]>1)
				{
					printf("һ�����ڽ�������gggggggg\n");
					for(j = 0;j<LAMP_NUMBER;j++)
					{
						lamp_on_count[j]=0;
					}
					return (_TRUE);																	//���ڽ���
				}
			}
		}
		last_green_passage = cur_passage_green;  				//���浱ǰ״̬		
	}
	return (_FALSE);																				//����û�н���
}
/*==============================================================================================
* �﷨��ʽ��    u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage)
* ʵ�ֹ��ܣ�    �ƹ��ϼ�⣬�趨·��ѭ�����ڣ�һ��������ͨ����ɫ�������һ��������״̬���������
* ����1��          cur_RYG_passage��ǰ��״̬
* ����2��          valid_RG_passage��Ч��
* ����ֵ��        �ƹ��ϣ�������0������״ֵ̬���޹��ϣ�����0
===============================================================================================*/
u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage,u32 cur_passage_green,u8* period_completed_flag,u8 reset_checking)
{
	static u32 period_lamp_state = 0;							//һ�����ڵĵ�״̬֮��
	u32 state_temp=0;
	u8 i;
	
	/*�ƹ��ϼ�������*/
	static u8 lamp_fault_count[LAMP_NUMBER]= {0};		
	/*������ڣ�2�����ƹ���״ֵ̬*/
	u32		lamp_fault_value 	 	  = 0;	
	
	if(reset_checking==1)
	{
		period_lamp_state = 0;
		for(i=0;i<LAMP_NUMBER;i++)
		{
			lamp_fault_count[i]=0;
		}
	}
	
	if(is_period_end(cur_passage_green,reset_checking) == 0)				//������û�н�������ѵ�ǰ��״̬����period_lamp_state
	{
		period_lamp_state |= cur_RYG_passage;  			//���浱ǰ״̬��period_lamp_state		
		return 0;																		//ͨ���޹���
	}
	else
	{
		state_temp = (~period_lamp_state & valid_RYG_passage);
		period_lamp_state = 0;
		
		*period_completed_flag = __TRUE;
		
		//������⵽2�εƹ��ϣ��ű���
		lamp_fault_value = 0;
		for(i=0;i<LAMP_NUMBER;i++)
		{
			if((state_temp>>i & 1) == 1)
			{
				lamp_fault_count[i]++;
				if(lamp_fault_count[i]>=2)		//������⵽2�εƹ��ϣ��򱨾�
				{
					lamp_fault_count[i] = 2;		//��ֹoverflow
					lamp_fault_value |= (1<<i);
				}
			}
			else
			{
				if(lamp_fault_count[i]>0)
				{
					lamp_fault_count[i]--;
				}
			}
			
		}
		
		return lamp_fault_value;														//ͨ�����޹���
	}
}
/*==============================================================================================
* �﷨��ʽ��    u32 lamp_phyLevel_detect(u32 cur_lamp_status,u16 standard_current[],u16 now_current[],u8 reset_checking)
* ʵ�ֹ��ܣ�    ������������ϼ�⣬������������Χ������й���
* ����1��          cur_lamp_status��ǰ��״̬
* ����2��          u16 standard_current[]ѧϰ�õ��ı�׼�Ƶ���,u16 now_current[]��ǰ�Ƶ���,u8 reset_checking
* ����ֵ��        �ƹ��ϣ�������0�����ع��ϵ�lamp_phy_error���޹��ϣ�����0
===============================================================================================*/
u32 lamp_phyLevel_detect(u8 *phyerr_level,u32 cur_lamp_status,u16 standard_current[],u16 now_current[],u8 reset_checking)
{
	static u8 lamp_phyerr_count[LAMP_NUMBER] = {0};							//һ�����ڵĵ�״̬֮��
	u8 i;
	u32 lamp_phy_error = 0;

	if(reset_checking==1)
	{
		for(i=0;i<LAMP_NUMBER;i++)
		{
			lamp_phyerr_count[i] = 0;
		}
	}
	
	for(i=0;i<LAMP_NUMBER;i++)
	{
		if((standard_current[i] > 0) && (now_current[i] > 0))
		{
			//�����ж���Χnow_current[i]>(u16)(standard_current[i]*1.2) || 				
			if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(4/4)))//��1�ȼ�
			{
				lamp_phyerr_count[i]++;
				//����״̬����,�����ۻ�����2���ϱ�����
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//��ֹ���
					lamp_phy_error|=(u32)(1<<i);														//ͨ���й���
					*phyerr_level = 1;//��1�ȼ�
				}
			}
			else if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(3/4)))//��2�ȼ�
			{
				lamp_phyerr_count[i]++;
				//����״̬����,�����ۻ�����2���ϱ�����
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//��ֹ���
					lamp_phy_error|=(u32)(1<<i);														//ͨ���й���
					*phyerr_level = 2;//��2�ȼ�
				}
			}
			else if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(2/4)))//��3�ȼ�
			{
				lamp_phyerr_count[i]++;
				//����״̬����,�����ۻ�����2���ϱ�����
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//��ֹ���
					lamp_phy_error|=(u32)(1<<i);														//ͨ���й���
					*phyerr_level = 3;//��3�ȼ�
				}
			}
			else if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(1/4)))//��4�ȼ�
			{
				lamp_phyerr_count[i]++;
				//����״̬����,�����ۻ�����2���ϱ�����
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//��ֹ���
					lamp_phy_error|=(u32)(1<<i);														//ͨ���й���
					*phyerr_level = 4;//��4�ȼ�
				}
			}
			else
			{
				if(lamp_phyerr_count[i]>0)
				{
					lamp_phyerr_count[i]--;
				}
			}
		}
	}
	
	return lamp_phy_error;														//ͨ���޹���

}
/*==============================================================================================
* �﷨��ʽ��    u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage)
* ʵ�ֹ��ܣ�    �ƹ��ϼ�⣬�趨·��ѭ�����ڣ�һ��������ͨ����ɫ�������һ��������״̬���������
* ����1��          cur_RYG_passage��ǰ��״̬
* ����2��          valid_RG_passage��Ч��
* ����ֵ��        �ƹ��ϣ�������0������״ֵ̬���޹��ϣ�����0
===============================================================================================*/
void save_learned_data(u32 passage_table[],u32 phase_clash_table[],u32 p_state_task[])
{
//	uint8_t i=0;
	/*
		�洢phase_clash_table[]��eeprom
	*/
	save_param(PHASE_CONFLICT_LIST,	(uint8_t *)phase_clash_table,sizeof(u32)*PHASE_CLASH_STATE_NUMBER);
	/*
		�洢passage_table[]��eeprom
	*/
	save_param(PASSAGE_STATUS_LIST,	(uint8_t *)passage_table,sizeof(u32)*PASSAGE_NUMBER);
	/*
		�洢��ʹ��״̬��eeprom
	*/	
	save_param(LAMP_USE,	(uint8_t *)&Flag_LampUse,sizeof(Flag_LampUse));
	/*
		�洢�����״̬��eeprom
	*/	
	save_param(LEARNED_RED_STATUS,	(uint8_t *)&p_state_task[0],sizeof(u32));
	save_param(LEARNED_YELLOW_STATUS,	(uint8_t *)&p_state_task[1],sizeof(u32));
	save_param(LEARNED_GREEN_STATUS,	(uint8_t *)&p_state_task[2],sizeof(u32));	
}
/*==============================================================================================
* �﷨��ʽ��    u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage)
* ʵ�ֹ��ܣ�    �ƹ��ϼ�⣬�趨·��ѭ�����ڣ�һ��������ͨ����ɫ�������һ��������״̬���������
* ����1��          cur_RYG_passage��ǰ��״̬
* ����2��          valid_RG_passage��Ч��
* ����ֵ��        �ƹ��ϣ�������0������״ֵ̬���޹��ϣ�����0
===============================================================================================*/
void read_learned_data(u32 passage_table[],u32 phase_clash_table[],u32 p_state_task[])
{
//	uint8_t i=0;
	/*
		�洢phase_clash_table[]��eeprom
	*/
	read_param(PHASE_CONFLICT_LIST,	(uint8_t *)phase_clash_table,sizeof(u32)*PHASE_CLASH_STATE_NUMBER);
	/*
		�洢passage_table[]��eeprom
	*/	
	read_param(PASSAGE_STATUS_LIST,	(uint8_t *)passage_table,sizeof(u32)*PASSAGE_NUMBER);
	/*
		�洢��ʹ��״̬��eeprom
	*/	
	read_param(LAMP_USE,	(uint8_t *)&Flag_LampUse,sizeof(Flag_LampUse));
	/*
		�洢�����״̬��eeprom
	*/	
	read_param(LEARNED_RED_STATUS,	(uint8_t *)&p_state_task[0],sizeof(u32));
	read_param(LEARNED_YELLOW_STATUS,	(uint8_t *)&p_state_task[1],sizeof(u32));
	read_param(LEARNED_GREEN_STATUS,	(uint8_t *)&p_state_task[2],sizeof(u32));

}
/*
*	
	u8   Phase_ParaOutputList[2][4],				//ͨ�������
	u32  *realtime_table,						//��ǰʵʱֵ
*	function:			����ͨ�����״̬��ʵʱ��ʾ��	
*/
void channel_output_table(vu8 Passage_ParaOutputList_temp[][4],u32 realtime_table)
{
	u8 red,yellow,green;
	u8 i;
	u32 rt_std = realtime_table;

	Passage_ParaOutputList_temp[0][0] = 1;

	red = 0;
	yellow = 0;
	green = 0;
	for(i = 0; i < 8; i++)
	{
		if(rt_std & (1<<(3*i+2)))red |= (1<<i);
		if(rt_std & (1<<(3*i+1)))yellow |= (1<<i);
		if(rt_std & (1<<(3*i)))green |= (1<<i);
	}
	Passage_ParaOutputList_temp[0][1] = red;
	Passage_ParaOutputList_temp[0][2] = yellow;
	Passage_ParaOutputList_temp[0][3] = green;

	Passage_ParaOutputList_temp[1][0] = 2;
	red = 0;
	yellow = 0;
	green = 0;

	rt_std = rt_std >> 24;
	
	for(i = 0; i < 3; i++)
	{
		if(rt_std & (1<<(3*i)))red |= (1<<i);
		if(rt_std & (1<<(3*i+1)))yellow |= (1<<i);
		if(rt_std & (1<<(3*i+2)))green |= (1<<i);
	}
	Passage_ParaOutputList_temp[1][1] = red;
	Passage_ParaOutputList_temp[1][2] = yellow;
	Passage_ParaOutputList_temp[1][3] = green;	
}

void lxbprint_status(u32 this_status)
{
	u8 i;
	for (i=0; i<12; i++)
		  {    
				if ((this_status&0x01) == 0x01)
				{
					printf("%d", 1);
				}
				else
				{
					printf("%d", 0);
				} 
				this_status >>= 1;
        }   
		printf("\n");
}
/*==============================================================================================
* �﷨��ʽ��    uint16_t ADC_lamp_current(uint16_t *pData,uint8_t len)
* ʵ�ֹ��ܣ�    �Ե���adֵ���й��˼��㣬�õ���ǰ����Ч����
* ����1��         
* ����2��         
* ����ֵ��       
===============================================================================================*/
uint16_t ADC_lamp_current(uint16_t *pData,uint8_t len)
{
	uint16_t value_return=0;
	uint16_t temp=0;
	uint16_t sum=0;

	uint32_t i,j;
	
	for(i=0;i<len-1;i++) 
	{
		for(j=0;j<len-i-1;j++) 
		{
			if(*(pData+j) > *(pData+j+1))
			{
				temp	=*(pData+j);
				*(pData+j)=*(pData+j+1);
				*(pData+j+1)=temp;
			}
		}
	}
	
	if(len>8)
	{
		for(i=len/3;i<(len-len/3);i++) 
		{
			 sum +=*(pData+i);
		}
		value_return =sum/(len-len*2/3);
	}
	else
	{
		for(i=0;i<len;i++) 
		{
			 sum +=*(pData+i);
		}
		value_return =sum/len;
	}
	return (value_return);
}
/*==============================================================================================
* �﷨��ʽ��    ADC_PwrVolProc(void)
* ʵ�ֹ��ܣ�    �������ѹ���
* ����1��       learning_transformer_flag	web�޸ĵ�ѹֵ���flagΪ�棬ѧϰ������
* ����2��       float web_change_vol web���͵ı�׼��ѹֵ
* ����ֵ��      
===============================================================================================*/
//���ֵ����ֵ
#define ABS(x, y)        ((x)>(y)?(x-y):(y-x))
float bbb =0;
u8 ADC_PwrVolProc(u8* learning_transformer_flag,float web_change_vol)
{
	u8    i                 = 0;
	u8    error_flag        = 0;
// static	u8	Pwr_Vol_Index_temp= 0;
	float Pwr_Volatage      = 0;
	float Freq_temp         = 0;
	float Vol_temp          = 0;
		
	uint16_t		aaa;
	uint16_t		time;
	static uint8_t		Pwr_Freq_Index_temp = 0;
	static u16  Pwr_Freq_Record_temp[10] = {0};          //��ͨ�Ƶ�ѹƵ�ʻ���
	static float  Pwr_Vol_Record_temp[10] = {0};           //��ͨ�Ƶ�ѹ��ѹ����  
	static u8		first_start_learning_flag = 0;					//��һ�ο�ʼѧϰ

	if((*learning_transformer_flag == __TRUE)
			&& (first_start_learning_flag == __FALSE))//��һ�ο�ʼѧϰ��������棬���²ɼ����� 
	{
		first_start_learning_flag = __TRUE;
		
		Pwr_Freq_Index_temp = 0;
		for(i=0;i<10;i++)
		{
			Pwr_Freq_Record_temp[i] = 0;
			Pwr_Vol_Record_temp[i] = 0;
		}
	}

	TIM_Cmd(TIM1, DISABLE);
	time = TIM1->CNT;
	TIM1->CNT = 0;
	TIM_Cmd(TIM1, ENABLE);
	Pwr_Freq_Record_temp[Pwr_Freq_Index_temp] = (u16)((time) / 6 / 2);       //������/30�� 
				
	for (i=0; i<10; i++)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		os_dly_wait(5);
		aaa = ADC_GetConversionValue(ADC2);
// 		bbb = (float) aaa/ 4095 * 3.3;
// 		printf("%f	",bbb);
		Pwr_Volatage += (float) aaa/ 4095 * 3.3;
	}
// 	printf("\n");
	Pwr_Volatage = Pwr_Volatage / 10;
	Pwr_Volatage = Pwr_Volatage * (270+100)*100/(100*(10+20));    //U=VOL*(R27+R26)*R24/(R26*(R17+R18))
	Pwr_Volatage = Pwr_Volatage / 1.2;  
	
	if(*learning_transformer_flag == __FALSE)//��������ѧϰ��ת��
	{
		Pwr_Volatage = Pwr_Volatage * (turn_num_lxb/100);            //��ѹ��
	}
	else
	{
		printf("������������ѧϰ\n");
	}

	Pwr_Vol_Record_temp[Pwr_Freq_Index_temp++] = Pwr_Volatage;

	if (Pwr_Freq_Index_temp >= 10)
	{		
			Pwr_Freq_Index_temp = 0;
			printf("��������1min\n");  
			printf("%f\n",turn_num_lxb);        
			for (i=0; i<10; i++)                            //10�����ݴ���
			{
          Freq_temp += Pwr_Freq_Record_temp[i];
					Vol_temp  += Pwr_Vol_Record_temp[i];
			}

			Freq_temp /= 10;
			Vol_temp  /= 10;

			if(*learning_transformer_flag == __TRUE)
			{
				if(Vol_temp>0)//ѧϰ�ɹ�
				{
					turn_num_lxb = web_change_vol*100/Vol_temp;
					Vol_temp = web_change_vol;
					*learning_transformer_flag = __FALSE;
					first_start_learning_flag  = __FALSE;
					
					save_param(REAL_TURN_NUM_ADDR,	(uint8_t *)&turn_num_lxb,		sizeof(turn_num_lxb));
					
					printf("������ѧϰ�ɹ�\n");  
					printf("%f\n",turn_num_lxb); 
				}
				else//����ѧϰ������
				{
					*learning_transformer_flag = __TRUE;
				}
			}
			else
			{
					if (Freq_temp < FREQ_VALVE_COUNT)//����������Χ�⿪ʼ���� //Ƶ�ʱ仯������ֵ 
					{
						error_flag = 1;  
						printf("�������ԴƵ������\n");    
						printf("%f\n",Freq_temp);        
					}
					else if(Vol_temp < 180 || Vol_temp > 260)//����������Χ�⿪ʼ���� //Ƶ�ʱ仯������ֵ 
					{
						error_flag = 2;  
						printf("�������ԴƵ������\n"); 
						printf("%f\n",Vol_temp);         
					}
			}			
			os_mut_wait(table_global_mut,0xFFFF);
			Pwr_Freq = (u16)Freq_temp;
			Pwr_Vol  = (u16)Vol_temp;   				
			os_mut_release(table_global_mut);		
	}
	return error_flag;
}
/*======================================================================
�������ƣ�
u8 Power3G_Detect(void)
//			 
˵����3G��Դ��⣬�е緵��1�����緵��0
=====================================================================*/
u8  Power3G_Detect(void)
{
 	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
		return (1);
	else
	{
		os_dly_wait(5);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
			return(1);
		 else
			return(0);
	}
}
/*======================================================================
�������ƣ�
void Report_Lamp_PhyLevel(void)
//			 
˵���s�ϱ�����
=====================================================================*/
u8 lamp_report_table[77];
void Report_Lamp_PhyLevel()
{
    u32 temp = 0,i;
		u16 lamp_current_temp[LAMP_NUMBER]={0}; 
		u8	count=0;

		/*�̻ƺ�ת��Ϊ�����*//*ȫ�ֵ�����С���飬�Ա��ѯ�ϱ�*/
		os_mut_wait(table_global_mut,0xFFFF);
		if(1)
		{
			lamp_current_temp[0]=Lamp_Broken_Level[2];
			lamp_current_temp[1]=Lamp_Broken_Level[1];
			lamp_current_temp[2]=Lamp_Broken_Level[0];
			lamp_current_temp[3]=Lamp_Broken_Level[5];
			lamp_current_temp[4]=Lamp_Broken_Level[4];
			lamp_current_temp[5]=Lamp_Broken_Level[3];
			lamp_current_temp[6]=Lamp_Broken_Level[8];
			lamp_current_temp[7]=Lamp_Broken_Level[7];
			lamp_current_temp[8]=Lamp_Broken_Level[6];
			lamp_current_temp[9]=Lamp_Broken_Level[11];
			lamp_current_temp[10]=Lamp_Broken_Level[10];
			lamp_current_temp[11]=Lamp_Broken_Level[9];
			
			lamp_current_temp[12]=Lamp_Broken_Level[14];
			lamp_current_temp[13]=Lamp_Broken_Level[13];
			lamp_current_temp[14]=Lamp_Broken_Level[12];
			lamp_current_temp[15]=Lamp_Broken_Level[17];
			lamp_current_temp[16]=Lamp_Broken_Level[16];
			lamp_current_temp[17]=Lamp_Broken_Level[15];
			lamp_current_temp[18]=Lamp_Broken_Level[20];
			lamp_current_temp[19]=Lamp_Broken_Level[19];
			lamp_current_temp[20]=Lamp_Broken_Level[18];
			lamp_current_temp[21]=Lamp_Broken_Level[23];
			lamp_current_temp[22]=Lamp_Broken_Level[22];
			lamp_current_temp[23]=Lamp_Broken_Level[21];
			lamp_current_temp[24]=Lamp_Broken_Level[26];
			lamp_current_temp[25]=Lamp_Broken_Level[25];
			lamp_current_temp[26]=Lamp_Broken_Level[24];
			lamp_current_temp[27]=Lamp_Broken_Level[29];
			lamp_current_temp[28]=Lamp_Broken_Level[28];
			lamp_current_temp[29]=Lamp_Broken_Level[27];
			lamp_current_temp[30]=Lamp_Broken_Level[31];
			lamp_current_temp[31]=Lamp_Broken_Level[30];
		}
		os_mut_release(table_global_mut);
	
		for (i=0; i<LAMP_NUMBER; i++)
		{   
			if ((i%3) == 0)
			{   
				temp = (i / 3) + 1;
				lamp_report_table[count++] = (u8)temp;    //ͨ����           
				temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
				lamp_report_table[count++] = (u8)temp;    //�Ƶ�������  ���ֽ���ǰ
				temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
				lamp_report_table[count++] = (u8)temp;    //�Ƶ�������                
			}
			else
			{
				temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
				lamp_report_table[count++]= (u8)temp;    //�Ƶ�������
				temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
				lamp_report_table[count++] = (u8)temp;    //�Ƶ�������                
			}
		}
}

/*==============================================================================================
* �﷨��ʽ��    get_stage_table(void)
* ʵ�ֹ��ܣ�    �õ��׶α�
* ����1��       
* ����2��       
* ����ֵ��      
===============================================================================================*/
u16 lamp_stage_count[LAMP_NUMBER] = {0};//�׶ε����ļ���ֵ
STAGE_TIMING_TABLE_t stage_time_table_temp[PHASE_CLASH_STATE_NUMBER] = {0};//�׶���ʱ��
void get_stage_table(u8* period_completed_flag, u32 current_lamp_state,u32 learned_passage_green,u32 learned_passage_red,u8 reset_flag)
{
	static u8 bPeriod_Start = __FALSE;//�׶μ�������ڿ�ʼ��־
	u8 i,j;
	static u32 last_green_state=0;//��һ�ε��̵�״̬�������жϽ׶ν���
	u32 current_green_state = 0;//��ǰ�̵�״̬
	static u8  stage_count=0;//�׶μ���
	static u16 period_count = 0;//���ڼ���
	
	if(reset_flag == __TRUE)
	{
		for(i=0;i<LAMP_NUMBER;i++)			
		{
			lamp_stage_count[i]=0;
		}
		bPeriod_Start = 0;
		last_green_state=0;
		stage_count=0;
		period_count = 0;
	}
	
	current_green_state = (current_lamp_state&learned_passage_green);

	if(bPeriod_Start == __TRUE)
	{
		if((current_green_state!=0) && (current_green_state!=last_green_state))
		{//˵��һ���׶ν���
			stage_time_table_temp[stage_count].stageId = stage_count;
			stage_time_table_temp[stage_count].relPhase = 0;
			for(i=0;i<LAMP_NUMBER;i++)			//��ǰ��״̬Ϊ1�������ֵ++
			{
				if(((last_green_state>>i) & 0x01) == 0x01)
				{
					stage_time_table_temp[stage_count].relPhase |= (1<<(i/3));
				}
			}
			/*����ý׶ε�ʱ������λ*/
			for(i=0;i<LAMP_NUMBER;i++)			//��ǰ��״̬Ϊ1�������ֵ++
			{
				if((((learned_passage_green>>i) & 0x01) == 0x01)&&(lamp_stage_count[i]>0))
				{
					if((current_green_state&last_green_state)!=0)
					{//����ǰ��λ���ϸ���λΪ������ϵ���������̵Ƽ��޹���ʱ��4800ms
						stage_time_table_temp[stage_count].stGreenTime = (lamp_stage_count[i])*(ADC_CYCLE_TIME+5)/1000;//green
					}
					else
					{
						stage_time_table_temp[stage_count].stGreenTime = (lamp_stage_count[i]+(4800/ADC_CYCLE_TIME))*(ADC_CYCLE_TIME+5)/1000;//green
					}
					if((learned_passage_red>>(i+1)& 0x01) == 0x01)//���̻Ƶ�ͨ��˳��
					{
						stage_time_table_temp[stage_count].stRedTime = lamp_stage_count[i+1]*(ADC_CYCLE_TIME+5)/1000;//red
						stage_time_table_temp[stage_count].stYellowTime = (lamp_stage_count[i-1]+(720/ADC_CYCLE_TIME))*(ADC_CYCLE_TIME+5)/1000;//yellow
					}
					else //if((learned_passage_red>>(i+2)& 0x01) == 0x01)//����̵�ͨ��˳��
					{
						stage_time_table_temp[stage_count].stYellowTime = (lamp_stage_count[i+1]+(720/ADC_CYCLE_TIME))*(ADC_CYCLE_TIME+5)/1000;//yellow
						stage_time_table_temp[stage_count].stRedTime = lamp_stage_count[i+2]*(ADC_CYCLE_TIME+5)/1000;//red
					}
				}
				lamp_stage_count[i] = 0;
			}
			last_green_state = current_green_state;
			
			if(stage_time_table_temp[stage_count].stGreenTime >0)//�̵�ʱ�����0�Ž׶���Ч
			{
				stage_count++;
			}
			if(stage_count == PHASE_CLASH_STATE_NUMBER)
			{
				stage_count=0;
			}
		}
		else
		{
			for(i=0;i<LAMP_NUMBER;i++)			//��ǰ��״̬Ϊ1�������ֵ++
			{
				if((current_lamp_state>>i & 0x01) == 0x01)
				{
					lamp_stage_count[i]++;
				}
			}
		}
		period_count++;
	}	
	if(*period_completed_flag == __TRUE)//�Ե�һ�����ڽ���Ϊ�����н׶μ���
	{
		*period_completed_flag = __FALSE;//�Ա��ж���һ�����ڽ���
		if(stage_count>0)
		{
			//�׶��Ѽ������
			
			os_mut_wait(table_global_mut,0xFFFF);
			current_stage_num = stage_count;//����׶���
			for(i=0;i<current_stage_num;i++)
			{
// // 				if((stage_time_table_temp[i].stRedTime == 1)&&(stage_time_table_temp[i+1].stRedTime != 0)&&i<PHASE_CLASH_STATE_NUMBER-2)
// // 				{
// // 					stage_time_table_temp[i].stRedTime = stage_time_table_temp[i+1].stRedTime;
// // 					stage_time_table_temp[i].stYellowTime = stage_time_table_temp[i+1].stYellowTime;
// // 				}
				stage_time_table[i] = stage_time_table_temp[i];
			}
			os_mut_release(table_global_mut);		
			
			for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)
			{
				stage_time_table_temp[i].stRedTime = 0;
				stage_time_table_temp[i].stGreenTime = 0;
				stage_time_table_temp[i].stYellowTime = 0;
				stage_time_table_temp[i].relPhase = 0;
				stage_time_table_temp[i].stageId = 0;				
			}
		}
		stage_count = 0;
		
		if(period_count>0)
		{
			period_time = period_count*(ADC_CYCLE_TIME+5)/1000;
		}
		period_count = 0;
		
		last_green_state = current_green_state;//���¿�ʼ�½׶μ���
	
		bPeriod_Start = __TRUE;
	}	
	
}

void tsk_watchdog_reload()
{
	/*1.���ڷǶ�ʱ���񣬼���־����̫��Ӧ��tcp���񣬵���������ʱ�������ź�xx_xx_run��
			���������������ڴ�������ź�xx_xx_over�����������ź���ȣ�˵��������������ι����
			����Щ����û���У���run��over�źž�Ϊ0�����ι��
		2.����ADC����ѧϰ���񣬵�Դ�������power_detect_task��������ʱ����������
			ֻ��Ҫ�����źż���
	*/
	if(	((tsk_tcp_run^tsk_tcp_over) ==(__FALSE))//tcp_tsk
		&&((tsk_log_run^tsk_log_over) ==(__FALSE))//log_write_task
		&&((tsk_eth_run^tsk_eth_over) ==(__FALSE))//eth_usart_communication_task
	
		&&(tsk_ADC_run ==(__TRUE))	//ADC_task��ʼ����
		&&(tsk_learn_run ==(__TRUE))//learn_detect_task
		&&(tsk_power_run ==(__TRUE))	)//power_detect_task
	{
		IWDG_ReloadCounter();
		
		tsk_tcp_run=(__FALSE);
		tsk_tcp_over=(__FALSE);
		tsk_log_run=(__FALSE);
		tsk_log_over=(__FALSE);
		tsk_eth_run=(__FALSE);
		tsk_eth_over=(__FALSE);
		tsk_ADC_run=(__FALSE);
		tsk_learn_run=(__FALSE);
		tsk_power_run=(__FALSE);
		
 		TIM_Cmd(TIM3, DISABLE);/*�رն�ʱ��ι��*/
		GPIO_PinReverse(GPIOB, GPIO_Pin_15);//ι��Ӳ�����Ź�
	}
}
/*******************************************************************************
* Function Name  : GPIO_PinReverse
* Description    : GPIO???
* Input          : GPIO?:GPIO_TypeDef* GPIOx
* Output         : GPIO?:uint16_t GPIO_Pin
* Return         : None
*******************************************************************************/
void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    
    GPIOx->ODR ^= GPIO_Pin;
}
