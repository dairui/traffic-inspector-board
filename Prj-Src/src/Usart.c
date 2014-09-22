#include "include.h"
//月份数据表,用于公历日期转换星期
const u8 table_week[12]={1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6}; //月修正数据表
//平年的月份表
const u8 mon_table[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};
//======================================================================
// 函数名称：
// USART_Tx(u8 data)
//			 
//说明：USART1数据发送
//======================================================================
void USART_Tx_Byte(USART_TypeDef* USARTx, u16 data)
{           
    USART_SendData(USARTx, data);
    
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);    //等待发送完成

    USART_ClearFlag(USARTx, USART_FLAG_TC);
}

//======================================================================
// 函数名称：
// USART1_Tx_nByte(u8 data)
//			 
//说明：USART1数据发送多个数据
//======================================================================
void USART_Tx_nByte(USART_TypeDef* USARTx, u8 *address, u16 n)
{   
		u16 i ;
    for(i=0; i<n; i++)
    {
        USART_Tx_Byte(USARTx, address[i]);
    }
}

//======================================================================
// 函数名称：
// USART_Tx_Display(uc8 *address)
//			 
//说明：发送文本
//======================================================================
void USART_Tx_Display(uc8 *address)
{    
    USART_Tx_String(USART2, address);
    USART_Tx_Byte(USART2, 10);
    //Delay_ms(5);
    os_dly_wait(50);
    USART_Tx_Byte(USART2, 10);
    //Delay_ms(5);
    os_dly_wait(50);
}

//======================================================================
// 函数名称：
// USART1_Tx_String(u8 data)
//			 
//说明：USART1发送字符串
//======================================================================
void USART_Tx_String(USART_TypeDef* USARTx, uc8 *address)
{
    u8 *str = (u8 *)address;

    if (str == NULL)
    {
        return;
    }

    while(*str != '\0')
    {
        USART_Tx_Byte(USARTx, *str);
        str++;
    }
}


/*======================================================================
函数名称：
USART_Protocol_Explain(void)
返回值: 0: 信息正确
        X: 错误编码 
说明：USART数据接收协议解析
======================================================================*/
u8 USART_Protocol_Explain(USART_DATA *p_usart_controlblk)
{
	u8 flag       = 0;
	u8 i          = 0;
	u8 index      = 0;
	u8 j          = 0;
	u8 data_count = 0;
	u8 temp       = 0;

	////添加地址查询代码(受到数据为90 D2 42 01 D2 43 01)////
	if((p_usart_controlblk->rx_count == 7)&&(p_usart_controlblk->usart_rx[0] == 0x90)&&
	(p_usart_controlblk->usart_rx[1] == 0xD2)&&(p_usart_controlblk->usart_rx[2] == 0x42)&&
	(p_usart_controlblk->usart_rx[3] == 0x01)&&(p_usart_controlblk->usart_rx[4] == 0xD2)&&
	(p_usart_controlblk->usart_rx[5] == 0x43)&&(p_usart_controlblk->usart_rx[6] == 0x01))
	{
			return 5;
	}
			
	if ((p_usart_controlblk->rx_count==4) && ((p_usart_controlblk->usart_rx[0]&0xF0)!=0x80))    //心跳报文+地址报文
	{  
			switch (p_usart_controlblk->usart_rx[0])
			{
					case 0xB1:
							Protocol_ControlBlk.operation_type = REPORT;
							Protocol_ControlBlk.data[0].ID     = 0xB1;
							break;
					case 0xBC:
							Protocol_ControlBlk.operation_type = REPORT;
							Protocol_ControlBlk.data[0].ID     = 0xBC;
							break;
					default:
							flag = 5;   //其他错误
			}
			return flag;
	}
	
	Protocol_ControlBlk.count_target   = ((p_usart_controlblk->usart_rx[0]&0x70) >> 4) + 1;  //对象个数
	Protocol_ControlBlk.operation_type = (p_usart_controlblk->usart_rx[0]&0x0F);             //操作类型

	index = 1;
	for (i=0; i<Protocol_ControlBlk.count_target; i++)
	{
			Protocol_ControlBlk.data[i].ID          = p_usart_controlblk->usart_rx[index++];                    //对象标识
			Protocol_ControlBlk.data[i].count_index = (p_usart_controlblk->usart_rx[index]&0xC0) >> 6;          //索引个数
			Protocol_ControlBlk.data[i].sub_target  = (p_usart_controlblk->usart_rx[index++]&0x3F);             //子对象

			for (j=0; j<Protocol_ControlBlk.data[i].count_index; j++)                                        //索引解析   
			{
					Protocol_ControlBlk.data[i].sub_index[j] = p_usart_controlblk->usart_rx[index++];
			}

			//设置
			if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //读取设置数据
			{
					switch(Protocol_ControlBlk.data[i].ID)
					{
							case 0x86:          //公共时间
									data_count = 4;
									break;
							case 0x87:          //标准时间
									data_count = 0;
									break; 
							case 0x8D:          //调度配置表
									data_count = 1 + p_usart_controlblk->usart_rx[index]*9;
									break;
							case 0x8E:          //时段配置表
									data_count = 2 + p_usart_controlblk->usart_rx[index]*8;
									break; 
							case 0x88:          //本地时间
									data_count = 4;
									break; 
							case 0x92:          //事件日志表
								//	data_count = ;
									flag = 5;
							    return flag;                  
							case 0x96:          //相位输出状态表
									data_count = 0;
									break;
							case 0x97:          //相位冲突表
									data_count = 0;
									break;
							case 0xAA:          //允许远程控制实体激活信号机的某些功能
									data_count = 1;
									break;                    
							case 0xB0:          //通道参数表
									data_count = 0;
									break;                    
							case 0xB1:          //通道输出状态表
									data_count = 0;
									break;  
							case 0xC0:          //方案配置表
									data_count = 1 + p_usart_controlblk->usart_rx[index]*5;
									break;   
							case 0xC1:          //阶段配置表 从C1 00 后面截取数据                   
									//data_count = 2 + p_usart_controlblk->usart_rx[index]*p_usart_controlblk->usart_rx[index+1]*8;  
									data_count = p_usart_controlblk->usart_rx[index+1]*8;          //防止编译警告
									data_count *= p_usart_controlblk->usart_rx[index];
									data_count += 2;
									break;   
							case 0xD2:          //信号机地址参数 从C4 00 后面截取数据   
									temp        = 1 + 1 + 1 + p_usart_controlblk->usart_rx[index+2];       //第二条内容之前的数据个数
									data_count  = 1 + 1 + 1 + p_usart_controlblk->usart_rx[index+2];       //第一条内容数据个数
									data_count += 1 + p_usart_controlblk->usart_rx[index+temp];            //第二条内容数据个数
									data_count += 5*5 + 3 + 7;                                          //第三条和之后的数据个数    
									break;
							case 0xCA:          //信号灯亮度参数
									data_count = 0;
									break;                    
							case 0xCB:          //交流电源电压
									data_count = 0;
									break;  
							case 0xCD:          //交流电源频率
									data_count = 0;
									break;
							case 0xCE:          //后备电池参数
									data_count = 0;
									break;
							case 0xCF:          //信号灯亮度参数standard
									data_count = 0;
									break;
					}  
					
					for (j=0; j<data_count; j++)
					{
							Protocol_ControlBlk.data[i].data[j] = p_usart_controlblk->usart_rx[index++];                           //协议数据 
					} 
			}
			//上报应答
			else if (Protocol_ControlBlk.operation_type == REPORT_REPLAY)
			{
					index += 0;
			}
			
	}

	//数据校验
	if (index > p_usart_controlblk->rx_count)                                                              
	{
			flag = 1;                         //太长            
			return flag;
	}
	else if (index < p_usart_controlblk->rx_count)
	{
			flag = 4;                         //太短
			return flag;
	}
	
	if (Protocol_ControlBlk.count_target == 0)                                                           //对象个数校验
	{
			flag = 5;                         //其他错误
			return flag;
	}

	if (Protocol_ControlBlk.operation_type > 8)                                                         //操作类型校验
	{
			flag = 2;                         //类型错误
			return flag;
	}

	for (j=0; j<Protocol_ControlBlk.count_target; j++)                                                   //对象ID校验   
	{
			if ((Protocol_ControlBlk.data[j].ID < 0x81) || (Protocol_ControlBlk.data[j].ID > 0xdd))
			{
					flag = 5;                     //其他错误
					return flag;
			}
	}

	return flag;
}

/*======================================================================
函数名称：
USART_Protocol_Communication(void)

说明：信号机通信协议
======================================================================*/
void USART_Protocol_Communication(USART_DATA *p_usart_controlblk)
{
    u8 flag  = 0;
    u8 temp  = 0;
    u8 i     = 0;
		u8	*msg_send						= NULL;
	
		u8  return_for_addr[38] = {0x94, 0xD2, 0x42,0x01};                  //心跳+地址

    Flag_Communication = 1;        
    
    flag = USART_Protocol_Explain(p_usart_controlblk);      //协议数据解析
    if(flag>0)
    {  
			p_usart_controlblk->usart_tx[0]    = 0x86;
			p_usart_controlblk->usart_tx[1]    = flag;
			p_usart_controlblk->usart_tx[2]    = 0;
			p_usart_controlblk->tx_count       = 3;
			p_usart_controlblk->flag_tx_enable = 1;
    }
    else
    {   
        //上位机命令
        if ((Protocol_ControlBlk.operation_type != REPORT) && (Protocol_ControlBlk.operation_type != REPORT_REPLAY))
        {
            if (Protocol_ControlBlk.operation_type == SET_NO_REPLAY)
            {
                p_usart_controlblk->flag_tx_enable = 0;        //禁止发送
                Flag_Communication = 0;
                p_usart_controlblk->tx_count = 0;
                p_usart_controlblk->rx_count = 0; 
								/************************写日志--以太网修改参数*********************************/
								tsk_lock ();  			 
								write_log_wr_fifo[write_log_wr_index][0] =1;			
								write_log_wr_fifo[write_log_wr_index][1] = LOG_SET_ETH;
								write_log_wr_fifo[write_log_wr_index][2] = 0xD2;
								write_log_wr_fifo[write_log_wr_index][3] = 0x00;
								write_log_wr_fifo[write_log_wr_index][4] = 0x00;
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
            }
            else
            {
                p_usart_controlblk->flag_tx_enable = 1;        //允许发送
            }

            if (Protocol_ControlBlk.operation_type == INQUERY)
            {
                temp = REPLAY_INQUERY;    //查询应答
            }
            else
            {
                temp = REPLAY_SET;        //设置应答
								/************************写日志--以太网修改参数*********************************/
								tsk_lock ();  			 
								write_log_wr_fifo[write_log_wr_index][0] =1;			
								write_log_wr_fifo[write_log_wr_index][1] = LOG_SET_ETH;
								write_log_wr_fifo[write_log_wr_index][2] = 0xD2;
								write_log_wr_fifo[write_log_wr_index][3] = 0;
								write_log_wr_fifo[write_log_wr_index][4] = 0;
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
            }
            
            temp  |= 0x80 | ((Protocol_ControlBlk.count_target-1) << 4);        
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;

            for (i=0; i<Protocol_ControlBlk.count_target; i++)
            {
                switch(Protocol_ControlBlk.data[i].ID)
                {
                    case 0x86:          //公共时间
                        USART_Protocol_GlobalTime(i,p_usart_controlblk);
                        break;
                    case 0x88:          //本地时间
                        USART_Protocol_LocalTime(i,p_usart_controlblk);    
                        break;
                    case 0x92:          //查询日志表
                       USART_Protocol_NoteRequry(i,p_usart_controlblk);  
// 											 if(USART_Protocol_NoteRequry(i,p_usart_controlblk) != 0)
// 												 {
// 												 }
// 												 else
// 												 {													 
// 													 p_usart_controlblk->usart_tx[0]    = 5;//wrong 
// 													 p_usart_controlblk->usart_tx[1]    = 0;
// 													 p_usart_controlblk->tx_count       = 2;
//                            p_usart_controlblk->flag_tx_enable = 1;
// 												 }										
                        break;
										case 0x97:          //	相位冲突表									
												USART_Protocol_Phase_CollisionList(i,p_usart_controlblk);
                        break;
                    case 0xAA:          //允许远程控制实体激活信号机的某些功能
                        USART_Protocol_PromoteControl(i,p_usart_controlblk);
                        break;
                    case 0xB0:          //通道参数表
                        USART_Protocol_ParamterList(i,p_usart_controlblk);
                        break;                    
                    case 0xB1:          //通道输出状态表
                        USART_Protocol_ParamterOutputList(i,p_usart_controlblk);
                        break;       
                    case 0xC1:          //阶段配置表 
                        USART_Protocol_MachineStageSet(i,p_usart_controlblk);
                        break;                 
                    case 0xD2:          //信号机地址参数
                        USART_Protocol_MachineAddr(i,p_usart_controlblk);
                        break;                     
                    case 0xCA:          //信号灯亮度参数
                        USART_Protocol_Lamp_PhyLevel(i,p_usart_controlblk);
                        break; 
                    case 0xCF:          //信号灯亮度参数
                        USART_Protocol_STDLamp_PhyLevel(i,p_usart_controlblk);
                        break;                    
                    case 0xCB:          //交流电源电压
                        USART_Protocol_Volatage_Power(i,p_usart_controlblk);
                        break;  
                    case 0xCD:          //交流电源频率
                        USART_Protocol_Frequence_Power(i,p_usart_controlblk);
                        break;
                    case 0xCE:          //后备电池参数
                        USART_Protocol_Para_Charge(i,p_usart_controlblk);
                        break;   
                    default:
                    {
                        p_usart_controlblk->flag_tx_enable = 0;
                        Flag_Communication = 0;
                        p_usart_controlblk->tx_count = 0;
                        p_usart_controlblk->rx_count = 0; 
                    }
                }
            }  
            if(p_usart_controlblk->flag_tx_enable == 1)
                Host_Communication_flag = 1;
        }
        else if (Protocol_ControlBlk.operation_type == REPORT_REPLAY)
        {
					p_usart_controlblk->flag_tx_enable = 0;//
					
					if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xD1)
					  &&(Protocol_ControlBlk.count_target==2))//冲突
					{
						error_report_answer[0] = 1;
						error_report_answer[3] = 1;
					}
					else if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xCF)
					  &&(Protocol_ControlBlk.count_target==2))//灯故障
					{
						error_report_answer[1] = 1;
						error_report_answer[2] = 1;
					}
					else if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xCB)
					  &&(Protocol_ControlBlk.count_target==2))//灯故障
					{
						error_report_answer[6] = 1;
					}
					else if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xCE)
					  &&(Protocol_ControlBlk.count_target==2))//灯故障
					{
						error_report_answer[7] = 1;
					}
					else if(Protocol_ControlBlk.data[0].ID==0xD0
					  &&(Protocol_ControlBlk.count_target==1))//灯故障
					{
						error_report_answer[4] = 1;
						error_report_answer[5] = 1;
					}
				
					Flag_Communication = 0;
					p_usart_controlblk->tx_count = 0;
					p_usart_controlblk->rx_count = 0; 
        }
        else if (Protocol_ControlBlk.operation_type == REPORT)
        {   
            p_usart_controlblk->flag_tx_enable = 1;            //????
            switch(Protocol_ControlBlk.data[0].ID)
            {    
                case 0xB1:          //上位机查询ID
//                     USART_Protocol_ID_Report(0);
                    break;
                case 0xBC:          //上位机网络心跳认证
                    USART_Protocol_Network_Report(0,p_usart_controlblk);
                    break; 
            }
        }
				
    }    
    
}

/*======================================================================
函数名称：
USART_Protocol_Time(void)

说明：时间 读取、设定
======================================================================*/
void USART_Protocol_GlobalTime(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
    u8 time_set[7] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00};    //2000.1.1.周一 00:00:00  
    u8  time_read[7] = {0};  
    u16  daycnt = 0;
    u16  temp1 = 0;
    u16 t;

		u8	*msg_send						= NULL;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {
			temp = 0;
			for (j=0; j<4; j++)
			{               
					temp = temp << 8;
					temp |= Protocol_ControlBlk.data[i].data[j];   //前面字节在高位         
			}
			GlobalTime = temp;
			GlobalTime += 8 * 60 * 60;                         //8个小时时差 
			temp = GlobalTime / 86400;  //得到天数(秒钟数对应的)
	    if(daycnt != temp)//超过一天了
			{   
				daycnt = temp;
				temp1 = 1970;   //从1970年开始
				while(temp >= 365)
				{
					if(Is_Leap_Year(temp1))  //是闰年
					{
						if(temp >= 366)
						{
							temp -= 366;  //闰年的秒钟数
						}
						else 
						{
							temp1++;
							break;
						}
					} 
					else
					{
						temp -= 365; //平年
					}
					temp1++;
				}
				time_set[6] = temp1 - 1970;  //得到年份
				temp1 = 0;
				while(temp >= 28) //超过了一个月
				{
					if(Is_Leap_Year(time_set[6] + 1970) && (temp1 == 1))//当年是不是闰年/2月份
					{
						if(temp >= 29)
							temp -= 29;  //闰年的秒钟数
						else 
							break;
					}
					else
					{
						if(temp >= mon_table[temp1])
							temp -= mon_table[temp1];  //平年
						else 
							break;
					}
					temp1++;
				}
				time_set[5] = temp1 + 1;  //得到月份
				time_set[4] = temp + 1;    //得到日期
			}
			temp = GlobalTime % 86400; //得到秒钟数
			time_set[2] = temp / 3600; //小时
			time_set[1] = (temp % 3600) / 60; //分钟
			time_set[0] = (temp % 3600) % 60; //秒钟
			time_set[3] = RTC_Get_Week((u16)(time_set[6] + 2000),time_set[5],time_set[4]);//获取星期
			time_set[0] = time_set[0] / 10 *16 + time_set[0] %10;
			time_set[1] = time_set[1] / 10 *16 + time_set[1] %10;
			time_set[2] = time_set[2] / 10 *16 + time_set[2] %10;
			time_set[3] = time_set[3] / 10 *16 + time_set[3] %10;
			time_set[4] = time_set[4] / 10 *16 + time_set[4] %10;
			time_set[5] = time_set[5] / 10 *16 + time_set[5] %10;
											
			time_set[6] = (time_set[6] - 30) / 10 *16 + time_set[6] %10;//2000年后
		
      os_mut_wait(I2c_Mut_lm,0xFFFF);
      I2C_Write_DS3231(time_set); 					//  I2C_lm   有待改动的	
			os_mut_release(I2c_Mut_lm);    

      if(1)/*设置时间写日志*/
			{
// 				last_log_type = ALARM_LOG_TYPE_2;
				tsk_lock ();  			 
				write_log_wr_fifo[write_log_wr_index][0] =1;			
				write_log_wr_fifo[write_log_wr_index][1] = LOG_SET_TIME;
				write_log_wr_fifo[write_log_wr_index][2] = 0x86;
				write_log_wr_fifo[write_log_wr_index][3] = 0;
				write_log_wr_fifo[write_log_wr_index][4] = 0;
				write_log_wr_fifo[write_log_wr_index][5] = (0);						
				if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
				if (++write_log_counter  == WRITE_LOG_SIZE)
				{
					write_log_counter=1;
					write_log_buffer_overflow=__TRUE;
				}
				tsk_unlock (); 			
				os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	
			/************************写日志*********************************/
			}		         
    
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象
    }
    else                                                                                                        //读取        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象

        temp = 0;
				os_mut_wait(I2c_Mut_lm,0xFFFF);
				I2C_Read_DS3231(time_read);					//  I2C_lm   有待改动的	
				os_mut_release(I2c_Mut_lm);	
               
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
        GlobalTime= temp;
        GlobalTime -= 8 * 60 * 60; 
        temp = GlobalTime;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0xFF000000) >> 24);  //高位在前面字节
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x00FF0000) >> 16);  
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x0000FF00) >> 8);   
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x000000FF) >> 0);  
    }
}

//======================================================================
// 函数名称：
// USART_Protocol_Time(void)
// 
// 说明：本地时间 读取、设定
//======================================================================
void USART_Protocol_LocalTime(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
    u8 time_set[7] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00};    //2000.1.1.周一 00:00:00      
    u8  time_read[7] = {0};  
    u16 t;
    u16  daycnt = 0;
    u16  temp1 = 0;
    //平年的月份表
		const u8 mon_table[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};

		u8	*msg_send						= NULL;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {
			temp = 0;
			for (j=0; j<4; j++)
			{               
					temp = temp << 8;
					temp |= Protocol_ControlBlk.data[i].data[j];   //前面字节在高位         
			}
			LocalTime = temp;//秒数
			
			temp = LocalTime / 86400;  //得到天数(秒钟数对应的)
	    if(daycnt != temp)//超过一天了
			{   
				daycnt = temp;
				temp1 = 1970;   //从1970年开始
				while(temp >= 365)
				{
					if(Is_Leap_Year(temp1))  //是闰年
					{
						if(temp >= 366)
						{
							temp -= 366;  //闰年的秒钟数
						}
						else 
						{
							temp1++;
							break;
						}
					} 
					else
					{
						temp -= 365; //平年
					}
					temp1++;
				}
				time_set[6] = temp1 - 1970;  //得到年份
				temp1 = 0;
				while(temp >= 28) //超过了一个月
				{
					if(Is_Leap_Year(time_set[6] + 1970) && (temp1 == 1))//当年是不是闰年/2月份
					{
						if(temp >= 29)
							temp -= 29;  //闰年的秒钟数
						else 
							break;
					}
					else
					{
						if(temp >= mon_table[temp1])
							temp -= mon_table[temp1];  //平年
						else 
							break;
					}
					temp1++;
				}
				time_set[5] = temp1 + 1;  //得到月份
				time_set[4] = temp + 1;    //得到日期
			}
			temp = LocalTime % 86400; //得到秒钟数
			time_set[2] = temp / 3600; //小时
			time_set[1] = (temp % 3600) / 60; //分钟
			time_set[0] = (temp % 3600) % 60; //秒钟
			time_set[3] = RTC_Get_Week((u16)(time_set[6] + 2000),time_set[5],time_set[4]);//获取星期
			time_set[0] = time_set[0] / 10 *16 + time_set[0] %10;
			time_set[1] = time_set[1] / 10 *16 + time_set[1] %10;
			time_set[2] = time_set[2] / 10 *16 + time_set[2] %10;
			time_set[3] = time_set[3] / 10 *16 + time_set[3] %10;
			time_set[4] = time_set[4] / 10 *16 + time_set[4] %10;
			time_set[5] = time_set[5] / 10 *16 + time_set[5] %10;
			
			time_set[6] = (time_set[6] - 30) / 10 *16 + time_set[6] %10;//2000年后
					
			os_mut_wait(I2c_Mut_lm,0xFFFF);
      I2C_Write_DS3231(time_set); 					//  I2C_lm   有待改动的	
			os_mut_release(I2c_Mut_lm);	      

      if(1)/*设置时间写日志*/
			{
// 				last_log_type = ALARM_LOG_TYPE_2;
				tsk_lock ();  			 
				write_log_wr_fifo[write_log_wr_index][0] =1;			
				write_log_wr_fifo[write_log_wr_index][1] = LOG_SET_TIME;
				write_log_wr_fifo[write_log_wr_index][2] = 0x88;
				write_log_wr_fifo[write_log_wr_index][3] = 0;
				write_log_wr_fifo[write_log_wr_index][4] = 0;
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
			}	          
    
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象
    }
    else                                                                                                        //读取        
    {
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象

			temp = 0;
			
			os_mut_wait(I2c_Mut_lm,0xFFFF);
			I2C_Read_DS3231(time_read);					//  I2C_lm   有待改动的	
			os_mut_release(I2c_Mut_lm);	
			
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
			//debug
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
			
			LocalTime = temp;
			
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0xFF000000) >> 24);  //高位在前面字节
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x00FF0000) >> 16);  
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x0000FF00) >> 8);   
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x000000FF) >> 0);  
    }
}
/*======================================================================
函数名称：
USART_Protocol_Phase_ParamterList(void)

说明：通道参数表
======================================================================*/
void USART_Protocol_ParamterList(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
		u8 lamp_use_count = 0;
		u16 lamp_use_index = 0;
	
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    {   
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象 
        
				//根据Flag_LampUse计算出使用的通道	
        //temp = Channel_Max;
				lamp_use_index = p_usart_controlblk->tx_count;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //最大行数 = 信号机的类型，4相位或者8相位

				os_mut_wait(table_global_mut,0xFFFF);
				temp = Flag_LampUse;        
				os_mut_release(table_global_mut);
			
        for (j=0; j<LAMP_NUMBER/3; j++)
        {
					if(temp & 0x07)
					{
						lamp_use_count++;
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = j+ 1;//通道号					
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = j+ 1;//信号源= 通道号
					
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;//通道状态
						if(temp & 0x07 != 0x07)			//行人通道
						{
							p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 3;//控制类型
						}
						else
						{
							p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 2;//控制类型
						}
					}
					temp = temp>>3;
        }
				p_usart_controlblk->usart_tx[lamp_use_index]=lamp_use_count;
    }
}


/*=====================================================================
函数名称：void USART_Protocol_NoteRequry((u8 i,USART_DATA *p_usart_controlblk)
说明：日志查询
=====================================================================*/
U32  USART_Protocol_NoteRequry(u8 i,USART_DATA *p_usart_controlblk)
{
	u32  j    = 0;
	u32  k    = 0;
	u8  line_bytes_temp = 0;
	u8  ID_temp = 0x92;
	u32 No_line_temp = 0;
	u32 read_count = 0;
	u32 Index1_temp = 0;
	
	u32 Flag_addr_temp = 0;
	u32 Data_addr_temp = 0;

	if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
	{
		
	}
	else if (Protocol_ControlBlk.operation_type == INQUERY)                                                       //查询                                               //查询
	{
		p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = Protocol_ControlBlk.data[i].ID;            //对象标识
		p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (Protocol_ControlBlk.data[i].count_index << 6) 
																																	 | Protocol_ControlBlk.data[i].sub_target;  //索引数子对象 
		p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = Protocol_ControlBlk.data[i].sub_index[0];
		Index1_temp=Protocol_ControlBlk.data[i].sub_index[0];
		
		Flag_addr_temp 		= Object_table[ID_temp-0x81].Flag_Addr;     //标识地址	
		Data_addr_temp		= Object_table[ID_temp-0x81].Data_Addr;     //数据地址	
		Data_addr_temp++;
		if(Rd_u8(Flag_addr_temp++) == TAB_OK_FLAG )
		{
			line_bytes_temp  = Object_table[ID_temp-0X81].Line_leng;	   //获得行字节数	
			No_line_temp = ((Index1_temp-1)*TAB_92_S_NUMBER) + 1;                    // 从M=1开始读取  
			
			Data_addr_temp += ((No_line_temp-1)*(line_bytes_temp));		   //跳过前面N-1行的地址		
			Flag_addr_temp += No_line_temp;
			
			for(j=0; j<TAB_92_S_NUMBER; j++)
			{		 
				if(Rd_u8(Flag_addr_temp++)== LINE_OK_FLAG)	//获得行有效标志位
				{
					for(k=0;k<line_bytes_temp;k++)
					{										
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = Rd_u8(Data_addr_temp++);
						read_count++;
					}																
				}
			}
		}
		else
		{
			return (0);
		}
	}
	if(read_count != 0)
	{
		p_usart_controlblk->flag_tx_enable = 1;
	}
	return (read_count);
}
/*======================================================================
函数名称：
USART_Protocol_Phase_CollisionList(void)

说明：相位冲突表 读取、设定
======================================================================*/
void USART_Protocol_Phase_CollisionList(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
		u8 j=0,count =0;

		/*相位冲突表*/	
		u32		phase_clash_table[PHASE_CLASH_STATE_NUMBER]	=	{0};
	
		read_param(PHASE_CONFLICT_LIST,	(uint8_t *)phase_clash_table,sizeof(u32)*PHASE_CLASH_STATE_NUMBER);
	
		for(j=0;j<PHASE_CLASH_STATE_NUMBER;j++)
		{
			if(phase_clash_table[j]!=0)
			{
				count++;
			}
			else
			{
				break;
			}
		}
		
		if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    { 
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象  

			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)count;    //最大行数
	
			//相位冲突表
			for (j=0; j<count; j++)
			{
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = j+1;//行号
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>24);//数据 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>16);//数据 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>8);//数据 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>0);//数据 
			}
    }
}


/*======================================================================
函数名称：
USART_Protocol_Phase_ParamterOutputList(void)

说明：通道输出状态表
======================================================================*/
void USART_Protocol_ParamterOutputList(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
    u8  k    = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    { 
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象  

        temp = PASSAGE_ParaOutputList_MaxLine;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //最大行数
    
				//通道输出表 0xB1
				os_mut_wait(table_global_mut,0xFFFF);
        for (j=0; j<PASSAGE_ParaOutputList_MaxLine; j++)
				{
					for (k=0; k<4; k++)
					{
						temp = Passage_ParaOutputList[j][k];
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;//数据 
					}
        }
				os_mut_release(table_global_mut);
    }
}
/*======================================================================
函数名称：
USART_Protocol_MachineSet_4(u8 i)

说明：信号机阶段配置表
======================================================================*/
void USART_Protocol_MachineStageSet(u8 i,USART_DATA *p_usart_controlblk)
{
    u8  j, k,count;
    u8 index[8] = {0};
    u32 temp,temp1;
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {
    }
    else    //读取        
    {
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象
			temp = 1;                                                             //配时表数      //对象个数                                
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;            
        
			os_mut_wait(table_global_mut,0xFFFF);
			
			count = current_stage_num;															//相位个数、阶段行数
    	p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = count;     

			for (j=0; j<count; j++)
			{
				temp = 1;                                             //配时方案号
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = j + 1;                            										//阶段号                                                            
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = (stage_time_table[j].relPhase>>8)&0xFF;              												//放行相位高字节  (最多8相位，高字节为0)                                                      
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = stage_time_table[j].relPhase&0xFF;  
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

					//绿灯时间
				temp = stage_time_table[j].stGreenTime;
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

			 //黄灯时间
				temp = stage_time_table[j].stYellowTime;
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				//红灯时间   
				temp = stage_time_table[j].stRedTime;         
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = 0;                                                               //阶段选项参数 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
			}
			os_mut_release(table_global_mut);		          
    }
}
/*======================================================================
函数名称：
USART_Protocol_Lamp_PhyLevel(void)

说明：灯故障状态   读取、设定
======================================================================*/
void USART_Protocol_Lamp_PhyLevel(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u16 lamp_current_temp[LAMP_NUMBER]={0};

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    {   
			/*绿黄红转化为红黄绿*/
			
				/*全局电流大小数组，以便查询上报*/
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
			
        temp = REPORT;
        if (Protocol_ControlBlk.operation_type == REPORT)
        {
            temp  |= 0x80 | ((Protocol_ControlBlk.count_target-1) << 4);        
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;
        }
    
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象

        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 11;          //行数
        
        for (i=0; i<LAMP_NUMBER; i++)
        {   
            if ((i%3) == 0)
            {   
                temp = (i / 3) + 1;
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //通道数           
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据  高字节在前
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据                
            }
            else
            {
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据                
            }
        }
				
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;
    }
}
void USART_Protocol_STDLamp_PhyLevel(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u16 lamp_current_temp[LAMP_NUMBER]={0};

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    {   
			/*绿黄红转化为红黄绿*/
			
				/*全局电流大小数组，以便查询上报*/
				os_mut_wait(table_global_mut,0xFFFF);
				if(1)
				{
					lamp_current_temp[0]=standard_lamp_current[2];
					lamp_current_temp[1]=standard_lamp_current[1];
					lamp_current_temp[2]=standard_lamp_current[0];
					lamp_current_temp[3]=standard_lamp_current[5];
					lamp_current_temp[4]=standard_lamp_current[4];
					lamp_current_temp[5]=standard_lamp_current[3];
					lamp_current_temp[6]=standard_lamp_current[8];
					lamp_current_temp[7]=standard_lamp_current[7];
					lamp_current_temp[8]=standard_lamp_current[6];
					lamp_current_temp[9]=standard_lamp_current[11];
					lamp_current_temp[10]=standard_lamp_current[10];
					lamp_current_temp[11]=standard_lamp_current[9];
					
					lamp_current_temp[12]=standard_lamp_current[14];
					lamp_current_temp[13]=standard_lamp_current[13];
					lamp_current_temp[14]=standard_lamp_current[12];
					lamp_current_temp[15]=standard_lamp_current[17];
					lamp_current_temp[16]=standard_lamp_current[16];
					lamp_current_temp[17]=standard_lamp_current[15];
					lamp_current_temp[18]=standard_lamp_current[20];
					lamp_current_temp[19]=standard_lamp_current[19];
					lamp_current_temp[20]=standard_lamp_current[18];
					lamp_current_temp[21]=standard_lamp_current[23];
					lamp_current_temp[22]=standard_lamp_current[22];
					lamp_current_temp[23]=standard_lamp_current[21];
					lamp_current_temp[24]=standard_lamp_current[26];
					lamp_current_temp[25]=standard_lamp_current[25];
					lamp_current_temp[26]=standard_lamp_current[24];
					lamp_current_temp[27]=standard_lamp_current[29];
					lamp_current_temp[28]=standard_lamp_current[28];
					lamp_current_temp[29]=standard_lamp_current[27];
					lamp_current_temp[30]=standard_lamp_current[31];
					lamp_current_temp[31]=standard_lamp_current[30];
				}
				os_mut_release(table_global_mut);
			
        temp = REPORT;
        if (Protocol_ControlBlk.operation_type == REPORT)
        {
            temp  |= 0x80 | ((Protocol_ControlBlk.count_target-1) << 4);        
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;
        }
    
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象

        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 11;          //行数
        
        for (i=0; i<LAMP_NUMBER; i++)
        {   
            if ((i%3) == 0)
            {   
                temp = (i / 3) + 1;
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //通道数           
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据  高字节在前
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据                
            }
            else
            {
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //灯电流数据                
            }
        }
				
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;
    }
}
/*======================================================================
函数名称：
USART_Protocol_Volatage_Power(void)

说明：交流电源电压 读取、设定
======================================================================*/
void USART_Protocol_Volatage_Power(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象
				
				os_mut_wait(table_global_mut,0xFFFF);
			
        temp = (u8)((Pwr_Vol&0xFF00)>>8);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

        temp = (u8)((Pwr_Vol&0x00FF)>>0);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
			
				os_mut_release(table_global_mut);
                
    }
}

/*======================================================================
函数名称：
USART_Protocol_Frequence_Power(void)

说明：交流电源频率  读取、设定
======================================================================*/
void USART_Protocol_Frequence_Power(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象
				
				os_mut_wait(table_global_mut,0xFFFF);
			
        temp = (u8)((Pwr_Freq&0xFF00)>>8);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

        temp = (u8)((Pwr_Freq&0x00FF)>>0);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp; 

				os_mut_release(table_global_mut);			
    }
}

/*======================================================================
函数名称：
USART_Protocol_Para_Charge(void)

说明：后备电池参数   读取、设定
======================================================================*/
void USART_Protocol_Para_Charge(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {

    }
    else                                                                                                        //读取        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象

				os_mut_wait(table_global_mut,0xFFFF);
        temp = Para_Charge;
				os_mut_release(table_global_mut);		
			
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0xFF000000) >> 24);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x00FF0000) >> 16);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x0000FF00) >> 8);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x000000FF) >> 0);
    }
}
/*======================================================================
函数名称：
USART_Protocol_MachineAddr(void)

说明：信号机地址+ETHERNET参数
======================================================================*/
void USART_Protocol_MachineAddr(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u16 index = 0;
    int8_t j;
	
		os_mut_wait(table_global_mut,0xFFFF);
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {
        machine_addr_len = Protocol_ControlBlk.data[i].data[2];                           //地址长度       
        for (j=0; j<machine_addr_len; j++)
        {
            temp = Protocol_ControlBlk.data[i].data[3+j];                     //地址数据
            machine_addr[j] = (u8)temp;
        }
        index = 1 + 1 + Protocol_ControlBlk.data[i].data[2];                  //第一条内容长度
        index = index + 1; 
        machine_type_len  = Protocol_ControlBlk.data[i].data[index];                      //信号机类型长度       
        for (j=0; j<machine_type_len; j++)
        {
            temp = Protocol_ControlBlk.data[i].data[index+1+j];              //信号机类型
            machine_type[j] = (u8)temp;  
        }
				save_param(MACHINE_ADDR_ADDR,		&machine_addr_len,						sizeof(machine_addr_len));
				save_param(MACHINE_ADDR_ADDR+1,		machine_addr,						sizeof(machine_addr));
				save_param(MACHINE_TYPE_ADDR,		&machine_type_len,						sizeof(machine_type_len));
				save_param(MACHINE_TYPE_ADDR+1,		machine_type,						sizeof(machine_type));
				//-------------------------------------------	
				
        index  += machine_type_len + 1;                   //前两条内容长度,第三条第一个数据索引
        index  += 1;
							
				for(j=3;j>=0;j--)
				{
					temp  = Protocol_ControlBlk.data[i].data[index++];                     //IP
					localm[NETIF_ETH].IpAdr[j] = temp;        
				}

        index  += 1;                                                         //NET_MASK   

				for(j=3;j>=0;j--)
				{
					temp  = Protocol_ControlBlk.data[i].data[index++];                     //IP
					localm[NETIF_ETH].NetMask[j] = temp;        
				}
        
        index  += 1;                                                         //Gate_way   

				for(j=3;j>=0;j--)
				{
					temp  = Protocol_ControlBlk.data[i].data[index++];                     //IP
					localm[NETIF_ETH].DefGW[j] = temp;        
				}
        
        index  += 1;                                                         //DNS   

				for(j=3;j>=0;j--)
				{
					temp  = Protocol_ControlBlk.data[i].data[index++];                     //IP
					localm[NETIF_ETH].PriDNS[j] = temp;        
				}

        index  += 1;                                                         //Remote_ip   

				for(j=3;j>=0;j--)
				{
					temp  = Protocol_ControlBlk.data[i].data[index++];                     //IP
					server_ip[j] = temp;        
				}

        index  += 1;                                                         //port   

        temp  = Protocol_ControlBlk.data[i].data[index++];
        server_port = 0x00000000 | (temp<<8);
        temp  = Protocol_ControlBlk.data[i].data[index++];
        server_port |= temp; 

        index += 1;
				
				for(j=5;j>=0;j--)
				{
					temp  = Protocol_ControlBlk.data[i].data[index++];                     //IP
					mac_adr[j] = temp;        
				}
       
				save_param(LOCAL_IP_ADDR,				LocM.IpAdr,				sizeof(LocM.IpAdr));
				save_param(LOCAL_MASK_ADDR,			LocM.NetMask,			sizeof(LocM.NetMask));
				save_param(LOCAL_GATEWAY_ADDR,	LocM.DefGW,				sizeof(LocM.DefGW));
				save_param(LOCAL_DNS_ADDR,			LocM.PriDNS,			sizeof(LocM.PriDNS));
				save_param(LOCAL_PORT_ADDR,			(uint8_t *)&local_port,	sizeof(local_port));
				save_param(LOCAL_MAC_ADDR,			mac_adr,								sizeof(mac_adr));
				save_param(SERVER_IP_ADDR,			server_ip,							sizeof(server_ip));
				save_param(SERVER_PORT_ADDR,		(uint8_t *)&server_port,	sizeof(server_port));	
				
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //索引数子对象
    }
    else                                                                                                        //读取        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //索引数子对象

        temp = 0x01;                                                         //行数 
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;        

        temp = 0x01;                                                          //地址 
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        temp = machine_addr_len;       
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for (i=0; i<machine_addr_len; i++)
        {
            temp = machine_addr[i];
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp; 
        }

        temp = machine_type_len;                             //信号机型号
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for (i=0; i<machine_type_len; i++)
        {
            temp = machine_type[i];
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp; 
        }   

        temp = 0x04;                                                            //IP
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for(j=3;j>=0;j--)
				{
					temp  = localm[NETIF_ETH].IpAdr[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				}
				
        temp = 0x04;                                                            //掩码
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
				for(j=3;j>=0;j--)
				{
					temp  = localm[NETIF_ETH].NetMask[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				}
        
        temp = 0x04;                                                            //网关
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
				for(j=3;j>=0;j--)
				{
					temp  = localm[NETIF_ETH].DefGW[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				}
				
        temp = 0x04;                                                            //DNS
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for(j=3;j>=0;j--)
				{
					temp  = localm[NETIF_ETH].PriDNS[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				}

        temp = 0x04;                                                            //主机IP
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for(j=3;j>=0;j--)
				{
					temp  = server_ip[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				}
				
        temp = 0x02;                                                            //port
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        temp = (u8)((server_port & 0xFF00)>>8);        
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp; 
        temp = (u8)(server_port & 0x00FF);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

        temp = 0x06;                                                           //MAC
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for(j=5;j>=0;j--)
				{
					temp  = mac_adr[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				} 
    }
		os_mut_release(table_global_mut);
}
/*======================================================================
函数名称：
USART_Protocol_PromoteControl(void)

说明：
======================================================================*/
void USART_Protocol_PromoteControl(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8 j;
		u8 *msg_send = NULL;
    
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //设置
    {
        temp = 0;
        for (j=0; j<1; j++)
        {               
            temp = temp << 8;
            temp |= Protocol_ControlBlk.data[i].data[j];   //前面字节在高位         
        }

        if (temp&0x00000002)        //重启路口信号机
        {
					MACHINE_ON();
					os_dly_wait(5000);
					MACHINE_OFF();

					/************************写日志*********************************/
// 					tsk_lock ();  			 
// 					write_log_wr_fifo[write_log_wr_index][0] =1;			
// 					write_log_wr_fifo[write_log_wr_index][1] = LOG_START_MACHINE;
// 					write_log_wr_fifo[write_log_wr_index][2] = 0xA7;
// 					write_log_wr_fifo[write_log_wr_index][3] = 0x03;
// 					write_log_wr_fifo[write_log_wr_index][4] = 1;
// 					write_log_wr_fifo[write_log_wr_index][5] = 0;						
// 					if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
// 					if (++write_log_counter  == WRITE_LOG_SIZE)
// 					{
// 						write_log_counter=1;
// 						write_log_buffer_overflow=__TRUE;
// 					}
// 					tsk_unlock (); 			
// 					os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	           
        }

        if (temp&0x00000004)        //重启板子
        {
            /*  软件复位 */
					NVIC_SystemReset();	         
        }

        if (temp&0x00000008)        //重新学习
        {
					start_learning_flag = 1;
        }     

        if (temp&0x00000010)        //远程更新程序
        {
            BKP_WriteBackupRegister(BKP_DR11, 1);               //更改寄存器
            BKP_WriteBackupRegister(BKP_DR10 , 69);		//写入端口号      
            /*  软件复位 */
					
					NVIC_SystemReset();	                              //重启板子
        }   

        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //索引数子对象
    }
    else   //不支持查询       
    {
        p_usart_controlblk->usart_tx[0] = 0x86;    //对象标识
        p_usart_controlblk->usart_tx[1] = 0x05;  
        p_usart_controlblk->usart_tx[2] = 0x00;   
				p_usart_controlblk->tx_count = 3;
    }
}

// // // // //======================================================================
// // // // // 函数名称：
// // // // // USART_Protocol_ID_Report(void)
// // // // // 
// // // // // 说明：上位机查询ID
// // // // //======================================================================
// // // // void USART_Protocol_ID_Report(u8 i,USART_DATA *p_usart_controlblk)
// // // // {
// // // //     u32 temp = 0;
// // // //     u8  j    = 0;

// // // //     if (1)       
// // // //     {   
// // // //         p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xB9;
// // // //         p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xD8;
// // // //         p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xBC;
// // // //         p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xFC;
// // // //     
// // // //         for (j=0; j<12; j++)            //96位ID                                
// // // //         { 
// // // //             temp = Machine_Parameter.Node_ID[j];
// // // //             p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //对象标识            
// // // //         }
// // // //     }
// // // // }

/*======================================================================
函数名称：
USART_Protocol_Network_Report(void)

说明：网络状态心跳报文
======================================================================*/
void USART_Protocol_Network_Report(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;

    if (1)       
    {  
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xB9;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xD8;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xBC;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0xFC;

        for (j=0; j<12-machine_addr_len; j++)                   //路口长度不满12字节补0
        {
            temp = '0';
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    
        }
    
        for (j=0; j<machine_addr_len; j++)                      //路口名称
        {   
            temp = machine_addr[j];
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    
        }
    }
}

/******************************************************************************
函数名：uint8_t Is_Leap_Year(uint16_t year)
参数：uint16_t year
返回值：1,表示是闰年。 0，表示是平年
说明：判断是否为闰年
闰年：能被4整除但不能被100整除的为闰年， 能被400整除的为闰年
*******************************************************************************/
u8 Is_Leap_Year(u16 year)
{
  if((year % 4) == 0)  //如果能被4整除
  {
    if((year % 100) == 0) //如果能被100整除
    {
      if((year % 400) == 0)  //如果能被400整除
      {
        return 1;  //说明是闰年
      }
      else
      {
        return 0; //说明是平年
      }
    }
    else
    {
      return 1; ////说明是闰年
    }
  }
  else
  {
    return 0; //说明是平年
  }
}



/*******************************************************************************
注：因为从公元1582年10月15日(星期五)才开始使用现行公历(格里高利历法)，
计算公元1583年及其以后某年的某月某日是星期几，先把年份前两位数a除以4求余数，
把余数乘以-2后，加上5，所得的和称之为“世纪余数”；
再把年份末两位数b除以4求其商的整数部分，然后加上年份末两位数b，
所得的和除以7之后所得余数称之为“年届余数”；再把每个月对应的一个数字，
这个数字称之为“月余数”，除闰年1月份和2月份所对应的数字分别是0和3之外，
平时1至12月份所对应的数字分别是1，4，4。0，2，5。0，3，6。1，4，6；
最后把所求这一日的“日期数”加上月份对应的“月余数”
再加上“世纪余数”和“年届余数”，求得这四个数字的和，再除以7，所得余数是几，
这一天便是星期几，余数为0时则是星期日。
*******************************************************************************/
u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
  u8 yearH, yearL;
  u8 temp1, temp2, temp3; 
  yearH = year / 100; 
  yearL = year % 100;
  temp1 = (yearH % 4) * 2 + 5;  //世纪余数
  temp2 = ((yearL / 4) + yearL) % 7;  //年届余数
  temp3 = temp1 + temp2 + day + table_week[month-1];
  if(((yearL % 4) == 0) && (month < 3))
    temp3--;
  return((u8)(temp3 % 7));
}

