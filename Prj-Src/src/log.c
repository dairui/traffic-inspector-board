#include "include.h"
#include <stdio.h>

/**/
/*  Write Log FIFO size */
#define WRITE_LOG_SIZE        10  
#define EVERY_LOG_SIZE        21  
/*  Write Log FIFO  */
vu8 write_log_wr_fifo[WRITE_LOG_SIZE][EVERY_LOG_SIZE];

vu32 write_log_wr_index,write_log_rd_index,write_log_counter;
/* This flag is set on Write Log  Fifo Overflow */
vu8 write_log_buffer_overflow;

/**/

// // // // /*��һ�εı�����־������*/
// // // // u8	last_alarm_log_type = 0xff;
// // // // /******************************************************************************
// // // // ��������uint8_t Is_Leap_Year(uint16_t year)
// // // // ������uint16_t year
// // // // ����ֵ��1,��ʾ�����ꡣ 0����ʾ��ƽ��
// // // // ˵�����ж��Ƿ�Ϊ����
// // // // ���꣺�ܱ�4���������ܱ�100������Ϊ���꣬ �ܱ�400������Ϊ����
// // // // *******************************************************************************/
// // // // u8 Is_Leap_Year(u16 year)
// // // // {
// // // // 	  if((year % 4) == 0)  										//����ܱ�4����
// // // // 	  {
// // // // 			if((year % 100) == 0) 							//����ܱ�100����
// // // // 			{
// // // // 				  if((year % 400) == 0)  						//����ܱ�400����
// // // // 				  {
// // // // 					return 1;  										//˵��������
// // // // 				  }
// // // // 				  else
// // // // 				  {
// // // // 					return 0; 											//˵����ƽ��
// // // // 				  }
// // // // 		}
// // // // 		else
// // // // 		{
// // // // 		  return 1; 													//˵��������
// // // // 		}
// // // // 	  }
// // // // 	  else
// // // // 	  {
// // // // 		return 0; 														//˵����ƽ��
// // // // 	  }
// // // // }
extern u8 Is_Leap_Year(u16 year);
/******************************************************************************
��������u32 Read_DS3231_Time(void)
����ֵ����ǰʱ���4�ֽ�ʱ�� ����
*******************************************************************************/
u32 Read_DS3231_Time(void)
{
	u32 Time_second = 0;
	u16 t;
	
	//ƽ����·ݱ�
	const u8 mon_table[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};
	u8 Read_Time_Data[7] = {0};

	os_mut_wait(I2c_Mut_lm,0xFFFF);
	I2C_Read_DS3231(Read_Time_Data);					//  I2C_lm   �д��Ķ���	
	os_mut_release(I2c_Mut_lm);			
		

	for(t = 70;t < Read_Time_Data[6]+100;t++) 			//��������ݵ��������
	{
		if(Is_Leap_Year(1900 + t))
		{
		  Time_second+=31622400;								//�����������
		}
		else 
		{
		  Time_second+=31536000; 								//ƽ���������
		}
	}
	Read_Time_Data[5] -= 1;											//��
	for(t = 0;t < Read_Time_Data[5];t++) 						//��ǰ���·ݵ����������
	{
		Time_second += (u32)mon_table[t] * 86400;		//�·����������
		if(Is_Leap_Year(Read_Time_Data[6] + 1900) && (t == 1))
		{
		  Time_second += 86400;									//����2�·�����һ���������
		}
	}
	Time_second += (u32)((Read_Time_Data[4]-1) * 86400);			//��ǰ�����ڵ����������
	Time_second += (u32) (Read_Time_Data[2] * 3600);					//Сʱ������
	Time_second += (u32) (Read_Time_Data[1] * 60); 						//����������
	Time_second += Read_Time_Data[0];  											//�������Ӽ���ȥ

	return Time_second;
}

// // // // /*==============================================================================================
// // // // * �﷨��ʽ��    void alarmlog_to_db(ALARM_EVENT_LOG *alarm_data_log,LOG_TO_DB *log_db_data)
// // // // * ʵ�ֹ��ܣ�    д��־�ṹ��д�����ݿ�ṹ��
// // // // * ����1��         alarm_data_log��������־�ṹ��
// // // // * ����2��         log_db_data����־д�����ݿ�ṹ��
// // // // * ����ֵ��         first_all_passage����ֵ�����ͨ��״̬
// // // // ===============================================================================================*/
// // // // void alarmlog_to_db(ALARM_EVENT_LOG *alarm_data_log,LOG_TO_DB *log_db_data)
// // // // {
// // // // 		u8	i;
// // // // 		u32 current_time_sec = 0;														//��ǰʱ���Ӧ������
// // // // 	
// // // // 		os_mut_wait(I2c_Mut_lm,0xFFFF);
// // // // 		current_time_sec = Read_DS3231_Time();							//�¼�����ʱ��
// // // // 		os_mut_release(I2c_Mut_lm);
// // // // 	
// // // // 		//�����¼�ֵ�������1���¼�����
// // // // 		for(i=0;i<8;i++)
// // // // 		{
// // // // 			if((alarm_data_log->alarm_type & 1<<i) != 0) 						//�ɱ������͵õ���Ӧ�����ݿ������1.bit0��Ӧ0���Դ�����
// // // // 			{
// // // // 				log_db_data->index1 = i;			
// // // // 				break;
// // // // 			}
// // // // 		}
// // // // 		
// // // // 		/*��� д���ݿ�ṹ��log_db_data*/
// // // // 		log_db_data->log_ID 						= 0x92;	
// // // // 		log_db_data->index1 						= log_db_data->index1;	
// // // // 		log_db_data->index2 						= 0;	
// // // // 		log_db_data->common_time 				= current_time_sec;	
// // // // 		log_db_data->data_value[0]			= 0xA7;	
// // // // 		log_db_data->data_value[1]			= alarm_data_log->alarm_ID;	
// // // // 		log_db_data->data_value[2] 			= alarm_data_log->alarm_value[0];	
// // // // 		log_db_data->data_value[3] 			= alarm_data_log->alarm_value[1];	
// // // // }
