#ifndef __USART_H
#define __USART_H

//======================================================================
//                          USART COMMUNCATION COMMAND
//======================================================================
#define INQUERY          (0)
#define SET              (1)
#define SET_NO_REPLAY    (2)
#define REPLAY_INQUERY   (4)
#define REPLAY_SET       (5)
#define REPLAY_ERROR     (6)
#define REPORT           (3)
#define REPORT_REPLAY    (7)

//======================================================================
//                          USART COMMUNCATION STRUCT
//======================================================================

#define USART_RX_BUFSIZE          (200)                      //定义接收缓冲区最大长度
#define USART_TX_BUFSIZE          (700)                      //定义发送缓冲区最大长度,单日志最大长度

extern const u8 table_week[12]; //月修正数据表
//平年的月份表
extern const u8 mon_table[12] ;

//串口收、发结构体
typedef struct USART_data
{
    volatile u8  usart_tx[USART_TX_BUFSIZE];
    volatile u8  usart_rx[USART_RX_BUFSIZE];
    volatile u16 rx_count;                   //接收字节数量
    volatile u16 tx_count;                   //发送字节数量
    volatile u8  flag_rx_finish;             //接收完成标志 0:未完成 1:完成
    volatile u8  flag_tx_ready;              //发送准备好
    volatile u8  flag_tx_enable;             //是否有返回帧 0:没有   1:有    
}USART_DATA;


//协议对象数据结构体
typedef struct Target_Data
{
    volatile u8   ID;                        //操作对象标识 
    volatile u8   count_index;               //操作索引个数 
    volatile u8   sub_index[3];              //子索引
    volatile u8   sub_target;                //子对象 
    volatile u16  data[100];                 //操作数据
}Target_data;

//协议内容结构体
typedef struct Protocol
{
    volatile u8           count_target;              //操作对象数 
    volatile u8           operation_type;            //操作类型 
    volatile Target_data  data[8];                   //操作对象数据 
}Protocol_Data;

//信号机设置内容结构体
typedef struct MachineSetContent
{   
    volatile u32           phase_index;                //相位编号
    volatile u32           phase_1;                    //相位、通道
    volatile u32           phase_2;                    //相位、通道
    volatile u32           red_time;                   //红灯时间
    volatile u32           yellow_time;                //黄灯时间 
    volatile u32           green_time;                 //绿灯时间     
}MachineSetContent_Data;

//信号机设置结构体_部分
typedef struct MachineSet
{
    volatile s32           index;                     //操作索引
    volatile u32           flag_use;                  //是否设置标志
    volatile u32           month;                     //月
    volatile u32           week;                      //周天
    volatile u32           day;                       //月天
    volatile u32           hour;                      //小时
    volatile u32           minute;                    //分钟  
    volatile u32           phase_count;               //相位个数     
    volatile MachineSetContent_Data  data[8];        //相位数据 
}MachineSet_Data;

//信号机设置结构体_总
typedef struct MachineSet_Method
{
    volatile u32               count;                 //方案个数
    volatile MachineSet_Data   data[8];               //方案数据
}MachineSetMethod_Data;

void USART_Tx_nByte(USART_TypeDef* USARTx, u8 *address, u16 n);
void USART_Tx_String(USART_TypeDef* USARTx, uc8 *address);
void USART_Tx_Byte(USART_TypeDef* USARTx, u16 data);
u16 USART_Rx_Byte(USART_TypeDef* USARTx);
void USART_Tx_Display(uc8 *address);

void USART_Protocol_Communication(USART_DATA *p_usart_controlblk);
u8   USART_Protocol_Explain(USART_DATA *p_usart_controlblk);
void USART_Protocol_GlobalTime(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_LocalTime(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Phase_CollisionList(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Lamp_PhyLevel(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_STDLamp_PhyLevel(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Volatage_Power(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Frequence_Power(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Para_Charge(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Flag_Download(u8 i);
void USART_Protocol_ParamterList(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_ParamterOutputList(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Phase_OutputList(u8 i);
void USART_Protocol_CollisionReport(void);
void USART_Protocol_LampPhyLevelReport(void);
void USART_Protocol_MachinePara(u8 i);
void USART_Protocol_MachineAddr(u8 i,USART_DATA *p_usart_controlblk);
void USART_DebugDataInit(void);
void USART_Protocol_MachineSet_1(u8 i);
void USART_Protocol_MachineSet_2(u8 i);
void USART_Protocol_MachineSet_3(u8 i);
void USART_Protocol_MachineStageSet(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_MachineSet_Calculate(void);
void USART_Protocol_PromoteControl(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_ID_Report(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_Network_Report(u8 i,USART_DATA *p_usart_controlblk);
U32  USART_Protocol_NoteRequry(u8 i,USART_DATA *p_usart_controlblk);
void USART_Protocol_ReadNote(void);
void USART_Protocol_ChargeReport(void);
void USART_Protocol_Charge_Report(u8 i);
void USART_Protocol_FrequencyReport(void);
void USART_Protocol_VolatageReport(void);
void USART_Protocol_Frequency_Report(u8 i);
void USART_Protocol_Volatage_Report(u8 i);
void USART_Protocol_AllErrorReport(u8 error);
void USART_Protocol_Error_Report(u8 i);


__task void Analyze_Task(void);
u8 Is_Leap_Year(u16 year);
u8 RTC_Get_Week(u16 year, u8 month, u8 day);

#endif

