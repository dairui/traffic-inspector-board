#ifndef __DEFINE_LM_H
#define __DEFINE_LM_H

/* 每个INPUT一个周期内转换的次数，必须为4的倍数 */
#define TRANSFORM_TIMES 	(12)  
	
/* =100*操作系统时钟周期 */	
#define ADC_CYCLE_TIME    240   


#define ADC1_SampleTime_cicr   ADC_SampleTime_55Cycles5   

//=========================================
//=========================================
//=========================================

#define EE_START_ADDR 				0x0000    	//EEPROM的起始地址
#define EE_END_ADDR   					0x1FFF    	//EEPROM的终止地址
/*   参数存储   */
#define EE_PARAMETER_START  		(EE_END_ADDR -255)  		//参数存储 起始地址  (0x1FFF - 0x1F00 =255)

#define FLAG_1_ADDR							(EE_PARAMETER_START)											/*  一些标志位   */  
#define ALARM_RE_FLAG_ADDR			(FLAG_1_ADDR)												 		 	/*  定时重启设备标志   */  
#define ALARM_RE_FLAG_BIT				(0)												//bit: 0  		 
#define DHCP_FLAG_ADDR					(FLAG_1_ADDR)															/*  DHCP启用标志   */ 
#define DHCP_FLAG_BIT						(1)												//bit: 1  	

#define PASSWORD_ADDR						(FLAG_1_ADDR 					+1)		/* 16B */  		 	/*  密码   */

#define AC_PW_VOLT_ADDR					(PASSWORD_ADDR 				+16)	/* 2B */  		 	/*  交流电源电压   */
#define AC_PW_FREQ_ADDR					(AC_PW_VOLT_ADDR 			+2)		/* 2B */   			/*  交流电源频率   */
#define BAT_TIME_ADDR						(AC_PW_FREQ_ADDR 			+2)		/* 4B */   			/*  后备电池参数(上次放电时间)   */

#define ALARM_RE_HOUR_ADDR			(BAT_TIME_ADDR 		  +4)	/* 1B */   			/*  定时重启－时   */
#define ALARM_RE_minu_ADDR			(ALARM_RE_HOUR_ADDR 	+1)		/* 1B */   			/*  定时重启－分   */

#define TURN_NUM_ADDR						(ALARM_RE_minu_ADDR 	+1)		/* 8B */   			/*  电压匝数比 LXB已将之改为标准变压220v  */
#define REAL_TURN_NUM_ADDR			(TURN_NUM_ADDR 				+8)		/* 8B */   			/*  电压匝数比  REAL */
#define BAT_MIN_ADDR						(REAL_TURN_NUM_ADDR				+8)		/* 2B */   			/*  后备电池报警阀值(分钟)   */
#define CUR_MIN_ADDR						(BAT_MIN_ADDR					+2)		/* 2B */   			/*  信号灯电流报警阀值(mA)   */
#define SEND_TIME_ADDR					(CUR_MIN_ADDR					+2)		/* 2B */   			/*  信号灯电流上报时间(分钟)   */

#define MACHINE_ADDR_ADDR				(SEND_TIME_ADDR				+2)		/* 31B  */   		/*  信号机地址长度及内容   */
#define MACHINE_TYPE_ADDR				(MACHINE_ADDR_ADDR		+31)	/* 8B */   		  /*  信号机型号长度及内容 */



typedef struct
{
    uint32_t bit0:1;
    uint32_t bit1:1;
    uint32_t bit2:1;
    uint32_t bit3:1;
    uint32_t bit4:1;
    uint32_t bit5:1;
    uint32_t bit6:1;
    uint32_t bit7:1;
    uint32_t bit8:1;
    uint32_t bit9:1;
    uint32_t bit10:1;
    uint32_t bit11:1;
    uint32_t bit12:1;
    uint32_t bit13:1;
    uint32_t bit14:1;
    uint32_t bit15:1;
    uint32_t bit16:1;
    uint32_t bit17:1;
    uint32_t bit18:1;
    uint32_t bit19:1;
    uint32_t bit20:1;
    uint32_t bit21:1;
    uint32_t bit22:1;
    uint32_t bit23:1;
    uint32_t bit24:1;
    uint32_t bit25:1;
    uint32_t bit26:1;
    uint32_t bit27:1;
    uint32_t bit28:1;
    uint32_t bit29:1;
    uint32_t bit30:1;
    uint32_t bit31:1;
}BIT_BAND_FLAG __attribute__((bitband));

//================   变量状态发生改变的标志    start  ==============
#define local_ip_flag      			(bit_band_flag_1.bit0) 
#define local_mask_flag       	(bit_band_flag_1.bit1) 	
#define local_gateway_flag    	(bit_band_flag_1.bit2) 	
#define local_port_flag    			(bit_band_flag_1.bit3) 
#define local_mac_flag    			(bit_band_flag_1.bit4)  
#define local_dns_flag					(bit_band_flag_1.bit5) 	
#define server_ip_flag    			(bit_band_flag_1.bit6)  
#define server_port_flag    		(bit_band_flag_1.bit7)
#define server_www_flag    			(bit_band_flag_1.bit8)

#define userName_flag 					(bit_band_flag_1.bit9)   	// 有户名是否匹配
#define password_flag 					(bit_band_flag_1.bit10)   // 密码是否正确
#define ch_pw_flag 							(bit_band_flag_1.bit11)   // 是否修改密码

#define reset_all_flag 					(bit_band_flag_1.bit12)   //  恢复出厂设置标志
#define restudy_flag 						(bit_band_flag_1.bit13)   //  重新学习标志
#define restart_flag 						(bit_band_flag_1.bit14)   //  重启设备标志

#define turn_num_flag		 	 			(bit_band_flag_1.bit17)		//  交流电标准电压
#define bat_min_flag			  		(bit_band_flag_1.bit18) 	//  后备电池报警阀值(分钟)
#define cur_min_flag						(bit_band_flag_1.bit19)   //  信号灯电流异常报警阀值(mA)
#define send_time_flag					(bit_band_flag_1.bit20)   //  信号灯电流上报时间(分钟)

#define change_alarm_restart_flag 		(bit_band_flag_1.bit15)   //  alarm_restart_flag标志发生改变
#define change_DHCP_flag   						(bit_band_flag_1.bit16)  	//  DHCP_flag标志发生改变
//================   变量状态发生改变的标志    end  ==============


#define turn_num_lxb_flag		 	 			(bit_band_flag_1.bit23)		//  变压器匝数比
#define net_state_flag		 	 			(bit_band_flag_1.bit24)		//  NET灯状态bit0

//================   巡检仪运行参数(存入EEPROM) 发生改变的标志    start  ==============  
#define alarm_restart_flag 			(bit_band_flag_1.bit21)   //  定时重启设备标志
#define DHCP_flag   						(bit_band_flag_1.bit22)  	//	DHCP 是否启用 状态
//================   巡检仪运行参数(存入EEPROM) 发生改变的标志    end  ==============  

//================   任务运行与运行结束标志标志   start  ==============
#define tsk_tcp_run      			(bit_band_flag_tsk.bit0)   	// tcp_tsk运行
#define tsk_log_run       		(bit_band_flag_tsk.bit1) 	  // log_write_task运行
#define tsk_eth_run    				(bit_band_flag_tsk.bit2)   	// eth_usart_communication_task运行 	
#define tsk_ADC_run    				(bit_band_flag_tsk.bit3)   	// ADC_task运行 
#define tsk_learn_run    			(bit_band_flag_tsk.bit4)   	// learn_detect_task运行  
#define tsk_power_run					(bit_band_flag_tsk.bit5)   	// power_detect_task运行 

#define tsk_tcp_over    			(bit_band_flag_tsk.bit6)   	// tcp_tsk运行 完成 
#define tsk_log_over    			(bit_band_flag_tsk.bit7)   	// log_write_task运行	完成
#define tsk_eth_over    			(bit_band_flag_tsk.bit8)   	// eth_usart_communication_task运行	完成
//================   任务运行与运行结束标志标志    end  ==============

#define start_learning_flag		(bit_band_flag_tsk.bit9)   	//学习标志
#define first_start_learning	(bit_band_flag_tsk.bit13)   	//第一次学习标志
#define learning_success_flag	(bit_band_flag_tsk.bit14)   	//学习成功标志
#define learning_fail_flag	(bit_band_flag_tsk.bit16)   	//学习失败标志
#define bSet_Valid_Passage	(bit_band_flag_tsk.bit15)   	//web组合通道设定标志

#define report_lampcurrent_flag		(bit_band_flag_tsk.bit10)   	//上报灯电流标志
#define Flag_ErrorReportHalt		(bit_band_flag_tsk.bit11) //例常错误暂停上报

#define bfeed_hard_dog		(bit_band_flag_tsk.bit12) //喂狗硬件看门狗标志
//=========================================

//=========================================
//=========================================



#endif


