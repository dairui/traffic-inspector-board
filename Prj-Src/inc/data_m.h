//FOR  Ѳ���
#ifndef __DATA_M__
#define __DATA_M__

//********************** �궨�� ��ʼ ********************
#define SUBOBJ_NUM_MAX   12

#define   SUB_TARGET_BUFSIZE   700
#define   TAB_OK_FLAG  0x01
#define   LINE_OK_FLAG 0x01


#define   MES_IPI   			0x21
#define   MES_OPTION_MIN  0x80

#define   ID_MIN          0x81
#define   ID_MAX          0xD0   //Ŀǰ�������Ϊ0xD0�����ʶ

#define   RIGHT   			0x01
#define   WRONG   			0x00

#define  TAB_92_S_NUMBER      64

//********************** �궨�� ���� ********************

typedef signed char     s8;
typedef unsigned char   u8;
typedef short           s16;
typedef unsigned short  u16;
typedef int             s32;
typedef unsigned int    u32;
typedef unsigned int    BOOL;

 typedef enum
{
	ERROR_NO = 0,
	ERROR_TOOLONG,   	//1
	ERROR_TYPE,				//2
	ERROR_EXCEED,			//3
	ERROR_TOOSHORT,		//4
	ERROR_OTHER,			//5
	ERROR_NONE,
	ERROR_IGNORE
}ERROR_TYPE_E;


//********************** �ṹ�嶨�� ��ʼ ********************
//Э���������  �ṹ��
typedef struct Target_Data_Db
{
     u8   ID;                           //���������ʶ 
     u8   count_index;                  //������������ 
     u8   index[3];                     //����
     u8   sub_target;                   //�Ӷ��� 
     u8   sub_target_type;              //�Ӷ���������� 
     u8   sub_buf[SUB_TARGET_BUFSIZE];  //��������
		 u32  data_count;
	
}TARGET_DATA_DB;

//Э�����ݽṹ��
typedef struct Protocol_Data
{
     u8           count_target;              //���������� 
     u8           operation_type;            //�������� 
     TARGET_DATA_DB  Taget_dat[8];       //������������ 
}PROTOCOL_DATA_DB;

//fifo �ṹ��

typedef struct ee_data
{
			u32 leng;          //���ݳ���
//			u32 ee_add;        //������eerom��ʼ��ַ
			u32 Rx_fifo_add;      //������fifo��ʼ��ַ
}EE_DATA;

/* ������̫��  */
 typedef struct
{
	u8 			data[2600];   //  ����,������̫��������
	u32 		lenth;		//  ������̫�������ݵĳ���
	BOOL    error;	
	ERROR_TYPE_E  error_type;
}TO_ETH_STRUCT;



//********************** �ṹ�嶨�� ���� ********************


//======================= �������� ��ʼ =========================
extern u32 Write_Taget_to_Eerom(TARGET_DATA_DB * p_Taget_set);
extern u32 Read_Eerom_to_Taget(TARGET_DATA_DB * p_Taget_set);

//======================= �������� ���� =========================


//================================================
//================================================

/*  ���ֽ��� �궨�� xxxxxx */
#define LINE_81_BYTES 				0 //2
#define	LINE_82_BYTES					0 //1
#define	LINE_83_BYTES					0 //2
#define	LINE_84_BYTES					0 //2
#define	LINE_85_BYTES					0 ////102
#define	LINE_86_BYTES					0 ////4
#define	LINE_87_BYTES					0 //4
#define	LINE_88_BYTES					0 ////4
#define	LINE_89_BYTES					0 //1
#define	LINE_8A_BYTES					0 //1
#define	LINE_8B_BYTES					0 //1
#define	LINE_8C_BYTES					0 //1
#define	LINE_8D_BYTES					0 ////9
#define	LINE_8E_BYTES					0 ////8
#define	LINE_8F_BYTES					0 //1
#define	LINE_90_BYTES					0 //1
#define	LINE_91_BYTES					0 //8
#define	LINE_92_BYTES					10
#define	LINE_93_BYTES					0 //1
#define	LINE_94_BYTES					0 //1
#define	LINE_95_BYTES					0 //12
#define	LINE_96_BYTES					0 ////4
#define	LINE_97_BYTES					0 ////3
#define	LINE_98_BYTES					0 //1
#define	LINE_99_BYTES					0 //1
#define	LINE_9A_BYTES					0 //1
#define	LINE_9B_BYTES					0 //1
#define	LINE_9C_BYTES					0 //1
#define	LINE_9D_BYTES					0 //1
#define	LINE_9E_BYTES					0 //1
#define	LINE_9F_BYTES					0 //9
#define	LINE_A0_BYTES					0 //3
#define	LINE_A1_BYTES					0 //7
#define	LINE_A2_BYTES					0 //3
#define	LINE_A3_BYTES					0 //1
#define	LINE_A4_BYTES					0 //1
#define	LINE_A5_BYTES					0 //1
#define	LINE_A6_BYTES					0 //1
#define	LINE_A7_BYTES					0 ////1
#define	LINE_A8_BYTES					0 //1
#define	LINE_A9_BYTES					0 //1
#define	LINE_AA_BYTES					0 ////1
#define	LINE_AB_BYTES					0 //1
#define	LINE_AC_BYTES					0 //4
#define	LINE_AD_BYTES					0 //4
#define	LINE_AE_BYTES					0 //1
#define	LINE_AF_BYTES					0 //1
#define	LINE_B0_BYTES					0 ////4
#define	LINE_B1_BYTES					0 ////4
#define	LINE_B2_BYTES					0 //1
#define	LINE_B3_BYTES					0 //1
#define	LINE_B4_BYTES					0 //1
#define	LINE_B5_BYTES					0 //1
#define	LINE_B6_BYTES					0 //1
#define	LINE_B7_BYTES					0 //1
#define	LINE_B8_BYTES					0 //1
#define	LINE_B9_BYTES					0 //1
#define	LINE_BA_BYTES					0 //1
#define	LINE_BB_BYTES					0 //1
#define	LINE_BC_BYTES					0 //1
#define	LINE_BD_BYTES					0 //14
#define	LINE_BE_BYTES					0 //16
#define	LINE_BF_BYTES					0 //16
#define	LINE_C0_BYTES					0 ////5
#define	LINE_C1_BYTES					0 ////8
#define	LINE_C2_BYTES					0 //1
#define	LINE_C3_BYTES					0 //1
#define	LINE_C4_BYTES					0 ////2
#define	LINE_C5_BYTES					0 //1
#define	LINE_C6_BYTES					0 //1
#define	LINE_C7_BYTES					0 //1
#define	LINE_C8_BYTES					0 //39
#define	LINE_C9_BYTES					0 //4
#define	LINE_CA_BYTES					0 ////5
#define	LINE_CB_BYTES					0 ////3
#define	LINE_CC_BYTES					0 //7
#define	LINE_CD_BYTES					0 ////17
#define	LINE_CE_BYTES					0 ////7
#define	LINE_CF_BYTES					0 //9
#define	LINE_D0_BYTES					0 //71

/*  �������� �궨�� xxxxxx */
#define TAB_81_LINES 					0 //1
#define	TAB_82_LINES					0 //1
#define	TAB_83_LINES					0 //1
#define	TAB_84_LINES					0 //1
#define	TAB_85_LINES					0 //16
#define	TAB_86_LINES					0 //1
#define	TAB_87_LINES					0 //1
#define	TAB_88_LINES					0 //1
#define	TAB_89_LINES					0 //1
#define	TAB_8A_LINES					0 //1
#define	TAB_8B_LINES					0 //1
#define	TAB_8C_LINES					0 //1
#define	TAB_8D_LINES					0 //40
#define	TAB_8E_LINES					0 //768
#define	TAB_8F_LINES					0 //1
#define	TAB_90_LINES					0 //1
#define	TAB_91_LINES					0 //64
#define	TAB_92_LINES					(TAB_92_S_NUMBER*6)//16320 //255 or 16320  ???????
#define	TAB_93_LINES					0 //1
#define	TAB_94_LINES					0 //1
#define	TAB_95_LINES					0 //16
#define	TAB_96_LINES					0 //4
#define	TAB_97_LINES					0 //32 //32 or 16   ??
#define	TAB_98_LINES					0 //1
#define	TAB_99_LINES					0 //1
#define	TAB_9A_LINES					0 //1
#define	TAB_9B_LINES					0 //1
#define	TAB_9C_LINES					0 //1
#define	TAB_9D_LINES					0 //1
#define	TAB_9E_LINES					0 //1
#define	TAB_9F_LINES					0 //48
#define	TAB_A0_LINES					0 //8
#define	TAB_A1_LINES					0 //48
#define	TAB_A2_LINES					0 //48
#define	TAB_A3_LINES					0 //1
#define	TAB_A4_LINES					0 //1
#define	TAB_A5_LINES					0 //1
#define	TAB_A6_LINES					0 //1
#define	TAB_A7_LINES					0 //1
#define	TAB_A8_LINES					0 //1
#define	TAB_A9_LINES					0 //1
#define	TAB_AA_LINES					0 //1
#define	TAB_AB_LINES					0 //1
#define	TAB_AC_LINES					0 //1
#define	TAB_AD_LINES					0 //1
#define	TAB_AE_LINES					0 //1
#define	TAB_AF_LINES					0 //1
#define	TAB_B0_LINES					0 //16
#define	TAB_B1_LINES					0 //2
#define	TAB_B2_LINES					0 //1
#define	TAB_B3_LINES					0 //1
#define	TAB_B4_LINES					0 //1
#define	TAB_B5_LINES					0 //1
#define	TAB_B6_LINES					0 //1
#define	TAB_B7_LINES					0 //1
#define	TAB_B8_LINES					0 //1
#define	TAB_B9_LINES					0 //1
#define	TAB_BA_LINES					0 //1
#define	TAB_BB_LINES					0 //1
#define	TAB_BC_LINES					0 //1
#define	TAB_BD_LINES					0 //1
#define	TAB_BE_LINES					0 //1
#define	TAB_BF_LINES					0 //1
#define	TAB_C0_LINES					0 //32
#define	TAB_C1_LINES					0 //256
#define	TAB_C2_LINES					0 //1
#define	TAB_C3_LINES					0 //1
#define	TAB_C4_LINES					0 //1
#define	TAB_C5_LINES					0 //1
#define	TAB_C6_LINES					0 //1
#define	TAB_C7_LINES					0 //1
#define	TAB_C8_LINES					0 //8
#define	TAB_C9_LINES					0 //1
#define	TAB_CA_LINES					0 //64
#define	TAB_CB_LINES					0 //64
#define	TAB_CC_LINES					0 //16
#define	TAB_CD_LINES					0 //1
#define	TAB_CE_LINES					0 //1
#define	TAB_CF_LINES					0 //1
#define	TAB_D0_LINES					0 //17

/*  �����ֽ��� �궨�� xxxxxx */
#define TAB_81_BYTES 				0//	((TAB_81_LINES) * (LINE_81_BYTES))
#define	TAB_82_BYTES				0//	((TAB_82_LINES) * (LINE_82_BYTES))
#define	TAB_83_BYTES				0//	((TAB_83_LINES) * (LINE_83_BYTES))
#define	TAB_84_BYTES				0//	((TAB_84_LINES) * (LINE_84_BYTES))
#define	TAB_85_BYTES				0//	((TAB_85_LINES) * (LINE_85_BYTES) + 1)
#define	TAB_86_BYTES				0//	((TAB_86_LINES) * (LINE_86_BYTES))
#define	TAB_87_BYTES				0//	((TAB_87_LINES) * (LINE_87_BYTES))
#define	TAB_88_BYTES				0//	((TAB_88_LINES) * (LINE_88_BYTES))
#define	TAB_89_BYTES				0//	((TAB_89_LINES) * (LINE_89_BYTES))
#define	TAB_8A_BYTES				0//	((TAB_8A_LINES) * (LINE_8A_BYTES))
#define	TAB_8B_BYTES				0//	((TAB_8B_LINES) * (LINE_8B_BYTES))
#define	TAB_8C_BYTES				0//	((TAB_8C_LINES) * (LINE_8C_BYTES))
#define	TAB_8D_BYTES				0//	((TAB_8D_LINES) * (LINE_8D_BYTES) + 1)
#define	TAB_8E_BYTES				0//	((TAB_8E_LINES) * (LINE_8E_BYTES) + 2)
#define	TAB_8F_BYTES				0//	((TAB_8F_LINES) * (LINE_8F_BYTES))
#define	TAB_90_BYTES				0//	((TAB_90_LINES) * (LINE_90_BYTES))
#define	TAB_91_BYTES				0//	((TAB_91_LINES) * (LINE_91_BYTES))
#define	TAB_92_BYTES				((TAB_92_LINES) * (LINE_92_BYTES) + 1)
#define	TAB_93_BYTES				0//	((TAB_93_LINES) * (LINE_93_BYTES))
#define	TAB_94_BYTES				0//	((TAB_94_LINES) * (LINE_94_BYTES))
#define	TAB_95_BYTES				0//	((TAB_95_LINES) * (LINE_95_BYTES) + 1)
#define	TAB_96_BYTES				0//	((TAB_96_LINES) * (LINE_96_BYTES) + 1)
#define	TAB_97_BYTES				0//	((TAB_97_LINES) * (LINE_97_BYTES) + 1)
#define	TAB_98_BYTES				0//	((TAB_98_LINES) * (LINE_98_BYTES))
#define	TAB_99_BYTES				0//	((TAB_99_LINES) * (LINE_99_BYTES))
#define	TAB_9A_BYTES				0//	((TAB_9A_LINES) * (LINE_9A_BYTES))
#define	TAB_9B_BYTES				0//	((TAB_9B_LINES) * (LINE_9B_BYTES))
#define	TAB_9C_BYTES				0//	((TAB_9C_LINES) * (LINE_9C_BYTES))
#define	TAB_9D_BYTES				0//	((TAB_9D_LINES) * (LINE_9D_BYTES))
#define	TAB_9E_BYTES				0//	((TAB_9E_LINES) * (LINE_9E_BYTES))
#define	TAB_9F_BYTES				0//	((TAB_9F_LINES) * (LINE_9F_BYTES) + 1)
#define	TAB_A0_BYTES				0//	((TAB_A0_LINES) * (LINE_A0_BYTES) + 1)
#define	TAB_A1_BYTES				0//	((TAB_A1_LINES) * (LINE_A1_BYTES) + 1)
#define	TAB_A2_BYTES				0//	((TAB_A2_LINES) * (LINE_A2_BYTES) + 1)
#define	TAB_A3_BYTES				0//	((TAB_A3_LINES) * (LINE_A3_BYTES))
#define	TAB_A4_BYTES				0//	((TAB_A4_LINES) * (LINE_A4_BYTES))
#define	TAB_A5_BYTES				0//	((TAB_A5_LINES) * (LINE_A5_BYTES))
#define	TAB_A6_BYTES				0//	((TAB_A6_LINES) * (LINE_A6_BYTES))
#define	TAB_A7_BYTES				0//	((TAB_A7_LINES) * (LINE_A7_BYTES))
#define	TAB_A8_BYTES				0//	((TAB_A8_LINES) * (LINE_A8_BYTES))
#define	TAB_A9_BYTES				0//	((TAB_A9_LINES) * (LINE_A9_BYTES))
#define	TAB_AA_BYTES				0//	((TAB_AA_LINES) * (LINE_AA_BYTES))
#define	TAB_AB_BYTES				0//	((TAB_AB_LINES) * (LINE_AB_BYTES))
#define	TAB_AC_BYTES				0//	((TAB_AC_LINES) * (LINE_AC_BYTES))
#define	TAB_AD_BYTES				0//	((TAB_AD_LINES) * (LINE_AD_BYTES))
#define	TAB_AE_BYTES				0//	((TAB_AE_LINES) * (LINE_AE_BYTES))
#define	TAB_AF_BYTES				0//	((TAB_AF_LINES) * (LINE_AF_BYTES))
#define	TAB_B0_BYTES				0//	((TAB_B0_LINES) * (LINE_B0_BYTES) + 1)
#define	TAB_B1_BYTES				0//	((TAB_B1_LINES) * (LINE_B1_BYTES) + 1)
#define	TAB_B2_BYTES				0//	((TAB_B2_LINES) * (LINE_B2_BYTES))
#define	TAB_B3_BYTES				0//	((TAB_B3_LINES) * (LINE_B3_BYTES))
#define	TAB_B4_BYTES				0//	((TAB_B4_LINES) * (LINE_B4_BYTES))
#define	TAB_B5_BYTES				0//	((TAB_B5_LINES) * (LINE_B5_BYTES))
#define	TAB_B6_BYTES				0//	((TAB_B6_LINES) * (LINE_B6_BYTES))
#define	TAB_B7_BYTES				0//	((TAB_B7_LINES) * (LINE_B7_BYTES))
#define	TAB_B8_BYTES				0//	((TAB_B8_LINES) * (LINE_B8_BYTES))
#define	TAB_B9_BYTES				0//	((TAB_B9_LINES) * (LINE_B9_BYTES))
#define	TAB_BA_BYTES				0//	((TAB_BA_LINES) * (LINE_BA_BYTES))
#define	TAB_BB_BYTES				0//	((TAB_BB_LINES) * (LINE_BB_BYTES))
#define	TAB_BC_BYTES				0//	((TAB_BC_LINES) * (LINE_BC_BYTES))
#define	TAB_BD_BYTES				0//	((TAB_BD_LINES) * (LINE_BD_BYTES))
#define	TAB_BE_BYTES				0//	((TAB_BE_LINES) * (LINE_BE_BYTES))
#define	TAB_BF_BYTES				0//	((TAB_BF_LINES) * (LINE_BF_BYTES))
#define	TAB_C0_BYTES				0//	((TAB_C0_LINES) * (LINE_C0_BYTES) + 1)
#define	TAB_C1_BYTES				0//	((TAB_C1_LINES) * (LINE_C1_BYTES) + 2)
#define	TAB_C2_BYTES				0//	((TAB_C2_LINES) * (LINE_C2_BYTES))
#define	TAB_C3_BYTES				0//	((TAB_C3_LINES) * (LINE_C3_BYTES))
#define	TAB_C4_BYTES				0//	((TAB_C4_LINES) * (LINE_C4_BYTES))
#define	TAB_C5_BYTES				0//	((TAB_C5_LINES) * (LINE_C5_BYTES))
#define	TAB_C6_BYTES				0//	((TAB_C6_LINES) * (LINE_C6_BYTES))
#define	TAB_C7_BYTES				0//	((TAB_C7_LINES) * (LINE_C7_BYTES))
#define	TAB_C8_BYTES				0//	((TAB_C8_LINES) * (LINE_C8_BYTES) + 1)
#define	TAB_C9_BYTES				0//	((TAB_C9_LINES) * (LINE_C9_BYTES) + 1)
#define	TAB_CA_BYTES				0//	((TAB_CA_LINES) * (LINE_CA_BYTES) + 2)
#define	TAB_CB_BYTES				0//	((TAB_CB_LINES) * (LINE_CB_BYTES) + 1)
#define	TAB_CC_BYTES				0//	((TAB_CC_LINES) * (LINE_CC_BYTES) + 1)
#define	TAB_CD_BYTES				0//	((TAB_CD_LINES) * (LINE_CD_BYTES))
#define	TAB_CE_BYTES				0//	((TAB_CE_LINES) * (LINE_CE_BYTES))
#define	TAB_CF_BYTES				0//	((TAB_CF_LINES) * (LINE_CF_BYTES))
#define	TAB_D0_BYTES				0//	((TAB_D0_LINES) * (LINE_D0_BYTES))



//============================================================================
//============================================================================
//��־����
/* �����ʶ�� ��Ч��־  ��ַ�궨�� xxxxxx */
#define OBJ_FLAG_ADDR_START 			((u32)NOTE_START)	//((u32)0x0200)												/* ��־����ʼλ��     */
#define OBJ_FLAG_ADDR_SET_FLAG		OBJ_FLAG_ADDR_START									/* ��ַ��������־λ */
#define TAB_81_FLAG_ADDR 					((u32)OBJ_FLAG_ADDR_SET_FLAG + 4)
#define	TAB_82_FLAG_ADDR					((u32)TAB_81_FLAG_ADDR + TAB_81_LINES + 1)
#define	TAB_83_FLAG_ADDR					((u32)TAB_82_FLAG_ADDR + TAB_82_LINES + 1)
#define	TAB_84_FLAG_ADDR					((u32)TAB_83_FLAG_ADDR + TAB_83_LINES + 1)
#define	TAB_85_FLAG_ADDR					((u32)TAB_84_FLAG_ADDR + TAB_84_LINES + 1)
#define	TAB_86_FLAG_ADDR					((u32)TAB_85_FLAG_ADDR + TAB_85_LINES + 1)
#define	TAB_87_FLAG_ADDR					((u32)TAB_86_FLAG_ADDR + TAB_86_LINES + 1)
#define	TAB_88_FLAG_ADDR					((u32)TAB_87_FLAG_ADDR + TAB_87_LINES + 1)
#define	TAB_89_FLAG_ADDR					((u32)TAB_88_FLAG_ADDR + TAB_88_LINES + 1)
#define	TAB_8A_FLAG_ADDR					((u32)TAB_89_FLAG_ADDR + TAB_89_LINES + 1)
#define	TAB_8B_FLAG_ADDR					((u32)TAB_8A_FLAG_ADDR + TAB_8A_LINES + 1)
#define	TAB_8C_FLAG_ADDR					((u32)TAB_8B_FLAG_ADDR + TAB_8B_LINES + 1)
#define	TAB_8D_FLAG_ADDR					((u32)TAB_8C_FLAG_ADDR + TAB_8C_LINES + 1)
#define	TAB_8E_FLAG_ADDR					((u32)TAB_8D_FLAG_ADDR + TAB_8D_LINES + 1)
#define	TAB_8F_FLAG_ADDR					((u32)TAB_8E_FLAG_ADDR + TAB_8E_LINES + 1)
#define	TAB_90_FLAG_ADDR					((u32)TAB_8F_FLAG_ADDR + TAB_8F_LINES + 1)
#define	TAB_91_FLAG_ADDR					((u32)TAB_90_FLAG_ADDR + TAB_90_LINES + 1)
#define	TAB_92_FLAG_ADDR					((u32)TAB_91_FLAG_ADDR + TAB_91_LINES + 1)
#define	TAB_93_FLAG_ADDR					((u32)TAB_92_FLAG_ADDR + TAB_92_LINES + 1)
#define	TAB_94_FLAG_ADDR					((u32)TAB_93_FLAG_ADDR + TAB_93_LINES + 1)
#define	TAB_95_FLAG_ADDR					((u32)TAB_94_FLAG_ADDR + TAB_94_LINES + 1)
#define	TAB_96_FLAG_ADDR					((u32)TAB_95_FLAG_ADDR + TAB_95_LINES + 1)
#define	TAB_97_FLAG_ADDR					((u32)TAB_96_FLAG_ADDR + TAB_96_LINES + 1)
#define	TAB_98_FLAG_ADDR					((u32)TAB_97_FLAG_ADDR + TAB_97_LINES + 1)
#define	TAB_99_FLAG_ADDR					((u32)TAB_98_FLAG_ADDR + TAB_98_LINES + 1)
#define	TAB_9A_FLAG_ADDR					((u32)TAB_99_FLAG_ADDR + TAB_99_LINES + 1)
#define	TAB_9B_FLAG_ADDR					((u32)TAB_9A_FLAG_ADDR + TAB_9A_LINES + 1)
#define	TAB_9C_FLAG_ADDR					((u32)TAB_9B_FLAG_ADDR + TAB_9B_LINES + 1)
#define	TAB_9D_FLAG_ADDR					((u32)TAB_9C_FLAG_ADDR + TAB_9C_LINES + 1)
#define	TAB_9E_FLAG_ADDR					((u32)TAB_9D_FLAG_ADDR + TAB_9D_LINES + 1)
#define	TAB_9F_FLAG_ADDR					((u32)TAB_9E_FLAG_ADDR + TAB_9E_LINES + 1)
#define	TAB_A0_FLAG_ADDR					((u32)TAB_9F_FLAG_ADDR + TAB_9F_LINES + 1)
#define	TAB_A1_FLAG_ADDR					((u32)TAB_A0_FLAG_ADDR + TAB_A0_LINES + 1)
#define	TAB_A2_FLAG_ADDR					((u32)TAB_A1_FLAG_ADDR + TAB_A1_LINES + 1)
#define	TAB_A3_FLAG_ADDR					((u32)TAB_A2_FLAG_ADDR + TAB_A2_LINES + 1)
#define	TAB_A4_FLAG_ADDR					((u32)TAB_A3_FLAG_ADDR + TAB_A3_LINES + 1)
#define	TAB_A5_FLAG_ADDR					((u32)TAB_A4_FLAG_ADDR + TAB_A4_LINES + 1)
#define	TAB_A6_FLAG_ADDR					((u32)TAB_A5_FLAG_ADDR + TAB_A5_LINES + 1)
#define	TAB_A7_FLAG_ADDR					((u32)TAB_A6_FLAG_ADDR + TAB_A6_LINES + 1)
#define	TAB_A8_FLAG_ADDR					((u32)TAB_A7_FLAG_ADDR + TAB_A7_LINES + 1)
#define	TAB_A9_FLAG_ADDR					((u32)TAB_A8_FLAG_ADDR + TAB_A8_LINES + 1)
#define	TAB_AA_FLAG_ADDR					((u32)TAB_A9_FLAG_ADDR + TAB_A9_LINES + 1)
#define	TAB_AB_FLAG_ADDR					((u32)TAB_AA_FLAG_ADDR + TAB_AA_LINES + 1)
#define	TAB_AC_FLAG_ADDR					((u32)TAB_AB_FLAG_ADDR + TAB_AB_LINES + 1)
#define	TAB_AD_FLAG_ADDR					((u32)TAB_AC_FLAG_ADDR + TAB_AC_LINES + 1)
#define	TAB_AE_FLAG_ADDR					((u32)TAB_AD_FLAG_ADDR + TAB_AD_LINES + 1)
#define	TAB_AF_FLAG_ADDR					((u32)TAB_AE_FLAG_ADDR + TAB_AE_LINES + 1)
#define	TAB_B0_FLAG_ADDR					((u32)TAB_AF_FLAG_ADDR + TAB_AF_LINES + 1)
#define	TAB_B1_FLAG_ADDR					((u32)TAB_B0_FLAG_ADDR + TAB_B0_LINES + 1)
#define	TAB_B2_FLAG_ADDR					((u32)TAB_B1_FLAG_ADDR + TAB_B1_LINES + 1)
#define	TAB_B3_FLAG_ADDR					((u32)TAB_B2_FLAG_ADDR + TAB_B2_LINES + 1)
#define	TAB_B4_FLAG_ADDR					((u32)TAB_B3_FLAG_ADDR + TAB_B3_LINES + 1)
#define	TAB_B5_FLAG_ADDR					((u32)TAB_B4_FLAG_ADDR + TAB_B4_LINES + 1)
#define	TAB_B6_FLAG_ADDR					((u32)TAB_B5_FLAG_ADDR + TAB_B5_LINES + 1)
#define	TAB_B7_FLAG_ADDR					((u32)TAB_B6_FLAG_ADDR + TAB_B6_LINES + 1)
#define	TAB_B8_FLAG_ADDR					((u32)TAB_B7_FLAG_ADDR + TAB_B7_LINES + 1)
#define	TAB_B9_FLAG_ADDR					((u32)TAB_B8_FLAG_ADDR + TAB_B8_LINES + 1)
#define	TAB_BA_FLAG_ADDR					((u32)TAB_B9_FLAG_ADDR + TAB_B9_LINES + 1)
#define	TAB_BB_FLAG_ADDR					((u32)TAB_BA_FLAG_ADDR + TAB_BA_LINES + 1)
#define	TAB_BC_FLAG_ADDR					((u32)TAB_BB_FLAG_ADDR + TAB_BB_LINES + 1)
#define	TAB_BD_FLAG_ADDR					((u32)TAB_BC_FLAG_ADDR + TAB_BC_LINES + 1)
#define	TAB_BE_FLAG_ADDR					((u32)TAB_BD_FLAG_ADDR + TAB_BD_LINES + 1)
#define	TAB_BF_FLAG_ADDR					((u32)TAB_BE_FLAG_ADDR + TAB_BE_LINES + 1)
#define	TAB_C0_FLAG_ADDR					((u32)TAB_BF_FLAG_ADDR + TAB_BF_LINES + 1)
#define	TAB_C1_FLAG_ADDR					((u32)TAB_C0_FLAG_ADDR + TAB_C0_LINES + 1)
#define	TAB_C2_FLAG_ADDR					((u32)TAB_C1_FLAG_ADDR + TAB_C1_LINES + 1)
#define	TAB_C3_FLAG_ADDR					((u32)TAB_C2_FLAG_ADDR + TAB_C2_LINES + 1)
#define	TAB_C4_FLAG_ADDR					((u32)TAB_C3_FLAG_ADDR + TAB_C3_LINES + 1)
#define	TAB_C5_FLAG_ADDR					((u32)TAB_C4_FLAG_ADDR + TAB_C4_LINES + 1)
#define	TAB_C6_FLAG_ADDR					((u32)TAB_C5_FLAG_ADDR + TAB_C5_LINES + 1)
#define	TAB_C7_FLAG_ADDR					((u32)TAB_C6_FLAG_ADDR + TAB_C6_LINES + 1)
#define	TAB_C8_FLAG_ADDR					((u32)TAB_C7_FLAG_ADDR + TAB_C7_LINES + 1)
#define	TAB_C9_FLAG_ADDR					((u32)TAB_C8_FLAG_ADDR + TAB_C8_LINES + 1)
#define	TAB_CA_FLAG_ADDR					((u32)TAB_C9_FLAG_ADDR + TAB_C9_LINES + 1)
#define	TAB_CB_FLAG_ADDR					((u32)TAB_CA_FLAG_ADDR + TAB_CA_LINES + 1)
#define	TAB_CC_FLAG_ADDR					((u32)TAB_CB_FLAG_ADDR + TAB_CB_LINES + 1)
#define	TAB_CD_FLAG_ADDR					((u32)TAB_CC_FLAG_ADDR + TAB_CC_LINES + 1)
#define	TAB_CE_FLAG_ADDR					((u32)TAB_CD_FLAG_ADDR + TAB_CD_LINES + 1)
#define	TAB_CF_FLAG_ADDR					((u32)TAB_CE_FLAG_ADDR + TAB_CE_LINES + 1)
#define	TAB_D0_FLAG_ADDR					((u32)TAB_CF_FLAG_ADDR + TAB_CF_LINES + 1)
#define OBJ_FLAG_ADDR_END 				((u32)TAB_D0_FLAG_ADDR + TAB_D0_LINES + 1)


//================================================
//���ݱ���
/* �����ʶ ���� ��ַ�궨�� xxxxxx */
#define OBJ_ADDR_START 			((u32)0x0000)												/* ���ݱ���ʼλ��     */
#define OBJ_ADDR_SET_FLAG		(OBJ_ADDR_START + OBJ_FLAG_ADDR_END	)	/* ��ַ��������־λ */
#define TAB_81_ADDR 				((u32)OBJ_ADDR_SET_FLAG + 4)
#define	TAB_82_ADDR					((u32)TAB_81_ADDR + TAB_81_BYTES)
#define	TAB_83_ADDR					((u32)TAB_82_ADDR + TAB_82_BYTES)
#define	TAB_84_ADDR					((u32)TAB_83_ADDR + TAB_83_BYTES)
#define	TAB_85_ADDR					((u32)TAB_84_ADDR + TAB_84_BYTES)
#define	TAB_86_ADDR					((u32)TAB_85_ADDR + TAB_85_BYTES)
#define	TAB_87_ADDR					((u32)TAB_86_ADDR + TAB_86_BYTES)
#define	TAB_88_ADDR					((u32)TAB_87_ADDR + TAB_87_BYTES)
#define	TAB_89_ADDR					((u32)TAB_88_ADDR + TAB_88_BYTES)
#define	TAB_8A_ADDR					((u32)TAB_89_ADDR + TAB_89_BYTES)
#define	TAB_8B_ADDR					((u32)TAB_8A_ADDR + TAB_8A_BYTES)
#define	TAB_8C_ADDR					((u32)TAB_8B_ADDR + TAB_8B_BYTES)
#define	TAB_8D_ADDR					((u32)TAB_8C_ADDR + TAB_8C_BYTES)
#define	TAB_8E_ADDR					((u32)TAB_8D_ADDR + TAB_8D_BYTES)
#define	TAB_8F_ADDR					((u32)TAB_8E_ADDR + TAB_8E_BYTES)
#define	TAB_90_ADDR					((u32)TAB_8F_ADDR + TAB_8F_BYTES)
#define	TAB_91_ADDR					((u32)TAB_90_ADDR + TAB_90_BYTES)
#define	TAB_92_ADDR					((u32)TAB_91_ADDR + TAB_91_BYTES)
#define	TAB_93_ADDR					((u32)TAB_92_ADDR + TAB_92_BYTES)
#define	TAB_94_ADDR					((u32)TAB_93_ADDR + TAB_93_BYTES)
#define	TAB_95_ADDR					((u32)TAB_94_ADDR + TAB_94_BYTES)
#define	TAB_96_ADDR					((u32)TAB_95_ADDR + TAB_95_BYTES)
#define	TAB_97_ADDR					((u32)TAB_96_ADDR + TAB_96_BYTES)
#define	TAB_98_ADDR					((u32)TAB_97_ADDR + TAB_97_BYTES)
#define	TAB_99_ADDR					((u32)TAB_98_ADDR + TAB_98_BYTES)
#define	TAB_9A_ADDR					((u32)TAB_99_ADDR + TAB_99_BYTES)
#define	TAB_9B_ADDR					((u32)TAB_9A_ADDR + TAB_9A_BYTES)
#define	TAB_9C_ADDR					((u32)TAB_9B_ADDR + TAB_9B_BYTES)
#define	TAB_9D_ADDR					((u32)TAB_9C_ADDR + TAB_9C_BYTES)
#define	TAB_9E_ADDR					((u32)TAB_9D_ADDR + TAB_9D_BYTES)
#define	TAB_9F_ADDR					((u32)TAB_9E_ADDR + TAB_9E_BYTES)
#define	TAB_A0_ADDR					((u32)TAB_9F_ADDR + TAB_9F_BYTES)
#define	TAB_A1_ADDR					((u32)TAB_A0_ADDR + TAB_A0_BYTES)
#define	TAB_A2_ADDR					((u32)TAB_A1_ADDR + TAB_A1_BYTES)
#define	TAB_A3_ADDR					((u32)TAB_A2_ADDR + TAB_A2_BYTES)
#define	TAB_A4_ADDR					((u32)TAB_A3_ADDR + TAB_A3_BYTES)
#define	TAB_A5_ADDR					((u32)TAB_A4_ADDR + TAB_A4_BYTES)
#define	TAB_A6_ADDR					((u32)TAB_A5_ADDR + TAB_A5_BYTES)
#define	TAB_A7_ADDR					((u32)TAB_A6_ADDR + TAB_A6_BYTES)
#define	TAB_A8_ADDR					((u32)TAB_A7_ADDR + TAB_A7_BYTES)
#define	TAB_A9_ADDR					((u32)TAB_A8_ADDR + TAB_A8_BYTES)
#define	TAB_AA_ADDR					((u32)TAB_A9_ADDR + TAB_A9_BYTES)
#define	TAB_AB_ADDR					((u32)TAB_AA_ADDR + TAB_AA_BYTES)
#define	TAB_AC_ADDR					((u32)TAB_AB_ADDR + TAB_AB_BYTES)
#define	TAB_AD_ADDR					((u32)TAB_AC_ADDR + TAB_AC_BYTES)
#define	TAB_AE_ADDR					((u32)TAB_AD_ADDR + TAB_AD_BYTES)
#define	TAB_AF_ADDR					((u32)TAB_AE_ADDR + TAB_AE_BYTES)
#define	TAB_B0_ADDR					((u32)TAB_AF_ADDR + TAB_AF_BYTES)
#define	TAB_B1_ADDR					((u32)TAB_B0_ADDR + TAB_B0_BYTES)
#define	TAB_B2_ADDR					((u32)TAB_B1_ADDR + TAB_B1_BYTES)
#define	TAB_B3_ADDR					((u32)TAB_B2_ADDR + TAB_B2_BYTES)
#define	TAB_B4_ADDR					((u32)TAB_B3_ADDR + TAB_B3_BYTES)
#define	TAB_B5_ADDR					((u32)TAB_B4_ADDR + TAB_B4_BYTES)
#define	TAB_B6_ADDR					((u32)TAB_B5_ADDR + TAB_B5_BYTES)
#define	TAB_B7_ADDR					((u32)TAB_B6_ADDR + TAB_B6_BYTES)
#define	TAB_B8_ADDR					((u32)TAB_B7_ADDR + TAB_B7_BYTES)
#define	TAB_B9_ADDR					((u32)TAB_B8_ADDR + TAB_B8_BYTES)
#define	TAB_BA_ADDR					((u32)TAB_B9_ADDR + TAB_B9_BYTES)
#define	TAB_BB_ADDR					((u32)TAB_BA_ADDR + TAB_BA_BYTES)
#define	TAB_BC_ADDR					((u32)TAB_BB_ADDR + TAB_BB_BYTES)
#define	TAB_BD_ADDR					((u32)TAB_BC_ADDR + TAB_BC_BYTES)
#define	TAB_BE_ADDR					((u32)TAB_BD_ADDR + TAB_BD_BYTES)
#define	TAB_BF_ADDR					((u32)TAB_BE_ADDR + TAB_BE_BYTES)
#define	TAB_C0_ADDR					((u32)TAB_BF_ADDR + TAB_BF_BYTES)
#define	TAB_C1_ADDR					((u32)TAB_C0_ADDR + TAB_C0_BYTES)
#define	TAB_C2_ADDR					((u32)TAB_C1_ADDR + TAB_C1_BYTES)
#define	TAB_C3_ADDR					((u32)TAB_C2_ADDR + TAB_C2_BYTES)
#define	TAB_C4_ADDR					((u32)TAB_C3_ADDR + TAB_C3_BYTES)
#define	TAB_C5_ADDR					((u32)TAB_C4_ADDR + TAB_C4_BYTES)
#define	TAB_C6_ADDR					((u32)TAB_C5_ADDR + TAB_C5_BYTES)
#define	TAB_C7_ADDR					((u32)TAB_C6_ADDR + TAB_C6_BYTES)
#define	TAB_C8_ADDR					((u32)TAB_C7_ADDR + TAB_C7_BYTES)
#define	TAB_C9_ADDR					((u32)TAB_C8_ADDR + TAB_C8_BYTES)
#define	TAB_CA_ADDR					((u32)TAB_C9_ADDR + TAB_C9_BYTES)
#define	TAB_CB_ADDR					((u32)TAB_CA_ADDR + TAB_CA_BYTES)
#define	TAB_CC_ADDR					((u32)TAB_CB_ADDR + TAB_CB_BYTES)
#define	TAB_CD_ADDR					((u32)TAB_CC_ADDR + TAB_CC_BYTES)
#define	TAB_CE_ADDR					((u32)TAB_CD_ADDR + TAB_CD_BYTES)
#define	TAB_CF_ADDR					((u32)TAB_CE_ADDR + TAB_CE_BYTES)
#define	TAB_D0_ADDR					((u32)TAB_CF_ADDR + TAB_CF_BYTES)
#define OBJ_ADDR_END 				((u32)TAB_D0_ADDR + TAB_D0_BYTES)

//-------92����������ǰֵ�����ַ-----------------------
#define INDEX_92_ADDR_START ((u32)OBJ_ADDR_END + 2)	
#define INDEX_92_ADDR_END ((u32)INDEX_92_ADDR_START + 255)	
//
//================================================
//================================================
/* �����ʶ���ͱ� */
#define ID_ALL 						0			/*  �����Բ���  */
#define ID_DB							1			/*  ���ݿ����  */
#define ID_NEVER_SET 			2			/*  ��Զ�������ã����ñ������Բ�ѯ */
#define ID_NO_SET 				3			/*  �������ã������ñ������Բ�ѯ  */
#define ID_TIME		 				4			/*  ʱ�����ݲ�д�����ݱ�  */
#define ID_REAL_TIME			5			/*  ʵʱ�������ݲ�д�����ݱ� */
#define ID_PARAMETER			6			/*  �������ñ���ʱ�������ã��ɲ�ѯ������һ����֧ */
#define ID_REPORT					7			/*  д�뱨����־������,�������ã������ñ����ܲ�ѯ */
#define ID_92_NEVER_SET	  8


//================================================
//================================================
/* ��������Խṹ�� xxxxxx */
typedef struct 
{
	u8 	ID;			  						/* �����ʶ					*/
	u32	Data_Addr;					  /* ���ݵ�ַ					*/
	u32	Flag_Addr;			 			/* ��־��ַ					*/
	u16	Line_Number;					/* ֧������					*/
	u8	Subobj_Number;			  /* �Ӷ�����					*/
	u8	Line_leng;			    	/* ÿ���ֽ���				*/
	u32 Tab_leng;			      	/* ������ֽ���			*/
	u8  Index1;								/* ����1���ֵ			*/
	u8  Index2;								/* ����2���ֵ			*/
	u8  ID_type;            /* ��־���� */
}object_attribute_struct;

extern const object_attribute_struct  Object_table[];
extern const u8 subobject_table[][SUBOBJ_NUM_MAX+1];

// 	0  ID;			  	        /* �����ʶ					*/
// 	1	 Data_Addr;					  /* ���ݵ�ַ					*/
// 	2	 Flag_Addr;					  /* ��־��ַ					*/
// 	3	 Line_Number;			    /* ֧������					*/
// 	4	 Subobj_Number;				/* �Ӷ�����				  */
// 	5  Line_leng;						/* ÿ���ֽ���		    */
// 	6  Tab_leng;				    /* ������ֽ���			*/
// 	7  Index1;							/* ����1 						*/
// 	8  Index2;							/* ����2 						*/
// 	9  ID_type;							/* ��־���� 				*/
//==============================================================================================
#endif

