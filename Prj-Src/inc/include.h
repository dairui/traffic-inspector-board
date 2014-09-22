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


//���������
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

//����������
//================================================================
#define DEBUG_MODE                               //���ڵ���ģʽ
#define LAMP_DETECT                              //�Ƿ�ִ�н�ͨ�Ƽ������
#define LAMP_PHYLEVEL_REPORT                     //����������Ƿ��ϱ� 
#define HEART_TIME_TO_HOST      (2*60*10)             //��λ���������ķ��ͼ��: n*0.5�� 
#define HEART_TIME_TO_COMPANY   (360)             //��˾�������������ķ��ͼ��: n*0.5�� 
#define RE_CONNECTION_TIME      (1)              //������ߺ����½�����������ʱ���趨 n*HEART_TIME
#define LOGIN_VALIDTIME         (120)            //�����¼������Чʱ��: n*0.5�� 
//#define WATCH_DOG                              //�Ƿ�ʹ�ÿ��Ź�   
#define ERROR_REPORT_TIME       (12)             //��������ϱ�: n*0.5�� 

#define U32 u32
//================================================================
//��ⲿ�����в�������
//================================================================
//�źŲ����̶��ж���Χ
//#define AD_THRESHOLD     (0.15)

//״̬��ʱ����0.5x��,���Ͻ綨
#define LAMP_TIME_SCALE  (2000)

//AD����ʱ���ӳ�(΢��)
#define AD_DELAY         (500)  

//AD20MS�ڲ�������
#define AD_COUNT    (10)//     ((20000)/(AD_DELAY))  //#define AD_DELAY         (500)  

//AD���ڲ���ʱ��(��)
//#define AD_TIME          (2)

//������������ֵ
#define VALUE_MA         (2000)

//����״̬��
#define MIN_STATUS       (1)

//��ͨ��"��"״̬�ж���ֵ
//#define AD_PORPROTION    (0.3)

//��ͨ�Ƶ�����ֵ�����ж���Χ
//#define THRESH_PORPROTION    (0.8)

//��ͨ�ƹ�����ֵ����λmA
#define Broken_ALARM_THRESH     (10)  

//��طŵ籨����ֵ����λminute
#define Charge_ALARM_THRESH     (10) 

//��������ѹ������ֵ���ٷֱ� 
#define POWER_ALARM_THRESH              (0.2)  

//���������ʼ����ť��Чʱ�� n*0.5
#define NET_RECOVER_TIME                (20)  
//================================================================

//ѧϰ����״̬��
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


#define SetBit(x , y)  x |= (1<<(y)) //��X�ĵ�Yλ��1

#define ClrBit(x , y)  x &= !(1<<(y)) //��X�ĵ�Yλ��0


//���������

#define ID_MAX_LENGTH (40)
//������̼�����
typedef struct _MACHINE_PARA
{
    volatile u8  Node_ID_Length;                    //��㳤��
    volatile u8  Node_ID[ID_MAX_LENGTH];
    volatile u8  Manufacture_Length;                //�����̳���
    volatile u8  Manufacture_ID[ID_MAX_LENGTH];
    volatile u8  Type_Length;                       //ģ���ͺų���
    volatile u8  Type[ID_MAX_LENGTH]; 
    volatile u8  Version_Length;                    //�汾�ų���
    volatile u8  Version[ID_MAX_LENGTH];               
}MACHINE_PARA;

// // //�źŻ���ַ
// // typedef struct _MACHINE_ADDR
// // {
// //     volatile u8   Node_Addr_Length;                  //����ַ����
// //     volatile u32  Node_Addr[ID_MAX_LENGTH];       
// //     volatile u8   Machine_Type_Length;               //�źŻ�����
// //     volatile u32  Machine_Type[ID_MAX_LENGTH];    
// // }MACHINE_ADDR;

//��̫������
typedef struct _ETHERNET_PARAMETER
{
     u32   Local_ip;                  //����ַ����
     u32   Net_mask;
     u32   Gate_way;
     u32   Remote_ip;
     u32   port;
     u32   MAC[6];
     u32   DNS;
}ETHERNET_PARAMETER;

//��طŵ����
typedef struct _CHARGE_PARA
{
     u32   Start_Time;                 
     u32   End_Time;
}CHARGE_PARA;

extern __IO uint16_t ADC_buffer[];                  //DMA����
// // // extern volatile u32 Sys_Time;                        //ϵͳʱ�����ֵ
// // // extern          u32 Lamp_Status[30];                 //��ͨ��״̬��
// // // extern volatile u32  Lamp_Time[30];                  //��ͨ��״̬���ʱ���
// // // extern volatile u32 Count_Status;                    //��ͨ��״̬��״̬����
// // // extern          u8  Index_Status_Cur;                //��ͨ�Ƶ�ǰ״̬���� 
// // // extern          u32  Lamp_Mask_Bit;                  //��ͨ��״̬����λ
// // // extern          u32  Hardware_Mask_Bit;              //Ӳ������λ
// // // extern          u32  Lamp_Error_Recovery[32];        //��ͨ�ƴ���״̬λ
// // // extern          u32  Lamp_PhyLevelr_Detect[32];      //��ͨ��������ϴ������������������ϱ���
extern volatile u16  Lamp_Broken_Level[32];          //��ͨ�Ƹ���������ϵȼ�
// // // extern          u32  Lamp_Yellow;                    //��ͨ������
// // // extern          u32  Lamp_Red;                       //��ͨ������
// // // extern          u32  Lamp_Green;                     //��ͨ������
// // // extern volatile u32  Lamp_PhyLevel_ErrorHistory;     //��ͨ�����������ʷ��¼ 
// // // extern volatile u16  Flag_Pwr_Freq;                  //��ͨ�Ƶ�ѹƵ�ʲɼ���־
extern volatile u16  Pwr_Freq_Index;                 //��ͨ�Ƶ�ѹƵ������
extern volatile u16  Pwr_Freq_Record[10];            //��ͨ�Ƶ�ѹƵ�ʻ���
extern volatile u16  Pwr_Vol_Record[10];             //��ͨ�Ƶ�ѹ��ѹ����  
extern volatile u16  Pwr_Freq;                       //��ͨ�Ƶ�ѹƵ��
extern volatile u16  Pwr_Vol;                        //��ͨ�Ƶ�ѹ��ѹ
extern volatile u32  Counter;                         //���5����״̬���
// // // extern volatile u8   Flag_Report;                    //������������־
// // // extern volatile u8   Flag_Learn;                     //״̬ѧϰ��־ 
// // // extern volatile u8   Flag_Init;			             //�ϵ�״̬�Լ�
// // // extern volatile u8   Flag_DetectError;               //״̬����ϱ�־
// // // extern volatile u8   Flag_NetTest;                   //�����������
// // // extern volatile u8   Flag_NetRecover;                //���������ʼ��
// // // extern volatile u32  SysTime_NetRecover;             //���������ʼ����ʱ
// // // extern volatile u8   Flag_ErrorReport;               //���������ϱ�
// extern volatile u8   Flag_ErrorReportHalt;           //����������ͣ�ϱ�



extern volatile u32  Flag_LampUse;                   //�źŵ�ʹ�ü�¼  

// // // extern volatile u32  Standard_Vol[32];               //�źŵƳ�ʼ��׼������ѹ
// // // extern volatile u32 Standard_Vol_High[32];           //�źŵƳ�ʼ��׼������ѹ���ֵ
// // // extern volatile u32 Standard_Vol_Low[32];            //�źŵƳ�ʼ��׼������ѹ��Сֵ

// // // extern volatile u8  Flag_RestartBoard;               //�����豸 

extern volatile u8 Dtae_Month;                      //��طŵ��·ݼ�¼ 
extern          CHARGE_PARA Charge_Para;             //��طŵ����
extern volatile u32 Flag_Charge;                     //��طŵ��־ 

// extern volatile u32 Transformer_A;                //��ѹ��A
// // extern volatile u32 Transformer_B;                //��ѹ��B 
// extern volatile u32 Alarm_Charge;                 //��ر���ʱ��
// extern volatile u32 Alarm_LampmA;                 //�źŵƱ�����ֵ 

// extern volatile u32 LampmA_ReportTime;            //�źŵƵ����ϱ�ʱ�� 

extern volatile u32 LampBroken_Type;              //�źŵƹ�������:��ͻ����




// // // //USARTͨ��
extern volatile    u8             Flag_UsartProtocol;          //USARTͨ�Ž�����־
extern volatile    u8             Flag_Communication;          //USARTͨ���б�ʶ

extern volatile    USART_DATA     USART_ControlBlk;            //USARTͨ�ſ��ƿ� 
// // // extern volatile    Target_data    Data_ControlBlk;             //�������ݿ��ƿ� 
extern volatile    Protocol_Data  Protocol_ControlBlk;         //ͨ��Э����ƿ� 
// // // //extern volatile    MachineSetMethod_Data MachineSet_ControlBlk;       //�źŻ����ÿ��ƿ�
// // // //extern volatile    MachineSet_Data       MachineSet_Record;           //��ǰ�������źŻ�״̬
// // // //extern volatile    u8                    MachineSet_Buffer[100];      //��ǰ�������źŻ�״̬���ݻ�����


extern volatile    u32            GlobalTime;                  //����ʱ��           4�ֽ�
extern volatile    u32            LocalTime;                   //����ʱ��                4�ֽ�
// // // //extern volatile    u8             Phase_OutputList_MaxLine;    //��λ���״̬���������  1�ֽ�
// // // //extern volatile    u8             Phase_OutputList[30][4];     //��λ���״̬��     1�ֽ�
// // // extern volatile    u8             Phase_CollisionList_MaxLine; //��λ��ͻ���������      1�ֽ�
// // // extern volatile    u8             Phase_CollisionList[30*6];   //��λ��ͻ��         1�ֽ�
// // // //extern volatile    u8             Phase_Para_MaxLine;          //ͨ���������������      1�ֽ�
// // // //extern volatile    u8             Phase_Para[30][4];           //ͨ��������              1�ֽ�
// // // extern volatile    u8             Phase_ParaOutputList_MaxLine;  //ͨ��������������      1�ֽ�
extern volatile    u8    Passage_ParaOutputList[PASSAGE_ParaOutputList_MaxLine][4];   //ͨ�������              1�ֽ�


// // // extern volatile    u8             Flag_Download;               //��λ�����ز����Ŀ�ʼ������־ 1�ֽ�

// // // //extern volatile    u8             FollowPhase_MaxLine;           //������λ���������         1�ֽ�
// // // //extern volatile    u8             FollowPhase_StatusMaxLine;     //������λ״̬���������     1�ֽ�
// // // //extern volatile    u8             FollowPhase_StatusList[30][4]; //������λ״̬��             1�ֽ�

// // // extern volatile    u8             Lamp_PhyLevel;               //�ƹ���״̬                 1�ֽ�
// // // extern volatile    u16            Volatage_Power;              //������Դ��ѹ               2�ֽ�
// // // extern volatile    u8             Frequence_Power;             //������ԴƵ��               1�ֽ�
extern volatile    u32            Para_Charge;                 //�󱸵�ز���               4�ֽ�

// // // //extern volatile    u8 			Phase_StageAssignTimeList[30][7];
// // // //extern volatile    u8           Phase_StageAssignTimeList_MaxLine; 

// // // extern volatile    MACHINE_PARA   Machine_Parameter;           //������̼�������Ϣ
// // // extern volatile    MACHINE_ADDR   Machine_Address;             //�������ַ��Ϣ

// // // extern volatile u32 Flag_Note;                                //���¼���־��Ҫ��¼
// // // extern volatile   u32 Note_Count[NOTE_COUNT];                 //�¼���־����ĳһ������־����
// // // extern volatile   u32 Note_Index[NOTE_COUNT];                 //�¼���־����ĳһ���͵�ǰ����
// // // extern volatile   NOTE_RECORD Note_Data;                        //�¼���Ϣ�ṹ��



// // // //��̫��
// // // extern volatile    u32 LocalTime; 
// // // extern             u8  connect_timeout;
// // // extern             u8  connect_flag;
// // // extern volatile    u32  User_Name[16];
// // // extern volatile    u32  User_Passord[16];
// // // extern             ETHERNET_PARAMETER   Ethernet_Parameter;
// // // //extern volatile    u32                  Flag_Login;     
// // // //extern volatile    u32                  Count_Login;   
// // // //extern volatile    u32                  Flag_RirstDisConnect;  
// // // extern volatile    u32                  Flag_ReadNote;                 //��ȡ��־��־
// // // extern volatile    u32                  Index_CurrentNote;             //��ǰ��־����
// // // //extern volatile    u32   				Flag_Connect;                  //��̫�����ӱ�־


// // // ////BATTON////
extern u8 Batton_Break_flag;
extern u8 test_batton_flag;//���Ե�غû�
// // // ////BATTON////

// // // /////COMMUNICATION////
// // // //extern u8 Communication_Flag;
// // // /////COMMUNICATION////

//////POWER-3G///////
extern u8 POWER_ERROR_Recorded;
//////POWER-3G///////

// // // extern u8 AD_Vol_Count[3*Channel_Max] ;//�����˲�����

// // // extern u32 AD_Vol_Value[3*Channel_Max][AD_Vol_Sum_Num] ;//�����˲�����


// // // extern u32  Status_Last_print;
// // // //for fliter
// // // extern u32 Fliter_Count ;//�����㷨�м���ʱ��ı�����ÿ0.5S��һ
// // // extern u32 Channel_Count[Channel_Max] ;//ÿ��ͨ���ļ�����
// // // extern u32 Last_State ;//��һʱ�̵�����
// // // #define Threshold_Time (5)//ʱ�䷧ֵ
// // // extern u32 Now_State;
// // // extern u8 First_Status_Flag;//��һ�ε�״̬��ʶ����־λ0ʱ��Last_Stateû�б���ֵ��Ϊ1ʱ������ֵ

// // // #define	ETH_RestartBoard_Max_Count    (1800*2)  //1800�룬30���ӡ�������������ֵ

// // // /***for Ethernet***/
// // // extern u16 Test_Data ;
// // // extern u8 Flag_ReInit_Eth;
// // // //extern void eth_re_init(void);

// // // /***for 2400***/
extern u8 Data_Port[8] ;//�¶˿ڷ�������

// // // /***for Lamp Detect***/
// // // //extern u32 Lamp_Error_Light ;//��ǵƳ�������λ
// // // //extern u32 Lamp_Error_NotLight ;//��ǵ������λ
// // // //extern u32 Lamp_Error_Time[32] ;//��¼���Ϸ���ʱ��
// // // //extern u32 Circle_Time ;//״̬������ʱ��
// // // extern u32 Status_Error;
// // // extern u32 Status_Error_time;


// // // extern u8 SelfDetect_Complete_Flag;	//�Լ�״̬��ʶ
// // // extern u8 Waite_StatusChange_flag_1;//�Ⱥ�״̬�仯��ʶ,�Լ춨λʹ��
// // // extern u8 Waite_StatusChange_flag_2;
// // // extern u8 Index_Status_next;

// // // extern u8 Alarm_Data;//������־����A7Э��Ĺ涨�����
// // // extern u8 Alarm_Data_Old;//��¼�ϴα�����ʾ

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
// // // extern u8 debug_type;//��λʱ����ӡ�����Լ컹�Ǵ����ض�λ



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
extern __IO uint8_t 	server_ip[4]			;         //�����IP
extern __IO uint8_t 	server_www[40]		;   			//���������
extern __IO uint16_t server_port				;         //�����PORT
extern __IO uint16_t local_port 				;         //���ط���˿ں�

extern __I  uint8_t 	r_server_ip[4]		;         //�����IP
extern __I  uint8_t 	r_server_www[40]	;   			//���������
extern __I  uint16_t r_server_port			;         //�����PORT
extern __I  uint16_t r_local_port 			;         //���ط���˿ں�

extern __IO uint8_t 	r_myself_server_ip[4];          	//��˾�����IP
extern __IO uint16_t r_myself_server_port	 ;            //��˾�����PORT

extern __IO uint8_t 	myself_server_ip[4]	;          	//��˾�����IP
extern __IO uint16_t myself_server_port		;           //��˾�����PORT




extern u8 eth_send_data_flag ;      //��̫���������ݱ�־
extern u8 client_data_type;         //�����ϱ��������� 1--���� 2-��������    //no_liming
extern u8 cli_connect_ser_flag ;    //�ͻ������ӷ�������־�����������ϱ�����
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







extern vu8 tcp_soc_server;              //�����TCP
extern vu8 tcp_soc_client;              //�ͻ���TCP
extern vu8 tcp_soc_myself;              //��˾�ĵ���TCP����

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

extern vu8 re_ch_pw_count 	;           // �޸�����  ���������ж�

extern __IO uint8_t alarm_restart_hour			;
extern __IO uint8_t alarm_restart_minute		;
extern __I  uint8_t r_alarm_restart_hour			;
extern __I  uint8_t r_alarm_restart_minute		;

extern __IO  float		turn_num				;  	 	//  ��ѹ��������
#define  Transformer_A turn_num 
extern __IO float 		turn_num_lxb		;  	 	//  ��ѹ��������

extern __IO uint16_t 	bat_min					;    		//  �󱸵�ر�����ֵ(����)
#define Alarm_Charge bat_min

extern __IO uint16_t 	cur_min					;    		//  �źŵƵ����쳣������ֵ(mA)
#define Alarm_LampmA cur_min

extern __IO uint16_t 	send_time				;    		//  �źŵƵ����ϱ�ʱ��(����)
#define LampmA_ReportTime send_time

extern __I  float 		r_turn_num			;  	 	// ��׼��ѹ
extern __I  uint16_t 	r_bat_min				;    		//  �󱸵�ر�����ֵ(����)
extern __I  uint16_t 	r_cur_min				;    		//  �źŵƵ����쳣������ֵ(mA)
extern __I  uint16_t 	r_send_time			;    		//  �źŵƵ����ϱ�ʱ��(����)
extern __IO float 		r_turn_num_lxb		;  	//  ��ѹ��������lxb


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

extern  __IO uint8_t 	last_log_type	;    		//�ϴε�д��־���͡���ĿǰҪд���¼����ϴ�һ�£���д��־
extern  __IO uint16_t ledlamp_err_count;
extern  __IO uint16_t eth_connect_count;//2min
extern  __IO uint16_t eth_restart_count;//30min

extern STAGE_TIMING_TABLE_t stage_time_table[16];//�׶���ʱ��
extern __IO u8 current_stage_num;//�׶���
extern __IO u16 standard_lamp_current[LAMP_NUMBER];

/*��ϵ���Чͨ����������*/
 #define COMBINE_VALID_PASSAGE_NUM 6
extern __IO uint16_t combine_valid_passage[COMBINE_VALID_PASSAGE_NUM];

extern u8 period_time;
extern u8 learn_count;//ѧϰ����
//LXB DEFINE

extern u16 error_report_count;//�����ϱ��������
#define ERROR_REPORT_INTERVAL (60*10)

#define REPORT_WATCHDOG_TIME (24)	//24h--timebase=1h

#define ERROR_COUNT	10
extern u16 error_report_time[ERROR_COUNT];
extern u16 error_report_answer[ERROR_COUNT];//�����ϱ��ɹ�flag

extern u8   passage_clash_data[PASSAGE_ParaOutputList_MaxLine][4];//��ͻ�����ϱ�
extern u8   passage_broken_data[PASSAGE_ParaOutputList_MaxLine][4];//�ƹ����ϱ�
// #define DEBUG_LXB	1
// #define WATCH_DOG	1
#endif


