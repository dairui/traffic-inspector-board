#ifndef _GLOBAL_C
#define _GLOBAL_C

#include "include.h"
//======================================================================
//                            GLOBAL VARIABLE
//======================================================================
//volatile u16  ADC_buffer[4*AD_COUNT+10];      //DMA����
__IO uint16_t  ADC_buffer[4*4];     						 //DMA����
volatile u32  Sys_Time;                       //ϵͳʱ�����ֵ��ÿ0.5S��1
         u32  Lamp_Status[30];                //��ͨ��״̬��
volatile u32  Lamp_Time[30];                  //��ͨ��״̬���ʱ���
volatile u32  Count_Status;                   //��ͨ��״̬����   (״̬�������)
         u8   Index_Status_Cur;               //��ͨ�Ƶ�ǰ״̬����
         u32  Lamp_Mask_Bit;                  //��ͨ��״̬����λ
         u32  Hardware_Mask_Bit;              //Ӳ������λ
         u32  Lamp_Error_Recovery[32];        //��ͨ�ƴ���״̬λ,��¼�ָ������������
         u32  Lamp_PhyLevelr_Detect[32];      //��ͨ��������ϴ������������������ϱ���
volatile u16  Lamp_Broken_Level[32]={0};          //��ͨ�Ƹ���������ϵȼ�A~E
         u32  Lamp_Yellow;                    //��ͨ������
         u32  Lamp_Red;                       //��ͨ������
         u32  Lamp_Green;                     //��ͨ������

volatile u16  Pwr_Freq_Index = 0;             //��ͨ�Ƶ�ѹƵ������
// volatile u16  Flag_Pwr_Freq  = 0;             //��ͨ�Ƶ�ѹƵ�ʲɼ���־
// volatile u16  Pwr_Freq_Record[10];            //��ͨ�Ƶ�ѹƵ�ʻ���
// volatile u16  Pwr_Vol_Record[10];             //��ͨ�Ƶ�ѹ��ѹ����         
volatile u16  Pwr_Freq = 50;                   //��ͨ�Ƶ�ѹƵ��
volatile u16  Pwr_Vol = 220;                    //��ͨ�Ƶ�ѹ��ѹ
// volatile u32  Counter        = 0;             //�������ʱ���¼
volatile u8   Flag_Report    = 0;             //������������־ 
volatile u8   Flag_Learn     = 0;             //״̬ѧϰ��־
volatile u8   Flag_Init      = 0;             //�ϵ�״̬�Լ�
volatile u8   Flag_DetectError= 0;            //״̬����ϱ�־
volatile u8   Flag_NetTest   = 0;             //�����������
volatile u8   Flag_NetRecover= 0;             //���������ʼ��
volatile u32  SysTime_NetRecover    = 0;      //���������ʼ����ʱ
volatile u8   Flag_ErrorReport      = 0;      //���������ϱ� 
// volatile u8   Flag_ErrorReportHalt  = 0;      //����������ͣ�ϱ�

volatile u32  Flag_LampUse=0;                   //�źŵ�ʹ�ü�¼  

// volatile u32 Standard_Vol[32] = {0};               //�źŵƳ�ʼ��׼������ѹ
// volatile u32 Standard_Vol_High[32] = {0};          //�źŵƳ�ʼ��׼������ѹ���ֵ
// volatile u32 Standard_Vol_Low[32] = {0};           //�źŵƳ�ʼ��׼������ѹ��Сֵ

volatile u8  Flag_RestartBoard = 0;          //�����豸 

// volatile u32 Flag_Note;                      //���¼���־��Ҫ��¼
// volatile u32 Note_Count[NOTE_COUNT];         //�¼���־����ĳһ������־����
// volatile u32 Note_Index[NOTE_COUNT];         //�¼���־����ĳһ���͵�ǰ����
// volatile NOTE_RECORD Note_Data;              //�¼���Ϣ�ṹ��

volatile u8 Dtae_Month = 0;                //��طŵ��·ݼ�¼ 
         CHARGE_PARA Charge_Para;           //��طŵ����
volatile u32 Flag_Charge = 0;               //��طŵ��־ 

// volatile u32 Transformer_A= 1650;                //��ѹ��A
// // volatile u32 Transformer_B;                //��ѹ��B 
// volatile u32 Alarm_Charge=20;                 //��ر���ʱ��
// volatile u32 Alarm_LampmA;                 //�źŵƱ�����ֵ 

// volatile u32 LampmA_ReportTime;            //�źŵƵ����ϱ�ʱ�� 

volatile u32 LampBroken_Type;              //�źŵƹ�������::0x01:��ͻ 0x02:���� 0x03:��ͻ+����

//USARTͨ��
volatile    u8              Flag_UsartProtocol;          //USARTͨ�Ž�����־
volatile    u8              Flag_Communication;          //USARTͨ���б�ʶ

volatile    USART_DATA            USART_ControlBlk;            //USARTͨ�ſ��ƿ� 
// // volatile    Target_data           Data_ControlBlk;             //�������ݿ��ƿ� 
volatile    Protocol_Data         Protocol_ControlBlk;         //ͨ��Э����ƿ� 
//volatile    MachineSetMethod_Data MachineSet_ControlBlk;       //�źŻ����ÿ��ƿ�
//volatile    MachineSet_Data       MachineSet_Record;           //��ǰ�������źŻ�״̬
//volatile    u8                    MachineSet_Buffer[100];      //��ǰ�������źŻ�״̬���ݻ�����

volatile    u32            GlobalTime;                  //����ʱ��                4�ֽ�
volatile    u32            LocalTime;                   //����ʱ��                4�ֽ�             1�ֽ�

//volatile    u8             Phase_OutputList_MaxLine;    //96��λ���״̬���������  1�ֽ�
//volatile    u8             Phase_OutputList[30][4];     //96��λ���״̬��          1�ֽ�
volatile    u8             Phase_CollisionList_MaxLine; //97��λ��ͻ���������      1�ֽ�
volatile    u8             Phase_CollisionList[6*30];   //97��λ��ͻ��              1�ֽ�
//volatile    u8             Phase_Para_MaxLine;          //B0ͨ���������������      1�ֽ�
//volatile    u8             Phase_Para[30][4];           //B0ͨ��������              1�ֽ�
//volatile    u8             Phase_ParaOutputList_MaxLine;  //B1ͨ��������������      1�ֽ�
volatile    u8   Passage_ParaOutputList[PASSAGE_ParaOutputList_MaxLine][4];   //B1ͨ�������              1�ֽ�

volatile    u8             Flag_Download;               //��λ�����ز����Ŀ�ʼ������־ 1�ֽ�

//volatile    u8             FollowPhase_MaxLine;           //������λ���������         1�ֽ�
//volatile    u8             FollowPhase_StatusMaxLine;     //������λ״̬���������     1�ֽ�
//volatile    u8             FollowPhase_List;              //������λ��                 1�ֽ�
//volatile    u8             FollowPhase_StatusList[30][4]; //������λ״̬��             1�ֽ�

volatile    u8             Lamp_PhyLevel;               //�ƹ���״̬                 1�ֽ�
volatile    u16            Volatage_Power;              //������Դ��ѹ               2�ֽ�
volatile    u8             Frequence_Power;             //������ԴƵ��               1�ֽ�
volatile    u32            Para_Charge=240;                 //��һ�ηŵ�ʱ��    4h         4�ֽ�

//volatile    u8 			Phase_StageAssignTimeList[30][7];
//volatile    u8           Phase_StageAssignTimeList_MaxLine; 

volatile    MACHINE_PARA   Machine_Parameter;           //������̼�������Ϣ
//volatile    MACHINE_ADDR   Machine_Address;             //�������ַ��Ϣ

//��̫��
volatile    u32                  LocalTime   = 0; 
volatile    u32 			     User_Name[16];
volatile    u32 			     User_Passord[16];
            ETHERNET_PARAMETER   Ethernet_Parameter;
//volatile    u32                  Flag_Login = 0;     
//volatile    u32                  Count_Login = 0;   
//volatile    u32                  Flag_RirstDisConnect = 0;          //��һ�ε��߱�־
volatile    u32                  Flag_ReadNote = 0;                 //��ȡ��־��־
volatile    u32                  Index_CurrentNote = 0;             //��ǰ��־����
//volatile    u32   				 Flag_Connect = 0;                  //��̫�����ӱ�־

////BATTON////
u8 Batton_Break_flag = 0;//����𻵱�־
u8 test_batton_flag = 0;//���Ե�غû�
////BATTON////

/////COMMUNICATION////
//u8 Communication_Flag = 0;
/////COMMUNICATION////

//////POWER-3G///////
u8 POWER_ERROR_Recorded = 0;
//////POWER-3G///////

u8 AD_Vol_Count[3*Channel_Max] = {0};//�����˲�����
u32 AD_Vol_Value[3*Channel_Max][AD_Vol_Sum_Num] = {0};//�����˲�����*/
u32  Status_Last_print = 0;
//for fliter
u32 Fliter_Count = 0;//�����㷨�м���ʱ��ı�����ÿ0.5S��һ
u32 Channel_Count[Channel_Max] = {0};//ÿ��ͨ���ļ�����

u8 First_Status_Flag = 0;//��һ�ε�״̬��ʶ����־λ0ʱ��Last_Stateû�б���ֵ��Ϊ1ʱ������ֵ

/***for Ethernet***/
u16 Test_Data = 0;
u8 Flag_ReInit_Eth = 0;

/***for 2400***/
u8 Data_Port[8] = {0};//�¶˿ڷ�������


u32 Status_Error = 0;
u32 Status_Error_time = 0;

u8 SelfDetect_Complete_Flag = 0;	//�Լ�״̬��ʶ
u8 Waite_StatusChange_flag_1 = 0;//�Ⱥ�״̬�仯��ʶ,�Լ춨λʹ��
u8 Waite_StatusChange_flag_2 = 0;
u8 Index_Status_next = 0;

u8 Alarm_Data = 0;//������־����A7Э��Ĺ涨�����
u8 Alarm_Data_Old = 0;//��¼�ϴα�����ʾ

/**********Զ�˷�����IP��PORT����*********/
//U8 rem_ip[4] = {192, 168, 1, 3};    //�����IP
//u16 port = 2046;                    //����˶˿ں�
//u8 company_ip[4] = {192, 168, 1, 3};//��˾������IP
//u16 company_port = 9000;            //��˾������PORT

// // U8 tcp_soc_server;              //�����TCP
// // u8 tcp_soc_client;              //�ͻ���TCP
////u8 tcp_soc_company_cli;         //�빫˾������ͨ�ŵĿͻ���TCP
u8 Company_Data_Flag = 0;       //����˾��ص������ݱ�־��0-������ 1-ͨ��״̬ (5�ֽ�)2-������Ϣ(4�ֽ�)3-����(4�ֽ�)
u8 status_collect_complete = 0; //ƽ��״̬�ɼ���ϱ�־
u8 collect_count = 0;           //������������5��ó�һ��ƽ��״̬
u32 collect_status[5];          //����״ֵ̬������ƽ��
u32 collect_value[3][3*Channel_Max];//��������ֵ     
u32 Last_State = 0;//��һʱ�̵�����
u32 Now_State = 0;//��ǰ�ɼ���״̬
u32 Now_AD_Value[3*Channel_Max];
u8 Work_State = 0;//��������״̬ 0-������ʼֵ 1-�Լ�״̬ 2- �������״̬ 3-ѧϰ״̬
u8 Host_Communication_flag = 0;//����λ��ͨ�ű�־ 0--��ͨѶ 1--����λ����������лظ� 2--�����ϱ����� (����������)3--����
u8 debug_type = 0;//��λʱ����ӡ�����Լ컹�Ǵ����ض�λ








//  by_lm  start

BIT_BAND_FLAG bit_band_flag_1;  //  λ����־λ������ 1
BIT_BAND_FLAG	bit_band_flag_tsk;	//���������б�־��λ��


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


u8 eth_send_data_flag = 0;      //��̫���������ݱ�־
u8 client_data_type= 0;         //�����ϱ��������� 1--���� 2-��������    //no_liming
u8 cli_connect_ser_flag = 0;    //�ͻ������ӷ�������־�����������ϱ�����
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

vu8 tcp_soc_server;              //�����TCP
vu8 tcp_soc_client;              //�ͻ���TCP
vu8 tcp_soc_myself;              //��˾�ĵ���TCP����

vu8 soc_state_lm;
vu8 wait_ack_lm;
vu8 server_wait_ack =__TRUE; 


__IO uint8_t  mac_adr[6] = {0x0,0x27,0xe,0xc9,0x87,0xb1};

__IO int8_t userName[17]	="admin";
__IO int8_t password[17]	="admin";


vu8 re_ch_pw_count 				=0;         // �޸�����  �����������

__IO uint8_t alarm_restart_hour			=0;
__IO uint8_t alarm_restart_minute		=0;

__IO float 			turn_num				=220.0;  	 	//  ��ѹ��������==�Ѹĳɱ�׼��ѹֵlxb
__IO uint16_t 	bat_min					=20;    		//  �󱸵�ر�����ֵ(����)
__IO uint16_t 	cur_min					=60;    		//  �źŵƵ����쳣������ֵ(%)
__IO uint16_t 	send_time				=30;    		//  �źŵƵ����ϱ�ʱ��(����)
__IO float 		turn_num_lxb			=2494.2;  	//  ��ѹ��������lxb

__IO uint8_t machine_addr[30]="����̫����";
__IO uint8_t machine_addr_len=30;

__IO int8_t machine_type[8]="ZL-D";//�źŻ��ź�
__IO uint8_t machine_type_len=8;

/****ETH Par******/
__IO uint8_t 	server_ip[4]			={11,1,1,11};          	//�����IP
// __IO uint8_t 	server_www[40]		={"www.baidu.com"};   			//���������
__IO uint16_t server_port				=2046;               				//�����PORT
__IO uint16_t local_port 				= 90;            						//���ط���˿ں�

// __IO uint8_t 	myself_server_ip[4]		={10,1,2,253};          	//��˾�����IP
// __IO uint16_t myself_server_port		=9000;               				//��˾�����PORT
__IO uint8_t 	myself_server_ip[4]		={0,0,0,0};          	//��˾�����IP
__IO uint16_t myself_server_port		=0;               				//��˾�����PORT



//=======================		default start	 =======================
__I  uint8_t 	r_server_ip[4]		={11,1,1,11};          	//�����IP
// __I  uint8_t 	r_server_www[40]	={"www.baidu.com"};   			//���������
__I  uint16_t r_server_port			=2046;               				//�����PORT
__I  uint16_t r_local_port 			= 90;            						//���ط���˿ں�

// __IO uint8_t 	r_myself_server_ip[4]		={10,1,2,253};          	//��˾�����IP
// __IO uint16_t r_myself_server_port		=9000;               				//��˾�����PORT
__IO uint8_t 	r_myself_server_ip[4]		={0,0,0,0};          	//��˾�����IP
__IO uint16_t r_myself_server_port		=0;               				//��˾�����PORT

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

__I  float 			r_turn_num			=220.0;  	 	//  ��ѹ��������
__I  uint16_t 	r_bat_min				=20;    		//  �󱸵�ر�����ֵ(����)
__I  uint16_t 	r_cur_min				=60;    		//  �źŵƵ����쳣������ֵ(mA)
__I  uint16_t 	r_send_time			=30;    		//  �źŵƵ����ϱ�ʱ��(����)

__IO float  		r_turn_num_lxb	= 2494.2;  	//  ��ѹ��������lxb


__I  int8_t r_machine_addr[30]="����̫����";
__IO uint8_t r_machine_addr_len=30;

__I  int8_t r_machine_type[8]="ZL-D";
__IO uint8_t r_machine_type_len=8;

__I  int8_t r_xjj_machine_type[8]="V4.0";
__I  int8_t xjj_machine_type[8]="V4.0";
//=======================		default end	 =======================

//   by_lm  end

//LXB DEFINE

__IO uint8_t 	last_log_type			=0xFF;    		//�ϴε�д��־���͡���ĿǰҪд���¼����ϴ�һ�£���д��־
__IO uint16_t ledlamp_err_count  = 0;					//	���ϼ��������й��ϣ���ֵ���ñ�������com����˸3min
__IO uint16_t eth_connect_count = 240;//2min
__IO uint16_t eth_restart_count = NET_STOP_TIME;//30min

STAGE_TIMING_TABLE_t stage_time_table[PHASE_CLASH_STATE_NUMBER] = {0};//�׶���ʱ��
__IO u8 current_stage_num = 0;//�׶���
__IO u16 standard_lamp_current[LAMP_NUMBER] = {0};	

/*��ϵ���Чͨ����������*/
// #define COMBINE_VALID_PASSAGE_NUM 6
__IO uint16_t combine_valid_passage[COMBINE_VALID_PASSAGE_NUM] = {0};

u8 period_time = 0;//��ǰ����ʱ��
u8 learn_count = 0;//ѧϰ����
//LXB DEFINE

u16 error_report_count = 0;//�����ϱ��������

/*
	0,ͨ����ͻ
	1,�ƹ���
	2,���������
	3,��λ��ͻ
	4,ѧϰ���β��ɹ�
	5,3G��Դ����
	6,220V��ѹ����
	7,�ŵ��쳣
*/
u16 error_report_time[ERROR_COUNT] = {0};

/*
	0,��ͻ
	1,�ƹ���
	2,���������
	3,��λ��ͻ
	4,ѧϰ���β��ɹ�
	5,3G��Դ����
	6,220V��ѹ����
	7,�ŵ��쳣
*/
u16 error_report_answer[ERROR_COUNT] = {1,1,1,1,1,1,1,1,1,1};//�����ϱ��ɹ�flag

u8   passage_clash_data[PASSAGE_ParaOutputList_MaxLine][4]={0};//��ͻ�����ϱ�
u8   passage_broken_data[PASSAGE_ParaOutputList_MaxLine][4]={0};//�ƹ����ϱ�

#endif
