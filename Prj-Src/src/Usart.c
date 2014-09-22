#include "include.h"
//�·����ݱ�,���ڹ�������ת������
const u8 table_week[12]={1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6}; //���������ݱ�
//ƽ����·ݱ�
const u8 mon_table[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};
//======================================================================
// �������ƣ�
// USART_Tx(u8 data)
//			 
//˵����USART1���ݷ���
//======================================================================
void USART_Tx_Byte(USART_TypeDef* USARTx, u16 data)
{           
    USART_SendData(USARTx, data);
    
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);    //�ȴ��������

    USART_ClearFlag(USARTx, USART_FLAG_TC);
}

//======================================================================
// �������ƣ�
// USART1_Tx_nByte(u8 data)
//			 
//˵����USART1���ݷ��Ͷ������
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
// �������ƣ�
// USART_Tx_Display(uc8 *address)
//			 
//˵���������ı�
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
// �������ƣ�
// USART1_Tx_String(u8 data)
//			 
//˵����USART1�����ַ���
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
�������ƣ�
USART_Protocol_Explain(void)
����ֵ: 0: ��Ϣ��ȷ
        X: ������� 
˵����USART���ݽ���Э�����
======================================================================*/
u8 USART_Protocol_Explain(USART_DATA *p_usart_controlblk)
{
	u8 flag       = 0;
	u8 i          = 0;
	u8 index      = 0;
	u8 j          = 0;
	u8 data_count = 0;
	u8 temp       = 0;

	////��ӵ�ַ��ѯ����(�ܵ�����Ϊ90 D2 42 01 D2 43 01)////
	if((p_usart_controlblk->rx_count == 7)&&(p_usart_controlblk->usart_rx[0] == 0x90)&&
	(p_usart_controlblk->usart_rx[1] == 0xD2)&&(p_usart_controlblk->usart_rx[2] == 0x42)&&
	(p_usart_controlblk->usart_rx[3] == 0x01)&&(p_usart_controlblk->usart_rx[4] == 0xD2)&&
	(p_usart_controlblk->usart_rx[5] == 0x43)&&(p_usart_controlblk->usart_rx[6] == 0x01))
	{
			return 5;
	}
			
	if ((p_usart_controlblk->rx_count==4) && ((p_usart_controlblk->usart_rx[0]&0xF0)!=0x80))    //��������+��ַ����
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
							flag = 5;   //��������
			}
			return flag;
	}
	
	Protocol_ControlBlk.count_target   = ((p_usart_controlblk->usart_rx[0]&0x70) >> 4) + 1;  //�������
	Protocol_ControlBlk.operation_type = (p_usart_controlblk->usart_rx[0]&0x0F);             //��������

	index = 1;
	for (i=0; i<Protocol_ControlBlk.count_target; i++)
	{
			Protocol_ControlBlk.data[i].ID          = p_usart_controlblk->usart_rx[index++];                    //�����ʶ
			Protocol_ControlBlk.data[i].count_index = (p_usart_controlblk->usart_rx[index]&0xC0) >> 6;          //��������
			Protocol_ControlBlk.data[i].sub_target  = (p_usart_controlblk->usart_rx[index++]&0x3F);             //�Ӷ���

			for (j=0; j<Protocol_ControlBlk.data[i].count_index; j++)                                        //��������   
			{
					Protocol_ControlBlk.data[i].sub_index[j] = p_usart_controlblk->usart_rx[index++];
			}

			//����
			if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //��ȡ��������
			{
					switch(Protocol_ControlBlk.data[i].ID)
					{
							case 0x86:          //����ʱ��
									data_count = 4;
									break;
							case 0x87:          //��׼ʱ��
									data_count = 0;
									break; 
							case 0x8D:          //�������ñ�
									data_count = 1 + p_usart_controlblk->usart_rx[index]*9;
									break;
							case 0x8E:          //ʱ�����ñ�
									data_count = 2 + p_usart_controlblk->usart_rx[index]*8;
									break; 
							case 0x88:          //����ʱ��
									data_count = 4;
									break; 
							case 0x92:          //�¼���־��
								//	data_count = ;
									flag = 5;
							    return flag;                  
							case 0x96:          //��λ���״̬��
									data_count = 0;
									break;
							case 0x97:          //��λ��ͻ��
									data_count = 0;
									break;
							case 0xAA:          //����Զ�̿���ʵ�弤���źŻ���ĳЩ����
									data_count = 1;
									break;                    
							case 0xB0:          //ͨ��������
									data_count = 0;
									break;                    
							case 0xB1:          //ͨ�����״̬��
									data_count = 0;
									break;  
							case 0xC0:          //�������ñ�
									data_count = 1 + p_usart_controlblk->usart_rx[index]*5;
									break;   
							case 0xC1:          //�׶����ñ� ��C1 00 �����ȡ����                   
									//data_count = 2 + p_usart_controlblk->usart_rx[index]*p_usart_controlblk->usart_rx[index+1]*8;  
									data_count = p_usart_controlblk->usart_rx[index+1]*8;          //��ֹ���뾯��
									data_count *= p_usart_controlblk->usart_rx[index];
									data_count += 2;
									break;   
							case 0xD2:          //�źŻ���ַ���� ��C4 00 �����ȡ����   
									temp        = 1 + 1 + 1 + p_usart_controlblk->usart_rx[index+2];       //�ڶ�������֮ǰ�����ݸ���
									data_count  = 1 + 1 + 1 + p_usart_controlblk->usart_rx[index+2];       //��һ���������ݸ���
									data_count += 1 + p_usart_controlblk->usart_rx[index+temp];            //�ڶ����������ݸ���
									data_count += 5*5 + 3 + 7;                                          //��������֮������ݸ���    
									break;
							case 0xCA:          //�źŵ����Ȳ���
									data_count = 0;
									break;                    
							case 0xCB:          //������Դ��ѹ
									data_count = 0;
									break;  
							case 0xCD:          //������ԴƵ��
									data_count = 0;
									break;
							case 0xCE:          //�󱸵�ز���
									data_count = 0;
									break;
							case 0xCF:          //�źŵ����Ȳ���standard
									data_count = 0;
									break;
					}  
					
					for (j=0; j<data_count; j++)
					{
							Protocol_ControlBlk.data[i].data[j] = p_usart_controlblk->usart_rx[index++];                           //Э������ 
					} 
			}
			//�ϱ�Ӧ��
			else if (Protocol_ControlBlk.operation_type == REPORT_REPLAY)
			{
					index += 0;
			}
			
	}

	//����У��
	if (index > p_usart_controlblk->rx_count)                                                              
	{
			flag = 1;                         //̫��            
			return flag;
	}
	else if (index < p_usart_controlblk->rx_count)
	{
			flag = 4;                         //̫��
			return flag;
	}
	
	if (Protocol_ControlBlk.count_target == 0)                                                           //�������У��
	{
			flag = 5;                         //��������
			return flag;
	}

	if (Protocol_ControlBlk.operation_type > 8)                                                         //��������У��
	{
			flag = 2;                         //���ʹ���
			return flag;
	}

	for (j=0; j<Protocol_ControlBlk.count_target; j++)                                                   //����IDУ��   
	{
			if ((Protocol_ControlBlk.data[j].ID < 0x81) || (Protocol_ControlBlk.data[j].ID > 0xdd))
			{
					flag = 5;                     //��������
					return flag;
			}
	}

	return flag;
}

/*======================================================================
�������ƣ�
USART_Protocol_Communication(void)

˵�����źŻ�ͨ��Э��
======================================================================*/
void USART_Protocol_Communication(USART_DATA *p_usart_controlblk)
{
    u8 flag  = 0;
    u8 temp  = 0;
    u8 i     = 0;
		u8	*msg_send						= NULL;
	
		u8  return_for_addr[38] = {0x94, 0xD2, 0x42,0x01};                  //����+��ַ

    Flag_Communication = 1;        
    
    flag = USART_Protocol_Explain(p_usart_controlblk);      //Э�����ݽ���
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
        //��λ������
        if ((Protocol_ControlBlk.operation_type != REPORT) && (Protocol_ControlBlk.operation_type != REPORT_REPLAY))
        {
            if (Protocol_ControlBlk.operation_type == SET_NO_REPLAY)
            {
                p_usart_controlblk->flag_tx_enable = 0;        //��ֹ����
                Flag_Communication = 0;
                p_usart_controlblk->tx_count = 0;
                p_usart_controlblk->rx_count = 0; 
								/************************д��־--��̫���޸Ĳ���*********************************/
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
								/************************д��־*********************************/
            }
            else
            {
                p_usart_controlblk->flag_tx_enable = 1;        //������
            }

            if (Protocol_ControlBlk.operation_type == INQUERY)
            {
                temp = REPLAY_INQUERY;    //��ѯӦ��
            }
            else
            {
                temp = REPLAY_SET;        //����Ӧ��
								/************************д��־--��̫���޸Ĳ���*********************************/
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
								/************************д��־*********************************/
            }
            
            temp  |= 0x80 | ((Protocol_ControlBlk.count_target-1) << 4);        
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;

            for (i=0; i<Protocol_ControlBlk.count_target; i++)
            {
                switch(Protocol_ControlBlk.data[i].ID)
                {
                    case 0x86:          //����ʱ��
                        USART_Protocol_GlobalTime(i,p_usart_controlblk);
                        break;
                    case 0x88:          //����ʱ��
                        USART_Protocol_LocalTime(i,p_usart_controlblk);    
                        break;
                    case 0x92:          //��ѯ��־��
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
										case 0x97:          //	��λ��ͻ��									
												USART_Protocol_Phase_CollisionList(i,p_usart_controlblk);
                        break;
                    case 0xAA:          //����Զ�̿���ʵ�弤���źŻ���ĳЩ����
                        USART_Protocol_PromoteControl(i,p_usart_controlblk);
                        break;
                    case 0xB0:          //ͨ��������
                        USART_Protocol_ParamterList(i,p_usart_controlblk);
                        break;                    
                    case 0xB1:          //ͨ�����״̬��
                        USART_Protocol_ParamterOutputList(i,p_usart_controlblk);
                        break;       
                    case 0xC1:          //�׶����ñ� 
                        USART_Protocol_MachineStageSet(i,p_usart_controlblk);
                        break;                 
                    case 0xD2:          //�źŻ���ַ����
                        USART_Protocol_MachineAddr(i,p_usart_controlblk);
                        break;                     
                    case 0xCA:          //�źŵ����Ȳ���
                        USART_Protocol_Lamp_PhyLevel(i,p_usart_controlblk);
                        break; 
                    case 0xCF:          //�źŵ����Ȳ���
                        USART_Protocol_STDLamp_PhyLevel(i,p_usart_controlblk);
                        break;                    
                    case 0xCB:          //������Դ��ѹ
                        USART_Protocol_Volatage_Power(i,p_usart_controlblk);
                        break;  
                    case 0xCD:          //������ԴƵ��
                        USART_Protocol_Frequence_Power(i,p_usart_controlblk);
                        break;
                    case 0xCE:          //�󱸵�ز���
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
					  &&(Protocol_ControlBlk.count_target==2))//��ͻ
					{
						error_report_answer[0] = 1;
						error_report_answer[3] = 1;
					}
					else if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xCF)
					  &&(Protocol_ControlBlk.count_target==2))//�ƹ���
					{
						error_report_answer[1] = 1;
						error_report_answer[2] = 1;
					}
					else if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xCB)
					  &&(Protocol_ControlBlk.count_target==2))//�ƹ���
					{
						error_report_answer[6] = 1;
					}
					else if((Protocol_ControlBlk.data[0].ID==0xD0)
						&&(Protocol_ControlBlk.data[1].ID==0xCE)
					  &&(Protocol_ControlBlk.count_target==2))//�ƹ���
					{
						error_report_answer[7] = 1;
					}
					else if(Protocol_ControlBlk.data[0].ID==0xD0
					  &&(Protocol_ControlBlk.count_target==1))//�ƹ���
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
                case 0xB1:          //��λ����ѯID
//                     USART_Protocol_ID_Report(0);
                    break;
                case 0xBC:          //��λ������������֤
                    USART_Protocol_Network_Report(0,p_usart_controlblk);
                    break; 
            }
        }
				
    }    
    
}

/*======================================================================
�������ƣ�
USART_Protocol_Time(void)

˵����ʱ�� ��ȡ���趨
======================================================================*/
void USART_Protocol_GlobalTime(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
    u8 time_set[7] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00};    //2000.1.1.��һ 00:00:00  
    u8  time_read[7] = {0};  
    u16  daycnt = 0;
    u16  temp1 = 0;
    u16 t;

		u8	*msg_send						= NULL;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {
			temp = 0;
			for (j=0; j<4; j++)
			{               
					temp = temp << 8;
					temp |= Protocol_ControlBlk.data[i].data[j];   //ǰ���ֽ��ڸ�λ         
			}
			GlobalTime = temp;
			GlobalTime += 8 * 60 * 60;                         //8��Сʱʱ�� 
			temp = GlobalTime / 86400;  //�õ�����(��������Ӧ��)
	    if(daycnt != temp)//����һ����
			{   
				daycnt = temp;
				temp1 = 1970;   //��1970�꿪ʼ
				while(temp >= 365)
				{
					if(Is_Leap_Year(temp1))  //������
					{
						if(temp >= 366)
						{
							temp -= 366;  //�����������
						}
						else 
						{
							temp1++;
							break;
						}
					} 
					else
					{
						temp -= 365; //ƽ��
					}
					temp1++;
				}
				time_set[6] = temp1 - 1970;  //�õ����
				temp1 = 0;
				while(temp >= 28) //������һ����
				{
					if(Is_Leap_Year(time_set[6] + 1970) && (temp1 == 1))//�����ǲ�������/2�·�
					{
						if(temp >= 29)
							temp -= 29;  //�����������
						else 
							break;
					}
					else
					{
						if(temp >= mon_table[temp1])
							temp -= mon_table[temp1];  //ƽ��
						else 
							break;
					}
					temp1++;
				}
				time_set[5] = temp1 + 1;  //�õ��·�
				time_set[4] = temp + 1;    //�õ�����
			}
			temp = GlobalTime % 86400; //�õ�������
			time_set[2] = temp / 3600; //Сʱ
			time_set[1] = (temp % 3600) / 60; //����
			time_set[0] = (temp % 3600) % 60; //����
			time_set[3] = RTC_Get_Week((u16)(time_set[6] + 2000),time_set[5],time_set[4]);//��ȡ����
			time_set[0] = time_set[0] / 10 *16 + time_set[0] %10;
			time_set[1] = time_set[1] / 10 *16 + time_set[1] %10;
			time_set[2] = time_set[2] / 10 *16 + time_set[2] %10;
			time_set[3] = time_set[3] / 10 *16 + time_set[3] %10;
			time_set[4] = time_set[4] / 10 *16 + time_set[4] %10;
			time_set[5] = time_set[5] / 10 *16 + time_set[5] %10;
											
			time_set[6] = (time_set[6] - 30) / 10 *16 + time_set[6] %10;//2000���
		
      os_mut_wait(I2c_Mut_lm,0xFFFF);
      I2C_Write_DS3231(time_set); 					//  I2C_lm   �д��Ķ���	
			os_mut_release(I2c_Mut_lm);    

      if(1)/*����ʱ��д��־*/
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
			/************************д��־*********************************/
			}		         
    
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���
    }
    else                                                                                                        //��ȡ        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���

        temp = 0;
				os_mut_wait(I2c_Mut_lm,0xFFFF);
				I2C_Read_DS3231(time_read);					//  I2C_lm   �д��Ķ���	
				os_mut_release(I2c_Mut_lm);	
               
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
        GlobalTime= temp;
        GlobalTime -= 8 * 60 * 60; 
        temp = GlobalTime;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0xFF000000) >> 24);  //��λ��ǰ���ֽ�
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x00FF0000) >> 16);  
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x0000FF00) >> 8);   
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x000000FF) >> 0);  
    }
}

//======================================================================
// �������ƣ�
// USART_Protocol_Time(void)
// 
// ˵��������ʱ�� ��ȡ���趨
//======================================================================
void USART_Protocol_LocalTime(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
    u8 time_set[7] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00};    //2000.1.1.��һ 00:00:00      
    u8  time_read[7] = {0};  
    u16 t;
    u16  daycnt = 0;
    u16  temp1 = 0;
    //ƽ����·ݱ�
		const u8 mon_table[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,31};

		u8	*msg_send						= NULL;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {
			temp = 0;
			for (j=0; j<4; j++)
			{               
					temp = temp << 8;
					temp |= Protocol_ControlBlk.data[i].data[j];   //ǰ���ֽ��ڸ�λ         
			}
			LocalTime = temp;//����
			
			temp = LocalTime / 86400;  //�õ�����(��������Ӧ��)
	    if(daycnt != temp)//����һ����
			{   
				daycnt = temp;
				temp1 = 1970;   //��1970�꿪ʼ
				while(temp >= 365)
				{
					if(Is_Leap_Year(temp1))  //������
					{
						if(temp >= 366)
						{
							temp -= 366;  //�����������
						}
						else 
						{
							temp1++;
							break;
						}
					} 
					else
					{
						temp -= 365; //ƽ��
					}
					temp1++;
				}
				time_set[6] = temp1 - 1970;  //�õ����
				temp1 = 0;
				while(temp >= 28) //������һ����
				{
					if(Is_Leap_Year(time_set[6] + 1970) && (temp1 == 1))//�����ǲ�������/2�·�
					{
						if(temp >= 29)
							temp -= 29;  //�����������
						else 
							break;
					}
					else
					{
						if(temp >= mon_table[temp1])
							temp -= mon_table[temp1];  //ƽ��
						else 
							break;
					}
					temp1++;
				}
				time_set[5] = temp1 + 1;  //�õ��·�
				time_set[4] = temp + 1;    //�õ�����
			}
			temp = LocalTime % 86400; //�õ�������
			time_set[2] = temp / 3600; //Сʱ
			time_set[1] = (temp % 3600) / 60; //����
			time_set[0] = (temp % 3600) % 60; //����
			time_set[3] = RTC_Get_Week((u16)(time_set[6] + 2000),time_set[5],time_set[4]);//��ȡ����
			time_set[0] = time_set[0] / 10 *16 + time_set[0] %10;
			time_set[1] = time_set[1] / 10 *16 + time_set[1] %10;
			time_set[2] = time_set[2] / 10 *16 + time_set[2] %10;
			time_set[3] = time_set[3] / 10 *16 + time_set[3] %10;
			time_set[4] = time_set[4] / 10 *16 + time_set[4] %10;
			time_set[5] = time_set[5] / 10 *16 + time_set[5] %10;
			
			time_set[6] = (time_set[6] - 30) / 10 *16 + time_set[6] %10;//2000���
					
			os_mut_wait(I2c_Mut_lm,0xFFFF);
      I2C_Write_DS3231(time_set); 					//  I2C_lm   �д��Ķ���	
			os_mut_release(I2c_Mut_lm);	      

      if(1)/*����ʱ��д��־*/
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
			/************************д��־*********************************/
			}	          
    
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���
    }
    else                                                                                                        //��ȡ        
    {
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���

			temp = 0;
			
			os_mut_wait(I2c_Mut_lm,0xFFFF);
			I2C_Read_DS3231(time_read);					//  I2C_lm   �д��Ķ���	
			os_mut_release(I2c_Mut_lm);	
			
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
			//debug
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
			
			LocalTime = temp;
			
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0xFF000000) >> 24);  //��λ��ǰ���ֽ�
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x00FF0000) >> 16);  
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x0000FF00) >> 8);   
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)((temp & 0x000000FF) >> 0);  
    }
}
/*======================================================================
�������ƣ�
USART_Protocol_Phase_ParamterList(void)

˵����ͨ��������
======================================================================*/
void USART_Protocol_ParamterList(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
		u8 lamp_use_count = 0;
		u16 lamp_use_index = 0;
	
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    {   
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ��� 
        
				//����Flag_LampUse�����ʹ�õ�ͨ��	
        //temp = Channel_Max;
				lamp_use_index = p_usart_controlblk->tx_count;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //������� = �źŻ������ͣ�4��λ����8��λ

				os_mut_wait(table_global_mut,0xFFFF);
				temp = Flag_LampUse;        
				os_mut_release(table_global_mut);
			
        for (j=0; j<LAMP_NUMBER/3; j++)
        {
					if(temp & 0x07)
					{
						lamp_use_count++;
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = j+ 1;//ͨ����					
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = j+ 1;//�ź�Դ= ͨ����
					
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;//ͨ��״̬
						if(temp & 0x07 != 0x07)			//����ͨ��
						{
							p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 3;//��������
						}
						else
						{
							p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 2;//��������
						}
					}
					temp = temp>>3;
        }
				p_usart_controlblk->usart_tx[lamp_use_index]=lamp_use_count;
    }
}


/*=====================================================================
�������ƣ�void USART_Protocol_NoteRequry((u8 i,USART_DATA *p_usart_controlblk)
˵������־��ѯ
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

	if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
	{
		
	}
	else if (Protocol_ControlBlk.operation_type == INQUERY)                                                       //��ѯ                                               //��ѯ
	{
		p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = Protocol_ControlBlk.data[i].ID;            //�����ʶ
		p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (Protocol_ControlBlk.data[i].count_index << 6) 
																																	 | Protocol_ControlBlk.data[i].sub_target;  //�������Ӷ��� 
		p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = Protocol_ControlBlk.data[i].sub_index[0];
		Index1_temp=Protocol_ControlBlk.data[i].sub_index[0];
		
		Flag_addr_temp 		= Object_table[ID_temp-0x81].Flag_Addr;     //��ʶ��ַ	
		Data_addr_temp		= Object_table[ID_temp-0x81].Data_Addr;     //���ݵ�ַ	
		Data_addr_temp++;
		if(Rd_u8(Flag_addr_temp++) == TAB_OK_FLAG )
		{
			line_bytes_temp  = Object_table[ID_temp-0X81].Line_leng;	   //������ֽ���	
			No_line_temp = ((Index1_temp-1)*TAB_92_S_NUMBER) + 1;                    // ��M=1��ʼ��ȡ  
			
			Data_addr_temp += ((No_line_temp-1)*(line_bytes_temp));		   //����ǰ��N-1�еĵ�ַ		
			Flag_addr_temp += No_line_temp;
			
			for(j=0; j<TAB_92_S_NUMBER; j++)
			{		 
				if(Rd_u8(Flag_addr_temp++)== LINE_OK_FLAG)	//�������Ч��־λ
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
�������ƣ�
USART_Protocol_Phase_CollisionList(void)

˵������λ��ͻ�� ��ȡ���趨
======================================================================*/
void USART_Protocol_Phase_CollisionList(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
		u8 j=0,count =0;

		/*��λ��ͻ��*/	
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
		
		if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    { 
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���  

			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)count;    //�������
	
			//��λ��ͻ��
			for (j=0; j<count; j++)
			{
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = j+1;//�к�
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>24);//���� 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>16);//���� 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>8);//���� 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)(phase_clash_table[j]>>0);//���� 
			}
    }
}


/*======================================================================
�������ƣ�
USART_Protocol_Phase_ParamterOutputList(void)

˵����ͨ�����״̬��
======================================================================*/
void USART_Protocol_ParamterOutputList(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8  j    = 0;
    u8  k    = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    { 
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���  

        temp = PASSAGE_ParaOutputList_MaxLine;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������
    
				//ͨ������� 0xB1
				os_mut_wait(table_global_mut,0xFFFF);
        for (j=0; j<PASSAGE_ParaOutputList_MaxLine; j++)
				{
					for (k=0; k<4; k++)
					{
						temp = Passage_ParaOutputList[j][k];
						p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;//���� 
					}
        }
				os_mut_release(table_global_mut);
    }
}
/*======================================================================
�������ƣ�
USART_Protocol_MachineSet_4(u8 i)

˵�����źŻ��׶����ñ�
======================================================================*/
void USART_Protocol_MachineStageSet(u8 i,USART_DATA *p_usart_controlblk)
{
    u8  j, k,count;
    u8 index[8] = {0};
    u32 temp,temp1;
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {
    }
    else    //��ȡ        
    {
			temp = Protocol_ControlBlk.data[i].ID;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
			temp  = Protocol_ControlBlk.data[i].count_index << 6;
			temp |= Protocol_ControlBlk.data[i].sub_target;
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���
			temp = 1;                                                             //��ʱ����      //�������                                
			p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;            
        
			os_mut_wait(table_global_mut,0xFFFF);
			
			count = current_stage_num;															//��λ�������׶�����
    	p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = count;     

			for (j=0; j<count; j++)
			{
				temp = 1;                                             //��ʱ������
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = j + 1;                            										//�׶κ�                                                            
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = (stage_time_table[j].relPhase>>8)&0xFF;              												//������λ���ֽ�  (���8��λ�����ֽ�Ϊ0)                                                      
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = stage_time_table[j].relPhase&0xFF;  
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

					//�̵�ʱ��
				temp = stage_time_table[j].stGreenTime;
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

			 //�Ƶ�ʱ��
				temp = stage_time_table[j].stYellowTime;
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				//���ʱ��   
				temp = stage_time_table[j].stRedTime;         
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

				temp = 0;                                                               //�׶�ѡ����� 
				p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
			}
			os_mut_release(table_global_mut);		          
    }
}
/*======================================================================
�������ƣ�
USART_Protocol_Lamp_PhyLevel(void)

˵�����ƹ���״̬   ��ȡ���趨
======================================================================*/
void USART_Protocol_Lamp_PhyLevel(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u16 lamp_current_temp[LAMP_NUMBER]={0};

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    {   
			/*�̻ƺ�ת��Ϊ�����*/
			
				/*ȫ�ֵ�����С���飬�Ա��ѯ�ϱ�*/
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
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���

        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 11;          //����
        
        for (i=0; i<LAMP_NUMBER; i++)
        {   
            if ((i%3) == 0)
            {   
                temp = (i / 3) + 1;
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //ͨ����           
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������  ���ֽ���ǰ
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������                
            }
            else
            {
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������                
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

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    {   
			/*�̻ƺ�ת��Ϊ�����*/
			
				/*ȫ�ֵ�����С���飬�Ա��ѯ�ϱ�*/
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
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���

        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 11;          //����
        
        for (i=0; i<LAMP_NUMBER; i++)
        {   
            if ((i%3) == 0)
            {   
                temp = (i / 3) + 1;
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //ͨ����           
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������  ���ֽ���ǰ
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������                
            }
            else
            {
                temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������
                temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
                p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�Ƶ�������                
            }
        }
				
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = 0;
    }
}
/*======================================================================
�������ƣ�
USART_Protocol_Volatage_Power(void)

˵����������Դ��ѹ ��ȡ���趨
======================================================================*/
void USART_Protocol_Volatage_Power(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���
				
				os_mut_wait(table_global_mut,0xFFFF);
			
        temp = (u8)((Pwr_Vol&0xFF00)>>8);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

        temp = (u8)((Pwr_Vol&0x00FF)>>0);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
			
				os_mut_release(table_global_mut);
                
    }
}

/*======================================================================
�������ƣ�
USART_Protocol_Frequence_Power(void)

˵����������ԴƵ��  ��ȡ���趨
======================================================================*/
void USART_Protocol_Frequence_Power(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���
				
				os_mut_wait(table_global_mut,0xFFFF);
			
        temp = (u8)((Pwr_Freq&0xFF00)>>8);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;

        temp = (u8)((Pwr_Freq&0x00FF)>>0);
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp; 

				os_mut_release(table_global_mut);			
    }
}

/*======================================================================
�������ƣ�
USART_Protocol_Para_Charge(void)

˵�����󱸵�ز���   ��ȡ���趨
======================================================================*/
void USART_Protocol_Para_Charge(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;

    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {

    }
    else                                                                                                        //��ȡ        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���

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
�������ƣ�
USART_Protocol_MachineAddr(void)

˵�����źŻ���ַ+ETHERNET����
======================================================================*/
void USART_Protocol_MachineAddr(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u16 index = 0;
    int8_t j;
	
		os_mut_wait(table_global_mut,0xFFFF);
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {
        machine_addr_len = Protocol_ControlBlk.data[i].data[2];                           //��ַ����       
        for (j=0; j<machine_addr_len; j++)
        {
            temp = Protocol_ControlBlk.data[i].data[3+j];                     //��ַ����
            machine_addr[j] = (u8)temp;
        }
        index = 1 + 1 + Protocol_ControlBlk.data[i].data[2];                  //��һ�����ݳ���
        index = index + 1; 
        machine_type_len  = Protocol_ControlBlk.data[i].data[index];                      //�źŻ����ͳ���       
        for (j=0; j<machine_type_len; j++)
        {
            temp = Protocol_ControlBlk.data[i].data[index+1+j];              //�źŻ�����
            machine_type[j] = (u8)temp;  
        }
				save_param(MACHINE_ADDR_ADDR,		&machine_addr_len,						sizeof(machine_addr_len));
				save_param(MACHINE_ADDR_ADDR+1,		machine_addr,						sizeof(machine_addr));
				save_param(MACHINE_TYPE_ADDR,		&machine_type_len,						sizeof(machine_type_len));
				save_param(MACHINE_TYPE_ADDR+1,		machine_type,						sizeof(machine_type));
				//-------------------------------------------	
				
        index  += machine_type_len + 1;                   //ǰ�������ݳ���,��������һ����������
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
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //�������Ӷ���
    }
    else                                                                                                        //��ȡ        
    {
        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;   //�������Ӷ���

        temp = 0x01;                                                         //���� 
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;        

        temp = 0x01;                                                          //��ַ 
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        temp = machine_addr_len;       
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
        for (i=0; i<machine_addr_len; i++)
        {
            temp = machine_addr[i];
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp; 
        }

        temp = machine_type_len;                             //�źŻ��ͺ�
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
				
        temp = 0x04;                                                            //����
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;
				for(j=3;j>=0;j--)
				{
					temp  = localm[NETIF_ETH].NetMask[j];                     //IP
					p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = temp;        
				}
        
        temp = 0x04;                                                            //����
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

        temp = 0x04;                                                            //����IP
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
�������ƣ�
USART_Protocol_PromoteControl(void)

˵����
======================================================================*/
void USART_Protocol_PromoteControl(u8 i,USART_DATA *p_usart_controlblk)
{
    u32 temp = 0;
    u8 j;
		u8 *msg_send = NULL;
    
    if ((Protocol_ControlBlk.operation_type == SET) || (Protocol_ControlBlk.operation_type == SET_NO_REPLAY))   //����
    {
        temp = 0;
        for (j=0; j<1; j++)
        {               
            temp = temp << 8;
            temp |= Protocol_ControlBlk.data[i].data[j];   //ǰ���ֽ��ڸ�λ         
        }

        if (temp&0x00000002)        //����·���źŻ�
        {
					MACHINE_ON();
					os_dly_wait(5000);
					MACHINE_OFF();

					/************************д��־*********************************/
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

        if (temp&0x00000004)        //��������
        {
            /*  �����λ */
					NVIC_SystemReset();	         
        }

        if (temp&0x00000008)        //����ѧϰ
        {
					start_learning_flag = 1;
        }     

        if (temp&0x00000010)        //Զ�̸��³���
        {
            BKP_WriteBackupRegister(BKP_DR11, 1);               //���ļĴ���
            BKP_WriteBackupRegister(BKP_DR10 , 69);		//д��˿ں�      
            /*  �����λ */
					
					NVIC_SystemReset();	                              //��������
        }   

        temp = Protocol_ControlBlk.data[i].ID;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ
        temp  = Protocol_ControlBlk.data[i].count_index << 6;
        temp |= Protocol_ControlBlk.data[i].sub_target;
        p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�������Ӷ���
    }
    else   //��֧�ֲ�ѯ       
    {
        p_usart_controlblk->usart_tx[0] = 0x86;    //�����ʶ
        p_usart_controlblk->usart_tx[1] = 0x05;  
        p_usart_controlblk->usart_tx[2] = 0x00;   
				p_usart_controlblk->tx_count = 3;
    }
}

// // // // //======================================================================
// // // // // �������ƣ�
// // // // // USART_Protocol_ID_Report(void)
// // // // // 
// // // // // ˵������λ����ѯID
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
// // // //         for (j=0; j<12; j++)            //96λID                                
// // // //         { 
// // // //             temp = Machine_Parameter.Node_ID[j];
// // // //             p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    //�����ʶ            
// // // //         }
// // // //     }
// // // // }

/*======================================================================
�������ƣ�
USART_Protocol_Network_Report(void)

˵��������״̬��������
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

        for (j=0; j<12-machine_addr_len; j++)                   //·�ڳ��Ȳ���12�ֽڲ�0
        {
            temp = '0';
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    
        }
    
        for (j=0; j<machine_addr_len; j++)                      //·������
        {   
            temp = machine_addr[j];
            p_usart_controlblk->usart_tx[p_usart_controlblk->tx_count++] = (u8)temp;    
        }
    }
}

/******************************************************************************
��������uint8_t Is_Leap_Year(uint16_t year)
������uint16_t year
����ֵ��1,��ʾ�����ꡣ 0����ʾ��ƽ��
˵�����ж��Ƿ�Ϊ����
���꣺�ܱ�4���������ܱ�100������Ϊ���꣬ �ܱ�400������Ϊ����
*******************************************************************************/
u8 Is_Leap_Year(u16 year)
{
  if((year % 4) == 0)  //����ܱ�4����
  {
    if((year % 100) == 0) //����ܱ�100����
    {
      if((year % 400) == 0)  //����ܱ�400����
      {
        return 1;  //˵��������
      }
      else
      {
        return 0; //˵����ƽ��
      }
    }
    else
    {
      return 1; ////˵��������
    }
  }
  else
  {
    return 0; //˵����ƽ��
  }
}



/*******************************************************************************
ע����Ϊ�ӹ�Ԫ1582��10��15��(������)�ſ�ʼʹ�����й���(�����������)��
���㹫Ԫ1583�꼰���Ժ�ĳ���ĳ��ĳ�������ڼ����Ȱ����ǰ��λ��a����4��������
����������-2�󣬼���5�����õĺͳ�֮Ϊ��������������
�ٰ����ĩ��λ��b����4�����̵��������֣�Ȼ��������ĩ��λ��b��
���õĺͳ���7֮������������֮Ϊ��������������ٰ�ÿ���¶�Ӧ��һ�����֣�
������ֳ�֮Ϊ������������������1�·ݺ�2�·�����Ӧ�����ֱַ���0��3֮�⣬
ƽʱ1��12�·�����Ӧ�����ֱַ���1��4��4��0��2��5��0��3��6��1��4��6��
����������һ�յġ��������������·ݶ�Ӧ�ġ���������
�ټ��ϡ������������͡������������������ĸ����ֵĺͣ��ٳ���7�����������Ǽ���
��һ��������ڼ�������Ϊ0ʱ���������ա�
*******************************************************************************/
u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
  u8 yearH, yearL;
  u8 temp1, temp2, temp3; 
  yearH = year / 100; 
  yearL = year % 100;
  temp1 = (yearH % 4) * 2 + 5;  //��������
  temp2 = ((yearL / 4) + yearL) % 7;  //�������
  temp3 = temp1 + temp2 + day + table_week[month-1];
  if(((yearL % 4) == 0) && (month < 3))
    temp3--;
  return((u8)(temp3 % 7));
}

