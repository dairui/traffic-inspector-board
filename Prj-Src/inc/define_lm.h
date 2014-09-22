#ifndef __DEFINE_LM_H
#define __DEFINE_LM_H

/* ÿ��INPUTһ��������ת���Ĵ���������Ϊ4�ı��� */
#define TRANSFORM_TIMES 	(12)  
	
/* =100*����ϵͳʱ������ */	
#define ADC_CYCLE_TIME    240   


#define ADC1_SampleTime_cicr   ADC_SampleTime_55Cycles5   

//=========================================
//=========================================
//=========================================

#define EE_START_ADDR 				0x0000    	//EEPROM����ʼ��ַ
#define EE_END_ADDR   					0x1FFF    	//EEPROM����ֹ��ַ
/*   �����洢   */
#define EE_PARAMETER_START  		(EE_END_ADDR -255)  		//�����洢 ��ʼ��ַ  (0x1FFF - 0x1F00 =255)

#define FLAG_1_ADDR							(EE_PARAMETER_START)											/*  һЩ��־λ   */  
#define ALARM_RE_FLAG_ADDR			(FLAG_1_ADDR)												 		 	/*  ��ʱ�����豸��־   */  
#define ALARM_RE_FLAG_BIT				(0)												//bit: 0  		 
#define DHCP_FLAG_ADDR					(FLAG_1_ADDR)															/*  DHCP���ñ�־   */ 
#define DHCP_FLAG_BIT						(1)												//bit: 1  	

#define PASSWORD_ADDR						(FLAG_1_ADDR 					+1)		/* 16B */  		 	/*  ����   */

#define AC_PW_VOLT_ADDR					(PASSWORD_ADDR 				+16)	/* 2B */  		 	/*  ������Դ��ѹ   */
#define AC_PW_FREQ_ADDR					(AC_PW_VOLT_ADDR 			+2)		/* 2B */   			/*  ������ԴƵ��   */
#define BAT_TIME_ADDR						(AC_PW_FREQ_ADDR 			+2)		/* 4B */   			/*  �󱸵�ز���(�ϴηŵ�ʱ��)   */

#define ALARM_RE_HOUR_ADDR			(BAT_TIME_ADDR 		  +4)	/* 1B */   			/*  ��ʱ������ʱ   */
#define ALARM_RE_minu_ADDR			(ALARM_RE_HOUR_ADDR 	+1)		/* 1B */   			/*  ��ʱ��������   */

#define TURN_NUM_ADDR						(ALARM_RE_minu_ADDR 	+1)		/* 8B */   			/*  ��ѹ������ LXB�ѽ�֮��Ϊ��׼��ѹ220v  */
#define REAL_TURN_NUM_ADDR			(TURN_NUM_ADDR 				+8)		/* 8B */   			/*  ��ѹ������  REAL */
#define BAT_MIN_ADDR						(REAL_TURN_NUM_ADDR				+8)		/* 2B */   			/*  �󱸵�ر�����ֵ(����)   */
#define CUR_MIN_ADDR						(BAT_MIN_ADDR					+2)		/* 2B */   			/*  �źŵƵ���������ֵ(mA)   */
#define SEND_TIME_ADDR					(CUR_MIN_ADDR					+2)		/* 2B */   			/*  �źŵƵ����ϱ�ʱ��(����)   */

#define MACHINE_ADDR_ADDR				(SEND_TIME_ADDR				+2)		/* 31B  */   		/*  �źŻ���ַ���ȼ�����   */
#define MACHINE_TYPE_ADDR				(MACHINE_ADDR_ADDR		+31)	/* 8B */   		  /*  �źŻ��ͺų��ȼ����� */



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

//================   ����״̬�����ı�ı�־    start  ==============
#define local_ip_flag      			(bit_band_flag_1.bit0) 
#define local_mask_flag       	(bit_band_flag_1.bit1) 	
#define local_gateway_flag    	(bit_band_flag_1.bit2) 	
#define local_port_flag    			(bit_band_flag_1.bit3) 
#define local_mac_flag    			(bit_band_flag_1.bit4)  
#define local_dns_flag					(bit_band_flag_1.bit5) 	
#define server_ip_flag    			(bit_band_flag_1.bit6)  
#define server_port_flag    		(bit_band_flag_1.bit7)
#define server_www_flag    			(bit_band_flag_1.bit8)

#define userName_flag 					(bit_band_flag_1.bit9)   	// �л����Ƿ�ƥ��
#define password_flag 					(bit_band_flag_1.bit10)   // �����Ƿ���ȷ
#define ch_pw_flag 							(bit_band_flag_1.bit11)   // �Ƿ��޸�����

#define reset_all_flag 					(bit_band_flag_1.bit12)   //  �ָ��������ñ�־
#define restudy_flag 						(bit_band_flag_1.bit13)   //  ����ѧϰ��־
#define restart_flag 						(bit_band_flag_1.bit14)   //  �����豸��־

#define turn_num_flag		 	 			(bit_band_flag_1.bit17)		//  �������׼��ѹ
#define bat_min_flag			  		(bit_band_flag_1.bit18) 	//  �󱸵�ر�����ֵ(����)
#define cur_min_flag						(bit_band_flag_1.bit19)   //  �źŵƵ����쳣������ֵ(mA)
#define send_time_flag					(bit_band_flag_1.bit20)   //  �źŵƵ����ϱ�ʱ��(����)

#define change_alarm_restart_flag 		(bit_band_flag_1.bit15)   //  alarm_restart_flag��־�����ı�
#define change_DHCP_flag   						(bit_band_flag_1.bit16)  	//  DHCP_flag��־�����ı�
//================   ����״̬�����ı�ı�־    end  ==============


#define turn_num_lxb_flag		 	 			(bit_band_flag_1.bit23)		//  ��ѹ��������
#define net_state_flag		 	 			(bit_band_flag_1.bit24)		//  NET��״̬bit0

//================   Ѳ�������в���(����EEPROM) �����ı�ı�־    start  ==============  
#define alarm_restart_flag 			(bit_band_flag_1.bit21)   //  ��ʱ�����豸��־
#define DHCP_flag   						(bit_band_flag_1.bit22)  	//	DHCP �Ƿ����� ״̬
//================   Ѳ�������в���(����EEPROM) �����ı�ı�־    end  ==============  

//================   �������������н�����־��־   start  ==============
#define tsk_tcp_run      			(bit_band_flag_tsk.bit0)   	// tcp_tsk����
#define tsk_log_run       		(bit_band_flag_tsk.bit1) 	  // log_write_task����
#define tsk_eth_run    				(bit_band_flag_tsk.bit2)   	// eth_usart_communication_task���� 	
#define tsk_ADC_run    				(bit_band_flag_tsk.bit3)   	// ADC_task���� 
#define tsk_learn_run    			(bit_band_flag_tsk.bit4)   	// learn_detect_task����  
#define tsk_power_run					(bit_band_flag_tsk.bit5)   	// power_detect_task���� 

#define tsk_tcp_over    			(bit_band_flag_tsk.bit6)   	// tcp_tsk���� ��� 
#define tsk_log_over    			(bit_band_flag_tsk.bit7)   	// log_write_task����	���
#define tsk_eth_over    			(bit_band_flag_tsk.bit8)   	// eth_usart_communication_task����	���
//================   �������������н�����־��־    end  ==============

#define start_learning_flag		(bit_band_flag_tsk.bit9)   	//ѧϰ��־
#define first_start_learning	(bit_band_flag_tsk.bit13)   	//��һ��ѧϰ��־
#define learning_success_flag	(bit_band_flag_tsk.bit14)   	//ѧϰ�ɹ���־
#define learning_fail_flag	(bit_band_flag_tsk.bit16)   	//ѧϰʧ�ܱ�־
#define bSet_Valid_Passage	(bit_band_flag_tsk.bit15)   	//web���ͨ���趨��־

#define report_lampcurrent_flag		(bit_band_flag_tsk.bit10)   	//�ϱ��Ƶ�����־
#define Flag_ErrorReportHalt		(bit_band_flag_tsk.bit11) //����������ͣ�ϱ�

#define bfeed_hard_dog		(bit_band_flag_tsk.bit12) //ι��Ӳ�����Ź���־
//=========================================

//=========================================
//=========================================



#endif


