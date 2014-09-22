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

#define USART_RX_BUFSIZE          (200)                      //������ջ�������󳤶�
#define USART_TX_BUFSIZE          (700)                      //���巢�ͻ�������󳤶�,����־��󳤶�

extern const u8 table_week[12]; //���������ݱ�
//ƽ����·ݱ�
extern const u8 mon_table[12] ;

//�����ա����ṹ��
typedef struct USART_data
{
    volatile u8  usart_tx[USART_TX_BUFSIZE];
    volatile u8  usart_rx[USART_RX_BUFSIZE];
    volatile u16 rx_count;                   //�����ֽ�����
    volatile u16 tx_count;                   //�����ֽ�����
    volatile u8  flag_rx_finish;             //������ɱ�־ 0:δ��� 1:���
    volatile u8  flag_tx_ready;              //����׼����
    volatile u8  flag_tx_enable;             //�Ƿ��з���֡ 0:û��   1:��    
}USART_DATA;


//Э��������ݽṹ��
typedef struct Target_Data
{
    volatile u8   ID;                        //���������ʶ 
    volatile u8   count_index;               //������������ 
    volatile u8   sub_index[3];              //������
    volatile u8   sub_target;                //�Ӷ��� 
    volatile u16  data[100];                 //��������
}Target_data;

//Э�����ݽṹ��
typedef struct Protocol
{
    volatile u8           count_target;              //���������� 
    volatile u8           operation_type;            //�������� 
    volatile Target_data  data[8];                   //������������ 
}Protocol_Data;

//�źŻ��������ݽṹ��
typedef struct MachineSetContent
{   
    volatile u32           phase_index;                //��λ���
    volatile u32           phase_1;                    //��λ��ͨ��
    volatile u32           phase_2;                    //��λ��ͨ��
    volatile u32           red_time;                   //���ʱ��
    volatile u32           yellow_time;                //�Ƶ�ʱ�� 
    volatile u32           green_time;                 //�̵�ʱ��     
}MachineSetContent_Data;

//�źŻ����ýṹ��_����
typedef struct MachineSet
{
    volatile s32           index;                     //��������
    volatile u32           flag_use;                  //�Ƿ����ñ�־
    volatile u32           month;                     //��
    volatile u32           week;                      //����
    volatile u32           day;                       //����
    volatile u32           hour;                      //Сʱ
    volatile u32           minute;                    //����  
    volatile u32           phase_count;               //��λ����     
    volatile MachineSetContent_Data  data[8];        //��λ���� 
}MachineSet_Data;

//�źŻ����ýṹ��_��
typedef struct MachineSet_Method
{
    volatile u32               count;                 //��������
    volatile MachineSet_Data   data[8];               //��������
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

