#ifndef _GLOBAL_C
#define _GLOBAL_C

#include "include.h"
//======================================================================
//                            GLOBAL VARIABLE
//======================================================================
//volatile u16  ADC_buffer[4*AD_COUNT+10];      //DMA缓冲
__IO uint16_t  ADC_buffer[4*4];     						 //DMA缓冲
volatile u32  Sys_Time;                       //系统时间计数值，每0.5S加1
         u32  Lamp_Status[30];                //交通灯状态表
volatile u32  Lamp_Time[30];                  //交通灯状态间隔时间表
volatile u32  Count_Status;                   //交通灯状态个数   (状态表的行数)
         u8   Index_Status_Cur;               //交通灯当前状态索引
         u32  Lamp_Mask_Bit;                  //交通灯状态屏蔽位
         u32  Hardware_Mask_Bit;              //硬件屏蔽位
         u32  Lamp_Error_Recovery[32];        //交通灯错误状态位,记录恢复正常所需次数
         u32  Lamp_PhyLevelr_Detect[32];      //交通灯物理故障错误计数，用于物理故障报警
volatile u16  Lamp_Broken_Level[32]={0};          //交通灯各等物理故障等级A~E
         u32  Lamp_Yellow;                    //交通灯类型
         u32  Lamp_Red;                       //交通灯类型
         u32  Lamp_Green;                     //交通灯类型

volatile u16  Pwr_Freq_Index = 0;             //交通灯电压频率索引
// volatile u16  Flag_Pwr_Freq  = 0;             //交通灯电压频率采集标志
// volatile u16  Pwr_Freq_Record[10];            //交通灯电压频率缓冲
// volatile u16  Pwr_Vol_Record[10];             //交通灯电压电压缓冲         
volatile u16  Pwr_Freq = 50;                   //交通灯电压频率
volatile u16  Pwr_Vol = 220;                    //交通灯电压电压
// volatile u32  Counter        = 0;             //例常检测时间记录
volatile u8   Flag_Report    = 0;             //例常检测输出标志 
volatile u8   Flag_Learn     = 0;             //状态学习标志
volatile u8   Flag_Init      = 0;             //上电状态自检
volatile u8   Flag_DetectError= 0;            //状态表故障标志
volatile u8   Flag_NetTest   = 0;             //例常网络测试
volatile u8   Flag_NetRecover= 0;             //网络参数初始化
volatile u32  SysTime_NetRecover    = 0;      //网络参数初始化计时
volatile u8   Flag_ErrorReport      = 0;      //例常错误上报 
// volatile u8   Flag_ErrorReportHalt  = 0;      //例常错误暂停上报

volatile u32  Flag_LampUse=0;                   //信号灯使用记录  

// volatile u32 Standard_Vol[32] = {0};               //信号灯初始标准工作电压
// volatile u32 Standard_Vol_High[32] = {0};          //信号灯初始标准工作电压最大值
// volatile u32 Standard_Vol_Low[32] = {0};           //信号灯初始标准工作电压最小值

volatile u8  Flag_RestartBoard = 0;          //重启设备 

// volatile u32 Flag_Note;                      //有事件日志需要记录
// volatile u32 Note_Count[NOTE_COUNT];         //事件日志表内某一类型日志个数
// volatile u32 Note_Index[NOTE_COUNT];         //事件日志表内某一类型当前索引
// volatile NOTE_RECORD Note_Data;              //事件信息结构体

volatile u8 Dtae_Month = 0;                //电池放电月份记录 
         CHARGE_PARA Charge_Para;           //电池放电参数
volatile u32 Flag_Charge = 0;               //电池放电标志 

// volatile u32 Transformer_A= 1650;                //变压器A
// // volatile u32 Transformer_B;                //变压器B 
// volatile u32 Alarm_Charge=20;                 //电池报警时间
// volatile u32 Alarm_LampmA;                 //信号灯报警阈值 

// volatile u32 LampmA_ReportTime;            //信号灯电流上报时间 

volatile u32 LampBroken_Type;              //信号灯故障类型::0x01:冲突 0x02:故障 0x03:冲突+故障

//USART通信
volatile    u8              Flag_UsartProtocol;          //USART通信结束标志
volatile    u8              Flag_Communication;          //USART通信中标识

volatile    USART_DATA            USART_ControlBlk;            //USART通信控制块 
// // volatile    Target_data           Data_ControlBlk;             //对象数据控制块 
volatile    Protocol_Data         Protocol_ControlBlk;         //通信协议控制块 
//volatile    MachineSetMethod_Data MachineSet_ControlBlk;       //信号机设置控制块
//volatile    MachineSet_Data       MachineSet_Record;           //当前反馈的信号机状态
//volatile    u8                    MachineSet_Buffer[100];      //当前反馈的信号机状态数据缓冲区

volatile    u32            GlobalTime;                  //公共时间                4字节
volatile    u32            LocalTime;                   //本地时间                4字节             1字节

//volatile    u8             Phase_OutputList_MaxLine;    //96相位输出状态表最大行数  1字节
//volatile    u8             Phase_OutputList[30][4];     //96相位输出状态表          1字节
volatile    u8             Phase_CollisionList_MaxLine; //97相位冲突表最大行数      1字节
volatile    u8             Phase_CollisionList[6*30];   //97相位冲突表              1字节
//volatile    u8             Phase_Para_MaxLine;          //B0通道参数表最大行数      1字节
//volatile    u8             Phase_Para[30][4];           //B0通道参数表              1字节
//volatile    u8             Phase_ParaOutputList_MaxLine;  //B1通道输出表最大行数      1字节
volatile    u8   Passage_ParaOutputList[PASSAGE_ParaOutputList_MaxLine][4];   //B1通道输出表              1字节

volatile    u8             Flag_Download;               //上位机下载参数的开始结束标志 1字节

//volatile    u8             FollowPhase_MaxLine;           //跟随相位表最大行数         1字节
//volatile    u8             FollowPhase_StatusMaxLine;     //跟随相位状态表最大行数     1字节
//volatile    u8             FollowPhase_List;              //跟随相位表                 1字节
//volatile    u8             FollowPhase_StatusList[30][4]; //跟随相位状态表             1字节

volatile    u8             Lamp_PhyLevel;               //灯故障状态                 1字节
volatile    u16            Volatage_Power;              //交流电源电压               2字节
volatile    u8             Frequence_Power;             //交流电源频率               1字节
volatile    u32            Para_Charge=240;                 //上一次放电时间    4h         4字节

//volatile    u8 			Phase_StageAssignTimeList[30][7];
//volatile    u8           Phase_StageAssignTimeList_MaxLine; 

volatile    MACHINE_PARA   Machine_Parameter;           //检测器固件参数信息
//volatile    MACHINE_ADDR   Machine_Address;             //检测器地址信息

//以太网
volatile    u32                  LocalTime   = 0; 
volatile    u32 			     User_Name[16];
volatile    u32 			     User_Passord[16];
            ETHERNET_PARAMETER   Ethernet_Parameter;
//volatile    u32                  Flag_Login = 0;     
//volatile    u32                  Count_Login = 0;   
//volatile    u32                  Flag_RirstDisConnect = 0;          //第一次掉线标志
volatile    u32                  Flag_ReadNote = 0;                 //读取日志标志
volatile    u32                  Index_CurrentNote = 0;             //当前日志索引
//volatile    u32   				 Flag_Connect = 0;                  //以太网连接标志

////BATTON////
u8 Batton_Break_flag = 0;//电池损坏标志
u8 test_batton_flag = 0;//测试电池好坏
////BATTON////

/////COMMUNICATION////
//u8 Communication_Flag = 0;
/////COMMUNICATION////

//////POWER-3G///////
u8 POWER_ERROR_Recorded = 0;
//////POWER-3G///////

u8 AD_Vol_Count[3*Channel_Max] = {0};//滑动滤波计数
u32 AD_Vol_Value[3*Channel_Max][AD_Vol_Sum_Num] = {0};//滑动滤波数组*/
u32  Status_Last_print = 0;
//for fliter
u32 Fliter_Count = 0;//过滤算法中计算时间的变量，每0.5S加一
u32 Channel_Count[Channel_Max] = {0};//每个通道的计数器

u8 First_Status_Flag = 0;//第一次的状态标识，标志位0时，Last_State没有被赋值，为1时，被赋值

/***for Ethernet***/
u16 Test_Data = 0;
u8 Flag_ReInit_Eth = 0;

/***for 2400***/
u8 Data_Port[8] = {0};//新端口发送数据


u32 Status_Error = 0;
u32 Status_Error_time = 0;

u8 SelfDetect_Complete_Flag = 0;	//自检状态标识
u8 Waite_StatusChange_flag_1 = 0;//等候状态变化标识,自检定位使用
u8 Waite_StatusChange_flag_2 = 0;
u8 Index_Status_next = 0;

u8 Alarm_Data = 0;//报警标志，与A7协议的规定相符合
u8 Alarm_Data_Old = 0;//记录上次报警标示

/**********远端服务器IP和PORT定义*********/
//U8 rem_ip[4] = {192, 168, 1, 3};    //服务端IP
//u16 port = 2046;                    //服务端端口号
//u8 company_ip[4] = {192, 168, 1, 3};//公司监测电脑IP
//u16 company_port = 9000;            //公司监测电脑PORT

// // U8 tcp_soc_server;              //服务端TCP
// // u8 tcp_soc_client;              //客户端TCP
////u8 tcp_soc_company_cli;         //与公司服务器通信的客户端TCP
u8 Company_Data_Flag = 0;       //给公司监控电脑数据标志，0-无数据 1-通道状态 (5字节)2-报警信息(4字节)3-心跳(4字节)
u8 status_collect_complete = 0; //平均状态采集完毕标志
u8 collect_count = 0;           //采样次数，满5后得出一次平均状态
u32 collect_status[5];          //采样状态值，用于平均
u32 collect_value[3][3*Channel_Max];//采样电流值     
u32 Last_State = 0;//上一时刻的样本
u32 Now_State = 0;//当前采集的状态
u32 Now_AD_Value[3*Channel_Max];
u8 Work_State = 0;//检测机工作状态 0-开机初始值 1-自检状态 2- 正常监测状态 3-学习状态
u8 Host_Communication_flag = 0;//与上位机通信标志 0--不通讯 1--对上位机的命令进行回复 2--主动上报数据 (不包括心跳)3--心跳
u8 debug_type = 0;//定位时，打印开机自检还是错误重定位








//  by_lm  start

BIT_BAND_FLAG bit_band_flag_1;  //  位带标志位定义区 1
BIT_BAND_FLAG	bit_band_flag_tsk;	//给任务运行标志的位段


// // /*  USART2 Receiver buffer size */
// // //#define USART2_RX_SIZE        20   
// // /*  USART2 Receiver Fifo  */
// // vu8 usart2_rx_fifo[USART2_RX_SIZE];
// // /*  USART2 Receiver Lenth  */
// // vu8 usart2_rx_len=0;
// // vu32 usart2_rx_wr_index,usart2_rx_rd_index,usart2_rx_counter;
// // /* This flag is set on USART2 Receiver Fifo Overflow */
// // vu8 usart2_rx_buffer_overflow;


/*  USART2 Receiver buffer size */
//#define USART2_RX_SIZE        20   
/*  USART2 Receiver buffer  */
vu8 usart2_rx_buff[USART2_RX_SIZE];
/*  USART2 Receiver Lenth  */
vu32 usart2_rx_counter;


u8 eth_send_data_flag = 0;      //以太网发送数据标志
u8 client_data_type= 0;         //主动上报数据类型 1--心跳 2-其他数据    //no_liming
u8 cli_connect_ser_flag = 0;    //客户端连接服务器标志，用于主动上报数据
u8 cli_connect_ser_state= 0;    //by_lm
u8 cli_connect_wait_ack=__TRUE; 




// // // // /* Ethernet Receiver Fifo size*/
// // // // //#define RX_FIFO_SIZE        2000
// // // // /*  Ethernet Receiver Data Fifo */
// // // // vu8 eth_rx_fifo[RX_FIFO_SIZE];
// // // // /*  Ethernet Receiver Lenth Fifo */
// // // // vu32 eth_rx_len=0 ;

// // // // vu32 rx_wr_index,rx_rd_index,rx_counter;

// // // // /* This flag is set on Ethernet Receiver Fifo Overflow */
// // // // vu8 rx_buffer_overflow;


/* Ethernet Send Fifo size*/
//#define TX_FIFO_SIZE        2000
/*  Ethernet Receiver Data Fifo */
vu8 eth_tx_fifo[TX_FIFO_SIZE];
/*  Ethernet Receiver Lenth Fifo */
//#define TX_LEN_SIZE        1000
vu32 eth_tx_len[TX_LEN_SIZE] ;

vu32 tx_wr_index,			tx_rd_index,			tx_counter;
vu32 tx_len_wr_index,	tx_len_rd_index,	tx_len_counter;

/* This flag is set on Ethernet Receiver Fifo Overflow */
vu8 tx_buffer_overflow;
/* This flag is set on Ethernet Lenth Fifo Overflow */
vu8 tx_len_overflow;




//-----------------------------------------------
/*  Write Client FIFO size */
//#define WRITE_Client_SIZE        10  
/*  Write Client FIFO  */
vu8 write_Client_fifo[WRITE_Client_SIZE][SEND_LARGE_CLIENT_SIZE];

vu32 write_Client_wr_index,write_Client_rd_index,write_Client_counter;
/* This flag is set on Write Client  Fifo Overflow */
vu8 write_Client_buffer_overflow;
//-----------------------------------------------
/*  Write MySelf FIFO size */
//#define WRITE_MySelf_SIZE        10  
/*  Write MySelf FIFO  */
vu8 write_MySelf_fifo[WRITE_MySelf_SIZE][12];

vu32 write_MySelf_wr_index,write_MySelf_rd_index,write_MySelf_counter;
/* This flag is set on Write MySelf  Fifo Overflow */
vu8 write_MySelf_buffer_overflow;
//-----------------------------------------------

vu8 tcp_soc_server;              //服务端TCP
vu8 tcp_soc_client;              //客户端TCP
vu8 tcp_soc_myself;              //公司的电脑TCP链接

vu8 soc_state_lm;
vu8 wait_ack_lm;
vu8 server_wait_ack =__TRUE; 


__IO uint8_t  mac_adr[6] = {0x0,0x27,0xe,0xc9,0x87,0xb1};

__IO int8_t userName[17]	="admin";
__IO int8_t password[17]	="admin";


vu8 re_ch_pw_count 				=0;         // 修改密码  两次输入计算

__IO uint8_t alarm_restart_hour			=0;
__IO uint8_t alarm_restart_minute		=0;

__IO float 			turn_num				=220.0;  	 	//  变压器匝数比==已改成标准电压值lxb
__IO uint16_t 	bat_min					=20;    		//  后备电池报警阀值(分钟)
__IO uint16_t 	cur_min					=60;    		//  信号灯电流异常报警阀值(%)
__IO uint16_t 	send_time				=30;    		//  信号灯电流上报时间(分钟)
__IO float 		turn_num_lxb			=2494.2;  	//  变压器匝数比lxb

__IO uint8_t machine_addr[30]="北京太阳宫";
__IO uint8_t machine_addr_len=30;

__IO int8_t machine_type[8]="ZL-D";//信号机信号
__IO uint8_t machine_type_len=8;

/****ETH Par******/
__IO uint8_t 	server_ip[4]			={11,1,1,11};          	//服务端IP
// __IO uint8_t 	server_www[40]		={"www.baidu.com"};   			//服务端域名
__IO uint16_t server_port				=2046;               				//服务端PORT
__IO uint16_t local_port 				= 90;            						//本地服务端口号

// __IO uint8_t 	myself_server_ip[4]		={10,1,2,253};          	//公司服务端IP
// __IO uint16_t myself_server_port		=9000;               				//公司服务端PORT
__IO uint8_t 	myself_server_ip[4]		={0,0,0,0};          	//公司服务端IP
__IO uint16_t myself_server_port		=0;               				//公司服务端PORT



//=======================		default start	 =======================
__I  uint8_t 	r_server_ip[4]		={11,1,1,11};          	//服务端IP
// __I  uint8_t 	r_server_www[40]	={"www.baidu.com"};   			//服务端域名
__I  uint16_t r_server_port			=2046;               				//服务端PORT
__I  uint16_t r_local_port 			= 90;            						//本地服务端口号

// __IO uint8_t 	r_myself_server_ip[4]		={10,1,2,253};          	//公司服务端IP
// __IO uint16_t r_myself_server_port		=9000;               				//公司服务端PORT
__IO uint8_t 	r_myself_server_ip[4]		={0,0,0,0};          	//公司服务端IP
__IO uint16_t r_myself_server_port		=0;               				//公司服务端PORT

__I uint8_t   r_mac_adr[6] = {0x0,0x50,0xc2,0x48,0xf4,0xf1 };//{0x0,0x27,0xe,0xc9,0x87,0xb1 };
__I LOCALM    r_ip_config = {
   { 192,168,1,2},             // IP address
   { 192,168,1,1 },               // Default Gateway
   { 255,255,255,0 },             // Net mask
   { 0,0,0,0 },              // Primary DNS server
   {  0,0,0,0 }               // Secondary DNS server
};


__I  int8_t r_password[17]="admin";

__I uint8_t r_alarm_restart_hour			=0;
__I uint8_t r_alarm_restart_minute		=0;

__I  float 			r_turn_num			=220.0;  	 	//  变压器匝数比
__I  uint16_t 	r_bat_min				=20;    		//  后备电池报警阀值(分钟)
__I  uint16_t 	r_cur_min				=60;    		//  信号灯电流异常报警阀值(mA)
__I  uint16_t 	r_send_time			=30;    		//  信号灯电流上报时间(分钟)

__IO float  		r_turn_num_lxb	= 2494.2;  	//  变压器匝数比lxb


__I  int8_t r_machine_addr[30]="北京太阳宫";
__IO uint8_t r_machine_addr_len=30;

__I  int8_t r_machine_type[8]="ZL-D";
__IO uint8_t r_machine_type_len=8;

__I  int8_t r_xjj_machine_type[8]="V4.0";
__I  int8_t xjj_machine_type[8]="V4.0";
//=======================		default end	 =======================

//   by_lm  end

//LXB DEFINE

__IO uint8_t 	last_log_type			=0xFF;    		//上次的写日志类型。若目前要写的事件与上次一致，则不写日志
__IO uint16_t ledlamp_err_count  = 0;					//	故障计数；若有故障，则赋值给该变量，让com灯闪烁3min
__IO uint16_t eth_connect_count = 240;//2min
__IO uint16_t eth_restart_count = NET_STOP_TIME;//30min

STAGE_TIMING_TABLE_t stage_time_table[PHASE_CLASH_STATE_NUMBER] = {0};//阶段配时表
__IO u8 current_stage_num = 0;//阶段数
__IO u16 standard_lamp_current[LAMP_NUMBER] = {0};	

/*组合的有效通道数和数组*/
// #define COMBINE_VALID_PASSAGE_NUM 6
__IO uint16_t combine_valid_passage[COMBINE_VALID_PASSAGE_NUM] = {0};

u8 period_time = 0;//当前周期时长
u8 learn_count = 0;//学习次数
//LXB DEFINE

u16 error_report_count = 0;//错误上报计数间隔

/*
	0,通道冲突
	1,灯故障
	2,灯物理故障
	3,相位冲突
	4,学习三次不成功
	5,3G电源故障
	6,220V电压故障
	7,放电异常
*/
u16 error_report_time[ERROR_COUNT] = {0};

/*
	0,冲突
	1,灯故障
	2,灯物理故障
	3,相位冲突
	4,学习三次不成功
	5,3G电源故障
	6,220V电压故障
	7,放电异常
*/
u16 error_report_answer[ERROR_COUNT] = {1,1,1,1,1,1,1,1,1,1};//错误上报成功flag

u8   passage_clash_data[PASSAGE_ParaOutputList_MaxLine][4]={0};//冲突故障上报
u8   passage_broken_data[PASSAGE_ParaOutputList_MaxLine][4]={0};//灯故障上报

#endif
