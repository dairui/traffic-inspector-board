#ifndef __FLASH_H
#define __FLASH_H

#define _TRUE 		1  
#define _FALSE 	0 

#define _FAULT		1
#define _FINE			0

/*���������ж�ʱ�䣬������������̫��*/
#define NET_STOP_TIME 120*30
/* ADC��ֵ��Ȩֵ */
#define  BINARYZATION_WEIGHT	0.6
/*	ͨ������	*/
#define LAMP_NUMBER 				32
#define	PHASE_CLASH_STATE_NUMBER	16
#define	PASSAGE_NUMBER		12				//�������ɵ�ͨ����

/*	ad�ɼ����˴���0��С�ڸ�ֵ����0*/
#define LAMP_CURRENT_MIN				50

/*	���ι��˴���1�������˻Ƶ�*/
#define WINDOW_RYG							(720/ADC_CYCLE_TIME)
/*	���ι��˴���2�����˵��Ƶ�7.2s*/
#define WINDOW_RG								(4800/ADC_CYCLE_TIME)

/*  �������ڶ����˲�FIFO: ���ι��˴����ܳ��� */
#define PASSAGE_FILTER_SIZE     (WINDOW_RG+10)   

/*  �ɼ�ͨ���Ƶ����Ĺ��˴��� ��С����λms*/
#define PASSAGE_CURRENT_WINDOWS     2400   

#define PASSAGE_ParaOutputList_MaxLine	2		//ͨ���������������

#define LEDLAMP_ERR_FLICKER_TIME	360		//360*0.5s=3min��com�ƹ�����˸ʱ��

extern OS_MUT I2c_Mut_lm;                     /* ��ȡʱ�ӻ����� ���۱�*/
extern OS_MUT communication_rx_mut;						/* ͨѶ���տ黥���� ���۱�*/
extern OS_MUT table_global_mut;         /* B0/B1/97/����ֵ��/�ȵĻ����� ���۱�*/
extern OS_MUT data_to_comany_mut;         /*����˾���͵����ݻ����� ���۱�*/

// /*����д��־����(���࣬��ʾ�����ݿ��е��к�)*/
// #define ALARM_START_MACHINE       		(1)                //����0������
// #define ALARM_RELEASE_POWER       		(2)                //����1���ŵ�ʱ���쳣
// #define ALARM_AC_ABNOMAL          		(3)                //����0���������Դ����
// #define ALARM_PASSAGE_ABNOMAL     		(4)                //����0��ͨ���쳣
// #define ALARM_SET_DATA            		(5)                //����0������
// #define ALARM_3G_ABNOMAL          		(6)                //����0��3G��Դ������־
// #define ALARM_YELLOW_FLICKER      		(7)                //����0������
// #define ALARM_RED_ALL             		(8)                //����0��ȫ��

/*����д��־����*/
#define ALARM_LOG_TYPE_1              (0)                //����0������
#define ALARM_LOG_TYPE_2              (1)                //����1���ŵ�ʱ���쳣
#define ALARM_LOG_TYPE_3              (2)                //����0���������Դ����
#define ALARM_LOG_TYPE_4              (3)                //����0��ͨ���쳣֮�̳�
#define ALARM_LOG_TYPE_5              (4)                //����0��ͨ���쳣֮ͨ����ͻ
#define ALARM_LOG_TYPE_6              (5)                //����0��ͨ���쳣֮�ƹ���
#define ALARM_LOG_TYPE_7              (6)                //����0������֮ѧϰ
#define ALARM_LOG_TYPE_8              (7)                //����0������֮ʱ������
#define ALARM_LOG_TYPE_9              (8)                //����0������֮ͨ��web�޸Ĳ���
#define ALARM_LOG_TYPE_10             (9)                //����0������֮ͨ����̫���޸Ĳ���
#define ALARM_LOG_TYPE_11             (10)               //����0��3G��Դ������־
#define ALARM_LOG_TYPE_12             (11)               //����0������
#define ALARM_LOG_TYPE_13             (12)               //����0��ȫ��
#define ALARM_LOG_TYPE_14             (13)               //����0�����������
#define ALARM_LOG_TYPE_15             (14)               //����0�����ѧϰ���ɹ�

/*����д��־����*/
#define LOG_START_MACHINE              (0)                //����0������,����,ȫ��,����·���źŻ�
#define LOG_RELEASE_POWER              (1)                //����1���ŵ�ʱ���쳣���ŵ���ϣ�,�������Դ����,3G��Դ������־
#define LOG_PASSAGE_ABNOMAL              (2)                //����0��ͨ���쳣֮�̳�ͨ����ͻ�ƹ���
#define LOG_SET_TIME             (3)                //����0������֮ѧϰ,����֮ʱ������;
#define LOG_SET_WEB              (4)                //����0������֮ͨ��web�޸Ĳ���
#define LOG_SET_ETH              (5)                //����0������֮ͨ����̫���޸Ĳ���

// /*����д��־����*/
// #define ALARM_LOG_TYPE_1              (0)                //����0������,����,ȫ��,����·���źŻ�
// #define ALARM_LOG_TYPE_2              (1)                //����1���ŵ�ʱ���쳣,�������Դ����,3G��Դ������־
// // #define ALARM_LOG_TYPE_3              (2)                //����0���������Դ����
// #define ALARM_LOG_TYPE_3              (2)                //����0��ͨ���쳣֮�̳�ͨ����ͻ�ƹ���
// // #define ALARM_LOG_TYPE_5              (4)                //����0��ͨ���쳣֮ͨ����ͻ
// // #define ALARM_LOG_TYPE_6              (5)                //����0��ͨ���쳣֮�ƹ���
// #define ALARM_LOG_TYPE_4              (3)                //����0������֮ѧϰ
// #define ALARM_LOG_TYPE_5              (4)                //����0������֮ʱ������
// #define ALARM_LOG_TYPE_6              (5)                //����0������֮ͨ��web�޸Ĳ���
// #define ALARM_LOG_TYPE_7             (6)                //����0������֮ͨ����̫���޸Ĳ���
// // #define ALARM_LOG_TYPE_11             (10)               //����0��3G��Դ������־
// // #define ALARM_LOG_TYPE_12             (11)               //����0������,ȫ��
// // #define ALARM_LOG_TYPE_13             (12)               //����0��ȫ��

/*�����¼�д��־�ṹ��*/     
typedef struct _ALARM_EVENT_LOG
{
	/*��������
	Bit 7=1	ȫ��
	Bit 6=1	����
	Bit 5=1	�ⲿ3G��Դ���ϱ���
	Bit 4=1	����
	Bit 3=1	ͨ���쳣
	Bit 2=1	�������ѹ��Ƶ�ʱ仯�쳣  
	Bit 1=1	��Դ�ŵ�ʱ���쳣
	Bit 0=1	����
	*/
	u8	alarm_type;           	
	u8	alarm_ID;       					/*���������ʶ*/     
	u8	alarm_value[2];              /*��������Ķ���ֵ*/     
}ALARM_EVENT_LOG;

/*�����¼�д���ݿ�ṹ��*/   
typedef struct _LOG_TO_DB
{
	u8 	log_ID;     						//��־�����ʶ=0x92    
	u32 start_addr;    					//������eerom��ʼ��ַ
	u8	index1;  		//����1  �����ʶa7��ֵ
	u8	index2;   							//����2
	u32	common_time;   			//����ʱ��
	u8 	data_value[4];    			//������־���Ǳ�����ֵ
}LOG_TO_DB;

/*�����¼�д���ݿ�������*/   
typedef union _UNION_LOG_DB
{
	u8 						database[14];     																
	LOG_TO_DB		log_database_struct;    					
}UNION_LOG_DB;

/*��һ�εı�����־������*/
extern u8	last_alarm_log_type;

/*
�׶���ʱ˥����id=0xC1
16*16*8 = 2K
*/
typedef struct STAGE_TIMING_TABLE_S
{
	u8		index;					//	�׶���ʱ������
	u8		stageId;					//�׶κ�
	u16		relPhase;				//������λ2B
	u8		stGreenTime;		//�׶��̵�ʱͬ
	u8		stYellowTime;		//�׶λƵ�ʱ��
	u8		stRedTime;			//�׶κ��ʱ��
	u8		stAlterParam;		//�׶�ѡ�����
}STAGE_TIMING_TABLE_t;


/*��־��������*/
extern u32 Write_Log_Mbox[];

extern OS_SEM eth_send_data_semaphore;
extern OS_SEM eth_client_sche_semaphore;
// // // extern OS_SEM eth_MySelf_semaphore;

/*  Write Log FIFO size */
#define WRITE_LOG_SIZE        10  
/*  Write Log FIFO  */
extern	vu8 write_log_wr_fifo[WRITE_LOG_SIZE][21];

extern	vu32 write_log_wr_index,write_log_rd_index,write_log_counter;
/* This flag is set on Write Log  Fifo Overflow */
extern	vu8 write_log_buffer_overflow;

/////==================================lxb===

#define MAX_RECORD          ((u16)100)                          //�������״̬����+���ַ��+��źŻ�������
#define NOTE_MAX_RECORD     ((u16)38)                           //��־���洢�� 38*10+3 < ��̫���֡

//flashʹ�ñ�� 
#define FLASH_USED_FLAG        ((u16)0x0000)                    //����Ϊ0xBC??

//1 ��̫������
#define FLASH_ETHERNET_PARA    ((u16)0x0000)                    //��̫������
#define FLASH_USER_NAME        ((u16)FLASH_ETHERNET_PARA+4)     //�û��� 16Byte
#define FLASH_USER_PASSWORD    ((u16)FLASH_USER_NAME+4*16)      //����   16Byte

#define LOCAL_NET_INFO_ADDR     ((u16)FLASH_USER_PASSWORD+4*16) 	/* 0B */  

#define LOCAL_IP_ADDR      ((u16)FLASH_USER_PASSWORD+4*16)  //����IP
#define LOCAL_MASK_ADDR      ((u16)LOCAL_IP_ADDR+4)       //��������
#define LOCAL_GATEWAY_ADDR       ((u16)LOCAL_MASK_ADDR+4)       //����
#define SERVER_IP_ADDR     ((u16)LOCAL_GATEWAY_ADDR+4)        //Զ������IP 
#define LOCAL_PORT_ADDR          ((u16)SERVER_IP_ADDR+4)      //�˿ں�
#define LOCAL_DNS_ADDR           ((u16)LOCAL_PORT_ADDR+4)           //DNS 
#define LOCAL_MAC_ADDR           ((u16)LOCAL_DNS_ADDR+4)            //MAC  
#define ETHERNET_END           ((u16)LOCAL_MAC_ADDR+4*6)

// #define LOCAL_IP_ADDR      			(BAT_TIME_ADDR 				+4) 	/* 4B */  //  LOCAL_IP_ADDR=LOCAL_NET_INFO_ADDR
// #define LOCAL_GATEWAY_ADDR    	(LOCAL_IP_ADDR				+4)  	/* 4B */
// #define LOCAL_MASK_ADDR       	(LOCAL_GATEWAY_ADDR		+4) 	/* 4B */
// #define LOCAL_PRIDNS_ADDR    		(LOCAL_MASK_ADDR			+4) 	/* 4B */
// #define LOCAL_SECDNS_ADDR    		(LOCAL_PRIDNS_ADDR		+4) 	/* 4B */  
// #define LOCAL_PORT_ADDR    			(LOCAL_SECDNS_ADDR		+4)  	/* 2B */
// #define LOCAL_MAC_ADDR    			(LOCAL_PORT_ADDR			+2)  	/* 6B */
// #define LOCAL_DNS_ADDR					(LOCAL_MAC_ADDR 			+6)		/* 4B */ 		

// #define SERVER_IP_ADDR    			(LOCAL_DNS_ADDR				+4)  	/* 4B */
#define SERVER_PORT_ADDR    		((u16)ETHERNET_END				+4)  	/* 2B */
#define SERVER_IP2_ADDR    			((u16)SERVER_PORT_ADDR			+2)  	/* 40B ��˾IP*/
#define SERVER_PORT2_ADDR    		((u16)SERVER_IP2_ADDR			+4)  	/* 40B ��˾IP*/

//2 ��طŵ����ڼ�¼ ���ŵ�ʱ��
#define DATE_MONTH      			((u16)SERVER_PORT2_ADDR +2)
#define START_CHARGE_TIME     ((u16)DATE_MONTH +4)
#define CHARGE_TIME     			((u16)START_CHARGE_TIME +4)
// #define CHARGE_TIME     			((u16)CHARGE_TIME +4)
#define DATE_END        			((u16)CHARGE_TIME+4)

//3 �����״̬�洢������
#define StartAddr       			((u16)DATE_END)
#define LAMP_USE   						((u16)StartAddr)
#define STANDARD_VOL        	((u16)LAMP_USE + 4)/*�洢��ʹ��״̬��eeprom	*/	
#define PHASE_CONFLICT_LIST   ((u16)STANDARD_VOL + LAMP_NUMBER*2)          //32���Ƶ�ad����ֵ
#define PASSAGE_STATUS_LIST   ((u16)PHASE_CONFLICT_LIST + PHASE_CLASH_STATE_NUMBER*4)   //����PASSAGE_NUMBER��״̬
#define LEARNED_RED_STATUS  	((u16)PASSAGE_STATUS_LIST + PASSAGE_NUMBER*4) //ѧϰ�ĺ��״̬
#define LEARNED_YELLOW_STATUS ((u16)LEARNED_RED_STATUS + 4)         		//ѧϰ�ĻƵ�״̬
#define LEARNED_GREEN_STATUS  ((u16)LEARNED_YELLOW_STATUS + 4)         	//ѧϰ���̵�״̬
#define LAMP_END        			((u16)LEARNED_GREEN_STATUS + 4)         	//��ǽ���λ��

//6 �¼���־
#define NOTE_START             ((u16)LAMP_END)       
// #define NOTE_COUNT_FLASH       ((u16)NOTE_START)                                        //�����¼�����
// #define NOTE_INDEX_FLASH       ((u16)NOTE_COUNT_FLASH+(NOTE_COUNT)*4)                   //�����¼��洢����
// #define NOTE_CONTENT           ((u16)NOTE_INDEX_FLASH+(NOTE_COUNT)*4)                   //��־�洢��,2�ÿ��4�ֽ�,����130��
#define NOTE_END               ((u16)INDEX_92_ADDR_END) //((u16)NOTE_CONTENT+(NOTE_COUNT)*2*4*(NOTE_MAX_RECORD))   //��־�洢����

//7 ��Чͨ�����
#define PASSAGE_COMBINE_START     ((u16)NOTE_END)       
#define VALID_PASSAGE_COMBINE     ((u16)PASSAGE_COMBINE_START)                        //2*6
#define PASSAGE_COMBINE_END       ((u16)VALID_PASSAGE_COMBINE+2*COMBINE_VALID_PASSAGE_NUM) 

// //7 ϵͳ���в���
// #define PARA_RUN_START             ((u16)NOTE_END)      
// #define PARA_THRESH_POWER_A        ((u16)PARA_RUN_START+4)  
// #define PARA_THRESH_POWER_B        ((u16)PARA_THRESH_POWER_A+4)  
// #define PARA_THRESH_CHARGE         ((u16)PARA_THRESH_POWER_B+4)  
// #define PARA_THRESH_MA             ((u16)PARA_THRESH_CHARGE+4)  
// #define PARA_REPORT_TIME_MA        ((u16)PARA_THRESH_MA+4)  
// #define PARA_END                   ((u16)PARA_REPORT_TIME_MA+4)  

extern u32 Read_DS3231_Time(void);
extern uint16_t ADC_filter_one(uint16_t *pData,uint8_t len);
extern void second_Filtering(u32  first_all_passage,u32  *p_state);
extern u32 first_filtering_binaryzation(u16  filter_one_value_in[],u8 reset_checking);
extern u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage,u32 cur_passage_green,u8* period_completed_flag,u8 reset_checking);
extern u32 passage_clash_check(u32 cur_RYG_passage,u32 *passage_table);
extern u32 green_phase_clash_check(u32 cur_passage_green,u32 *phase_clash_table);
extern void passage_RYG_learn(u32 valid_RG_passage,u32 valid_RYG_passage,u32	*p_state);
extern void get_passage_table(u32 passage_red,u32 passage_yellow,u32 passage_green,u32 passage_table[]);
extern u8 get_phase_clash_table(u32 cur_passage_green,u32 *phase_clash_table,u8 reset_learning);
extern void lxbprint_status(u32 this_status);
extern u8 is_period_end_ryg(u32 cur_passage_green);
extern void alarmlog_to_db(ALARM_EVENT_LOG *alarm_data_log,LOG_TO_DB *log_db_data);
extern void save_learned_data(u32 passage_table[],u32 phase_clash_table[],u32 p_state_task[]);
extern void read_learned_data(u32 passage_table[],u32 phase_clash_table[],u32 p_state_task[]);
extern void channel_output_table(vu8 Passage_ParaOutputList_temp[][4],u32 realtime_table);
extern u32 lamp_phyLevel_detect(u8 *phyerr_level,u32 cur_lamp_status,u16 standard_current[],u16 now_current[],u8 reset_checking);
extern uint16_t ADC_lamp_current(uint16_t *pData,uint8_t len);
extern void tsk_watchdog_reload(void);
extern void Report_Lamp_PhyLevel(void);
extern void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void get_stage_table(u8* period_completed_flag, u32 current_lamp_state,u32 learned_passage_green,u32 learned_passage_red,u8 reset_flag);
extern u8 ADC_PwrVolProc(u8* learning_transformer_flag,float web_change_vol);
extern u8  Power3G_Detect(void);
extern void Machine_Set_Detect(void);

#endif
