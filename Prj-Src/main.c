/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_DEMO.C
 *      Purpose: HTTP Server demo example
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <RTL.h>
#include <Net_Config.h>
//#include <stm32f10x_cl.h>
#include <string.h>
#include "stm32f10x.h"
#include "include.h"


BOOL LEDrun;
BOOL LCDupdate;
BOOL tick;
U32  dhcp_tout;
U8   lcd_text[2][16+1] = {" ",                /* Buffer for LCD text         */
                          "Waiting for DHCP"};

extern LOCALM localm[];                       /* Local Machine Settings      */
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50                        /* DHCP timeout 5 seconds      */

U64 tcp_stack[3000/8];                       /* A bigger stack for tcp_task */
U64 ADC_stack[2000/8];                       /* A bigger stack for ADC_task */
U64 learn_detect_stack[2000/8];              /* A bigger stack for learn_detect_task */
U64 power_detect_stack[800/8];               /* A bigger stack for power_detect_task */
U64 log_write_stack[2000/8];                 /* A bigger stack for log_write_task */
U64 eth_usart_communication_stack[1000/8];     /* A bigger stack for eth_communication_task */
U64 key_stack[1200/8];                        /* A bigger stack for log_write_task */
U64 error_report_stack[800/8];                        /* A bigger stack for log_write_task */

OS_TID t_tcp_task;                      /* assigned task id of task: tcp_task */
OS_TID t_ADC_task;                      /* assigned task id of task: ADC_task */
OS_TID t_learn_detect_task;             /* assigned task id of task: learn_detect_task */
OS_TID t_power_detect_task;             /* assigned task id of task: power_detect_task */
OS_TID t_log_write_task;                /* assigned task id of task: log_write_task */
OS_TID t_eth_usart_communication_task;	/* assigned task id of task: eth_communication_task */
OS_TID t_key_task;                      /* assigned task id of task: log_write_task */
OS_TID t_error_report_task;                   /* assigned task id of task: log_write_task */

OS_MUT I2c_Mut_lm;                      /* 读取时钟互斥量 李雄斌*/
OS_MUT communication_rx_mut;         /* 通讯接收块互斥量 李雄斌*/
OS_MUT table_global_mut;         /* B0/B1/97/电流值表/等的互斥量 李雄斌*/
OS_MUT data_to_comany_mut;         /*给公司发送的数据互斥量 李雄斌*/

OS_SEM eth_send_data_semaphore;
OS_SEM eth_client_sche_semaphore;
// // // OS_SEM eth_MySelf_semaphore;

os_mbx_declare(Write_Log_Mbox, 		20);						//写日志任务
os_mbx_declare(Communication_Mbox,20);						//以太网串口通讯任务
os_mbx_declare(err_report_mbox,20);						//上报任务

 /* Forward references */
 __task void init       (void);
 __task void timer_task (void);
 __task void tcp_task   (void);
 __task void ADC_task   (void);
 __task void learn_detect_task   (void);
 __task void power_detect_task   (void);
 __task void log_write_task   (void);
 __task void eth_usart_communication_task(void);
 __task void key_task   (void);
 __task void error_report_task(void);


extern void Device_init(void);
/*--------------------------- init ------------------------------------------*/
__task void init (void) {
  /* Add System initialisation code here */ 

	u8 *msg_send = NULL;
	u8 index_temp = 0;
	uint32_t *pTemp;
	
	pTemp=(uint32_t *)&bit_band_flag_1;  
	*pTemp=0x0;	pTemp=NULL;  //标志位初始化

  /* Set the clocks. */
  SystemInit();
	
  RCC->APB2ENR |= 0x0000027D;
	
	/* Check if the system has resumed from SRT reset */
  if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)  /* Software  set */
  {


  }	
	
  /* Check if the system has resumed from IWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)  /* IWDGRST flag set */
  {

    /* Clear reset flags */
    RCC_ClearFlag();
		printf("=====================看门狗复位===============\n");
		printf("=====================看门狗复位===============\n");
		printf("=====================看门狗复位===============\n");
  }
  /* Configure UART2 for 115200 baud. */
  AFIO->MAPR |= 0x00000008;
  GPIOD->CRL &= 0xF00FFFFF;
  GPIOD->CRL |= 0x04B00000;

  RCC->APB1ENR |= 0x00020000;
  USART2->BRR = 0x0135;
  USART2->CR3 = 0x0000;
  USART2->CR2 = 0x0000;
  USART2->CR1 = 0x200C;
	
	Device_init();

  /* Initialize Tasks */
  os_tsk_prio_self (100);

	os_mut_init (&I2c_Mut_lm);  							//by_lxb读取时钟函数互斥量
	os_mut_init (&communication_rx_mut);
	os_mut_init (&table_global_mut);      /* B0/B1/97/电流值表/等的互斥量 李雄斌*/
	os_mut_init (&data_to_comany_mut);
	
	os_sem_init	(eth_send_data_semaphore,0);
	os_sem_init	(eth_client_sche_semaphore,0);
// // // 	os_sem_init	(eth_MySelf_semaphore,0);
			
	os_mbx_init (Write_Log_Mbox,sizeof(Write_Log_Mbox));
	os_mbx_init (Communication_Mbox,sizeof(Communication_Mbox));
	os_mbx_init (err_report_mbox,sizeof(err_report_mbox));
	
	os_tsk_create 			(timer_task, 30);
		
	t_ADC_task		=os_tsk_create_user 	(ADC_task, 		10, &ADC_stack, 		sizeof(ADC_stack));
	t_tcp_task			=os_tsk_create_user 	(tcp_task, 		0, 	&tcp_stack, 		sizeof(tcp_stack));
	t_learn_detect_task	=os_tsk_create_user 	(learn_detect_task,	12, 	&learn_detect_stack, 	sizeof(learn_detect_stack));
	t_power_detect_task	=os_tsk_create_user 	(power_detect_task,	17, 	&power_detect_stack, 	sizeof(power_detect_stack));
	t_log_write_task=os_tsk_create_user 	(log_write_task,	8, 	&log_write_stack, 	sizeof(log_write_stack));
	t_eth_usart_communication_task=os_tsk_create_user(eth_usart_communication_task,	18, 	&eth_usart_communication_stack, 	sizeof(eth_usart_communication_stack));
	t_key_task	=os_tsk_create_user 	(key_task,	20, 	&key_stack, 	sizeof(key_stack));
	t_error_report_task	=os_tsk_create_user 	(error_report_task,	5, 	&error_report_stack, 	sizeof(error_report_stack));
		
	
	/************************写日志--开机*********************************/
	tsk_lock ();  			 
	write_log_wr_fifo[write_log_wr_index][0] =1;			
	write_log_wr_fifo[write_log_wr_index][1] = LOG_START_MACHINE;
	write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
	write_log_wr_fifo[write_log_wr_index][3] = 0x01;
	write_log_wr_fifo[write_log_wr_index][4] = 00;
	write_log_wr_fifo[write_log_wr_index][5] = 00;						
	if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
	if (++write_log_counter  == WRITE_LOG_SIZE)
	{
		write_log_counter=1;
		write_log_buffer_overflow=__TRUE;
	}
	tsk_unlock (); 			
	os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	  
	/************************写日志--开机*********************************/
	/************************上报异常*********************************/
	if(1)/*上报*/
	{
		//添加发送报警数据
		tsk_lock ();  			//  disables task switching 
		index_temp=0;
		write_Client_fifo[write_Client_wr_index][index_temp++] = 4;	
		write_Client_fifo[write_Client_wr_index][index_temp++] = 0x83;		
		write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;		
		write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
		write_Client_fifo[write_Client_wr_index][index_temp++] = MACHINE_START;		//开机
		
		if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
		if (++write_Client_counter  == WRITE_Client_SIZE)
		{
			write_Client_counter=1;
			write_Client_buffer_overflow=__TRUE;
		};
		tsk_unlock (); 			//  enable  task switching 	
		os_sem_send(eth_client_sche_semaphore);
// // // 		/************************给公司监测电脑发送数据处理***************/
// // // 		if(1)
// // // 		{
// // // 			tsk_lock ();  			//  disables task switching 
// // // 			write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 			write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 			write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 			write_MySelf_fifo[write_MySelf_wr_index][3] = MACHINE_START;  //故障号

// // // 			if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 			if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 			{
// // // 				write_MySelf_counter=1;
// // // 				write_MySelf_buffer_overflow=__TRUE;
// // // 			};
// // // 			tsk_unlock (); 			//  enable  task switching 
// // // 			os_sem_send(eth_MySelf_semaphore);
// // // 		}
// // // 		/************************给公司监测电脑发送数据处理***************/
	}
	/************************上报异常*********************************/

	os_tsk_delete_self();
}


__task void timer_task (void) {
  /* System tick timer task */
  os_itv_set (10);
  while (1) {
    timer_tick ();
    tick = __TRUE;
    os_itv_wait ();
  }
}


u8 adc_result = 0;//交流电检测

__task void error_report_task(void)
{
	u8 index_temp = 0;
	u8 i;
	u8 report_once_count[ERROR_COUNT]={0};
	
	U8 *msg_recvd = NULL;
	U8	reportalarm_flag[ERROR_COUNT] = {0U};
	U8  recvd_type = 0xFF;
	U8 send_tcp_flag = 0;

	while(1)
	{
		/*
		byte0=消息类型
		0:定时器1小时触发
		1:通道冲突报警触发
		2:相位冲突报警触发
		3:灯故障报警触发
		4:灯物理故障报警触发
		5:学习报警触发
		6:3G故障报警触发
		7:交流电故障报警触发
		8:放电时间故障触发
		*/
		os_mbx_wait(err_report_mbox, (void *)&msg_recvd, 0xffff);
		recvd_type = *msg_recvd;
		
		if(recvd_type == 0)	//定时器触发
		{
			if(reportalarm_flag[0] == 1)//通道冲突上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD1;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][3];	
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][3];		

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
				
			}
			/************************上报异常*********************************/
			if(reportalarm_flag[1] == 1)//相位冲突上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD1;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][3];	
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][3];		

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
				
			}
			/************************上报异常*********************************/		
			if(reportalarm_flag[2] == 1)//灯故障上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCF;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][3];	
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][3];		

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
			}
			/************************上报异常*********************************/
			if(reportalarm_flag[3] == 1)//灯物理故障上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCF;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][3];	
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][0];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][1];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][2];
				write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][3];		

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
			}
			/************************上报异常*********************************/
			if(reportalarm_flag[4] == 1)//学习上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 4;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x83;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = LEARN_FAIL;		//多次学习不成功

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
			}
			/************************上报异常*********************************/
			if(reportalarm_flag[5] == 1)//3G故障上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 4;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x83;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = POWER3G_ERROR;		//3G电源故障	

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
			}
			/************************上报异常*********************************/
			if(reportalarm_flag[6] == 1)//交流电故障上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp = 0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 8;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = POWER_ERROR;		//电源故障		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCB;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;	
				if(adc_result == 1)//频率问题
				{
					write_Client_fifo[write_Client_wr_index][index_temp++] = 0;		
					write_Client_fifo[write_Client_wr_index][index_temp++] = 0;		//电源故障	
				}
				else if(adc_result == 2)//交流电压问题
				{
					write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)(Pwr_Vol>>8);		
					write_Client_fifo[write_Client_wr_index][index_temp++] = (Pwr_Vol&0xff);	
				}
				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
			}
			/************************上报异常*********************************/
			if(reportalarm_flag[7] == 1)//放电时间故障上报
			{
				//添加发送报警数据
				tsk_lock ();  			//  disables task switching 
				index_temp	= 0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 10;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = CHARGE_ERROR;		//蓄电池放电故障
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCE;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0xFF000000) >> 24);	
				write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0x00FF0000) >> 16);	
				write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0x0000FF00) >> 8);	
				write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0x000000FF) >> 0);		

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				send_tcp_flag = 1;
			}
			/************************上报异常*********************************/
			
			/* 清除报警标志 */
			for(i=0;i<ERROR_COUNT;i++)
			{
				reportalarm_flag[i] = 0;
				
				if(error_report_time[i]>0)
				{
					error_report_time[i]--;
				}
			}
		}
		else if((recvd_type>0)&&(recvd_type<=ERROR_COUNT))//报警触发
		{
			switch (recvd_type)
			{
				case 1:
				{
					if(reportalarm_flag[0] == 0)//通道冲突上报
					{
						reportalarm_flag[0] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp=0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD1;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][3];	
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][3];		

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;					
					}												
				}
					break;			
				case 2:
				{
					if(reportalarm_flag[1] == 0)//相位冲突上报
					{
						reportalarm_flag[1] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp=0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD1;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[0][3];	
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_clash_data[1][3];		

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;				
					}												
				}
					break;									
				case 3:
				{
					if(reportalarm_flag[2] == 0)//灯故障上报
					{
						reportalarm_flag[2] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp=0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCF;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][3];	
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][3];		

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;
					}
				}
					break;			
				case 4:
				{
					if(reportalarm_flag[3] == 0)//灯物理故障上报
					{
						reportalarm_flag[3] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp=0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 14;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = PASSAGE_EXCEPTION;		//通道故障
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCF;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		//通道冲突
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[0][3];	
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][0];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][1];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][2];
						write_Client_fifo[write_Client_wr_index][index_temp++] = passage_broken_data[1][3];		

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;
					}									
				}
					break;				
				case 5:
				{
					if(reportalarm_flag[4] == 0)//学习上报
					{
						reportalarm_flag[4] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp=0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 4;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x83;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = LEARN_FAIL;		//多次学习不成功

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;
					}											
				}
					break;				
				case 6:
				{
					if(reportalarm_flag[5] == 0)//3G故障上报
					{
						reportalarm_flag[5] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp=0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 4;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x83;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = POWER3G_ERROR;		//3G电源故障	

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;
					}
					
				}
					break;				
				case 7:
				{
					if(reportalarm_flag[6] == 0)//交流电故障上报
					{
						reportalarm_flag[6] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp = 0;
						write_Client_fifo[write_Client_wr_index][index_temp++] = 8;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = POWER_ERROR;		//电源故障		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCB;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;	
						if(adc_result == 1)//频率问题
						{
							write_Client_fifo[write_Client_wr_index][index_temp++] = 0;		
							write_Client_fifo[write_Client_wr_index][index_temp++] = 0;		//电源故障	
						}
						else if(adc_result == 2)//交流电压问题
						{
							write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)(Pwr_Vol>>8);		
							write_Client_fifo[write_Client_wr_index][index_temp++] = (Pwr_Vol&0xff);	
						}
						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;
					}
														
				}
					break;				
				case 8:
				{
					if(reportalarm_flag[7] == 0)//放电时间故障上报
					{
						reportalarm_flag[7] = 1;
						
						//添加发送报警数据
						tsk_lock ();  			//  disables task switching 
						index_temp	= 0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 10;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x93;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xD0;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = CHARGE_ERROR;		//蓄电池放电故障
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0xCE;	
						write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
						write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0xFF000000) >> 24);	
						write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0x00FF0000) >> 16);	
						write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0x0000FF00) >> 8);	
						write_Client_fifo[write_Client_wr_index][index_temp++] = (u8)((Para_Charge & 0x000000FF) >> 0);		

						if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
						if (++write_Client_counter  == WRITE_Client_SIZE)
						{
							write_Client_counter=1;
							write_Client_buffer_overflow=__TRUE;
						};
						tsk_unlock (); 			//  enable  task switching 	
						send_tcp_flag = 1;
					}				
					break;
				}
				
			default:
					break;
			}
		}
		if(send_tcp_flag == 1)
		{
			send_tcp_flag = 0;
			
			os_sem_send(eth_client_sche_semaphore);
		}
	}
}

extern volatile uint8_t DMA1_ADC_FLAG;//DMA1完成标志
extern void DMAReConfig(void);

/*	4个ADC_INPUT,一个周期内每个INPUT转换TRANSFORM_TIMES次。	*/
__IO uint16_t ADC_groups_data_once[4*TRANSFORM_TIMES];  //默认 4*TRANSFORM_TIMES =48
	
uint16_t	filter_one_value[32]={0};  //一级滤波得到的数据，现在使用的全局变量，先用着，以后再来优化

__task void ADC_task (void) {
  /* ADC task */
	/* 对于模拟开关来说，0---7路  */
	uint8_t 	channel_count=0;  
	
	/* 从采集到的4个INPUT的数据中提取1个INPUT的临时变量 */
	uint16_t  ADC_group_data[TRANSFORM_TIMES];
	
	uint16_t	value_temp=0;	

	uint32_t  i;
	uint32_t  j;
	uint32_t  k;

	
	AD_CS_ALL_ON();	
	AD_Channel0();	
	os_dly_wait(1);	//1ms	
	DMAReConfig();	//启动第一次ADC转换

  while (1) {

		os_evt_wait_and (0x0100, 0xFFFF); //等到DMA_ADC执行(TRANSFORM_TIMES/4)次
		
		channel_count++;
		
		tsk_ADC_run = (_TRUE);//喂狗
		
		switch (channel_count)  //判断当前模拟开关执行到哪一路，并马上切换到下一路
		{
			case 1: 	AD_Channel1();
				break;
			case 2: 	AD_Channel2();
				break;
			case 3: 	AD_Channel3();
				break;	
			case 4: 	AD_Channel4();
				break;
			case 5: 	AD_Channel5();
				break;
			case 6: 	AD_Channel6();
				break;	
			case 7: 	AD_Channel7();
				break;
			case 8: 	AD_Channel0();
				break;					
			default:	
				break;
		}
		
		for(i=0;i<4;i++)   			//每次转换结束都会得到4个INPUT所对应的通道的
		{
			for(j=0;j<(TRANSFORM_TIMES>>2);j++) //每个通道都启动了(TRANSFORM_TIMES/4)次DMA传输
			{
				for(k=0;k<4;k++)		//对于每一通道，每次DMA都传输4个转换值(ADC连续转换了4次)
					ADC_group_data[(j<<2)+k]=ADC_groups_data_once[(i<<2)+(j<<4)+k];
			}
			//在这滤波
			value_temp =ADC_filter_one(ADC_group_data ,TRANSFORM_TIMES);

			switch (channel_count)
			{
				case 1: 	filter_one_value[0+i]		=value_temp;
					break;
				case 2: 	filter_one_value[4+i]		=value_temp;
					break;
				case 3: 	filter_one_value[8+i]		=value_temp;
					break;	
				case 4: 	filter_one_value[12+i]	=value_temp;
					break;
				case 5: 	filter_one_value[16+i]	=value_temp;
					break;
				case 6: 	filter_one_value[20+i]	=value_temp;
					break;	
				case 7: 	filter_one_value[24+i]	=value_temp;
					break;
				case 8: 	filter_one_value[28+i]	=value_temp;
					break;					
				default:	
					break;
			}		

		}
	//	为了表意更明确，我做了如下调整。
		if(channel_count < 8) 
		{
			DMAReConfig();   //启动ADC转换
		}	
		else							 /* 4*channel_count ==32,表明所有的通道转换完毕 */
		{
			channel_count=0;
			
			//通知其它的任务，32个通道转换了一个周期�
			os_evt_set (0x0001, t_learn_detect_task); 	//ADC 采集了32个通道
			os_dly_wait(ADC_CYCLE_TIME);				//采集的周期控制
			DMAReConfig();   										//再次启动下个周期的ADC转换			
			
		}
		

// 	 printf("%d, %d, %d, %d\n ", ADC_buffer[8],ADC_buffer[9],ADC_buffer[10],ADC_buffer[11]);

  }
}

//======================================debug==================================
extern vu32 passage_filter_fifo[40];
/*  二次过滤窗口写、读、计数值  */
extern vu8 passage_filter_wr_index,passage_filter_rd_index,passage_filter_counter;
extern u16	last_filter_ADC[32];
//======================================debug==================================
U8 bLamp_Phyerr_Report = __FALSE;//灯物理故障标志，用于上报电流

/*		当前采集到的灯电流值到lamp_level_temp[]，临时变量	以及索引*/		
u16 current_lamp_level[LAMP_NUMBER][(PASSAGE_CURRENT_WINDOWS/ADC_CYCLE_TIME)+1] = {0};//每个灯取12组数据进行过滤

__task void learn_detect_task (void) 
{

	/*用于重新学习时重置各函数里边的静态变量*/
	u8		reset_learn 					= 0;
	/*用于学习完时重置各检查函数里边的静态变量*/
	u8		reset_checking				= 0;
	
	/*32位指针，保存各函数返回的值*/
	u32		 p_state_task[PASSAGE_NUMBER]={0}	;		
	
	/*第一次过滤得到的当前通道状态*/
	u32  first_all_passage 			   = 	0;
	
	/*当前红绿状态 cur_RG_passage,当前红黄绿状态 cur_RYG_passage*/
	u32 cur_RG_passage,cur_RYG_passage;

	/*有效红绿状态 valid_RG_passage,有效红黄绿状态 valid_RYG_passage*/
	static u32 valid_RG_passage		=	0;
	static u32 valid_RYG_passage	=	0;

	/*将实际的灯顺序转换为红黄绿一起的顺序*/
	u16	filter_one_rerank[LAMP_NUMBER]	= {0};

	/*通道冲突表*/
	u32		passage_clash			 = 0;			
	
	/*灯故障状态值*/
	u32		period_lamp_state 	  = 0;		
	
	/*灯物理故障状态值*/
	u32		lamp_phyerr_state 	 = 0;		
	/*物理故障检测的计数值，累计到4.8s进行检测*/
	u8		lamp_phyerr_time		 = 0;
	/*灯物理故障等级：1、2、3、4级*/
	u8		phyerr_level = 0;
	/*上一次的灯物理故障等级，用于是否上报，该值与当前物理等级一致，则不再重复上报*/
	u8		last_phyerr_level = 0;
	
	/*相位冲突状态*/
	u32		phase_clash_state	 = 0;

	/*周期结束标志*/
	u8 period_completed_flag = 0;
	
	/*学习得到的有效红、绿、黄灯状态*/
	static   u32 		passage_red 			= 		0;					//分离得到的红灯状态
	static   u32 		passage_yellow 		 = 		0;					//分离得到的黄灯状态
	static   u32 		passage_green 		 = 		0;					//分离得到的绿灯状态
	
	/*学习得到的有效通道表，数组每个元素代表一个相位的通道*/
	static   u32		passage_table[PASSAGE_NUMBER]	=	{0};//{0x07,0x38,0x1c0,0xe00};			
	
	/*学习得到的//相位冲突表*/	
	static   u32		phase_clash_table[PHASE_CLASH_STATE_NUMBER]	=	{0};//{0xfffffffe,0xfffffff7,0xffffffbf,0xfffffdff};		
	
	/*学习完成标志 成功为1*/
	static   u8		learning_flag							 =			0;				

// 	/*刚开始学习标志*/
// 	static   u8		first_start_learning					 =			0;
	
	/*当前绿灯状态*/
	u32		cur_passage_green	 = 0;
	
	//===================================debug
	u32 status=0;u32 i;	
	u32 last_rg,last_ryg;	
	//===================================debug
	
	u8	*msg_send						= NULL;
	u8 index_temp = 0;
	/*		当前采集到的灯电流值到lamp_level_temp[]，临时变量	以及索引*/		
// 	u16 current_lamp_level[LAMP_NUMBER][(PASSAGE_CURRENT_WINDOWS/ADC_CYCLE_TIME)+1] = {0};//每个灯取12组数据进行过滤
	u8 lamp_current_index[LAMP_NUMBER] =  {0};
	u16 lamp_level_temp[LAMP_NUMBER] = {0};

	unsigned char to_company_count = 0;//发送给公司时间的计数
	
	uint8_t  one_combine_passage[COMBINE_VALID_PASSAGE_NUM] = {0};//一组合并的有效通道
	uint8_t  one_combine_passage_count = 0;//一组合并的有效通道个数
	uint8_t  j,k,l = 0;
	
// 	u8   passage_error[PASSAGE_ParaOutputList_MaxLine][4]={0};//故障上报
	
	U8	err_type = 0;
	U8 *msg_send_report = NULL;
	
	u32 lamp_use_temp = 0;//灯是否使用过的标志，临时变量，等于Flag_LampUse
	//===============从eeprom读取数据	start======================
	/*		灯电流值到standard_lamp_level[]，作为标准电流值	*/	
	u16 standard_lamp_level[LAMP_NUMBER] = {0};	
	/*		读取灯电流值到standard_lamp_level[]，作为标准电流值	*/	
	read_param(STANDARD_VOL,(uint8_t *)standard_lamp_level,(sizeof(u16))*LAMP_NUMBER);		
	for(i=0;i<LAMP_NUMBER;i++)
	{
		standard_lamp_current[i]=standard_lamp_level[i];
	}
	read_learned_data(passage_table,phase_clash_table,p_state_task);
	passage_red      =  p_state_task[0];
	passage_yellow 	 =  p_state_task[1];
	passage_green  	 =  p_state_task[2];
	
	LED_LEARN_OFF();
// 	#ifdef DEBUG_LXB
	printf("*********************eeprom数据****************************\n");
	printf("*********************相位为以下***************************\n");
	for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)
	{
		if(~phase_clash_table[i] != 0 && phase_clash_table[i] != 0)
		{
			lxbprint_status((~phase_clash_table[i]));
			learning_success_flag = __TRUE;//若相位冲突表不为0，则说明已经学习
		}
		else
		{
			break;
		}
	}
	printf("*********************相位完毕****************************\n");
	printf("*********************红黄灯为以下***************************\n");
	lxbprint_status(passage_red);
	lxbprint_status(passage_yellow);
	lxbprint_status(passage_green);
	printf("*********************红黄灯完毕****************************\n");
// 	#endif
//===============从eeprom读取数据	end======================
	
	/* 在这个任务里做二级滤波之类的 */
  while (1) 
	{
		os_evt_wait_and (0x0001, 0xFFFF); //ADC 采集了32个通道
		
		tsk_learn_run = (_TRUE);//喂狗	
		
		if(1)
		{/*filter_one_rerank[32-0]表示11-1通道红黄绿 红黄绿。。。。*/
			filter_one_rerank[2] = filter_one_value[7];
			filter_one_rerank[1] = filter_one_value[11];
			filter_one_rerank[0] = filter_one_value[15];
			
			filter_one_rerank[5] = filter_one_value[2];
			filter_one_rerank[4] = filter_one_value[6];
			filter_one_rerank[3] = filter_one_value[10];
			
			filter_one_rerank[8] = filter_one_value[26];
			filter_one_rerank[7] = filter_one_value[30];
			filter_one_rerank[6] = filter_one_value[3];
			
			filter_one_rerank[11] = filter_one_value[14];
			filter_one_rerank[10] = filter_one_value[18];
			filter_one_rerank[9] = filter_one_value[22];
			
		}

		/*若web进行了有效的通道组合设置，则对相应的通道进行调整*/
		if(bSet_Valid_Passage == __TRUE)
		{
			bSet_Valid_Passage = __FALSE;
			/*
				存储有效通道组合表
			*/	
			save_param(VALID_PASSAGE_COMBINE,	(uint8_t *)&combine_valid_passage,sizeof(combine_valid_passage));	
		}
		if(1)
		{
			for(i=0;i<COMBINE_VALID_PASSAGE_NUM;i++)
			{
				if(combine_valid_passage[i]>0)
				{
					one_combine_passage_count = 0;
					for(j=0;j<PASSAGE_NUMBER;j++)
					{
						if((combine_valid_passage[i]>>j &1) == 1)
						{
							one_combine_passage[one_combine_passage_count] = j+1;//第j+1个通道被设置
							one_combine_passage_count++;
						}
					}	
					for(k=0;k<3;k++)
					{					
						for(j=0;j<one_combine_passage_count;j++)
						{
							if((filter_one_rerank[3*(one_combine_passage[j]-1)+k] > 0)&&one_combine_passage[j]>0)
							{
								for(l=0;l<one_combine_passage_count;l++)
								{
									filter_one_rerank[3*(one_combine_passage[l]-1)+k] = filter_one_rerank[3*(one_combine_passage[j]-1)+k];
								}
								break;
							}
						}
					}
				}
			}
		}
		
		
		/*一次过滤二值化*/
		/*first_all_passage[31-0]表示11-1通道红黄绿 红黄绿。。。。*/
		first_all_passage = first_filtering_binaryzation(filter_one_rerank,reset_checking);

		/*获得每个灯的有效电流大小*/
		for(i=0;i<LAMP_NUMBER;i++)
		{
			if (filter_one_rerank[i]>100)//取大于100的电流ad值进行计算
			{
				current_lamp_level[i][(lamp_current_index[i])]=filter_one_rerank[i];
				lamp_current_index[i]++;
				if(lamp_current_index[i]>=(PASSAGE_CURRENT_WINDOWS/ADC_CYCLE_TIME))//取2.4s内采集的电流ad值
				{
					lamp_current_index[i]=0;
					lamp_level_temp[i] = ADC_lamp_current(current_lamp_level[i],(PASSAGE_CURRENT_WINDOWS/ADC_CYCLE_TIME));
					
					/*转换成电流*/
					lamp_level_temp[i] = (u32)(3.3/4095*lamp_level_temp[i]/Sampling_Resistance * VALUE_MA);
					/*赋值全局电流大小数组，以便查询上报*/
					os_mut_wait(table_global_mut,0xFFFF);
					Lamp_Broken_Level[i] = lamp_level_temp[i];
					lamp_use_temp = Flag_LampUse;
					os_mut_release(table_global_mut);
					
					if(1)//start_learning_flag == __FALSE)
					{
						if(((lamp_use_temp>>i)&1 == 1)&&(standard_lamp_level[i] == 0))
						{
							standard_lamp_level[i] = lamp_level_temp[i];
							standard_lamp_current[i] = lamp_level_temp[i];
							save_param(STANDARD_VOL+2*i,(uint8_t *)&standard_lamp_level[i],(sizeof(u16)));		
						}
					}
				}
			}
		}
		
// 		last_rg=cur_RG_passage;		///debug
// 		last_ryg=cur_RYG_passage;	///debug
			
		/*二次过滤*/
		second_Filtering(first_all_passage,p_state_task);		
		cur_RG_passage   = p_state_task[0];
		cur_RYG_passage = p_state_task[1];	
		p_state_task[0]=0;
		p_state_task[1]=0;	
		
		/*将当前通道状态赋给有效通道状态*/
		if(start_learning_flag == __FALSE)
		{//非学习下，读取学习时得到的eeprom中的有效通道
			valid_RG_passage	=	(passage_red|passage_green);
			valid_RYG_passage	=	(passage_red|passage_green)|passage_yellow;
		}
		else
		{//学习时，获取实时的有效通道
			if(first_start_learning == _FALSE)			  //刚开始学习，初始化								
			{
				valid_RG_passage	=0;
				valid_RYG_passage	=0;
			}
			valid_RG_passage	|=	cur_RG_passage;
			valid_RYG_passage	|=	cur_RYG_passage;
		}
		
		/*用红绿通道状态过滤红绿黄中的红绿通道状态，原因：红绿通道状态的窗口大于红绿黄的窗口，如此更准确*/
		cur_RYG_passage = (cur_RYG_passage&cur_RG_passage)|(((~valid_RG_passage) & valid_RYG_passage)&cur_RYG_passage);
		
		//通道输出表 0xB1
		os_mut_wait(table_global_mut,0xFFFF);
 		channel_output_table(Passage_ParaOutputList,cur_RYG_passage); 
		os_mut_release(table_global_mut);
		
		//关闭公司数据发送
// // // 		to_company_count++;
// // // 		/************************给公司监测电脑发送数据处理***************/
// // // 		if(to_company_count >= 2)//2*ADC_CYCLE_TIME ms给公司发送一次数据
// // // 		{
// // // 			to_company_count = 0;
// // // 			
// // // 			tsk_lock ();  			//  disables task switching 
// // // 			write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 			write_MySelf_fifo[write_MySelf_wr_index][1] = 0xB1;	
// // // 			write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 			write_MySelf_fifo[write_MySelf_wr_index][3] = 1;		
// // // 			for (i=0; i<4; i++)
// // // 			{
// // // 				write_MySelf_fifo[write_MySelf_wr_index][4+i] = Passage_ParaOutputList[0][i]; 
// // // 			}
// // // 			
// // // 			if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 			if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 			{
// // // 				write_MySelf_counter=1;
// // // 				write_MySelf_buffer_overflow=__TRUE;
// // // 			};
// // // 			tsk_unlock (); 			//  enable  task switching 
// // // 			os_sem_send(eth_MySelf_semaphore);
// // // 		}
// // // 		/************************给公司监测电脑发送数据处理***************/
				
// 		/*用红绿通道状态过滤红绿黄中的红绿通道状态，原因：红绿通道状态的窗口大于红绿黄的窗口，如此更准确*/
// 		cur_RYG_passage = (cur_RYG_passage&cur_RG_passage)|(((~valid_RG_passage) & valid_RYG_passage)&cur_RYG_passage);
			
		#ifdef DEBUG_LXB
		status = cur_RG_passage;
		if(cur_RYG_passage!=last_ryg)
		{
			printf("*****第一行为红绿通道状态，第二行为红绿黄通道状态*********\n ");
			lxbprint_status(status);
		}
		status = cur_RYG_passage;
		if(cur_RYG_passage!=last_ryg)
		{
			printf(" ");
			lxbprint_status(status);
// 			lxbprint_status(valid_RG_passage);
// 	lxbprint_status(valid_RYG_passage);
		}
		#endif

		/*冲突检测、故障判断等；只有在非学习状态下才能进行*/   
		if(start_learning_flag == _FALSE)		
		{
			LED_LEARN_OFF();
			
			/*通道是否冲突，大于0则冲突*/
			passage_clash = passage_clash_check(cur_RYG_passage,passage_table);
			if(passage_clash>0)
			{
				printf("通道冲突\n");
				lxbprint_status(passage_clash);//通道冲突，发送冲突表
				
				channel_output_table(passage_clash_data,passage_clash);
				
				ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min
				
				/************************上报异常*********************************/
				err_type = 1;
				msg_send_report = &err_type;
				os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
				if(error_report_time[0]==0)
				{
					error_report_time[0] = REPORT_WATCHDOG_TIME;
					/************************写日志*********************************/
					if(1)/*通道冲突，且与上次写日志不一致，避免重复写日志*/
					{
						last_log_type = ALARM_LOG_TYPE_5;
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_PASSAGE_ABNOMAL;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0xD1;
						write_log_wr_fifo[write_log_wr_index][4] = (u8)(passage_clash>>8);
						write_log_wr_fifo[write_log_wr_index][5] = (u8)(passage_clash);						
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
					
					
					error_report_answer[0] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
					
// // // 					/************************给公司监测电脑发送数据处理***************/
// // // 					if(1)
// // // 					{
// // // 						tsk_lock ();  			//  disables task switching 
// // // 						write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][1] = 0x97;	
// // // 						write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][3] = 0x01;//LAMP_COLLISION;		//通道故障

// // // 						if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 						if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 						{
// // // 							write_MySelf_counter=1;
// // // 							write_MySelf_buffer_overflow=__TRUE;
// // // 						};
// // // 						tsk_unlock (); 			//  enable  task switching 
// // // 						os_sem_send(eth_MySelf_semaphore);
// // // 					}
// // // 					/************************给公司监测电脑发送数据处理***************/
				}
				else
				{
					error_report_time[0] = REPORT_WATCHDOG_TIME;
				}
			}
			
			/*灯是否有故障，大于0则冲突*/
			cur_passage_green  = passage_green & cur_RG_passage;								//得到当前的绿灯状态
			period_lamp_state = lamp_fault_check(cur_RYG_passage,valid_RYG_passage,cur_passage_green,&period_completed_flag,reset_checking);
			if(period_lamp_state>0)
			{					
				printf("灯故障\n");
				lxbprint_status(period_lamp_state);																		//灯有故障，写入日志，上报数据
				
				channel_output_table(passage_broken_data,period_lamp_state);
				
				ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min	
				
				/************************上报异常*********************************/
				err_type = 3;
				msg_send_report = &err_type;
				os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
				if(error_report_time[1]==0)
				{
					error_report_time[1] = REPORT_WATCHDOG_TIME;
					/************************写日志*********************************/
					if(1)//last_log_type != ALARM_LOG_TYPE_6)/*灯冲突，且与上次写日志不一致，避免重复写日志*/
					{
						last_log_type = ALARM_LOG_TYPE_6;
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_PASSAGE_ABNOMAL;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0xCF;
						write_log_wr_fifo[write_log_wr_index][4] = (u8)(period_lamp_state>>8);
						write_log_wr_fifo[write_log_wr_index][5] = (u8)(period_lamp_state);						
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
					
					
					error_report_answer[1] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
					
// // // 					/************************给公司监测电脑发送数据处理***************/
// // // 					if(1)
// // // 					{
// // // 						tsk_lock ();  			//  disables task switching 
// // // 						write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 						write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][3] = 0x10;  //故障号

// // // 						if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 						if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 						{
// // // 							write_MySelf_counter=1;
// // // 							write_MySelf_buffer_overflow=__TRUE;
// // // 						};
// // // 						tsk_unlock (); 			//  enable  task switching 
// // // 						os_sem_send(eth_MySelf_semaphore);
// // // 					}
// // // 					/************************给公司监测电脑发送数据处理***************/

					/************************上报异常*********************************/
				}
				else
				{
					error_report_time[1] = REPORT_WATCHDOG_TIME;
				}
			}
			
			
			/*灯是否有物理故障，大于0则冲突*/
			lamp_phyerr_time++;
			if(lamp_phyerr_time >=(480000/ADC_CYCLE_TIME))			//每480s进行一次物理故障检测
			{
				lamp_phyerr_time = 0;
				lamp_phyerr_state = lamp_phyLevel_detect(&phyerr_level,cur_RYG_passage,standard_lamp_level,lamp_level_temp,reset_checking);
				if(lamp_phyerr_state>0)
				{
					bLamp_Phyerr_Report = (__TRUE);
					printf("灯物理故障\n");
					lxbprint_status(lamp_phyerr_state);																		//灯有故障，写入日志，上报数据
					
				 channel_output_table(passage_broken_data,lamp_phyerr_state);
					
				 ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min
					
					/************************上报异常*********************************/
					err_type = 4;
					msg_send_report = &err_type;
					os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
										
					if((error_report_time[2]==0)||(last_phyerr_level != phyerr_level))
					{
						error_report_time[2] = REPORT_WATCHDOG_TIME;
						
						last_phyerr_level = phyerr_level;
						error_report_answer[2] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
						
						/************************写日志*********************************/
						if(1)//last_log_type != ALARM_LOG_TYPE_14)/*灯物理故障，且与上次写日志不一致，避免重复写日志*/
						{
							last_log_type = ALARM_LOG_TYPE_14;
							tsk_lock ();  			 
							write_log_wr_fifo[write_log_wr_index][0] =1;			
							write_log_wr_fifo[write_log_wr_index][1] = LOG_PASSAGE_ABNOMAL;
							write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
							write_log_wr_fifo[write_log_wr_index][3] = 0xCF;
							write_log_wr_fifo[write_log_wr_index][4] = (u8)(lamp_phyerr_state>>8);
							write_log_wr_fifo[write_log_wr_index][5] = (u8)(lamp_phyerr_state);				
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
						
						/************************上报异常*********************************/
						
// // // 						/************************给公司监测电脑发送数据处理***************/
// // // 						if(1)
// // // 						{
// // // 							tsk_lock ();  			//  disables task switching 
// // // 							write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 							write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 							write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 							write_MySelf_fifo[write_MySelf_wr_index][3] = 0x08;  //故障号

// // // 							if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 							if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 							{
// // // 								write_MySelf_counter=1;
// // // 								write_MySelf_buffer_overflow=__TRUE;
// // // 							};
// // // 							tsk_unlock (); 			//  enable  task switching 
// // // 							os_sem_send(eth_MySelf_semaphore);
// // // 						}
// // // 						/************************给公司监测电脑发送数据处理***************/
					}
					else
					{
						error_report_time[2] = REPORT_WATCHDOG_TIME;
					}
						/************************上报异常*********************************/
				}
			}
				
			/*相位是否冲突，大于0则冲突*/
			cur_passage_green  = passage_green & cur_RG_passage;								//得到当前的绿灯状态
			phase_clash_state  = green_phase_clash_check(cur_passage_green,phase_clash_table);
			if(phase_clash_state>0)
			{
				printf("相位冲突\n");
				lxbprint_status(phase_clash_state);																		//相位冲突，写入日志，上报数据
				
				channel_output_table(passage_clash_data,phase_clash_state);
				
				ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min
				
				/************************上报异常*********************************/
				err_type = 2;
				msg_send_report = &err_type;
				os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
				if(error_report_time[3]==0)
				{
					error_report_time[3] = REPORT_WATCHDOG_TIME;
					if(1)//last_log_type != ALARM_LOG_TYPE_4)/*绿冲突，且与上次写日志不一致，避免重复写日志*/
					{
						last_log_type = ALARM_LOG_TYPE_4;
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_PASSAGE_ABNOMAL;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0xD1;
						write_log_wr_fifo[write_log_wr_index][4] = (u8)(phase_clash_state>>8);
						write_log_wr_fifo[write_log_wr_index][5] = (u8)(phase_clash_state);					
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
					
					/************************上报异常*********************************/
					error_report_answer[3] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
					
// // // 					/************************给公司监测电脑发送数据处理***************/
// // // 					if(1)
// // // 					{
// // // 						tsk_lock ();  			//  disables task switching 
// // // 						write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][1] = 0x97;	
// // // 						write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][3] = 0x01;//LAMP_COLLISION;  //故障号

// // // 						if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 						if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 						{
// // // 							write_MySelf_counter=1;
// // // 							write_MySelf_buffer_overflow=__TRUE;
// // // 						};
// // // 						tsk_unlock (); 			//  enable  task switching 
// // // 						os_sem_send(eth_MySelf_semaphore);
// // // 					}
// // // 					/************************给公司监测电脑发送数据处理***************/
				}
				else
				{
					error_report_time[3] = REPORT_WATCHDOG_TIME;
				}
				/************************上报异常*********************************/
			}
			
			if(learning_success_flag == __TRUE)//进行阶段学习
			{
				get_stage_table(&period_completed_flag,cur_RYG_passage,passage_green,passage_red,reset_checking);
			}
			
			if(reset_checking == _TRUE)
			{
				reset_checking = _FALSE;				
			}
		}	
		/*若开始学习*/
		else			
		{
				
			if(first_start_learning == _FALSE)																		  //刚开始学习，初始化各个数组内容									
			{
				printf("*************************开始学习***************************\n");
				first_start_learning = _TRUE;
				
				reset_learn			= _TRUE;
				
				passage_red    = 0;
				passage_yellow = 0;
				passage_green  = 0;
				for(i=0;i<PASSAGE_NUMBER;i++)
					passage_table[i] = 0;
				for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)
					phase_clash_table[i] = 0;
				p_state_task[0]=0;	p_state_task[1]=0;	p_state_task[2]=0;
				
				/************************写日志*********************************/
				tsk_lock ();  						
				write_log_wr_fifo[write_log_wr_index][0] =1;			
				write_log_wr_fifo[write_log_wr_index][1] = LOG_SET_TIME;
				write_log_wr_fifo[write_log_wr_index][2] = 0xAA;
				write_log_wr_fifo[write_log_wr_index][3] = 1;
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
			/* 分离红、黄、绿灯的有效通道状态*/
			passage_RYG_learn(valid_RG_passage,valid_RYG_passage, p_state_task);
			passage_red      =  p_state_task[0];
			passage_yellow 	 =  p_state_task[1];
			passage_green  	 =  p_state_task[2];
			p_state_task[0]=0;	p_state_task[1]=0;	p_state_task[2]=0;	
			
			/* 学习得到有效通道表*/
			get_passage_table(passage_red,passage_yellow,passage_green,p_state_task);
			for(i=0;i<PASSAGE_NUMBER;i++)
			{
				passage_table[i] = *(p_state_task+i);
				p_state_task[i]=0;	
			}
			
			/*	学习进度判断与得到相位冲突表passage_clash_table*/
			cur_passage_green  = passage_green & cur_RG_passage;							//得到当前的绿灯状态
			if(cur_passage_green>0)																						//至少有一个绿灯亮
			{
				learning_flag = 0;
				learning_flag = get_phase_clash_table(cur_passage_green,phase_clash_table,reset_learn);
				reset_learn			= _FALSE;
				if(learning_flag>0&&learning_flag!=2)																								//学习校验完成
				{
					learning_success_flag = __TRUE;
					p_state_task[0]=passage_red;
					p_state_task[1]=passage_yellow;
					p_state_task[2]=passage_green;
					save_learned_data(passage_table,phase_clash_table,p_state_task);		

					os_mut_wait(table_global_mut,0xFFFF);
					for(i=0;i<LAMP_NUMBER;i++)
					{
						standard_lamp_level[i] = Lamp_Broken_Level[i];
						standard_lamp_current[i] = Lamp_Broken_Level[i];
					}
					os_mut_release(table_global_mut);					
					/*
						存储灯电流值Lamp_Broken_Level[]到eeprom，作为标准电流值
					*/	
					save_param(STANDARD_VOL,(uint8_t *)standard_lamp_level,(sizeof(u16))*LAMP_NUMBER);
					
					printf("*********************学习成功****************************\n");
					printf("*********************相位为以下***************************\n");
					for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)
					{
						if(~phase_clash_table[i] != 0 && phase_clash_table[i] != 0)
						{
							lxbprint_status((~phase_clash_table[i]));
						}
						else
						{
							break;
						}
					}
					printf("*********************相位完毕****************************\n");
					start_learning_flag = 0;																				//结束学习
					first_start_learning = _FALSE;																				//以便下次学习
					reset_checking		= _TRUE;																	//学习完则重置检查函数
				}
				else if(learning_flag==2)
				{
					learn_count++;	//learn 次数
					if(learn_count%3 == 0)
					{
						//report
// 						if(last_log_type != ALARM_LOG_TYPE_15)/*多次学习不成功报警，且与上次写日志不一致，避免重复写日志*/
// 						{
// 							last_log_type = ALARM_LOG_TYPE_15;
// 							tsk_lock ();  			 
// 							write_log_wr_fifo[write_log_wr_index][0] =1;			
// 							write_log_wr_fifo[write_log_wr_index][1] = LOG_RELEASE_POWER;
// 							write_log_wr_fifo[write_log_wr_index][2] = 0xA7;
// 							write_log_wr_fifo[write_log_wr_index][3] = 0x10;
// 							write_log_wr_fifo[write_log_wr_index][4] = (_FAULT);
// 							write_log_wr_fifo[write_log_wr_index][5] = 0;						
// 							if (++write_log_wr_index == WRITE_LOG_SIZE) write_log_wr_index=0;										
// 							if (++write_log_counter  == WRITE_LOG_SIZE)
// 							{
// 								write_log_counter=1;
// 								write_log_buffer_overflow=__TRUE;
// 							}
// 							tsk_unlock (); 			
// 							os_mbx_send (Write_Log_Mbox, msg_send, 0x00FF);	
// 						/************************写日志*********************************/
// 						}		

						/************************上报异常*********************************/
						/************************上报异常*********************************/
						err_type = 5;
						msg_send_report = &err_type;
						os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
						if(error_report_time[4]==0)
						{
							error_report_time[4] = REPORT_WATCHDOG_TIME;
							error_report_answer[4] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
							
// // // 							/************************给公司监测电脑发送数据处理***************/
// // // 							if(1)
// // // 							{
// // // 								tsk_lock ();  			//  disables task switching 
// // // 								write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 								write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 								write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 								write_MySelf_fifo[write_MySelf_wr_index][3] = 0x50;  //故障号

// // // 								if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 								if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 								{
// // // 									write_MySelf_counter=1;
// // // 									write_MySelf_buffer_overflow=__TRUE;
// // // 								};
// // // 								tsk_unlock (); 			//  enable  task switching 
// // // 								os_sem_send(eth_MySelf_semaphore);
// // // 							}
// // // 							/************************给公司监测电脑发送数据处理***************/
						}
						else
						{
							error_report_time[4] = REPORT_WATCHDOG_TIME;
						}
						/************************上报异常*********************************/							
			
					}
					
					start_learning_flag = 1;																				//重新学习
					first_start_learning = _FALSE;																	//重新开始学习
				}
			}
		}
  }
}


/*上报灯电流,不须保护，顺序执行得到*/
extern u8 lamp_report_table[];

__task void power_detect_task(void)
{
	  
		uint32_t		time_count_temp = 0;			/*任务时间计数*/

		u8	*msg_send						= NULL;
		u8 i;
		u8 index_temp = 0;
	
		u8 restart_timer_tick = 0;//定时重启计数
		u8 restart_timer_flag = 0;//定时重启标志

		u8 change_by_web = 0;//通过web修改参数，写日志标志
	
	
		U8	err_type = 0;
		U8 *msg_send_report = NULL;
	
		u8 learning_transformer_flag = __FALSE;//匝数比学习标志
		TIM_Cmd(TIM1, ENABLE);								/*电压脉冲采集定时开启*/	
	 	
		if(1)
		{
			printf("%d	",LocM.IpAdr[0]);
			printf("%d	",LocM.IpAdr[1]);
			printf("%d	",LocM.IpAdr[2]);
			printf("%d	",LocM.IpAdr[3]);
			printf("%d	",LocM.NetMask[0]);
			printf("%d	",LocM.NetMask[1]);
			printf("%d	",LocM.NetMask[2]);
			printf("%d	",LocM.NetMask[3]);
			printf("%d	",LocM.DefGW[0]);
			printf("%d	",LocM.DefGW[1]);
			printf("%d	",LocM.DefGW[2]);
			printf("%d	",LocM.DefGW[3]);
			printf("%d	",local_port);	
			printf("\n	");
		
			printf("%d	",OBJ_FLAG_ADDR_START);
			printf("%d	",OBJ_ADDR_SET_FLAG);
			printf("%d	",OBJ_ADDR_END);
			printf("%d	",INDEX_92_ADDR_END);
			printf("%d	",EE_PARAMETER_START);
			printf("%d	",MACHINE_TYPE_ADDR);			
			printf("\n	");
		}
		
		while (1) {
		  os_dly_wait(500);
		  tsk_power_run = (_TRUE);//喂狗	
		 
			time_count_temp++;
		 
		 if((turn_num_flag 							==__TRUE) ||
				(turn_num_lxb_flag 					==__TRUE) ||
				(bat_min_flag 							==__TRUE) ||
				(cur_min_flag 							==__TRUE) ||
				(send_time_flag 						==__TRUE) ||		 
				(reset_all_flag 						==__TRUE) ||
				(restudy_flag 							==__TRUE) ||
				(restart_flag 							==__TRUE) ||		 
				(change_alarm_restart_flag 	==__TRUE)	)  // 应用配置发生了变化。是否要枷锁？？？
		 {
			 change_by_web = 1;//web修改参数--写日志
			 if(turn_num_flag ==__TRUE)	 //
			 {
				 turn_num_flag =__FALSE;
				 save_param(TURN_NUM_ADDR,				(uint8_t *)&turn_num,		sizeof(turn_num));
				 learning_transformer_flag = _TRUE;
			 }
			 if(turn_num_lxb_flag ==__TRUE)	 //
			 {
				 turn_num_lxb_flag =__FALSE;
				 save_param(REAL_TURN_NUM_ADDR,				(uint8_t *)&turn_num_lxb,		sizeof(turn_num_lxb));
			 }
			 if(bat_min_flag ==__TRUE)	 //
			 {
				 bat_min_flag =__FALSE;
				 save_param(BAT_MIN_ADDR,				(uint8_t *)&bat_min,		sizeof(bat_min));
			 }
			 if(cur_min_flag ==__TRUE)	 //没做。是否要做lxb
			 {
				 cur_min_flag =__FALSE;
				 save_param(CUR_MIN_ADDR,				(uint8_t *)&cur_min,		sizeof(cur_min));
			 }
			 if(send_time_flag ==__TRUE)	 //灯电流上报时间设置
			 {
				 send_time_flag =__FALSE;
				 save_param(SEND_TIME_ADDR,				(uint8_t *)&send_time,		sizeof(send_time));
			 }	
				
			 if(reset_all_flag ==__TRUE)	 //出厂设置
			 {
				  reset_all_flag =__FALSE;
				 /*  擦除EEPROM中所有的数据 */
					EEP_erase_ALL();  
					/*  把程序默认的参数卸任EEPROM中 */
					save_default_parameter();
					/*  关闭中断 */
					__set_FAULTMASK(1); 	
					/*  软件复位 */
					NVIC_SystemReset();	
			 }
			 if(restudy_flag ==__TRUE)	 //重新学习 
			 {
				 restudy_flag =__FALSE;
				 start_learning_flag = 1;
				 first_start_learning = 0;
				 learning_success_flag = 0;
				 learn_count = 0;//学习次数清零
			 }
			 if(restart_flag ==__TRUE)	 //重启板子
			 {
				 restart_flag =__FALSE;
					/*  软件复位 */
					NVIC_SystemReset();	
			 }
			 if(change_alarm_restart_flag ==__TRUE)	 //定时重启状态变化
			 {
				 change_alarm_restart_flag =__FALSE;
				 if(alarm_restart_flag==__TRUE)//开启定时重启
				 {
					 restart_timer_flag=__TRUE;
					 restart_timer_tick=0;
				 }
				 else//取消定时重启
				 {					 
					 restart_timer_flag=__FALSE;
					 restart_timer_tick=0;
				 }
			 }
			 /*
				
				类似的还有一些其它的参数 没写完
				
			*/
			 
		 }
			
		 if(restart_timer_flag==__TRUE)//开启定时重启
		 {
			 change_by_web = 1;//web修改参数--写日志
			 
			 restart_timer_tick++;//定时重启计数
			 if(restart_timer_tick>=(alarm_restart_hour*60+alarm_restart_minute)*120)
			 {
					/*  软件复位 */
					NVIC_SystemReset();					 
			 }
		 }
		 
		 if(change_DHCP_flag ==__TRUE)	 //
		 {
			 change_by_web = 1;//web修改参数--写日志
			 
			  change_DHCP_flag =__FALSE;
				save_flag( DHCP_FLAG_ADDR, DHCP_flag ,   DHCP_FLAG_BIT);
		 }			 
		 
		 if(DHCP_flag ==__FALSE)   //只有在DHCP没启动的情况下才可以修改
		 {			 
				 if((local_ip_flag 				==__TRUE) ||
						(local_mask_flag 			==__TRUE) ||
						(local_gateway_flag 	==__TRUE) ||
						(local_port_flag 			==__TRUE) ||
						(local_mac_flag 			==__TRUE) ||
						(local_dns_flag 			==__TRUE) ||
						(server_ip_flag 			==__TRUE) ||
						(server_port_flag 		==__TRUE) ||
						(server_www_flag 			==__TRUE) )  // 网络配置发生了变化
				 {
					 change_by_web = 1;//web修改参数--写日志
					 
							if(local_ip_flag		==__TRUE)
							{
								local_ip_flag		=__FALSE;
								save_param(LOCAL_IP_ADDR,				LocM.IpAdr,				sizeof(LocM.IpAdr));
							}
							if(local_mask_flag		==__TRUE)
							{
								local_mask_flag		=__FALSE;
								save_param(LOCAL_MASK_ADDR,			LocM.NetMask,			sizeof(LocM.NetMask));
							}
							if(local_gateway_flag		==__TRUE)
							{
								local_gateway_flag		=__FALSE;
								save_param(LOCAL_GATEWAY_ADDR,	LocM.DefGW,				sizeof(LocM.DefGW));
							}
							if(local_port_flag		==__TRUE)
							{
								local_port_flag		=__FALSE;
								save_param(LOCAL_PORT_ADDR,			(uint8_t *)&local_port,	sizeof(local_port));
							}	
							if(local_mac_flag		==__TRUE)
							{
								local_mac_flag		=__FALSE;
								save_param(LOCAL_MAC_ADDR,			mac_adr,								sizeof(mac_adr));
							}
							if(local_dns_flag		==__TRUE)
							{
								local_dns_flag		=__FALSE;
								save_param(LOCAL_DNS_ADDR,			LocM.PriDNS,			sizeof(LocM.PriDNS));
							}
							if(server_ip_flag		==__TRUE)
							{
								server_ip_flag		=__FALSE;
								save_param(SERVER_IP_ADDR,			server_ip,							sizeof(server_ip));
							}
							if(server_port_flag		==__TRUE)
							{
								server_port_flag		=__FALSE;
								save_param(SERVER_PORT_ADDR,		(uint8_t *)&server_port,	sizeof(server_port));	
							}	
							if(server_www_flag		==__TRUE)
							{
								server_www_flag		=__FALSE;
								save_param(SERVER_IP2_ADDR,			myself_server_ip,			sizeof(myself_server_ip));
								save_param(SERVER_IP2_ADDR,			(uint8_t *)&myself_server_port,		sizeof(myself_server_port));
							}			
							
								/*   从任务中列表中删除tcp_task */
							 os_tsk_delete(t_tcp_task);
							 os_dly_wait(10);
								/*   重启任务tcp_task */
						   t_tcp_task			=os_tsk_create_user 	(tcp_task, 		0, 	&tcp_stack, 		sizeof(tcp_stack));
				 }
		 } //end of if(DHCP_flag ==__FALSE) 
		 
		 if(change_by_web == 1)//web修改参数--写日志
		 {
			  change_by_web = 0;
			  /************************写日志*********************************/
			  tsk_lock ();  			 
				write_log_wr_fifo[write_log_wr_index][0] =1;			
				write_log_wr_fifo[write_log_wr_index][1] = LOG_SET_WEB;
				write_log_wr_fifo[write_log_wr_index][2] = 0xAA;
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
		 
		
		//关闭公司数据发送
// // // 		  if ((time_count_temp % HEART_TIME_TO_COMPANY) == 0)  //给公司电脑心跳
// // // 			{
// // // 			 /************************给公司监测电脑发送数据处理***************/
// // // 				if(1)
// // // 				{
// // // 					tsk_lock ();  			//  disables task switching 
// // // 					write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 					write_MySelf_fifo[write_MySelf_wr_index][1] = 0x79;	
// // // 					write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 					write_MySelf_fifo[write_MySelf_wr_index][3] = 0xFF;  

// // // 					if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 					if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 					{
// // // 						write_MySelf_counter=1;
// // // 						write_MySelf_buffer_overflow=__TRUE;
// // // 					};
// // // 					tsk_unlock (); 			//  enable  task switching 
// // // 					os_sem_send(eth_MySelf_semaphore);
// // // 				}
// // // 				/************************给公司监测电脑发送数据处理***************/
// // // 			}
	  if ((time_count_temp % HEART_TIME_TO_HOST) == 1)  //给后台服务器电脑心跳
		{
		 /************************给后台服务器电脑发送数据处理***************/
			if(1)
			{
				tsk_lock ();  			//  disables task switching 
				index_temp=0;
				write_Client_fifo[write_Client_wr_index][index_temp++] = 4;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x83;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x79;	
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x00;		
				write_Client_fifo[write_Client_wr_index][index_temp++] = 0x41;		

				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				os_sem_send(eth_client_sche_semaphore);
			}
			/************************给后台服务器电脑发送数据处理***************/
		}
		//计算交流电频率6秒 //3G故障上报
		if ((time_count_temp % ERROR_REPORT_TIME) == 0)               
		{					
			if((Power3G_Detect() == 0))
			{  
				ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min
				
				/************************上报异常*********************************/
				err_type = 6;
				msg_send_report = &err_type;
				os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
				if(error_report_time[5]==0)
				{
					error_report_time[5] = REPORT_WATCHDOG_TIME;
					if(1)//last_log_type != ALARM_LOG_TYPE_11)/*外部3G电源故障报警，且与上次写日志不一致，避免重复写日志*/
					{
						last_log_type = ALARM_LOG_TYPE_11;
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_RELEASE_POWER;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0x20;
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
					/************************上报异常*********************************/
					error_report_answer[5] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
					
// // // 					/************************给公司监测电脑发送数据处理***************/
// // // 					if(1)
// // // 					{
// // // 						tsk_lock ();  			//  disables task switching 
// // // 						write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 						write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][3] = POWER3G_ERROR;  //故障号

// // // 						if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 						if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 						{
// // // 							write_MySelf_counter=1;
// // // 							write_MySelf_buffer_overflow=__TRUE;
// // // 						};
// // // 						tsk_unlock (); 			//  enable  task switching 
// // // 						os_sem_send(eth_MySelf_semaphore);
// // // 					}
// // // 					/************************给公司监测电脑发送数据处理***************/
				}
				else
				{
					error_report_time[5] = REPORT_WATCHDOG_TIME;
				}
				/************************上报异常*********************************/							
			}  

			adc_result = ADC_PwrVolProc(&learning_transformer_flag,turn_num);
			if(adc_result>0)                                             //采集电压标志置位
			{
				ledlamp_err_count = LEDLAMP_ERR_FLICKER_TIME; //故障灯COM（led7）闪烁3min
				
				/************************上报异常*********************************/
				err_type = 7;
				msg_send_report = &err_type;
				os_mbx_send(err_report_mbox,(void*) msg_send_report,0x0f);
					
				if(error_report_time[6]==0)
				{
					error_report_time[6] = REPORT_WATCHDOG_TIME;
					
					if(1)//last_log_type != ALARM_LOG_TYPE_3)/*交流电报警，且与上次写日志不一致，避免重复写日志*/
					{
						last_log_type = ALARM_LOG_TYPE_3;
						tsk_lock ();  			 
						write_log_wr_fifo[write_log_wr_index][0] =1;			
						write_log_wr_fifo[write_log_wr_index][1] = LOG_RELEASE_POWER;
						write_log_wr_fifo[write_log_wr_index][2] = 0xD0;
						write_log_wr_fifo[write_log_wr_index][3] = 0x04;
						write_log_wr_fifo[write_log_wr_index][4] = 0xCB;
						
						if(adc_result == 1)//频率问题
							write_log_wr_fifo[write_log_wr_index][5] = 0;
						else//电压问题
							write_log_wr_fifo[write_log_wr_index][5] = (u8)(Pwr_Vol);	
						
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
					/************************上报异常*********************************/
					error_report_answer[6] = 0;//将该标志清零，以便等待上位机进行错误上报应答将该标志置1
					
// // // 					/************************给公司监测电脑发送数据处理***************/
// // // 					if(1)
// // // 					{
// // // 						tsk_lock ();  			//  disables task switching 
// // // 						write_MySelf_fifo[write_MySelf_wr_index][0] = 0x83;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][1] = 0xA7;	
// // // 						write_MySelf_fifo[write_MySelf_wr_index][2] = 0x00;		
// // // 						write_MySelf_fifo[write_MySelf_wr_index][3] = POWER_ERROR;  //故障号

// // // 						if (++write_MySelf_wr_index == WRITE_MySelf_SIZE) write_MySelf_wr_index=0;													
// // // 						if (++write_MySelf_counter  == WRITE_MySelf_SIZE)
// // // 						{
// // // 							write_MySelf_counter=1;
// // // 							write_MySelf_buffer_overflow=__TRUE;
// // // 						};
// // // 						tsk_unlock (); 			//  enable  task switching 
// // // 						os_sem_send(eth_MySelf_semaphore);
// // // 					}
// // // 					/************************给公司监测电脑发送数据处理***************/						
				}
				else
				{
					error_report_time[6] = REPORT_WATCHDOG_TIME;
				}
				/************************上报异常*********************************/	
			}
		}
		
		//计算放电检测5s
		if ((time_count_temp % 10) == 0)
		{
 			Machine_Set_Detect();
		}		
		//定时上报灯电流值最小30min
		if (((time_count_temp % (LampmA_ReportTime*2*60)) == 0)||(bLamp_Phyerr_Report == (__TRUE)))
		{
			report_lampcurrent_flag = (__TRUE);
			if(bLamp_Phyerr_Report == (__TRUE))
			{
				bLamp_Phyerr_Report = (__FALSE);
			}
			else
			{
				time_count_temp = 0;
			}
			/************************上报电流*********************************/
			if(1)/*上报*/
			{
				//添加发送报警数据
				
				Report_Lamp_PhyLevel();
				
				client_data_type = 2;//数据类型  2--其他数据
				tsk_lock ();  			//  disables task switching 
				write_Client_fifo[write_Client_wr_index][0] = 81;	
				write_Client_fifo[write_Client_wr_index][1] = 0x83;		
				write_Client_fifo[write_Client_wr_index][2] = 0xCA;	
				write_Client_fifo[write_Client_wr_index][3] = 0x00;		
				write_Client_fifo[write_Client_wr_index][4] = 11;		//	
				
				for(i=0;i<11*7;i++)
				{
					write_Client_fifo[write_Client_wr_index][i+5]=lamp_report_table[i];
				}
				
				if (++write_Client_wr_index == WRITE_Client_SIZE) write_Client_wr_index=0;													
				if (++write_Client_counter  == WRITE_Client_SIZE)
				{
					write_Client_counter=1;
					write_Client_buffer_overflow=__TRUE;
				};
				tsk_unlock (); 			//  enable  task switching 	
				os_sem_send(eth_client_sche_semaphore);
			}
			/************************上报异常*********************************/		
		}
	}
	 
 }
__task void log_write_task(void)
{
	u8 *msg;
	u8 Log_Type;
	u8 Log_Count;
	u8 Word_data[21];
	u8 i,j,k;
		
	static	u8 read_temp[21];
	u8 *pRead = NULL;
		
	u32 	            Data_addr_temp 				= 0;
	u32 	            Data_addr_old 				= 0;
	u32 	            Flag_addr_temp 				= 0;	
	u32 	            Flag_addr_old 				= 0;	
	u32								No_line_temp					= 0;	//第几行
	
	u32								Index1_lines_count  = 0;
	u32								Index2_lines_count  = 0;
	u32								Index2_lines_count_temp  = 0;

	u8								line_bytes_temp			= 0;	//每行字节数

	u8							  ID_temp						  =0;
	u8							  count_index_temp		=0;
	u8							  sub_target_temp		  =0;
	
	u8							  index1_temp		      =1;
	u8							  index2_temp		      =0;

	u8  time_temp[4]={0};
	u8  write_temp[20]={0};
	
	u32  occur_time=0;
			
	ID_temp						=	0x92;
	count_index_temp	=	2;
	sub_target_temp	  =	0;
	
	Data_addr_old	= Object_table[ID_temp-0X81].Data_Addr;//获得数据地址
	Data_addr_temp = Data_addr_old;
	Flag_addr_old = Object_table[ID_temp-0X81].Flag_Addr;//获得标志地址
	Flag_addr_temp = Flag_addr_old;



	while(1)
	{
		os_mbx_wait(Write_Log_Mbox, (void *)&msg, 0xffff);
		
		tsk_log_run = (_TRUE);//启动看门狗监测该任务
		
		tsk_lock();           	 //  disables task switching 
		if(write_log_counter !=0)
		{
			for(i=0;i<21;i++)    //  把收到的所有的数据拷贝到temp里面，
			{
				read_temp[i] = write_log_wr_fifo[write_log_rd_index][i];
			}
			if (++write_log_rd_index == WRITE_LOG_SIZE) write_log_rd_index=0;
					
			--write_log_counter;				 			
		}
		tsk_unlock();            //  enable  task switching
					
		pRead=read_temp;
		
		Log_Count = *(pRead++);
		
		os_mut_wait(I2c_Mut_lm,0xFFFF);
		occur_time = Read_DS3231_Time();//事件发生时间//待开
		os_mut_release(I2c_Mut_lm);
			
		time_temp[0] = (u8)((occur_time & 0xFF000000) >> 24);
		time_temp[1] = (u8)((occur_time & 0x00FF0000) >> 16);
		time_temp[2] = (u8)((occur_time & 0x0000FF00) >> 8);
		time_temp[3] = (u8)( occur_time & 0x000000FF);	

//os_mut_wait(Spi_Mut_lm,0xFFFF);	
		
		for(k = 0; k < Log_Count; k++)
		{
			Log_Type = *(pRead++)+1;
			Word_data[0] = *(pRead++);
			Word_data[1] = *(pRead++);
			Word_data[2] = *(pRead++);
			Word_data[3] = *(pRead++);				
			
			Data_addr_temp	=	Data_addr_old+1;	//跳过整表前三个标志字节
			index1_temp 		=	Log_Type;
//				os_mut_wait(Spi_Mut_lm,0xFFFF);	
			index2_temp  	= Rd_u8(Log_Type - 1 + INDEX_92_ADDR_START); //((Log_Type-1)*(255*(10+2))+(old_address+3) +1 );
//				os_mut_release(Spi_Mut_lm);
			
			if(index2_temp ==TAB_92_S_NUMBER)	
			{
				index2_temp=1;
			}					
			else 
			{					
				index2_temp++;
			}
//=========================================				
			write_temp[0]=index1_temp;
			write_temp[1]=index2_temp;
			
			write_temp[2]=time_temp[0];
			write_temp[3]=time_temp[1];
			write_temp[4]=time_temp[2];
			write_temp[5]=time_temp[3];
			
			write_temp[6]=Word_data[0];
			write_temp[7]=Word_data[1];
			write_temp[8]=Word_data[2];
			write_temp[9]=Word_data[3];

//					os_mut_wait(Spi_Mut_lm,0xFFFF);	
		if(index1_temp <= Object_table[ID_temp-0X81].Index1)
		{			
			No_line_temp=(index1_temp-1)*TAB_92_S_NUMBER+index2_temp;	//应该写入的实际行号			
			line_bytes_temp  = 10;	//获得行字节数
			
			if(sub_target_temp ==0)  													//整行
			{	
				Wr_u8(Flag_addr_temp,TAB_OK_FLAG);						//写整表标志位		
				Wr_u8((Flag_addr_temp + No_line_temp),LINE_OK_FLAG) ;//写行标志位
				
				Data_addr_temp += (No_line_temp-1)*(line_bytes_temp);	//跳过前面N-1行的地址
				for(j=0;j<line_bytes_temp;j++)
				{
					Wr_u8(Data_addr_temp++,write_temp[j]) ;	
				}
				Wr_u8((Log_Type - 1 + INDEX_92_ADDR_START ),index2_temp); //=>>写到第几个索引2（流水号）  另开辟空间
			}						
		}
		//==========line 行数处理================================
		for(j=0 ; j< Object_table[ID_temp-0X81].Index1 ; j++)
		{
			for(i=0 ; i< Object_table[ID_temp-0X81].Index2 ; i++)
			{
				if(Rd_u8(Flag_addr_old + 1 + i + j*(Object_table[ID_temp-0X81].Index2)) == LINE_OK_FLAG)
				{
					Index2_lines_count_temp++;
				}
			}
			if(Index2_lines_count_temp != 0)
			{
				Index1_lines_count++;
			}
			if(Index2_lines_count < Index2_lines_count_temp)
			{
				Index2_lines_count = Index2_lines_count_temp;
				Index2_lines_count_temp = 0;
			}
		}
		Wr_u8(Data_addr_old , Index1_lines_count );					//写索引一有效行数
//					os_mut_release(Spi_Mut_lm);
		}
		pRead=read_temp;
		read_temp[0]=0;
		
		tsk_log_over = (_TRUE);//喂狗
	}	

}

u8  copy_to_eth(u8 *p_Data,u32 lenth)
{
	u32 			i    		 								=0;
	
	u32 			old_tx_wr_index					=0;
//	u32 			old_tx_counter					=0;
	
	u32 			old_tx_len_wr_index			=0;
	u32 			old_tx_len_counter			=0;	
	
	u8  			buff_overflow_temp      =0;
	u8  			len_overflow_temp     	=0;

	tsk_lock ();  			//  disables task switching 
	
	old_tx_wr_index 			=tx_wr_index;
	old_tx_wr_index 			=tx_counter;
	
	old_tx_len_wr_index 	=tx_wr_index;
	old_tx_len_counter 		=tx_counter;
	
	for(i = 0;i < lenth;i++)
	{
			eth_tx_fifo[tx_wr_index] = p_Data[i];
			if (++tx_wr_index == TX_FIFO_SIZE) tx_wr_index=0;
			if (++tx_counter  == TX_FIFO_SIZE)
			{
				//tx_counter=1;
				//tx_buffer_overflow=__TRUE;
				buff_overflow_temp	=__TRUE;
			};												
	}	

	eth_tx_len[tx_len_wr_index] = lenth;
	if (++tx_len_wr_index == TX_LEN_SIZE) tx_len_wr_index=0;
	if (++tx_len_counter  == TX_LEN_SIZE)
	{
		/*  发送的信息的条数大于最大值(now =1000)  */
		//tx_len_counter=1;
		//tx_len_overflow=__TRUE;   
		len_overflow_temp	=__TRUE;
	};
	
	if(buff_overflow_temp	==__TRUE)
	{
		tx_wr_index  	=old_tx_wr_index;
		tx_counter		=old_tx_wr_index;
		tx_wr_index		=old_tx_len_wr_index;
		tx_counter		=old_tx_len_counter;
	}
	/*
	有待进一步处理
	else if(len_overflow_temp	==__TRUE)     
	{
		len_overflow_temp			=__FALSE;
	}
	*/
	tsk_unlock (); 			//  enable  task switching 	

	if(buff_overflow_temp	==__TRUE) 
		return (1);
	else 
		return (0);
}

extern void USART_Protocol_Communication(USART_DATA *p_usart_controlblk);
USART_DATA   usart_controlblk_task = {0};            //USART临时通信控制块 
__task void eth_usart_communication_task(void)
{	
	u8 		*msg_send=NULL;
 	u8 		Type;
	u8  	i;
	
	for(;;)
	{
		os_mbx_wait (Communication_Mbox, (void *)&msg_send, 0xffff);
	
		tsk_eth_run = (_TRUE);//启动看门狗监测该任务
		
		Type = *msg_send;

		if(Type == 1)   /*以太网 */ 
		{
			os_mut_wait(communication_rx_mut,0xFFFF);
			
			usart_controlblk_task = USART_ControlBlk;
			
			USART_ControlBlk.tx_count = 0;  
			USART_ControlBlk.rx_count = 0;  
			
			os_mut_release(communication_rx_mut);
		}
		else   // USART2
		{
			//os_mut_wait(communication_rx_mut,0xFFFF);
			
			for(i=0;i<usart2_rx_counter;i++)
			{
				usart_controlblk_task.usart_rx[i] = usart2_rx_buff[i];
			}
			
			usart_controlblk_task.rx_count = usart2_rx_counter;
			
			//os_mut_release(communication_rx_mut);
		}
		
		USART_Protocol_Communication(&usart_controlblk_task);
		
		if(Type == 1)   /*以太网 */    
		{
			if(usart_controlblk_task.tx_count!=0)
			{
				if(copy_to_eth(usart_controlblk_task.usart_tx,usart_controlblk_task.tx_count) ==0)/*	已经正确发送到ETH-FIFO */  
				{
					os_sem_send(eth_send_data_semaphore);	
				}
				else
				{
					/*  reserve  */
				}
			}
		}
		else   					/*串口 */
		{		
			if(usart_controlblk_task.flag_tx_enable)                           //串口&以太网发送
			{
				USART_Tx_nByte(USART2, (u8*)usart_controlblk_task.usart_tx, usart_controlblk_task.tx_count);
			}
		}
		
		tsk_eth_over = (_TRUE);//喂狗
	}
}
/*--------------------------- fputc -----------------------------------------*/
int fputc (int ch, FILE *f)  {
  /* Debug output to serial port. */

  if (ch == '\n')  {
    while (!(USART2->SR & 0x0080));
    USART2->DR = 0x0D;
  }
  while (!(USART2->SR & 0x0080));
  USART2->DR = (ch & 0xFF);
  return (ch);
}

extern volatile unsigned char RTH_link_ok_lm;
extern  U16  read_PHY (U32 PhyReg);
/*--------------------------- dhcp_check ------------------------------------*/
static void dhcp_check () {
  /* Monitor DHCP IP address assignment. */

  if (tick == __FALSE || dhcp_tout == 0) {
    return;
  }

  tick = __FALSE;

  if (mem_test (&MY_IP, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) {
    /* Success, DHCP has already got the IP address. */
    dhcp_tout = 0;	
    LCDupdate = __TRUE;
    return;
  }
  if (--dhcp_tout == 0) {
    /* A timeout, disable DHCP and use static IP address. */
    dhcp_disable ();
    LCDupdate = __TRUE;
    dhcp_tout = 30 | 0x80000000;
    return;
  }
  if (dhcp_tout == 0x80000000) {
    dhcp_tout = 0;
		
// 		read_param(LOCAL_NET_INFO_ADDR,				(uint8_t*)&LocM,				sizeof(LocM));
	read_param(LOCAL_IP_ADDR,				LocM.IpAdr,				sizeof(LocM.IpAdr));
	read_param(LOCAL_MASK_ADDR,			LocM.NetMask,			sizeof(LocM.NetMask));
	read_param(LOCAL_GATEWAY_ADDR,	LocM.DefGW,				sizeof(LocM.DefGW));
	read_param(LOCAL_DNS_ADDR,			LocM.PriDNS,			sizeof(LocM.PriDNS));
		
    LCDupdate = __TRUE;
  }
}

/*************callback function************/
U16 tcp_callback_server (U8 soc, U8 event, U8 *ptr, U16 par)   //This function is called on TCP event 
{
	u32 i = 0;
	
	u8	*msg_send						= NULL;
	static u8 type = 1;
	msg_send = &type;
	switch (event) 
	{
	  case TCP_EVT_DATA:
      /* TCP data frame has arrived, data is located at *par1, */
					if((par > 0) && (ptr[0]==0x21))
					{
							tsk_lock ();  			//  disables task switching 
						
							/* Read one Frame from the TCP Ethernet receive   */
							for(i = 0;i < par-1;i++)
							{						
									USART_ControlBlk.usart_rx[i] = ptr[i+1];										
							}			
							soc_state_lm=__TRUE;
							USART_ControlBlk.rx_count =par-1;
							
							tsk_unlock (); 			//  enable  task switching 	
							
							os_mbx_send (Communication_Mbox, msg_send, 0x00FF);	
					}					
      return (1);	
				
    case TCP_EVT_CONREQ:
      /* Remote peer requested connect, accept it */
      return (1);
    case TCP_EVT_CONNECT:
      /* The TCP socket is connected */
      return (1);	
    case TCP_EVT_ACK: 
				server_wait_ack	=__FALSE;
            break;			
	}  
	return (0);
}
U16 tcp_callback_client (U8 soc, U8 event, U8 *ptr, U16 par)   //This function is called on TCP event
{   
    switch (event) 
    {
        case TCP_EVT_CONNECT:
             cli_connect_ser_flag = 1;				
            break;
        case TCP_EVT_ACK: 
						cli_connect_wait_ack=__FALSE;
            break;
        case TCP_EVT_CLOSE:
            break;
    }
    return (0);
}
U16 tcp_callback_myself (U8 soc, U8 event, U8 *ptr, U16 par)   //This function is called on TCP event
{   
    switch (event) 
    {
        case TCP_EVT_CONNECT:
             cli_connect_ser_flag = 1;				
            break;
        case TCP_EVT_ACK: 
						cli_connect_wait_ack=__FALSE;
            break;
        case TCP_EVT_CLOSE:
            break;
    }
    return (0);
}


/*---------------------------------------------------------------------------*/
__task void tcp_task (void) {
  /* Main Thread of the TcpNet. This task should have */
  /* the lowest priority because it is always READY. */

	u32 i = 0;//,q;
	int regv=0;
	u8 *msg;

	u32 semaphore_one_ok=0;	
	u32 semaphore_two_ok=0;
	u32 semaphore_thr_ok=0;
	
	u8 connect_index[WRITE_Client_SIZE+1] ={0};
	u32 connect_times =0;
	
	
	U8 *sendbuf;  
	u32 sendbuf_len; //by csj
	u32 maxlen;	
	u32 tx_lenth_temp;	
	
	U16	report_size = 0;				/*上报的字数，电流上报为81B，其他为8B*/
	
  dhcp_tout = DHCP_TOUT;

	RCC_MCOConfig(RCC_MCO_HSE);  /* 使能MCO输出25Mhz时钟到DP83848 */

	mem_copy (own_hw_adr, mac_adr, 6); /* 拷贝MAC地址  */

	init_TcpNet();

	read_param(LOCAL_IP_ADDR,				LocM.IpAdr,				sizeof(LocM.IpAdr));
	read_param(LOCAL_MASK_ADDR,			LocM.NetMask,			sizeof(LocM.NetMask));
	read_param(LOCAL_GATEWAY_ADDR,	LocM.DefGW,				sizeof(LocM.DefGW));
	read_param(LOCAL_DNS_ADDR,			LocM.PriDNS,			sizeof(LocM.PriDNS));

		tcp_soc_server = tcp_get_socket (TCP_TYPE_SERVER, 0, 120, tcp_callback_server);	
		if (tcp_soc_server != 0) 
				tcp_listen (tcp_soc_server, local_port);
		
		tcp_soc_client = tcp_get_socket (TCP_TYPE_CLIENT,0,120,tcp_callback_client);
		tcp_soc_myself = tcp_get_socket (TCP_TYPE_CLIENT,0,120,tcp_callback_myself);


  while (1) {
		
		tsk_tcp_run = (_TRUE);//喂狗
		
    main_TcpNet();
		if(DHCP_flag !=__FALSE)  dhcp_check();   //  动态IP获取
		
		if(soc_state_lm == __TRUE)
		{
			soc_state_lm = __FALSE;
			
// 			usart_rx[USART_RX_BUFSIZE];
// 			rx_count;                   //接收字节数量	


		}		
	
		if(tx_buffer_overflow == __TRUE)
			tx_buffer_overflow == __FALSE;

		//--------------------- start-AAAA--------------------------
		if(os_sem_wait(eth_send_data_semaphore,0) ==OS_R_OK )semaphore_one_ok +=1;
		if(semaphore_one_ok>0 )//服务器端口响应请求数据
		{
			if (tcp_check_send (tcp_soc_server)) 
			{
				semaphore_one_ok  -=1;

				
				tsk_lock();           	 //  disables task switching 
				if(tx_len_counter !=0)    																 //得到待发数据的长度
				{
					 tx_lenth_temp=eth_tx_len[tx_len_rd_index];
					 if (++tx_len_rd_index == TX_LEN_SIZE) tx_len_rd_index=0;
					 --tx_len_counter;
				}
				tsk_unlock();            //  enable  task switching 

				maxlen = tcp_max_dsize (tcp_soc_server);
				sendbuf = tcp_get_buf (maxlen);	
				sendbuf[0]=0x21;	
				
				if(tx_lenth_temp > (maxlen -1))
				{
						tsk_lock();           	 //  disables task switching 
						if(tx_counter !=0)
						{
							for(i=0;i<(maxlen-1);i++)    
							{
								sendbuf[1+i] = eth_tx_fifo[tx_rd_index];
								if (++tx_rd_index == TX_FIFO_SIZE) tx_rd_index=0;
							}
							tx_counter -= (maxlen-1);   /*   清空  ETH_FIFO  */
						}
						tsk_unlock();            //  enable  task switching 				
						
						sendbuf_len = maxlen;
						tcp_send (tcp_soc_server,sendbuf,sendbuf_len);
						
						tx_lenth_temp -=(maxlen-1);
				}	
				else
				{
						tsk_lock();           	 //  disables task switching 
						if(tx_counter !=0)
						{
							for(i=0;i<tx_lenth_temp;i++)    
							{
								sendbuf[1+i] = eth_tx_fifo[tx_rd_index];
								if (++tx_rd_index == TX_FIFO_SIZE) tx_rd_index=0;
							}
							tx_counter -= tx_lenth_temp;   /*   清空  ETH_FIFO  */
						}
						tsk_unlock();            //  enable  task switching 				
						
						sendbuf_len = tx_lenth_temp+1;
						tcp_send (tcp_soc_server,sendbuf,sendbuf_len);
						
						tx_lenth_temp =0;
				}
			//===========================
				do
				{
// 					IWDG_ReloadCounter();   /* 喂狗 */  // IWDG_LM
					main_TcpNet();
					if (tcp_check_send (tcp_soc_server)) 
					{
						if(tx_lenth_temp > (maxlen-1) )
						{
								maxlen = tcp_max_dsize (tcp_soc_server);
								sendbuf = tcp_get_buf (maxlen);							
							
								tsk_lock();           	 //  disables task switching 
								if(tx_counter !=0)
								{
									for(i=0;i<(maxlen-1);i++)    
									{
										sendbuf[i] = eth_tx_fifo[tx_rd_index];
										if (++tx_rd_index == TX_FIFO_SIZE) tx_rd_index=0;
									}
									tx_counter -= (maxlen-1);   /*   清空  ETH_FIFO  */
								}
								tsk_unlock();            //  enable  task switching 				
								
								sendbuf_len = maxlen-1;
								tcp_send (tcp_soc_server,sendbuf,sendbuf_len);
								
								tx_lenth_temp -=(maxlen-1);
						}
						else if(tx_lenth_temp >0)
						{
								maxlen = tcp_max_dsize (tcp_soc_server);
								sendbuf = tcp_get_buf (maxlen);							
								tsk_lock();           	 //  disables task switching 
								if(tx_counter !=0)
								{
									for(i=0;i<tx_lenth_temp;i++)   
									{
										sendbuf[i] = eth_tx_fifo[tx_rd_index];
										if (++tx_rd_index == TX_FIFO_SIZE) tx_rd_index=0;
									}
									tx_counter -= tx_lenth_temp;   /*   清空  ETH_FIFO  */
								}
								tsk_unlock();            //  enable  task switching 				
								
								sendbuf_len = tx_lenth_temp;
								tcp_send (tcp_soc_server,sendbuf,sendbuf_len);
								
								tx_lenth_temp =0;
						}
					
				}
			}while(tx_lenth_temp !=0);
			
			}
		}	
	//---------------------- end-AAAA -----------------------------

	//------------------------- start-BBBB -------------------------- 
		if(os_sem_wait(eth_client_sche_semaphore,0) ==OS_R_OK )
		{
			semaphore_two_ok +=1; 	//等待信号的到来
			
		}
		if(semaphore_two_ok>0 )	
		{
			switch (tcp_get_state (tcp_soc_client)) //client launch a connet to the server 
			{

				case TCP_STATE_FREE:
				case TCP_STATE_CLOSED:
						 if(connect_index[semaphore_two_ok] ==0)
						 {
							  connect_index[semaphore_two_ok]++;
								tcp_connect(tcp_soc_client, server_ip, server_port, 0);  
						 }
				case TCP_STATE_CONNECT:
						if (tcp_check_send (tcp_soc_client))
						{
							maxlen = tcp_max_dsize (tcp_soc_client);
							sendbuf = tcp_get_buf (maxlen);
							sendbuf[0]=0x21;
							
							tsk_lock();           	 //  disables task switching 
							if(write_Client_counter !=0)//仅仅只赋值一条??
							{
								report_size = write_Client_fifo[write_Client_rd_index][0];
								for(i=0;i<report_size;i++)    //  把收到的所有的数据拷贝到sendbuf里面，
								{
									sendbuf[1+i] = write_Client_fifo[write_Client_rd_index][i+1];
								}
								if (++write_Client_rd_index == WRITE_Client_SIZE) write_Client_rd_index=0;
												
								--write_Client_counter;
										
							}
							tsk_unlock();            //  enable  task switching 	
							
							eth_connect_count = 240;//2min
							eth_restart_count = 120*30;//30min

							//os_mut_release (Client_Data_Send_Mutex);			      //释放互斥锁		
							
							tcp_send (tcp_soc_client,sendbuf,report_size+1);
							
							connect_index[semaphore_two_ok]=0;
							connect_index[semaphore_two_ok-1]=0;//为下一个
			        
							semaphore_two_ok -=1;

						//	tcp_close(tcp_soc_client);
						}
 						IWDG_ReloadCounter();   /* 喂狗 */  // IWDG_LM
						 break;
			}	//end of switch	
		
			if(semaphore_two_ok>=WRITE_Client_SIZE)  //   以太网没能连接上，需要上传的数据条fifo满了。
			{
				semaphore_two_ok = WRITE_Client_SIZE;
				write_Client_buffer_overflow=__FALSE;
			}
		}
	//--------------------  edn-BBBB------------------------ 	

		tsk_tcp_over= (_TRUE);//喂狗
		os_tsk_pass();
  }
}
/*---------------------------------------------------------------------------*/
__task void key_task (void) {

	printf("复位啦\n");
	printf("%s\n",machine_addr);
	while(1)
	{
		os_dly_wait(10);
			
		/*若断网30min，则重启TCP*/
		if(net_state_flag == 1)
		{
			net_state_flag = 0;
			eth_restart_count = NET_STOP_TIME;
// 			/*  软件复位 */
// 			NVIC_SystemReset();		
			
			/*   从任务中列表中删除tcp_task */
		 os_tsk_delete(t_tcp_task);
		 os_dly_wait(5);
			/*   重启任务tcp_task */
		 t_tcp_task	=os_tsk_create_user 	(tcp_task, 		0, 	&tcp_stack, 		sizeof(tcp_stack));				 
		}
		tsk_watchdog_reload();
		switch (Scan_Key())
	  {

			case MAN_KEY: 	 			//  "人工"立即
			
				break;
			case STUDY_KEY: 			//  "学习"立即
				start_learning_flag = 1;
				break;	
			case RESET_ALL_KEY: 	//  "人工"延时(出厂化设置)
					/*  擦除EEPROM中所有的数据 */
					EEP_erase_ALL();  
					/*  把程序默认的参数卸任EEPROM中 */
					save_default_parameter();
					/*  关闭中断 */
		 			__set_FAULTMASK(1); 	
					/*  软件复位 */
					NVIC_SystemReset();		
				
				break;		
			case DELAY_STUDY_KEY: //  "学习"延时(保留)
				
				break;						

			default:

			break;
	  }	//  end of switch (Scan_Key())
		
	}
}

int main (void) {
  /* Start with 'init' task. */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x9000);
  os_sys_init(init);
  while(1);
}




/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
