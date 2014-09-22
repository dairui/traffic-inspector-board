#ifndef __FLASH_H
#define __FLASH_H

#define _TRUE 		1  
#define _FALSE 	0 

#define _FAULT		1
#define _FINE			0

/*允许网络中断时间，超过则重启以太网*/
#define NET_STOP_TIME 120*30
/* ADC二值化权值 */
#define  BINARYZATION_WEIGHT	0.6
/*	通道数量	*/
#define LAMP_NUMBER 				32
#define	PHASE_CLASH_STATE_NUMBER	16
#define	PASSAGE_NUMBER		12				//红黄绿组成的通道数

/*	ad采集过滤窗口0，小于该值则清0*/
#define LAMP_CURRENT_MIN				50

/*	二次过滤窗口1，不过滤黄灯*/
#define WINDOW_RYG							(720/ADC_CYCLE_TIME)
/*	二次过滤窗口2，过滤掉黄灯7.2s*/
#define WINDOW_RG								(4800/ADC_CYCLE_TIME)

/*  以下用于二次滤波FIFO: 二次过滤窗口总长度 */
#define PASSAGE_FILTER_SIZE     (WINDOW_RG+10)   

/*  采集通道灯电流的过滤窗口 大小，单位ms*/
#define PASSAGE_CURRENT_WINDOWS     2400   

#define PASSAGE_ParaOutputList_MaxLine	2		//通道参数输出表行数

#define LEDLAMP_ERR_FLICKER_TIME	360		//360*0.5s=3min。com灯故障闪烁时间

extern OS_MUT I2c_Mut_lm;                     /* 读取时钟互斥量 李雄斌*/
extern OS_MUT communication_rx_mut;						/* 通讯接收块互斥量 李雄斌*/
extern OS_MUT table_global_mut;         /* B0/B1/97/电流值表/等的互斥量 李雄斌*/
extern OS_MUT data_to_comany_mut;         /*给公司发送的数据互斥量 李雄斌*/

// /*报警写日志类型(大类，表示在数据库中的行号)*/
// #define ALARM_START_MACHINE       		(1)                //类型0，开机
// #define ALARM_RELEASE_POWER       		(2)                //类型1，放电时间异常
// #define ALARM_AC_ABNOMAL          		(3)                //类型0，交流电电源报警
// #define ALARM_PASSAGE_ABNOMAL     		(4)                //类型0，通道异常
// #define ALARM_SET_DATA            		(5)                //类型0，设置
// #define ALARM_3G_ABNOMAL          		(6)                //类型0，3G电源掉电日志
// #define ALARM_YELLOW_FLICKER      		(7)                //类型0，黄闪
// #define ALARM_RED_ALL             		(8)                //类型0，全红

/*报警写日志类型*/
#define ALARM_LOG_TYPE_1              (0)                //类型0，开机
#define ALARM_LOG_TYPE_2              (1)                //类型1，放电时间异常
#define ALARM_LOG_TYPE_3              (2)                //类型0，交流电电源报警
#define ALARM_LOG_TYPE_4              (3)                //类型0，通道异常之绿冲
#define ALARM_LOG_TYPE_5              (4)                //类型0，通道异常之通道冲突
#define ALARM_LOG_TYPE_6              (5)                //类型0，通道异常之灯故障
#define ALARM_LOG_TYPE_7              (6)                //类型0，设置之学习
#define ALARM_LOG_TYPE_8              (7)                //类型0，设置之时间设置
#define ALARM_LOG_TYPE_9              (8)                //类型0，设置之通过web修改参数
#define ALARM_LOG_TYPE_10             (9)                //类型0，设置之通过以太网修改参数
#define ALARM_LOG_TYPE_11             (10)               //类型0，3G电源掉电日志
#define ALARM_LOG_TYPE_12             (11)               //类型0，黄闪
#define ALARM_LOG_TYPE_13             (12)               //类型0，全红
#define ALARM_LOG_TYPE_14             (13)               //类型0，灯物理故障
#define ALARM_LOG_TYPE_15             (14)               //类型0，多次学习不成功

/*报警写日志类型*/
#define LOG_START_MACHINE              (0)                //类型0，开机,黄闪,全红,重启路口信号机
#define LOG_RELEASE_POWER              (1)                //类型1，放电时间异常（放电完毕）,交流电电源报警,3G电源掉电日志
#define LOG_PASSAGE_ABNOMAL              (2)                //类型0，通道异常之绿冲通道冲突灯故障
#define LOG_SET_TIME             (3)                //类型0，设置之学习,设置之时间设置;
#define LOG_SET_WEB              (4)                //类型0，设置之通过web修改参数
#define LOG_SET_ETH              (5)                //类型0，设置之通过以太网修改参数

// /*报警写日志类型*/
// #define ALARM_LOG_TYPE_1              (0)                //类型0，开机,黄闪,全红,重启路口信号机
// #define ALARM_LOG_TYPE_2              (1)                //类型1，放电时间异常,交流电电源报警,3G电源掉电日志
// // #define ALARM_LOG_TYPE_3              (2)                //类型0，交流电电源报警
// #define ALARM_LOG_TYPE_3              (2)                //类型0，通道异常之绿冲通道冲突灯故障
// // #define ALARM_LOG_TYPE_5              (4)                //类型0，通道异常之通道冲突
// // #define ALARM_LOG_TYPE_6              (5)                //类型0，通道异常之灯故障
// #define ALARM_LOG_TYPE_4              (3)                //类型0，设置之学习
// #define ALARM_LOG_TYPE_5              (4)                //类型0，设置之时间设置
// #define ALARM_LOG_TYPE_6              (5)                //类型0，设置之通过web修改参数
// #define ALARM_LOG_TYPE_7             (6)                //类型0，设置之通过以太网修改参数
// // #define ALARM_LOG_TYPE_11             (10)               //类型0，3G电源掉电日志
// // #define ALARM_LOG_TYPE_12             (11)               //类型0，黄闪,全红
// // #define ALARM_LOG_TYPE_13             (12)               //类型0，全红

/*报警事件写日志结构体*/     
typedef struct _ALARM_EVENT_LOG
{
	/*报警类型
	Bit 7=1	全红
	Bit 6=1	黄闪
	Bit 5=1	外部3G电源故障报警
	Bit 4=1	设置
	Bit 3=1	通道异常
	Bit 2=1	交流电电压、频率变化异常  
	Bit 1=1	电源放电时间异常
	Bit 0=1	开机
	*/
	u8	alarm_type;           	
	u8	alarm_ID;       					/*报警对象标识*/     
	u8	alarm_value[2];              /*报警对象的对象值*/     
}ALARM_EVENT_LOG;

/*报警事件写数据库结构体*/   
typedef struct _LOG_TO_DB
{
	u8 	log_ID;     						//日志对象标识=0x92    
	u32 start_addr;    					//数据在eerom起始地址
	u8	index1;  		//索引1  对象标识a7的值
	u8	index2;   							//索引2
	u32	common_time;   			//公共时间
	u8 	data_value[4];    			//产生日志或是报警的值
}LOG_TO_DB;

/*报警事件写数据库联合体*/   
typedef union _UNION_LOG_DB
{
	u8 						database[14];     																
	LOG_TO_DB		log_database_struct;    					
}UNION_LOG_DB;

/*上一次的报警日志的类型*/
extern u8	last_alarm_log_type;

/*
阶段配时衰行敛id=0xC1
16*16*8 = 2K
*/
typedef struct STAGE_TIMING_TABLE_S
{
	u8		index;					//	阶段配时方案号
	u8		stageId;					//阶段号
	u16		relPhase;				//放行相位2B
	u8		stGreenTime;		//阶段绿灯时同
	u8		stYellowTime;		//阶段黄灯时间
	u8		stRedTime;			//阶段红灯时间
	u8		stAlterParam;		//阶段选项参数
}STAGE_TIMING_TABLE_t;


/*日志任务邮箱*/
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

#define MAX_RECORD          ((u16)100)                          //定义最大状态表数+最长地址数+最长信号机方案数
#define NOTE_MAX_RECORD     ((u16)38)                           //日志最大存储量 38*10+3 < 以太网最长帧

//flash使用标记 
#define FLASH_USED_FLAG        ((u16)0x0000)                    //必须为0xBC??

//1 以太网参数
#define FLASH_ETHERNET_PARA    ((u16)0x0000)                    //以太网参数
#define FLASH_USER_NAME        ((u16)FLASH_ETHERNET_PARA+4)     //用户名 16Byte
#define FLASH_USER_PASSWORD    ((u16)FLASH_USER_NAME+4*16)      //密码   16Byte

#define LOCAL_NET_INFO_ADDR     ((u16)FLASH_USER_PASSWORD+4*16) 	/* 0B */  

#define LOCAL_IP_ADDR      ((u16)FLASH_USER_PASSWORD+4*16)  //本地IP
#define LOCAL_MASK_ADDR      ((u16)LOCAL_IP_ADDR+4)       //子网掩码
#define LOCAL_GATEWAY_ADDR       ((u16)LOCAL_MASK_ADDR+4)       //网关
#define SERVER_IP_ADDR     ((u16)LOCAL_GATEWAY_ADDR+4)        //远程主机IP 
#define LOCAL_PORT_ADDR          ((u16)SERVER_IP_ADDR+4)      //端口号
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
#define SERVER_IP2_ADDR    			((u16)SERVER_PORT_ADDR			+2)  	/* 40B 公司IP*/
#define SERVER_PORT2_ADDR    		((u16)SERVER_IP2_ADDR			+4)  	/* 40B 公司IP*/

//2 电池放电日期记录 及放电时间
#define DATE_MONTH      			((u16)SERVER_PORT2_ADDR +2)
#define START_CHARGE_TIME     ((u16)DATE_MONTH +4)
#define CHARGE_TIME     			((u16)START_CHARGE_TIME +4)
// #define CHARGE_TIME     			((u16)CHARGE_TIME +4)
#define DATE_END        			((u16)CHARGE_TIME+4)

//3 检测器状态存储区定义
#define StartAddr       			((u16)DATE_END)
#define LAMP_USE   						((u16)StartAddr)
#define STANDARD_VOL        	((u16)LAMP_USE + 4)/*存储灯使用状态到eeprom	*/	
#define PHASE_CONFLICT_LIST   ((u16)STANDARD_VOL + LAMP_NUMBER*2)          //32个灯的ad电流值
#define PASSAGE_STATUS_LIST   ((u16)PHASE_CONFLICT_LIST + PHASE_CLASH_STATE_NUMBER*4)   //最多放PASSAGE_NUMBER个状态
#define LEARNED_RED_STATUS  	((u16)PASSAGE_STATUS_LIST + PASSAGE_NUMBER*4) //学习的红灯状态
#define LEARNED_YELLOW_STATUS ((u16)LEARNED_RED_STATUS + 4)         		//学习的黄灯状态
#define LEARNED_GREEN_STATUS  ((u16)LEARNED_YELLOW_STATUS + 4)         	//学习的绿灯状态
#define LAMP_END        			((u16)LEARNED_GREEN_STATUS + 4)         	//标记结束位置

//6 事件日志
#define NOTE_START             ((u16)LAMP_END)       
// #define NOTE_COUNT_FLASH       ((u16)NOTE_START)                                        //类型事件个数
// #define NOTE_INDEX_FLASH       ((u16)NOTE_COUNT_FLASH+(NOTE_COUNT)*4)                   //类型事件存储索引
// #define NOTE_CONTENT           ((u16)NOTE_INDEX_FLASH+(NOTE_COUNT)*4)                   //日志存储区,2项，每项4字节,容量130个
#define NOTE_END               ((u16)INDEX_92_ADDR_END) //((u16)NOTE_CONTENT+(NOTE_COUNT)*2*4*(NOTE_MAX_RECORD))   //日志存储结束

//7 有效通道组合
#define PASSAGE_COMBINE_START     ((u16)NOTE_END)       
#define VALID_PASSAGE_COMBINE     ((u16)PASSAGE_COMBINE_START)                        //2*6
#define PASSAGE_COMBINE_END       ((u16)VALID_PASSAGE_COMBINE+2*COMBINE_VALID_PASSAGE_NUM) 

// //7 系统运行参数
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
