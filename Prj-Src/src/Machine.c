#include "include.h"
//======================================================================
// �������ƣ�
// Machine_Set_Detect(void)
//
// ˵�����źŻ����ü��
//======================================================================
extern os_mbx_declare(err_report_mbox,20);						//�ϱ�����
void Machine_Set_Detect(void)
{   
    u32 charge_month;  
    u8 time_read[7] = {0};
		u32 temp;
    u16 t;
    u16 charge_time;
    ////BATTON////
    u16 check_number = 0;//��⵽��BATT_DOW�ߵ�ƽ����
    u16 m = 0;//ѭ��������
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

    charge_month = ((time_read[5] & 0xF0) >> 4) * 10 + ((time_read[5] & 0x0F) >> 0);   //��
		
    if ((Flag_Charge == 1)||(Batton_Break_flag == 1))                                          //����طŵ���� 
    {
        if ((CHARGE_OVER() != 0)||(Batton_Break_flag == 1))//�ߵ�ƽ����ص������ͣ��ŵ����
        {
            CHARGE_OFF();                                          //�ŵ���� 
            Flag_Charge = 0;
					
						Charge_Para.Start_Time=0;
						read_param(START_CHARGE_TIME,	(uint8_t *)&Charge_Para.Start_Time,sizeof(Charge_Para.Start_Time));
				
						if (Batton_Break_flag == 0)
						{
							for(t = 70;t < time_read[6]+100;t++) //��������ݵ��������
							{
									if(Is_Leap_Year(1900 + t))
									{
											temp += 31622400;//�����������
									}
									else 
									{
											temp += 31536000; //ƽ���������
									}
							}
							time_read[5] -= 1;//��
							for(t = 0;t < time_read[5];t++) //��ǰ���·ݵ����������
							{
									temp += (u32)mon_table[t] * 86400;//�·����������
									if(Is_Leap_Year(time_read[6] + 1900) && (t == 1))
									{
											temp += 86400;//����2�·�����һ���������
									}
							}
							temp += (u32)((time_read[4]-1) * 86400);//��ǰ�����ڵ����������
							temp += (u32) (time_read[2] * 3600);//Сʱ������
							temp += (u32) (time_read[1] * 60); //����������
							temp += time_read[0];  //�������Ӽ���ȥ   
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
						         
            charge_time = (Charge_Para.End_Time - Charge_Para.Start_Time) / 60;                  //С�ڷŵ�ʱ�䣬�����ϱ�
            						
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_RELEASE_POWER;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0x02;
						write_log_wr_fifo[write_log_wr_index][4] = (Charge_Para.End_Time - Charge_Para.Start_Time) / 60;  //��λ:����;
						write_log_wr_fifo[write_log_wr_index][5] = 0;						
						if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
						if (++write_log_counter  == WRITE_LOG_SIZE)
						{
							write_log_counter=1;
							write_log_buffer_overflow=__TRUE;
						}
						tsk_unlock (); 			
						os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	
						/************************д��־*********************************/
						
						if ((charge_time < Alarm_Charge)||(Batton_Break_flag == 1))
            {
							printf("%s%d\n","�����쳣",charge_time);
							
							ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //���ϵ�COM��led7����˸3min
							/************************�ϱ��쳣*********************************/
							err_type = 8;
							msg_send_report = &err_type;
							os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
							if(error_report_time[7]==0)
							{
								error_report_time[7] = REPORT_WATCHDOG_TIME;
								
								if(1)
								{
									/************************д��־*********************************/
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
								/************************д��־*********************************/
								/************************�ϱ��쳣*********************************/
								error_report_answer[7] = 0;//���ñ�־���㣬�Ա�ȴ���λ�����д����ϱ�Ӧ�𽫸ñ�־��1
								
								/************************�ϱ��쳣*********************************/	
// // // 								/************************����˾�����Է������ݴ���***************/
// // // 								if(1)
// // // 								{
// // // 									tsk_lock ();  			//  disables task switching 
// // // 									write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 									write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 									write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 									write_MySelf_fifo[write_MySelf_wr_index][3] = CHARGE_ERROR;  //���Ϻ�

// // // 									if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 									if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 									{
// // // 										write_MySelf_counter=1;
// // // 										write_MySelf_buffer_overflow=__TRUE;
// // // 									};
// // // 									tsk_unlock (); 			//  enable  task switching 
// // // 									os_sem_send(eth_MySelf_semaphore);
// // // 								}
// // // 								/************************����˾�����Է������ݴ���***************/	
							}		
							else
							{
								error_report_time[7] = REPORT_WATCHDOG_TIME;
							}					
							Batton_Break_flag = 0;
            }
        }
    }

		if (charge_month != Dtae_Month)                                //��طŵ� 
    {
        Dtae_Month = charge_month;
				/*
					�����طŵ���
				*/	
				save_param(DATE_MONTH,	(uint8_t *)&Dtae_Month,sizeof(Dtae_Month));

        if (CHARGE_OVER() == 0)                                   //��ص�ѹ�㹻��  
        {     
            Flag_Charge = 1;                                      //�ŵ��־λ��1 
            for(t = 70;t < time_read[6]+100;t++) //��������ݵ��������
            {
                if(Is_Leap_Year(1900 + t))
                {
                    temp += 31622400;//�����������
                }
                else 
                {
                    temp += 31536000; //ƽ���������
                }
            }
            time_read[5] -= 1;//��
            for(t = 0;t < time_read[5];t++) //��ǰ���·ݵ����������
            {
                temp += (u32)mon_table[t] * 86400;//�·����������
                if(Is_Leap_Year(time_read[6] + 1900) && (t == 1))
                {
                    temp += 86400;//����2�·�����һ���������
                }
            }
            temp += (u32)((time_read[4]-1) * 86400);//��ǰ�����ڵ����������
            temp += (u32) (time_read[2] * 3600);//Сʱ������
            temp += (u32) (time_read[1] * 60); //����������
            temp += time_read[0];  //�������Ӽ���ȥ            
            Charge_Para.Start_Time = temp;

						/*
							�����طŵ���
						*/	
						save_param(START_CHARGE_TIME,	(uint8_t *)&Charge_Para.Start_Time,sizeof(Charge_Para.Start_Time));
						
						//��ʱ2min
						Alram_Time_Data[1] = (time_read[1] + 1);	
						if(Alram_Time_Data[1] >= 60)					
						{
							Alram_Time_Data[2] = time_read[2] + 1;
							Alram_Time_Data[2] = Alram_Time_Data[2] /10*16 + Alram_Time_Data[2]%10;	//�����ݿ��ж�����ʱ��Ϊ10����
							Alram_Time_Data[1] = Alram_Time_Data[1] - 60;	//дʱ��оƬ����ֵʱӦת��Ϊ16����
						}
						else
						{
								Alram_Time_Data[2] = time_read[2]/10*16 + time_read[2]%10;
						}
						Alram_Time_Data[1] = Alram_Time_Data[1]/10*16 + Alram_Time_Data[1]%10;
						Alram_Time_Data[0] = time_read[0]/10*16 + time_read[0]%10;
						Alram_Time_Data[3] = time_read[4]/10*16 + time_read[4]%10;
						
						os_mut_wait(I2c_Mut_lm,0xFFFF);   //  I2C_lm
						I2C_Write_Alarm(Alram_Time_Data);		//ʱ��д��ʱ��оƬ���ӼĴ���   
						I2C_Write_Alarm_Register(0x05);			//ʹ��ʱ��оƬ�����ж����		 
						os_mut_release(I2c_Mut_lm);	
						
						test_batton_flag = 1;
						
            CHARGE_ON();                                          //��ʼ�ŵ�   
        }
    }
		
		check_bat_count++;
    if ((Flag_Charge!=1)&&(check_bat_count ==20*60*60*2))          //ÿ�����һ�����غû����,�Ҳ����ڷŵ�״̬
    {
				check_bat_count = 0;
			  //��ʱ2min
			  Alram_Time_Data[1] = (time_read[1] + 1);	
				if(Alram_Time_Data[1] >= 60)					
				{
					Alram_Time_Data[2] = time_read[2] + 1;
					Alram_Time_Data[2] = Alram_Time_Data[2] /10*16 + Alram_Time_Data[2]%10;	//�����ݿ��ж�����ʱ��Ϊ10����
					Alram_Time_Data[1] = Alram_Time_Data[1] - 60;	//дʱ��оƬ����ֵʱӦת��Ϊ16����
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
				I2C_Write_Alarm(Alram_Time_Data);		//ʱ��д��ʱ��оƬ���ӼĴ���   
				I2C_Write_Alarm_Register(0x05);			//ʹ��ʱ��оƬ�����ж����		 
				os_mut_release(I2c_Mut_lm);	
				
				test_batton_flag = 1;
				CHARGE_ON();                                          //��ʼ�ŵ�   
		}		
}
