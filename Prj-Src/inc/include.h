#ifndef __INCLUDE_H
#define __INCLUDE_H

#include "rtl.h"
#include "stm32f10x.h"
#include "ETH_STM32x.h"
#include "net_config.h"
#include "stdio.h"
#include "string.h"
#include "define_eeprom_lxb.h"
#include "USART.h"
#include "data_m.h"
#include "stm32_eth.h"
//////======================================

#include "I2C.h"


//检测器部分
//#include "device_init.h"
//#include "USART.h"

#include "key.h"
#include "define_lm.h"
#include "Data_Init.h"
#include "adc.h"

#include "i2c.h"
// // #include "flash.h"
// // #include "arithmetic.h"
// // #include "machine.h"
#include "spi.h"
// // #include "note.h"

//程序功能配置
//================================================================
#define DEBUG_MODE                               //串口调试模式
#define LAMP_DETECT                              //是否执行交通灯检测任务
#define LAMP_PHYLEVEL_REPORT                     //灯物理故障是否上报 
#define HEART_TIME_TO_HOST      (2*60*10)             //上位机心跳报文发送间隔: n*0.5秒 
#define HEART_TIME_TO_COMPANY   (360)             //公司监测电脑心跳报文发送间隔: n*0.5秒 
#define RE_CONNECTION_TIME      (1)              //网络掉线后重新建立网络连接时间设定 n*HEART_TIME
#define LOGIN_VALIDTIME         (120)            //网络登录界面有效时间: n*0.5秒 
//#define WATCH_DOG                              //是否使用看门狗   
#define ERROR_REPORT_TIME       (12)             //常规错误上报: n*0.5秒 

#define U32 u32
//================================================================
//检测部分运行参数设置
//================================================================
//信号波动程度判定范围
//#define AD_THRESHOLD     (0.15)

//状态间时间差距0.5x秒,故障界定
#define LAMP_TIME_SCALE  (2000)

//AD采样时间延迟(微妙)
#define AD_DELAY         (500)  

//AD20MS内采样次数
#define AD_COUNT    (10)//     ((20000)/(AD_DELAY))  //#define AD_DELAY         (500)  

//AD窗口采样时间(秒)
//#define AD_TIME          (2)

//互感器电流比值
#define VALUE_MA         (2000)

//最少状态数
#define MIN_STATUS       (1)

//交通灯"亮"状态判定阈值
//#define AD_PORPROTION    (0.3)

//交通灯电流阈值计算判定范围
//#define THRESH_PORPROTION    (0.8)

//交通灯故障阈值：单位mA
#define Broken_ALARM_THRESH     (10)  

//电池放电报警阈值：单位minute
#define Charge_ALARM_THRESH     (10) 

//电流、电压报警阈值：百分比 
#define POWER_ALARM_THRESH              (0.2)  

//网络参数初始化按钮生效时间 n*0.5
#define NET_RECOVER_TIME                (20)  
//================================================================

//学习功能状态灯
#define LED_LEARN_ON()  (GPIO_ResetBits(GPIOD, GPIO_Pin_3)) 
#define LED_LEARN_OFF() (GPIO_SetBits(GPIOD, GPIO_Pin_3))
#define LED_NET_ON()   (GPIO_ResetBits(GPIOE, GPIO_Pin_11)) 
#define LED_NET_OFF()  (GPIO_SetBits(GPIOE, GPIO_Pin_11))
#define LED_COM_ON()   (GPIO_ResetBits(GPIOA, GPIO_Pin_15)) 
#define LED_COM_OFF()  (GPIO_SetBits(GPIOA, GPIO_Pin_15))

#define Sampling_Resistance  (3)
#define Channel_Max (4)
#define Phase_Record_Max (40) 
#define Lamp_Max (3*Channel_Max)
#define AD_Vol_Sum_Num  (20)


#define SetBit(x , y)  x |= (1<<(y)) //将X的第Y位置1

#define ClrBit(x , y)  x &= !(1<<(y)) //将X的第Y位清0


//检测器部分

#define ID_MAX_LENGTH (40)
//检测器固件参数
typedef struct _MACHINE_PARA
{
    volatile u8  Node_ID_Length;                    //结点长度
    volatile u8  Node_ID[ID_MAX_LENGTH];
    volatile u8  Manufacture_Length;                //制造商长度
    volatile u8  Manufacture_ID[ID_MAX_LENGTH];
    volatile u8  Type_Length;                       //模块型号长度
    volatile u8  Type[ID_MAX_LENGTH]; 
    volatile u8  Version_Length;                    //版本号长度
    volatile u8  Version[ID_MAX_LENGTH];               
}MACHINE_PARA;

// // //信号机地址
// // typedef struct _MACHINE_ADDR
// // {
// //     volatile u8   Node_Addr_Length;                  //结点地址长度
// //     volatile u32  Node_Addr[ID_MAX_LENGTH];       
// //     volatile u8   Machine_Type_Length;               //信号机长度
// //     volatile u32  Machine_Type[ID_MAX_LENGTH];    
// // }MACHINE_ADDR;

//以太网参数
typedef struct _ETHERNET_PARAMETER
{
     u32   Local_ip;                  //结点地址长度
     u32   Net_mask;
     u32   Gate_way;
     u32   Remote_ip;
     u32   port;
     u32   MAC[6];
     u32   DNS;
}ETHERNET_PARAMETER;

//电池放电参数
typedef struct _CHARGE_PARA
{
     u32   Start_Time;                 
     u32   End_Time;
}CHARGE_PARA;

extern __IO uint16_t ADC_buffer[];                  //DMA缓冲
// // // extern volatile u32 Sys_Time;                        //系统时间计数值
// // // extern          u32 Lamp_Status[30];                 //交通灯状态表
// // // extern volatile u32  Lamp_Time[30];                  //交通灯状态间隔时间表
// // // extern volatile u32 Count_Status;                    //交通灯状态表状态个数
// // // extern          u8  Index_Status_Cur;                //交通灯当前状态索引 
// // // extern          u32  Lamp_Mask_Bit;                  //交通灯状态屏蔽位
// // // extern          u32  Hardware_Mask_Bit;              //硬件屏蔽位
// // // extern          u32  Lamp_Error_Recovery[32];        //交通灯错误状态位
// // // extern          u32  Lamp_PhyLevelr_Detect[32];      //交通灯物理故障错误计数，用于物理故障报警
extern volatile u16  Lamp_Broken_Level[32];          //交通灯各等物理故障等级
// // // extern          u32  Lamp_Yellow;                    //交通灯类型
// // // extern          u32  Lamp_Red;                       //交通灯类型
// // // extern          u32  Lamp_Green;                     //交通灯类型
// // // extern volatile u32  Lamp_PhyLevel_ErrorHistory;     //交通灯物理故障历史记录 
// // // extern volatile u16  Flag_Pwr_Freq;                  //交通灯电压频率采集标志
extern volatile u16  Pwr_Freq_Index;                 //交通灯电压频率索引
extern volatile u16  Pwr_Freq_Record[10];            //交通灯电压频率缓冲
extern volatile u16  Pwr_Vol_Record[10];             //交通灯电压电压缓冲  
extern volatile u16  Pwr_Freq;                       //交通灯电压频率
extern volatile u16  Pwr_Vol;                        //交通灯电压电压
extern volatile u32  Counter;                         //检测5秒钟状态检测
// // // extern volatile u8   Flag_Report;                    //例常检测输出标志
// // // extern volatile u8   Flag_Learn;                     //状态学习标志 
// // // extern volatile u8   Flag_Init;			             //上电状态自检
// // // extern volatile u8   Flag_DetectError;               //状态表故障标志
// // // extern volatile u8   Flag_NetTest;                   //例常网络测试
// // // extern volatile u8   Flag_NetRecover;                //网络参数初始化
// // // extern volatile u32  SysTime_NetRecover;             //网络参数初始化计时
// // // extern volatile u8   Flag_ErrorReport;               //例常错误上报
// extern volatile u8   Flag_ErrorReportHalt;           //例常错误暂停上报



extern volatile u32  Flag_LampUse;                   //信号灯使用记录  

// // // extern volatile u32  Standard_Vol[32];               //信号灯初始标准工作电压
// // // extern volatile u32 Standard_Vol_High[32];           //信号灯初始标准工作电压最大值
// // // extern volatile u32 Standard_Vol_Low[32];            //信号灯初始标准工作电压最小值

// // // extern volatile u8  Flag_RestartBoard;               //重启设备 

extern volatile u8 Dtae_Month;                      //电池放电月份记录 
extern          CHARGE_PARA Charge_Para;             //电池放电参数
extern volatile u32 Flag_Charge;                     //电池放电标志 

// extern volatile u32 Transformer_A;                //变压器A
// // extern volatile u32 Transformer_B;                //变压器B 
// extern volatile u32 Alarm_Charge;                 //电池报警时间
// extern volatile u32 Alarm_LampmA;                 //信号灯报警阈值 

// extern volatile u32 LampmA_ReportTime;            //信号灯电流上报时间 

extern volatile u32 LampBroken_Type;              //信号灯故障类型:冲突、损坏




// // // //USART通信
extern volatile    u8             Flag_UsartProtocol;          //USART通信结束标志
extern volatile    u8             Flag_Communication;          //USART通信中标识

extern volatile    USART_DATA     USART_ControlBlk;            //USART通信控制块 
// // // extern volatile    Target_data    Data_ControlBlk;             //对象数据控制块 
extern volatile    Protocol_Data  Protocol_ControlBlk;         //通信协议控制块 
// // // //extern volatile    MachineSetMethod_Data MachineSet_ControlBlk;       //信号机设置控制块
// // // //extern volatile    MachineSet_Data       MachineSet_Record;           //当前反馈的信号机状态
// // // //extern volatile    u8                    MachineSet_Buffer[100];      //当前反馈的信号机状态数据缓冲区


extern volatile    u32            GlobalTime;                  //公共时间           4字节
extern volatile    u32            LocalTime;                   //本地时间                4字节
// // // //extern volatile    u8             Phase_OutputList_MaxLine;    //相位输出状态表最大行数  1字节
// // // //extern volatile    u8             Phase_OutputList[30][4];     //相位输出状态表     1字节
// // // extern volatile    u8             Phase_CollisionList_MaxLine; //相位冲突表最大行数      1字节
// // // extern volatile    u8             Phase_CollisionList[30*6];   //相位冲突表         1字节
// // // //extern volatile    u8             Phase_Para_MaxLine;          //通道参数表最大行数      1字节
// // // //extern volatile    u8             Phase_Para[30][4];           //通道参数表              1字节
// // // extern volatile    u8             Phase_ParaOutputList_MaxLine;  //通道输出表最大行数      1字节
extern volatile    u8    Passage_ParaOutputList[PASSAGE_ParaOutputList_MaxLine][4];   //通道输出表              1字节


// // // extern volatile    u8             Flag_Download;               //上位机下载参数的开始结束标志 1字节

// // // //extern volatile    u8             FollowPhase_MaxLine;           //跟随相位表最大行数         1字节
// // // //extern volatile    u8             FollowPhase_StatusMaxLine;     //跟随相位状态表最大行数     1字节
// // // //extern volatile    u8             FollowPhase_StatusList[30][4]; //跟随相位状态表             1字节

// // // extern volatile    u8             Lamp_PhyLevel;               //灯故障状态                 1字节
// // // extern volatile    u16            Volatage_Power;              //交流电源电压               2字节
// // // extern volatile    u8             Frequence_Power;             //交流电源频率               1字节
extern volatile    u32            Para_Charge;                 //后备电池参数               4字节

// // // //extern volatile    u8 			Phase_StageAssignTimeList[30][7];
// // // //extern volatile    u8           Phase_StageAssignTimeList_MaxLine; 

// // // extern volatile    MACHINE_PARA   Machine_Parameter;           //检测器固件参数信息
// // // extern volatile    MACHINE_ADDR   Machine_Address;             //检测器地址信息

// // // extern volatile u32 Flag_Note;                                //有事件日志需要记录
// // // extern volatile   u32 Note_Count[NOTE_COUNT];                 //事件日志表内某一类型日志个数
// // // extern volatile   u32 Note_Index[NOTE_COUNT];                 //事件日志表内某一类型当前索引
// // // extern volatile   NOTE_RECORD Note_Data;                        //事件信息结构体



// // // //以太网
// // // extern volatile    u32 LocalTime; 
// // // extern             u8  connect_timeout;
// // // extern             u8  connect_flag;
// // // extern volatile    u32  User_Name[16];
// // // extern volatile    u32  User_Passord[16];
// // // extern             ETHERNET_PARAMETER   Ethernet_Parameter;
// // // //extern volatile    u32                  Flag_Login;     
// // // //extern volatile    u32                  Count_Login;   
// // // //extern volatile    u32                  Flag_RirstDisConnect;  
// // // extern volatile    u32                  Flag_ReadNote;                 //读取日志标志
// // // extern volatile    u32                  Index_CurrentNote;             //当前日志索引
// // // //extern volatile    u32   				Flag_Connect;                  //以太网连接标志


// // // ////BATTON////
extern u8 Batton_Break_flag;
extern u8 test_batton_flag;//测试电池好坏
// // // ////BATTON////

// // // /////COMMUNICATION////
// // // //extern u8 Communication_Flag;
// // // /////COMMUNICATION////

//////POWER-3G///////
extern u8 POWER_ERROR_Recorded;
//////POWER-3G///////

// // // extern u8 AD_Vol_Count[3*Channel_Max] ;//滑动滤波计数

// // // extern u32 AD_Vol_Value[3*Channel_Max][AD_Vol_Sum_Num] ;//滑动滤波数组


// // // extern u32  Status_Last_print;
// // // //for fliter
// // // extern u32 Fliter_Count ;//过滤算法中计算时间的变量，每0.5S加一
// // // extern u32 Channel_Count[Channel_Max] ;//每个通道的计数器
// // // extern u32 Last_State ;//上一时刻的样本
// // // #define Threshold_Time (5)//时间阀值
// // // extern u32 Now_State;
// // // extern u8 First_Status_Flag;//第一次的状态标识，标志位0时，Last_State没有被赋值，为1时，被赋值

// // // #define	ETH_RestartBoard_Max_Count    (1800*2)  //1800秒，30分钟。无连接重启阀值

// // // /***for Ethernet***/
// // // extern u16 Test_Data ;
// // // extern u8 Flag_ReInit_Eth;
// // // //extern void eth_re_init(void);

// // // /***for 2400***/
extern u8 Data_Port[8] ;//新端口发送数据

// // // /***for Lamp Detect***/
// // // //extern u32 Lamp_Error_Light ;//标记灯常亮故障位
// // // //extern u32 Lamp_Error_NotLight ;//标记灯灭故障位
// // // //extern u32 Lamp_Error_Time[32] ;//记录故障发生时间
// // // //extern u32 Circle_Time ;//状态表周期时间
// // // extern u32 Status_Error;
// // // extern u32 Status_Error_time;


// // // extern u8 SelfDetect_Complete_Flag;	//自检状态标识
// // // extern u8 Waite_StatusChange_flag_1;//等候状态变化标识,自检定位使用
// // // extern u8 Waite_StatusChange_flag_2;
// // // extern u8 Index_Status_next;

// // // extern u8 Alarm_Data;//报警标志，与A7协议的规定相符合
// // // extern u8 Alarm_Data_Old;//记录上次报警标示

// // // extern U8 tcp_soc_server;
// // // extern u8 tcp_soc_client;
// // // extern u8 tcp_soc_company_cli;
// // // extern u8 status_collect_complete;
// // // extern U8 rem_ip[4];
// // // extern u16 port;
// // // extern u8 company_ip[4];
// // // extern u16 company_port;
// // // extern u8 collect_count;
// // // extern u32 collect_status[5];
// // // extern u32 collect_value[3][3*Channel_Max];
// // // extern u32 Now_AD_Value[3*Channel_Max];
// // // extern u8 Work_State;
extern u8 Host_Communication_flag;





// // // extern OS_TID t_tick_timer; 
// // // extern OS_TID t_tcp_task; 
// // // extern OS_TID t_LampStates_Collect_task;
// // // extern OS_TID t_Calculate_Task;
// // // extern OS_TID t_Learn_Task;

extern u8 Company_Data_Flag;  
// // // extern u8 debug_type;//定位时，打印开机自检还是错误重定位



//  by_lm  start




// // /*  USART2 Receiver buffer size */
// // #define USART2_RX_SIZE        20   
// // /*  USART2 Receiver Fifo  */
// // extern  vu8 usart2_rx_fifo[USART2_RX_SIZE];
// // /*  USART2 Receiver Lenth  */
// // extern  vu8 usart2_rx_len;
// // extern  vu32 usart2_rx_wr_index,usart2_rx_rd_index,usart2_rx_counter;
// // /* This flag is set on USART2 Receiver Fifo Overflow */
// // extern  vu8 usart2_rx_buffer_overflow;


 
/*  USART2 Receiver buffer size */
#define USART2_RX_SIZE        20   
/*  USART2 Receiver buffer  */
extern  vu8 usart2_rx_buff[USART2_RX_SIZE];
/*  USART2 Receiver Lenth  */
extern  vu32 usart2_rx_counter;

/* at_System.c */
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]


extern void DMAReConfig(void);


/****ETH Par******/
extern __IO uint8_t 	server_ip[4]			;         //服务端IP
extern __IO uint8_t 	server_www[40]		;   			//服务端域名
extern __IO uint16_t server_port				;         //服务端PORT
extern __IO uint16_t local_port 				;         //本地服务端口号

extern __I  uint8_t 	r_server_ip[4]		;         //服务端IP
extern __I  uint8_t 	r_server_www[40]	;   			//服务端域名
extern __I  uint16_t r_server_port			;         //服务端PORT
extern __I  uint16_t r_local_port 			;         //本地服务端口号

extern __IO uint8_t 	r_myself_server_ip[4];          	//公司服务端IP
extern __IO uint16_t r_myself_server_port	 ;            //公司服务端PORT

extern __IO uint8_t 	myself_server_ip[4]	;          	//公司服务端IP
extern __IO uint16_t myself_server_port		;           //公司服务端PORT




extern u8 eth_send_data_flag ;      //以太网发送数据标志
extern u8 client_data_type;         //主动上报数据类型 1--心跳 2-其他数据    //no_liming
extern u8 cli_connect_ser_flag ;    //客户端连接服务器标志，用于主动上报数据
extern u8 cli_connect_ser_state;    //by_lm
extern u8 cli_connect_wait_ack; 

// // // extern U8 tcp_soc_server;
// // // extern u8 tcp_soc_client;

// // // // /* Ethernet Receiver Fifo size*/
// // // // #define RX_FIFO_SIZE        200
// // // // /*  Ethernet Receiver Data Fifo */
// // // // extern vu8 eth_rx_fifo[RX_FIFO_SIZE];
// // // // /*  Ethernet Receiver Lenth Fifo */
// // // // extern vu32 eth_rx_len ;

// // // // extern vu32 rx_wr_index,rx_rd_index,rx_counter;

// // // // /* This flag is set on Ethernet Receiver Fifo Overflow */
// // // // extern vu8 rx_buffer_overflow;

/* Ethernet Send Fifo size*/
#define TX_FIFO_SIZE        2000
/*  Ethernet Receiver Data Fifo */
extern vu8 eth_tx_fifo[TX_FIFO_SIZE];
/*  Ethernet Receiver Lenth Fifo */
#define TX_LEN_SIZE        100
extern vu32 eth_tx_len[TX_LEN_SIZE] ;

extern vu32 tx_wr_index,			tx_rd_index,			tx_counter;
extern vu32 tx_len_wr_index,	tx_len_rd_index,	tx_len_counter;

/* This flag is set on Ethernet Receiver Fifo Overflow */
extern vu8 tx_buffer_overflow;
/* This flag is set on Ethernet Lenth Fifo Overflow */
extern vu8 tx_len_overflow;

/*  Write Client FIFO size */
#define WRITE_Client_SIZE        100  
#define SEND_LARGE_CLIENT_SIZE				90 
#define SEND_SMALL_CLIENT_SIZE				12
/*  Write Client FIFO  */
extern vu8 write_Client_fifo[WRITE_Client_SIZE][SEND_LARGE_CLIENT_SIZE];

extern vu32 write_Client_wr_index,write_Client_rd_index,write_Client_counter;
/* This flag is set on Write Client  Fifo Overflow */
extern vu8 write_Client_buffer_overflow;
//--------------------------------
/*  Write MySelf FIFO size */
#define WRITE_MySelf_SIZE        10  
/*  Write MySelf FIFO  */
extern vu8 write_MySelf_fifo[WRITE_MySelf_SIZE][12];

extern vu32 write_MySelf_wr_index,write_MySelf_rd_index,write_MySelf_counter;
/* This flag is set on Write MySelf  Fifo Overflow */
extern vu8 write_MySelf_buffer_overflow;
//-----------------------------------







extern vu8 tcp_soc_server;              //服务端TCP
extern vu8 tcp_soc_client;              //客户端TCP
extern vu8 tcp_soc_myself;              //公司的电脑TCP链接

extern vu8 soc_state_lm;
extern vu8 wait_ack_lm;
extern vu8 server_wait_ack ; 

extern __IO uint8_t   mac_adr[6] ;
//extern __IO LOCALM    ip_config  ;

extern __I  uint8_t   r_mac_adr[6] ;
extern __I  LOCALM    r_ip_config  ;

extern U8 own_hw_adr[];

extern __IO int8_t userName[17];
extern __IO int8_t password[17];
extern __I  int8_t r_password[17];

extern vu8 re_ch_pw_count 	;           // 修改密码  两次输入判断

extern __IO uint8_t alarm_restart_hour			;
extern __IO uint8_t alarm_restart_minute		;
extern __I  uint8_t r_alarm_restart_hour			;
extern __I  uint8_t r_alarm_restart_minute		;

extern __IO  float		turn_num				;  	 	//  变压器匝数比
#define  Transformer_A turn_num 
extern __IO float 		turn_num_lxb		;  	 	//  变压器匝数比

extern __IO uint16_t 	bat_min					;    		//  后备电池报警阀值(分钟)
#define Alarm_Charge bat_min

extern __IO uint16_t 	cur_min					;    		//  信号灯电流异常报警阀值(mA)
#define Alarm_LampmA cur_min

extern __IO uint16_t 	send_time				;    		//  信号灯电流上报时间(分钟)
#define LampmA_ReportTime send_time

extern __I  float 		r_turn_num			;  	 	// 标准电压
extern __I  uint16_t 	r_bat_min				;    		//  后备电池报警阀值(分钟)
extern __I  uint16_t 	r_cur_min				;    		//  信号灯电流异常报警阀值(mA)
extern __I  uint16_t 	r_send_time			;    		//  信号灯电流上报时间(分钟)
extern __IO float 		r_turn_num_lxb		;  	//  变压器匝数比lxb


extern __IO uint8_t machine_addr[30] ;
extern __IO uint8_t machine_addr_len;
extern __IO int8_t machine_type[8]  ;
extern __IO uint8_t machine_type_len;

extern __I  int8_t r_machine_addr[30] ;
extern __IO uint8_t r_machine_addr_len;
extern __I  int8_t r_machine_type[8];
extern __IO uint8_t r_machine_type_len;

extern __I  int8_t r_xjj_machine_type[8];
extern __I  int8_t xjj_machine_type[8];

extern BIT_BAND_FLAG bit_band_flag_1;

extern BIT_BAND_FLAG bit_band_flag_tsk;

//LXB DEFINE

extern  __IO uint8_t 	last_log_type	;    		//上次的写日志类型。若目前要写的事件与上次一致，则不写日志
extern  __IO uint16_t ledlamp_err_count;
extern  __IO uint16_t eth_connect_count;//2min
extern  __IO uint16_t eth_restart_count;//30min

extern STAGE_TIMING_TABLE_t stage_time_table[16];//阶段配时表
extern __IO u8 current_stage_num;//阶段数
extern __IO u16 standard_lamp_current[LAMP_NUMBER];

/*组合的有效通道数和数组*/
 #define COMBINE_VALID_PASSAGE_NUM 6
extern __IO uint16_t combine_valid_passage[COMBINE_VALID_PASSAGE_NUM];

extern u8 period_time;
extern u8 learn_count;//学习次数
//LXB DEFINE

extern u16 error_report_count;//错误上报计数间隔
#define ERROR_REPORT_INTERVAL (60*10)

#define REPORT_WATCHDOG_TIME (24)	//24h--timebase=1h

#define ERROR_COUNT	10
extern u16 error_report_time[ERROR_COUNT];
extern u16 error_report_answer[ERROR_COUNT];//错误上报成功flag

extern u8   passage_clash_data[PASSAGE_ParaOutputList_MaxLine][4];//冲突故障上报
extern u8   passage_broken_data[PASSAGE_ParaOutputList_MaxLine][4];//灯故障上报
// #define DEBUG_LXB	1
// #define WATCH_DOG	1
#endif


