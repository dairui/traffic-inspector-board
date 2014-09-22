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

// // // // /*上一次的报警日志的类型*/
// // // // u8	last_alarm_log_type = 0xff;
// // // // /******************************************************************************
// // // // 函数名：uint8_t Is_Leap_Year(uint16_t year)
// // // // 参数：uint16_t year
// // // // 返回值：1,表示是闰年。 0，表示是平年
// // // // 说明：判断是否为闰年
// // // // 闰年：能被4整除但不能被100整除的为闰年， 能被400整除的为闰年
// // // // *******************************************************************************/
// // // // u8 Is_Leap_Year(u16 year)
// // // // {
// // // // 	  if((year % 4) == 0)  										//如果能被4整除
// // // // 	  {
// // // // 			if((year % 100) == 0) 							//如果能被100整除
// // // // 			{
// // // // 				  if((year % 400) == 0)  						//如果能被400整除
// // // // 				  {
// // // // 					return 1;  										//说明是闰年
// // // // 				  }
// // // // 				  else
// // // // 				  {
// // // // 					return 0; 											//说明是平年
// // // // 				  }
// // // // 		}
// // // // 		else
// // // // 		{
// // // // 		  return 1; 													//说明是闰年
// // // // 		}
// // // // 	  }
// // // // 	  else
// // // // 	  {
// // // // 		return 0; 														//说明是平年
// // // // 	  }
// // // // }
extern u8 Is_Leap_Year(u16 year);
/******************************************************************************
函数名：u32 Read_DS3231_Time(void)
返回值：当前时间的4字节时间 秒钟
*******************************************************************************/
u32 Read_DS3231_Time(void)
{
	u32 Time_second = 0;
	u16 t;
	
	//平年的月份表
	const u8 mon_table[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};
	u8 Read_Time_Data[7] = {0};

	os_mut_wait(I2c_Mut_lm,0xFFFF);
	I2C_Read_DS3231(Read_Time_Data);					//  I2C_lm   有待改动的	
	os_mut_release(I2c_Mut_lm);			
		

	for(t = 70;t < Read_Time_Data[6]+100;t++) 			//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(1900 + t))
		{
		  Time_second+=31622400;								//闰年的秒钟数
		}
		else 
		{
		  Time_second+=31536000; 								//平年的秒钟数
		}
	}
	Read_Time_Data[5] -= 1;											//月
	for(t = 0;t < Read_Time_Data[5];t++) 						//把前面月份的秒钟数相加
	{
		Time_second += (u32)mon_table[t] * 86400;		//月份秒钟数相加
		if(Is_Leap_Year(Read_Time_Data[6] + 1900) && (t == 1))
		{
		  Time_second += 86400;									//闰年2月份增加一天的秒钟数
		}
	}
	Time_second += (u32)((Read_Time_Data[4]-1) * 86400);			//把前面日期的秒钟数相加
	Time_second += (u32) (Read_Time_Data[2] * 3600);					//小时秒钟数
	Time_second += (u32) (Read_Time_Data[1] * 60); 						//分钟秒钟数
	Time_second += Read_Time_Data[0];  											//最后的秒钟加上去

	return Time_second;
}

// // // // /*==============================================================================================
// // // // * 语法格式：    void alarmlog_to_db(ALARM_EVENT_LOG *alarm_data_log,LOG_TO_DB *log_db_data)
// // // // * 实现功能：    写日志结构体写入数据库结构体
// // // // * 参数1：         alarm_data_log：报警日志结构体
// // // // * 参数2：         log_db_data：日志写入数据库结构体
// // // // * 返回值：         first_all_passage：二值化后的通道状态
// // // // ===============================================================================================*/
// // // // void alarmlog_to_db(ALARM_EVENT_LOG *alarm_data_log,LOG_TO_DB *log_db_data)
// // // // {
// // // // 		u8	i;
// // // // 		u32 current_time_sec = 0;														//当前时间对应的秒数
// // // // 	
// // // // 		os_mut_wait(I2c_Mut_lm,0xFFFF);
// // // // 		current_time_sec = Read_DS3231_Time();							//事件发生时间
// // // // 		os_mut_release(I2c_Mut_lm);
// // // // 	
// // // // 		//根据事件值获得索引1的事件类型
// // // // 		for(i=0;i<8;i++)
// // // // 		{
// // // // 			if((alarm_data_log->alarm_type & 1<<i) != 0) 						//由报警类型得到对应在数据库的索引1.bit0对应0，以此类推
// // // // 			{
// // // // 				log_db_data->index1 = i;			
// // // // 				break;
// // // // 			}
// // // // 		}
// // // // 		
// // // // 		/*填充 写数据库结构体log_db_data*/
// // // // 		log_db_data->log_ID 						= 0x92;	
// // // // 		log_db_data->index1 						= log_db_data->index1;	
// // // // 		log_db_data->index2 						= 0;	
// // // // 		log_db_data->common_time 				= current_time_sec;	
// // // // 		log_db_data->data_value[0]			= 0xA7;	
// // // // 		log_db_data->data_value[1]			= alarm_data_log->alarm_ID;	
// // // // 		log_db_data->data_value[2] 			= alarm_data_log->alarm_value[0];	
// // // // 		log_db_data->data_value[3] 			= alarm_data_log->alarm_value[1];	
// // // // }
