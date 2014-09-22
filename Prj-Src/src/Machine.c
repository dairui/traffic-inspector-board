#include "include.h"
//======================================================================
// 函数名称：
// Machine_Set_Detect(void)
//
// 说明：信号机配置检测
//======================================================================
extern os_mbx_declare(err_report_mbox,20);						//上报任务
void Machine_Set_Detect(void)
{   
    u32 charge_month;  
    u8 time_read[7] = {0};
		u32 temp;
    u16 t;
    u16 charge_time;
    ////BATTON////
    u16 check_number = 0;//检测到的BATT_DOW高电平次数
    u16 m = 0;//循环检测次数
    ////BATTON////
    u8	*msg_send						= NULL;

		u8 index_temp = 0;

		u8 	Alram_Time_Data[4] = {0};
		static u32 check_bat_count = 0;


		static U8	err_type = 0;
		U8 *msg_send_report = NULL;

		os_mut_wait(I2c_Mut_lm,0xFFFF);
    I2C_Read_DS3231(time_read);
		os_mut_release(I2c_Mut_lm);		

    charge_month = ((time_read[5] & 0xF0) >> 4) * 10 + ((time_read[5] & 0x0F) >> 0);   //月
		
    if ((Flag_Charge == 1)||(Batton_Break_flag == 1))                                          //检测电池放电结束 
    {
        if ((CHARGE_OVER() != 0)||(Batton_Break_flag == 1))//高电平，电池电量过低，放电结束
        {
            CHARGE_OFF();                                          //放电结束 
            Flag_Charge = 0;
					
						Charge_Para.Start_Time=0;
						read_param(START_CHARGE_TIME,	(uint8_t *)&Charge_Para.Start_Time,sizeof(Charge_Para.Start_Time));
				
						if (Batton_Break_flag == 0)
						{
							for(t = 70;t < time_read[6]+100;t++) //把所有年份的秒钟相加
							{
									if(Is_Leap_Year(1900 + t))
									{
											temp += 31622400;//闰年的秒钟数
									}
									else 
									{
											temp += 31536000; //平年的秒钟数
									}
							}
							time_read[5] -= 1;//月
							for(t = 0;t < time_read[5];t++) //把前面月份的秒钟数相加
							{
									temp += (u32)mon_table[t] * 86400;//月份秒钟数相加
									if(Is_Leap_Year(time_read[6] + 1900) && (t == 1))
									{
											temp += 86400;//闰年2月份增加一天的秒钟数
									}
							}
							temp += (u32)((time_read[4]-1) * 86400);//把前面日期的秒钟数相加
							temp += (u32) (time_read[2] * 3600);//小时秒钟数
							temp += (u32) (time_read[1] * 60); //分钟秒钟数
							temp += time_read[0];  //最后的秒钟加上去   
							Charge_Para.End_Time = temp; 
						}
						else
						{
							Charge_Para.End_Time = 0;
							Charge_Para.Start_Time=0;
						}
						os_mut_wait(table_global_mut,0xFFFF);
            Para_Charge = Charge_Para.End_Time - Charge_Para.Start_Time;  
						save_param(CHARGE_TIME,	(uint8_t *)&Para_Charge,sizeof(Para_Charge));						
						os_mut_release(table_global_mut);	 
						         
            charge_time = (Charge_Para.End_Time - Charge_Para.Start_Time) / 60;                  //小于放电时间，故障上报
            						
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_RELEASE_POWER;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0x02;
						write_log_wr_fifo[write_log_wr_index][4] = (Charge_Para.End_Time - Charge_Para.Start_Time) / 60;  //单位:分钟;
						write_log_wr_fifo[write_log_wr_index][5] = 0;						
						if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
						if (++write_log_counter  == WRITE_LOG_SIZE)
						{
							write_log_counter=1;
							write_log_buffer_overflow=__TRUE;
						}
						tsk_unlock (); 			
						os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	
						/************************写日志*********************************/
						
						if ((charge_time < Alarm_Charge)||(Batton_Break_flag == 1))
            {
							printf("%s%d\n","蓄电池异常",charge_time);
							
							ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min
							/************************上报异常*********************************/
							err_type = 8;
							msg_send_report = &err_type;
							os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
							if(error_report_time[7]==0)
							{
								error_report_time[7] = REPORT_WATCHDOG_TIME;
								
								if(1)
								{
									/************************写日志*********************************/
									tsk_lock ();  						
									write_log_wr_fifo[write_log_wr_index][0] =1;			
									write_log_wr_fifo[write_log_wr_index][1] = LOG_RELEASE_POWER;
									write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
									write_log_wr_fifo[write_log_wr_index][3] = 0x02;
									write_log_wr_fifo[write_log_wr_index][4] = 0xCE;  
									write_log_wr_fifo[write_log_wr_index][5] = 1;						
									if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
									if (++write_log_counter  == WRITE_LOG_SIZE)
									{
										write_log_counter=1;
										write_log_buffer_overflow=__TRUE;
									}
									tsk_unlock (); 			
									os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	
								}
								/************************写日志*********************************/
								/************************上报异常*********************************/
								error_report_answer[7] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
								
								/************************上报异常*********************************/	
// // // 								/************************给公司监测电脑发送数据处理***************/
// // // 								if(1)
// // // 								{
// // // 									tsk_lock ();  			//  disables task switching 
// // // 									write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 									write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 									write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 									write_MySelf_fifo[write_MySelf_wr_index][3] = CHARGE_ERROR;  //故障号

// // // 									if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 									if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 									{
// // // 										write_MySelf_counter=1;
// // // 										write_MySelf_buffer_overflow=__TRUE;
// // // 									};
// // // 									tsk_unlock (); 			//  enable  task switching 
// // // 									os_sem_send(eth_MySelf_semaphore);
// // // 								}
// // // 								/************************给公司监测电脑发送数据处理***************/	
							}		
							else
							{
								error_report_time[7] = REPORT_WATCHDOG_TIME;
							}					
							Batton_Break_flag = 0;
            }
        }
    }

		if (charge_month != Dtae_Month)                                //电池放电 
    {
        Dtae_Month = charge_month;
				/*
					读蓄电池放电月
				*/	
				save_param(DATE_MONTH,	(uint8_t *)&Dtae_Month,sizeof(Dtae_Month));

        if (CHARGE_OVER() == 0)                                   //电池电压足够高  
        {     
            Flag_Charge = 1;                                      //放电标志位置1 
            for(t = 70;t < time_read[6]+100;t++) //把所有年份的秒钟相加
            {
                if(Is_Leap_Year(1900 + t))
                {
                    temp += 31622400;//闰年的秒钟数
                }
                else 
                {
                    temp += 31536000; //平年的秒钟数
                }
            }
            time_read[5] -= 1;//月
            for(t = 0;t < time_read[5];t++) //把前面月份的秒钟数相加
            {
                temp += (u32)mon_table[t] * 86400;//月份秒钟数相加
                if(Is_Leap_Year(time_read[6] + 1900) && (t == 1))
                {
                    temp += 86400;//闰年2月份增加一天的秒钟数
                }
            }
            temp += (u32)((time_read[4]-1) * 86400);//把前面日期的秒钟数相加
            temp += (u32) (time_read[2] * 3600);//小时秒钟数
            temp += (u32) (time_read[1] * 60); //分钟秒钟数
            temp += time_read[0];  //最后的秒钟加上去            
            Charge_Para.Start_Time = temp;

						/*
							读蓄电池放电月
						*/	
						save_param(START_CHARGE_TIME,	(uint8_t *)&Charge_Para.Start_Time,sizeof(Charge_Para.Start_Time));
						
						//定时2min
						Alram_Time_Data[1] = (time_read[1] + 1);	
						if(Alram_Time_Data[1] >= 60)					
						{
							Alram_Time_Data[2] = time_read[2] + 1;
							Alram_Time_Data[2] = Alram_Time_Data[2] /10*16 + Alram_Time_Data[2]%10;	//从数据库中读出的时间为10进制
							Alram_Time_Data[1] = Alram_Time_Data[1] - 60;	//写时钟芯片闹钟值时应转化为16进制
						}
						else
						{
								Alram_Time_Data[2] = time_read[2]/10*16 + time_read[2]%10;
						}
						Alram_Time_Data[1] = Alram_Time_Data[1]/10*16 + Alram_Time_Data[1]%10;
						Alram_Time_Data[0] = time_read[0]/10*16 + time_read[0]%10;
						Alram_Time_Data[3] = time_read[4]/10*16 + time_read[4]%10;
						
						os_mut_wait(I2c_Mut_lm,0xFFFF);   //  I2C_lm
						I2C_Write_Alarm(Alram_Time_Data);		//时间写入时钟芯片闹钟寄存器   
						I2C_Write_Alarm_Register(0x05);			//使能时钟芯片闹钟中断输出		 
						os_mut_release(I2c_Mut_lm);	
						
						test_batton_flag = 1;
						
            CHARGE_ON();                                          //开始放电   
        }
    }
		
		check_bat_count++;
    if ((Flag_Charge!=1)&&(check_bat_count ==20*60*60*2))          //每天进行一次蓄电池好坏检测,且不处于放电状态
    {
				check_bat_count = 0;
			  //定时2min
			  Alram_Time_Data[1] = (time_read[1] + 1);	
				if(Alram_Time_Data[1] >= 60)					
				{
					Alram_Time_Data[2] = time_read[2] + 1;
					Alram_Time_Data[2] = Alram_Time_Data[2] /10*16 + Alram_Time_Data[2]%10;	//从数据库中读出的时间为10进制
					Alram_Time_Data[1] = Alram_Time_Data[1] - 60;	//写时钟芯片闹钟值时应转化为16进制
				}
				else
				{
						Alram_Time_Data[2] = time_read[2]/10*16 + time_read[2]%10;
				}
				Alram_Time_Data[1] = Alram_Time_Data[1]/10*16 + Alram_Time_Data[1]%10;
				Alram_Time_Data[0] = time_read[0]/10*16 + time_read[0]%10;
				Alram_Time_Data[3] = time_read[4]/10*16 + time_read[4]%10;
			  
				os_mut_wait(I2c_Mut_lm,0xFFFF);   //  I2C_lm
				I2C_Clear_Alarm_Register();
				I2C_Write_Alarm(Alram_Time_Data);		//时间写入时钟芯片闹钟寄存器   
				I2C_Write_Alarm_Register(0x05);			//使能时钟芯片闹钟中断输出		 
				os_mut_release(I2c_Mut_lm);	
				
				test_batton_flag = 1;
				CHARGE_ON();                                          //开始放电   
		}		
}
