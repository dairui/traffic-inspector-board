//FOR  巡检机
#include "include.h"
/* 参数表  xxxxxx */
const object_attribute_struct  Object_table[] = 
{
/* 																					 B代表bytes：字节数			*/
/* 		0					 1							2				        3				  4 			       5			        6		   			 7					8      9  */
/* 对象标识;  数据地址  ;   标志地址       ;  支持行数;    子对象数;   每行B;       对象表B;     索引1数; 索引2数; 标志类型 */
  {	0X81	,	TAB_81_ADDR	,	TAB_81_FLAG_ADDR , TAB_81_LINES ,			1	, LINE_81_BYTES ,	TAB_81_BYTES	,		0 	, 	0 , ID_DB          },
  {	0X82	,	TAB_82_ADDR	,	TAB_82_FLAG_ADDR , TAB_82_LINES ,			1	, LINE_82_BYTES ,	TAB_82_BYTES	,		0 	, 	0 , ID_PARAMETER   },
  {	0X83	,	TAB_83_ADDR	,	TAB_83_FLAG_ADDR , TAB_83_LINES ,			1	, LINE_83_BYTES ,	TAB_83_BYTES	,		0 	, 	0 , ID_PARAMETER   },
  {	0X84	,	TAB_84_ADDR	,	TAB_84_FLAG_ADDR , TAB_84_LINES ,			1	, LINE_84_BYTES ,	TAB_84_BYTES	,		0 	, 	0 , ID_PARAMETER   },
  {	0X85	,	TAB_85_ADDR	,	TAB_85_FLAG_ADDR , TAB_85_LINES ,		 10 , LINE_85_BYTES ,	TAB_85_BYTES	,	 16 	, 	0 , ID_NEVER_SET   },
  {	0X86	,	TAB_86_ADDR	,	TAB_86_FLAG_ADDR , TAB_86_LINES ,			1	, LINE_86_BYTES ,	TAB_86_BYTES	,		0 	, 	0 , ID_TIME        },
  {	0X87	,	TAB_87_ADDR	,	TAB_87_FLAG_ADDR , TAB_87_LINES ,			1	, LINE_87_BYTES ,	TAB_87_BYTES	,		0 	, 	0 , ID_TIME        },
  {	0X88	,	TAB_88_ADDR	,	TAB_88_FLAG_ADDR , TAB_88_LINES ,			1	, LINE_88_BYTES ,	TAB_88_BYTES	,		0 	, 	0 , ID_TIME        },
  {	0X89	,	TAB_89_ADDR	,	TAB_89_FLAG_ADDR , TAB_89_LINES ,			1	, LINE_89_BYTES ,	TAB_89_BYTES	,		0 	, 	0 , ID_PARAMETER   },
  {	0X8A	,	TAB_8A_ADDR	,	TAB_8A_FLAG_ADDR , TAB_8A_LINES ,			1	, LINE_8A_BYTES ,	TAB_8A_BYTES	,		0 	, 	0 , ID_PARAMETER   },
  {	0X8B	,	TAB_8B_ADDR	,	TAB_8B_FLAG_ADDR , TAB_8B_LINES ,			1	, LINE_8B_BYTES ,	TAB_8B_BYTES	,		0 	, 	0 , ID_PARAMETER   },
  {	0X8C	,	TAB_8C_ADDR	,	TAB_8C_FLAG_ADDR , TAB_8C_LINES ,			1	, LINE_8C_BYTES ,	TAB_8C_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0X8D	,	TAB_8D_ADDR	,	TAB_8D_FLAG_ADDR , TAB_8D_LINES ,		  5	, LINE_8D_BYTES ,	TAB_8D_BYTES	,	 40 	, 	0 ,	ID_DB					 },
  {	0X8E	,	TAB_8E_ADDR	,	TAB_8E_FLAG_ADDR , TAB_8E_LINES ,	    8	, LINE_8E_BYTES ,	TAB_8E_BYTES	,	 16 	,  48 ,	ID_DB					 },
  {	0X8F	,	TAB_8F_ADDR	,	TAB_8F_FLAG_ADDR , TAB_8F_LINES ,			1	, LINE_8F_BYTES ,	TAB_8F_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0X90	,	TAB_90_ADDR	,	TAB_90_FLAG_ADDR , TAB_90_LINES ,			1	, LINE_90_BYTES ,	TAB_90_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0X91	,	TAB_91_ADDR	,	TAB_91_FLAG_ADDR , TAB_91_LINES ,		  5	, LINE_91_BYTES ,	TAB_91_BYTES	,	 64 	, 	0 ,	ID_NEVER_SET	 },
  {	0X92	,	TAB_92_ADDR	,	TAB_92_FLAG_ADDR , TAB_92_LINES ,     4	, LINE_92_BYTES ,	TAB_92_BYTES  ,	 6 	, TAB_92_S_NUMBER ,	ID_92_NEVER_SET},
  {	0X93	,	TAB_93_ADDR	,	TAB_93_FLAG_ADDR , TAB_93_LINES ,			1	, LINE_93_BYTES ,	TAB_93_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0X94	,	TAB_94_ADDR	,	TAB_94_FLAG_ADDR , TAB_94_LINES ,			1	, LINE_94_BYTES ,	TAB_94_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0X95	,	TAB_95_ADDR	,	TAB_95_FLAG_ADDR , TAB_95_LINES ,		 12	, LINE_95_BYTES ,	TAB_95_BYTES	,	 16 	, 	0 ,	ID_DB					 },
  {	0X96	,	TAB_96_ADDR	,	TAB_96_FLAG_ADDR , TAB_96_LINES ,			4	, LINE_96_BYTES ,	TAB_96_BYTES	,		2 	, 	0 ,	ID_DB					 },
  {	0X97	,	TAB_97_ADDR	,	TAB_97_FLAG_ADDR , TAB_97_LINES ,			2	, LINE_97_BYTES ,	TAB_97_BYTES	,	 16 	, 	0 ,	ID_REPORT			 },
  {	0X98	,	TAB_98_ADDR	,	TAB_98_FLAG_ADDR , TAB_98_LINES ,			1	, LINE_98_BYTES ,	TAB_98_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0X99	,	TAB_99_ADDR	,	TAB_99_FLAG_ADDR , TAB_99_LINES ,			1	, LINE_99_BYTES ,	TAB_99_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0X9A	,	TAB_9A_ADDR	,	TAB_9A_FLAG_ADDR , TAB_9A_LINES ,			1	, LINE_9A_BYTES ,	TAB_9A_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0X9B	,	TAB_9B_ADDR	,	TAB_9B_FLAG_ADDR , TAB_9B_LINES ,			1	, LINE_9B_BYTES ,	TAB_9B_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0X9C	,	TAB_9C_ADDR	,	TAB_9C_FLAG_ADDR , TAB_9C_LINES ,			1	, LINE_9C_BYTES ,	TAB_9C_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0X9D	,	TAB_9D_ADDR	,	TAB_9D_FLAG_ADDR , TAB_9D_LINES ,			1	, LINE_9D_BYTES ,	TAB_9D_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0X9E	,	TAB_9E_ADDR	,	TAB_9E_FLAG_ADDR , TAB_9E_LINES ,			1	, LINE_9E_BYTES ,	TAB_9E_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0X9F	,	TAB_9F_ADDR	,	TAB_9F_FLAG_ADDR , TAB_9F_LINES ,		  8	, LINE_9F_BYTES ,	TAB_9F_BYTES	,	 48 	, 	0 ,	ID_DB					 },
  {	0XA0	,	TAB_A0_ADDR	,	TAB_A0_FLAG_ADDR , TAB_A0_LINES ,			3	, LINE_A0_BYTES ,	TAB_A0_BYTES	,		8 	, 	0 ,	ID_NO_SET			 },
  {	0XA1	,	TAB_A1_ADDR	,	TAB_A1_FLAG_ADDR , TAB_A1_LINES ,		  7	, LINE_A1_BYTES ,	TAB_A1_BYTES	,	 48 	, 	0 ,	ID_NO_SET			 },
  {	0XA2	,	TAB_A2_ADDR	,	TAB_A2_FLAG_ADDR , TAB_A2_LINES ,		  3	, LINE_A2_BYTES ,	TAB_A2_BYTES	,	 48 	, 	0 ,	ID_REPORT			 },
  {	0XA3	,	TAB_A3_ADDR	,	TAB_A3_FLAG_ADDR , TAB_A3_LINES ,			1	, LINE_A3_BYTES ,	TAB_A3_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XA4	,	TAB_A4_ADDR	,	TAB_A4_FLAG_ADDR , TAB_A4_LINES ,			1	, LINE_A4_BYTES ,	TAB_A4_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XA5	,	TAB_A5_ADDR	,	TAB_A5_FLAG_ADDR , TAB_A5_LINES ,			1	, LINE_A5_BYTES ,	TAB_A5_BYTES	,		0 	, 	0 ,	ID_REPORT			 },
  {	0XA6	,	TAB_A6_ADDR	,	TAB_A6_FLAG_ADDR , TAB_A6_LINES ,			1	, LINE_A6_BYTES ,	TAB_A6_BYTES	,		0 	, 	0 ,	ID_REPORT			 },
  {	0XA7	,	TAB_A7_ADDR	,	TAB_A7_FLAG_ADDR , TAB_A7_LINES ,			1	, LINE_A7_BYTES ,	TAB_A7_BYTES	,		0 	, 	0 ,	ID_REPORT			 },
  {	0XA8	,	TAB_A8_ADDR	,	TAB_A8_FLAG_ADDR , TAB_A8_LINES ,			1	, LINE_A8_BYTES ,	TAB_A8_BYTES	,		0 	, 	0 ,	ID_REPORT			 },
  {	0XA9	,	TAB_A9_ADDR	,	TAB_A9_FLAG_ADDR , TAB_A9_LINES ,			1	, LINE_A9_BYTES ,	TAB_A9_BYTES	,		0 	, 	0 ,	ID_REPORT   	 },
  {	0XAA	,	TAB_AA_ADDR	,	TAB_AA_FLAG_ADDR , TAB_AA_LINES ,			1	, LINE_AA_BYTES ,	TAB_AA_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XAB	,	TAB_AB_ADDR ,	TAB_AB_FLAG_ADDR , TAB_AB_LINES ,			1	, LINE_AB_BYTES ,	TAB_AB_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XAC	,	TAB_AC_ADDR ,	TAB_AC_FLAG_ADDR , TAB_AC_LINES ,			1	, LINE_AC_BYTES ,	TAB_AC_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XAD	,	TAB_AD_ADDR ,	TAB_AD_FLAG_ADDR , TAB_AD_LINES ,			1	, LINE_AD_BYTES ,	TAB_AD_BYTES	,		0 	, 	0 ,	ID_DB	         },
  {	0XAE	,	TAB_AE_ADDR	,	TAB_AE_FLAG_ADDR , TAB_AE_LINES ,			1	, LINE_AE_BYTES ,	TAB_AE_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0XAF	,	TAB_AF_ADDR	,	TAB_AF_FLAG_ADDR , TAB_AF_LINES ,			1	, LINE_AF_BYTES ,	TAB_AF_BYTES	,		0 	, 	0 ,	ID_PARAMETER   },
  {	0XB0	,	TAB_B0_ADDR	,	TAB_B0_FLAG_ADDR , TAB_B0_LINES ,		  4	, LINE_B0_BYTES ,	TAB_B0_BYTES	,	 16 	, 	0 ,	ID_DB			     },
  {	0XB1	,	TAB_B1_ADDR	,	TAB_B1_FLAG_ADDR , TAB_B1_LINES ,			4	, LINE_B1_BYTES ,	TAB_B1_BYTES	,		2 	, 	0 ,	ID_NO_SET	     },
  {	0XB2	,	TAB_B2_ADDR	,	TAB_B2_FLAG_ADDR , TAB_B2_LINES ,			1	, LINE_B2_BYTES ,	TAB_B2_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0XB3	,	TAB_B3_ADDR	,	TAB_B3_FLAG_ADDR , TAB_B3_LINES ,			1	, LINE_B3_BYTES ,	TAB_B3_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0XB4	,	TAB_B4_ADDR	,	TAB_B4_FLAG_ADDR , TAB_B4_LINES ,			1	, LINE_B4_BYTES ,	TAB_B4_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0XB5	,	TAB_B5_ADDR	,	TAB_B5_FLAG_ADDR , TAB_B5_LINES ,			1	, LINE_B5_BYTES ,	TAB_B5_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XB6	,	TAB_B6_ADDR	,	TAB_B6_FLAG_ADDR , TAB_B6_LINES ,			1	, LINE_B6_BYTES ,	TAB_B6_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XB7	,	TAB_B7_ADDR	,	TAB_B7_FLAG_ADDR , TAB_B7_LINES ,			1	, LINE_B7_BYTES ,	TAB_B7_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XB8	,	TAB_B8_ADDR	,	TAB_B8_FLAG_ADDR , TAB_B8_LINES ,			1	, LINE_B8_BYTES ,	TAB_B8_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XB9	,	TAB_B9_ADDR	,	TAB_B9_FLAG_ADDR , TAB_B9_LINES ,			1	, LINE_B9_BYTES ,	TAB_B9_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XBA	,	TAB_BA_ADDR ,	TAB_BA_FLAG_ADDR , TAB_BA_LINES ,			1	, LINE_BA_BYTES ,	TAB_BA_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XBB	,	TAB_BB_ADDR ,	TAB_BB_FLAG_ADDR , TAB_BB_LINES ,			1	, LINE_BB_BYTES ,	TAB_BB_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XBC	,	TAB_BC_ADDR ,	TAB_BC_FLAG_ADDR , TAB_BC_LINES ,			1	, LINE_BC_BYTES ,	TAB_BC_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XBD	,	TAB_BD_ADDR ,	TAB_BD_FLAG_ADDR , TAB_BD_LINES ,		  1 , LINE_BD_BYTES ,	TAB_BD_BYTES	,		0 	, 	0 , ID_DB       	 },
  {	0XBE	,	TAB_BE_ADDR ,	TAB_BE_FLAG_ADDR , TAB_BE_LINES ,		  1 , LINE_BE_BYTES ,	TAB_BE_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XBF	,	TAB_BF_ADDR ,	TAB_BF_FLAG_ADDR , TAB_BF_LINES ,		  1 , LINE_BF_BYTES ,	TAB_BF_BYTES	,		0 	, 	0 ,	ID_REAL_TIME	 },
  {	0XC0	,	TAB_C0_ADDR	,	TAB_C0_FLAG_ADDR , TAB_C0_LINES ,			5	, LINE_C0_BYTES ,	TAB_C0_BYTES	,	 32 	, 	0 ,	ID_DB					 },
  {	0XC1	,	TAB_C1_ADDR	,	TAB_C1_FLAG_ADDR , TAB_C1_LINES ,			7	, LINE_C1_BYTES ,	TAB_C1_BYTES	,	 16 	,  16 ,	ID_ALL				 },
  {	0XC2	,	TAB_C2_ADDR	,	TAB_C2_FLAG_ADDR , TAB_C2_LINES ,			1	, LINE_C2_BYTES ,	TAB_C2_BYTES	,		0 	, 	0 ,	ID_DB				   },
  {	0XC3	,	TAB_C3_ADDR	,	TAB_C3_FLAG_ADDR , TAB_C3_LINES ,			1	, LINE_C3_BYTES ,	TAB_C3_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XC4	,	TAB_C4_ADDR	,	TAB_C4_FLAG_ADDR , TAB_C4_LINES ,			1	, LINE_C4_BYTES ,	TAB_C4_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XC5	,	TAB_C5_ADDR	,	TAB_C5_FLAG_ADDR , TAB_C5_LINES ,			1	, LINE_C5_BYTES ,	TAB_C5_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XC6	,	TAB_C6_ADDR	,	TAB_C6_FLAG_ADDR , TAB_C6_LINES ,			1	, LINE_C6_BYTES ,	TAB_C6_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0XC7	,	TAB_C7_ADDR	,	TAB_C7_FLAG_ADDR , TAB_C7_LINES ,			1	, LINE_C7_BYTES ,	TAB_C7_BYTES	,		0 	, 	0 ,	ID_PARAMETER	 },
  {	0XC8	,	TAB_C8_ADDR	,	TAB_C8_FLAG_ADDR , TAB_C8_LINES ,		  9 , LINE_C8_BYTES ,	TAB_C8_BYTES	,		8 	, 	0 ,	ID_DB					 },
  {	0XC9	,	TAB_C9_ADDR	,	TAB_C9_FLAG_ADDR , TAB_C9_LINES ,			4	, LINE_C9_BYTES ,	TAB_C9_BYTES	,		1 	, 	0 ,	ID_DB					 },
  {	0XCA	,	TAB_CA_ADDR ,	TAB_CA_FLAG_ADDR , TAB_CA_LINES ,			4	, LINE_CA_BYTES ,	TAB_CA_BYTES	,		4 	,  16 ,	ID_DB					 },
  {	0XCB	,	TAB_CB_ADDR ,	TAB_CB_FLAG_ADDR , TAB_CB_LINES ,			3	, LINE_CB_BYTES ,	TAB_CB_BYTES	,	 16 	, 	0 ,	ID_DB					 },
  {	0XCC	,	TAB_CC_ADDR ,	TAB_CC_FLAG_ADDR , TAB_CC_LINES ,			4	, LINE_CC_BYTES ,	TAB_CC_BYTES	,	 16 	, 	0 ,	ID_DB					 },
  {	0XCD	,	TAB_CD_ADDR ,	TAB_CD_FLAG_ADDR , TAB_CD_LINES ,		 10 , LINE_CD_BYTES ,	TAB_CD_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XCE	,	TAB_CE_ADDR ,	TAB_CE_FLAG_ADDR , TAB_CE_LINES ,			2	, LINE_CE_BYTES ,	TAB_CE_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XCF	,	TAB_CF_ADDR ,	TAB_CF_FLAG_ADDR , TAB_CF_LINES ,			4	, LINE_CF_BYTES ,	TAB_CF_BYTES	,		0 	, 	0 ,	ID_DB					 },
  {	0XD0	,	TAB_D0_ADDR	,	TAB_D0_FLAG_ADDR , TAB_D0_LINES ,			2	, LINE_D0_BYTES ,	TAB_D0_BYTES	,		0 	, 	0 ,	ID_DB					 },
};

//=========================================================================================
/* 子对象表 */
const u8 subobject_table[][SUBOBJ_NUM_MAX+1] = 
{
	/*ID   	1		2		 3		 4		 5		 6		 7		8		 9		10   11	 12	 */ 
	{0X81,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X82,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X83,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X84,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X85,	1	,	1	,	 24 ,	 1	,	24	,	 1	,	24	,	1	,	24	,	 1	,	0	,	0	},	/* 0X85 10个子对象 */
	{0X86,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X87,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X88,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X89,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X8A,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X8B,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X8C,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X8D,	1	,	2	,		1	,	 4	,	 1	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0X8D  5个子对象 */
	{0X8E,	1	,	1	,		1	,	 1	,	 1	,	 1	,	 1	,	1	,	 0	,	 0	,	0	,	0	},	/* 0X8E  8个子对象 双索引 */
	{0X8F,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X90,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X91,	1	,	4	,		1	,	 1	,	 1	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0X91  5个子对象 */
	{0X92,	1	,	1	,		4	,	 4	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0X92  4个子对象 */
	{0X93,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X94,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X95,	1	,	1	,		1	,	 1	,	 1	,	 1	,	 1	,	1	,	 1	,	 1	,	1	,	1	},	/* 0X95 12个子对象 */
	{0X96,	1	,	1	,		1	,	 1	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0X96  4个子对象 */
	{0X97,	1	,	2	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0X97  2个子对象 */
	{0X98,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X99,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X9A,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X9B,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X9C,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X9D,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X9E,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0X9F,	1	,	1	,		1	,	 1	,	 1	,	 1	,	 2	,	1	,	 0	,	 0	,	0	,	0	},	/* 0X9F  8个子对象 */
	{0XA0,	1	,	1	,		1	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XA0  3个子对象 */
	{0XA1,	1	,	1	,		1	,	 1	,	 1	,	 1	,	 1	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XA1  7个子对象 */
	{0XA2,	1	,	1	,		1	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XA2  3个子对象 */
	{0XA3,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XA4,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XA5,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XA6,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XA7,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XA8,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XA9,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XAA,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XAB,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XAC,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XAD,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XAE,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XAF,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB0,	1	,	1	,		1	,	 1	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XB0  4个子对象 */
	{0XB1,	1	,	1	,		1	,	 1	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XB1  4个子对象 */
	{0XB2,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB3,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB4,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB5,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB6,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB7,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB8,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XB9,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XBA,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XBB,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XBC,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XBD,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XBE,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XBF,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC0,	1	,	1	,		1	,	 1	,	 1	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XC0  5个子对象 */
	{0XC1,	1	,	1	,		2	,	 1	,	 1	,	 1	,	 1	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XC1  7个子对象 双索引 */
	{0XC2,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC3,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC4,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC5,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC6,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC7,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XC8,	1	,	1	,		1	,	16	,	 1	,	16	,	 1	,	1	,	 1	,	 0	,	0	,	0	},	/* 0XC8  9个子对象 */
	{0XC9,	1	,	1	,		1	,	 1	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XC9  4个子对象 */
	{0XCA,	1	,	1	,		2	,	 1	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XCA  4个子对象 */
	{0XCB,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XCC,	1	,	2	,		2	,	 2	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	},	/* 0XCC  4个子对象 */
	{0XCD,	1	,	4	,		4	,	 4	,	 4	,	 4	,	 2	,	6	,	12	,	12	,	0	,	0	},	/* 0XCD 10个子对象 */
	{0XCE,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XCF,	0	,	0	,		0	,	 0	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	, 0 },	
	{0XD0,	1	,	2	,		2	,	 2	,	 0	,	 0	,	 0	,	0	,	 0	,	 0	,	0	,	0	} 	/* 0XD0  4个子对象 */
};


//=======to lxb start=============================================================

/*****************************************/
//写数据和标识到EErom 
//返回写入的数据长度，不包括索引一有效值和索引二有效值 
/****************************************/
u32 Write_Taget_to_Eerom(TARGET_DATA_DB * p_Taget_set)
{
	u8  ID_temp =0;
	u8  Index1_lines_count = 0;
	u8  Index2_lines_count = 0;
	u8  Index2_lines_count_temp = 0;
	u32 Data_leng_temp;
	u32 Data_addr_temp;
	u32 Data_addr_old;
	u32 Flag_addr_temp;
	u32 Flag_addr_old;
	u32 Line_Number_temp;
	u32 i=0;
	u32 j=0;
	u32	write_count=0;
	
	ID_temp = p_Taget_set->ID;
	
 	
	//计算数据长度
	if(p_Taget_set->sub_target == 0) //设置时子对象为0，只支持整行或整表
	{
		Data_addr_old 		= Object_table[ID_temp-0x81].Data_Addr;     //数据地址
		Data_addr_temp	 	=	Data_addr_old	;
		Flag_addr_old			= Object_table[ID_temp-0x81].Flag_Addr;     //标识地址
		Flag_addr_temp 		= Flag_addr_old ;
		Line_Number_temp 	= Object_table[ID_temp-0X81].Line_Number; //支持行数
//----------------- 整表设置 -------------------------------------------------
		if((p_Taget_set->count_index == 0)
				&&(p_Taget_set->index[0] == 0)
				&&(p_Taget_set->index[1] == 0)
				&&(Object_table[ID_temp-0X81].Index2 == 0))//整表 //双索引不支持整表设置
		{
			
			if((Object_table[ID_temp-0X81].Index1 == 0)  && (Object_table[ID_temp-0X81].Index2 == 0))      //单对象
			{
				Data_leng_temp = Object_table[ID_temp-0X81].Tab_leng;
			}
			else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 == 0)) //单索引
			{
				Data_leng_temp = Object_table[ID_temp-0X81].Tab_leng - 1;
				Data_addr_temp += 1;
			}
			else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 != 0) && (ID_temp != 0x92)) //双索引
			{
				Data_leng_temp = Object_table[ID_temp-0X81].Tab_leng - 2;
				Data_addr_temp += 2;
			}
			else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 != 0) && (ID_temp == 0x92)) //双索引0x92表
			{
				Data_leng_temp = Object_table[ID_temp-0X81].Tab_leng - 1;
				Data_addr_temp += 1;
			}

			
			Wr_u8(Flag_addr_temp++,TAB_OK_FLAG) ;		//写整表标识，索引标识
			for(i=0; i<Line_Number_temp; i++)
			{
				Wr_u8(Flag_addr_temp++,LINE_OK_FLAG) ;	//写整行标识，索引标识
			}
			
			for(i=0;i<Data_leng_temp;i++)						 //写整表操作前 全清零
			{
				Wr_u8(Data_addr_temp++,0) ;	
			}	
			Data_addr_temp	=		Data_addr_old;			
			for(i=0;i<Data_leng_temp;i++)						//写整表数据
			{
				Wr_u8(Data_addr_temp++,(p_Taget_set->sub_buf[write_count++])) ;	
			}
			//========= line ================
			if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 == 0)) //单索引
			{
				Wr_u8(Data_addr_old , Object_table[ID_temp-0X81].Index1 );
			}
			else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 != 0) && (ID_temp != 0x92)) //双索引
			{
				Wr_u8(Data_addr_old,  Object_table[ID_temp-0X81].Index1);
				Wr_u8((Data_addr_old + 1), Object_table[ID_temp-0X81].Index2 );
			}
			else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 != 0) && (ID_temp == 0x92)) //双索引0x92表
			{
				Wr_u8(Data_addr_old,  Object_table[ID_temp-0X81].Index1);
			}
			//===============================
			
			
		}
//----------------- 单索引表 索引一方式 设置 -------------------------------------------------
		else if((p_Taget_set->count_index == 1)																														//一个索引
						 &&(p_Taget_set->index[0] != 0) 						 && (p_Taget_set->index[1] == 0)							//索引一有效
						 &&(Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 == 0) )//单索引表         索引一方式
		{
			Data_leng_temp = Object_table[ID_temp-0X81].Line_leng;
			Data_addr_old = Object_table[ID_temp-0x81].Data_Addr + (p_Taget_set->index[0]-1)*(Object_table[ID_temp-0X81].Line_leng);
																							//跳过前面N-1行的地址，N为索引1的当前要写入的值
			Data_addr_temp = Data_addr_old ;
			Data_addr_temp += 1;
			
			Wr_u8(Flag_addr_temp++,TAB_OK_FLAG) ;			//写整表标识，索引标识
			for(i=1; i<p_Taget_set->index[0]; i++)
			{
				Flag_addr_temp++;
			}
			Wr_u8(Flag_addr_temp,LINE_OK_FLAG) ;		//写整行标识，索引标识
			

			for(i=0;i<Data_leng_temp;i++)						//写整行操作前 全清零
			{
				Wr_u8(Data_addr_temp++,0) ;	
			}	
			Data_addr_temp	=		Data_addr_old;
			for(i=0;i<Data_leng_temp;i++)						//写整行数据
			{
				Wr_u8(Data_addr_temp++,(p_Taget_set->sub_buf[write_count++])) ;	
			}
			//=========line=====================
			for(i=0 ; i< Object_table[ID_temp-0X81].Index1 ; i++)
				{
					if(Rd_u8(Flag_addr_old + 1 + i) == LINE_OK_FLAG)
						Index1_lines_count++;
				}
			Wr_u8(Data_addr_old , Index1_lines_count );  //写索引一有效行数
			//==================================
			
		}
		
//----------------- 双索引表 索引一方式 设置 -------------------------------------------------
		else if((p_Taget_set->count_index == 2)																														//两个索引
						 &&(p_Taget_set->index[0] != 0) 						 && (p_Taget_set->index[1] == 0)							//索引一索引二有效
						 &&(Object_table[ID_temp-0X81].Index1 != 0)	 && (Object_table[ID_temp-0X81].Index2 != 0) )//双索引表          索引一方式
		{			
			Data_leng_temp = Object_table[ID_temp-0X81].Line_leng * Object_table[ID_temp-0X81].Index2;
			Data_addr_old  = Object_table[ID_temp-0x81].Data_Addr 
											 + ((p_Taget_set->index[0]-1)*Object_table[ID_temp-0X81].Index2 + p_Taget_set->index[1] - 1)*(Object_table[ID_temp-0X81].Line_leng);
				//跳过前面{(N-1)*index2_max +index[2]-1}行的地址，N为索引1的当前要写入的值，M为索引2的当前要写入的值
			Data_addr_temp = Data_addr_old;
			if(ID_temp != 0x92)
			{
				Data_addr_temp += 2;
			}
			else
			{
				Data_addr_temp += 1;
			}
			
			Wr_u8(Flag_addr_temp++,TAB_OK_FLAG) ;			//写整表标识，索引标识
			for(i=1; i<((p_Taget_set->index[0]-1)*Object_table[ID_temp-0X81].Index2); i++)
			{
				Flag_addr_temp++;
			}
			for(i=0; i<Object_table[ID_temp-0X81].Index2; i++)
			{
				Wr_u8(Flag_addr_temp++,LINE_OK_FLAG) ;		//写整行标识，索引标识	
			}
			
			for(i=0;i<Data_leng_temp;i++)						//写整行操作前 全清零
			{
				Wr_u8(Data_addr_temp++,0) ;	
			}
			Data_addr_temp	=		Data_addr_old;
			for(i=0;i<Data_leng_temp;i++)						//写整行数据
			{
				Wr_u8(Data_addr_temp++,(p_Taget_set->sub_buf[write_count++])) ;	
			}
			//============line=========================
			for(j=0 ; j< Object_table[ID_temp-0X81].Index1 ; j++)
			{
			  for(i=0 ; i< Object_table[ID_temp-0X81].Index2 ; i++)
				{
					if(Rd_u8(Flag_addr_old + 1 + i + j*(Object_table[ID_temp-0X81].Index2)) == LINE_OK_FLAG)
					{
						Index2_lines_count_temp++;
					}
				}
				if(Index2_lines_count_temp != 0)
				{
					Index1_lines_count++;
				}
				if(Index2_lines_count < Index2_lines_count_temp)
					Index2_lines_count = Index2_lines_count_temp;
			}
			Wr_u8(Data_addr_old , Index1_lines_count );					//写索引一有效行数
			if(ID_temp != 0x92)
			{
				Wr_u8((Data_addr_old + 1), Index2_lines_count );		//写索引二有效行数最大值
			}
			//=========================================
			
		}
		
//----------------- 双索引表 索引二方式 （双索引方式）设置 -------------------------------------------------
		else if((p_Taget_set->count_index == 2)																														//两个索引
						 &&(p_Taget_set->index[0] != 0) 						 && (p_Taget_set->index[1] != 0)							//索引一索引二有效
						 &&(Object_table[ID_temp-0X81].Index1 != 0)	 && (Object_table[ID_temp-0X81].Index2 != 0) )//双索引表          双索引方式
		{			
			Data_leng_temp = Object_table[ID_temp-0X81].Line_leng;
			Data_addr_old = Object_table[ID_temp-0x81].Data_Addr 
											 + ((p_Taget_set->index[0]-1)*Object_table[ID_temp-0X81].Index2 + p_Taget_set->index[1] - 1)*(Object_table[ID_temp-0X81].Line_leng);
				//跳过前面{(N-1)*index2_max +index[2]-1}行的地址，N为索引1的当前要写入的值，M为索引2的当前要写入的值
			Data_addr_temp =	Data_addr_old	;
			if(ID_temp != 0x92)
			{
				Data_addr_temp += 2;
			}
			else
			{
				Data_addr_temp += 1;
			}
			
			Wr_u8(Flag_addr_temp++,TAB_OK_FLAG) ;			//写整表标识，索引标识
			for(i=1; i<(((p_Taget_set->index[0]-1)*Object_table[ID_temp-0X81].Index2)+p_Taget_set->index[1]); i++)
			{
				Flag_addr_temp++;
			}
			Wr_u8(Flag_addr_temp,LINE_OK_FLAG) ;		//写整行标识，索引标识
			
			
			for(i=0;i<Data_leng_temp;i++)						//写整行操作前 全清零
			{
				Wr_u8(Data_addr_temp++,0) ;	
			}
			Data_addr_temp	=		Data_addr_old;						//写整表标识，索引标识
			for(i=0;i<Data_leng_temp;i++)						//写整行数据
			{
				Wr_u8(Data_addr_temp++,(p_Taget_set->sub_buf[write_count++])) ;	
			}
			//==========line================================
			for(j=0 ; j< Object_table[ID_temp-0X81].Index1 ; j++)
			{
			  for(i=0 ; i< Object_table[ID_temp-0X81].Index2 ; i++)
				{
					if(Rd_u8(Flag_addr_old + 1 + i + j*(Object_table[ID_temp-0X81].Index2)) == LINE_OK_FLAG)
					{
						Index2_lines_count_temp++;
					}
				}
				if(Index2_lines_count_temp != 0)
				{
					Index1_lines_count++;
				}
				if(Index2_lines_count < Index2_lines_count_temp)
				{
					Index2_lines_count = Index2_lines_count_temp;
					Index2_lines_count_temp = 0;
				}
			}
			Wr_u8(Data_addr_old , Index1_lines_count );					//写索引一有效行数
			if(ID_temp != 0x92)
			{
				Wr_u8((Data_addr_old + 1), Index2_lines_count );		//写索引二有效行数最大值
			}
			//==============================================
		}
		
  }
	return (write_count);

}

/*****************************************/
//从 EErom 读数据和标识 
//返回读取的数据长度， 包括索引一有效值和索引二有效值
/****************************************/
u32 Read_Eerom_to_Taget(TARGET_DATA_DB * p_Taget_set)
{
	u8  ID_temp = 0;
	u32 read_count  = 0;
	u32 Flag_addr_temp = 0;
	u32 Data_addr_temp = 0;
	u8 Tab_OK_flag_temp = 0;
	u32 line_bytes_temp = 0;
	u32 line_OK_count =0;
	u32 line_num_temp = 0;
	u32 i = 0;
	u32 j = 0;
	
	
	ID_temp = p_Taget_set->ID;

	Flag_addr_temp 		= Object_table[ID_temp-0x81].Flag_Addr;     //标识地址	
	Data_addr_temp		= Object_table[ID_temp-0x81].Data_Addr;     //数据地址	
	
	Tab_OK_flag_temp  = Rd_u8(Flag_addr_temp); 	//整表有效标志	
	if(p_Taget_set->sub_target == 0)
	{
		if(Tab_OK_flag_temp == TAB_OK_FLAG) //表有效
		{	
			line_bytes_temp  = Object_table[ID_temp-0x81].Line_leng; //每行字节数
//----------------- 整表(单索引,双索引)   --------------------------
			if((p_Taget_set->count_index == 0) && (p_Taget_set->index[0] == 0) && (p_Taget_set->index[1] == 0))//整表
			{
        if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 == 0)) //单索引
				{
					p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++); //索引一有效个数  行数
				}
				else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 != 0) && (ID_temp != 0x92)) //双索引
				{
					p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++); //索引一有效个数  行数
					p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++); //索引二有效个数
				}
				else if((Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 != 0) && (ID_temp != 0x92)) //双索引
				{
					p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++); //索引一有效个数  行数
				}
				
				line_num_temp 	 = Object_table[ID_temp-0x81].Line_leng;

				for(j=0; j<line_num_temp; j++)
				{
					if(Rd_u8(++Flag_addr_temp) == LINE_OK_FLAG) //行有效
					{
						line_OK_count++;
						for(i=0; i<line_bytes_temp; i++)
						{
							p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++);
						}
					}
					else
					{
						Data_addr_temp += Object_table[ID_temp-0x81].Line_leng; 
					}
				}
			}
//----------------- 单索引表 索引一方式 查询 -------------------------------------------------
			else if((p_Taget_set->count_index == 1)																														//一个索引
							 &&(p_Taget_set->index[0] != 0) 						 && (p_Taget_set->index[1] == 0)							//索引一有效
							 &&(Object_table[ID_temp-0X81].Index1 != 0)  && (Object_table[ID_temp-0X81].Index2 == 0) )//单索引表         索引一方式		
			{
				
				Flag_addr_temp += p_Taget_set->index[0];
				Data_addr_temp += 1;
				Data_addr_temp += (p_Taget_set->index[0]-1)*Object_table[ID_temp-0x81].Line_leng;
				
				if(Rd_u8(Flag_addr_temp) == LINE_OK_FLAG)
				{
						for(i=0; i<line_bytes_temp; i++)
						{
							p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++);
						}
				}
				
			}
//----------------- 双索引表 索引一方式 查询 -------------------------------------------------
			else if((p_Taget_set->count_index == 2)																													//两个索引
						 &&(p_Taget_set->index[0] != 0) 						 && (p_Taget_set->index[1] == 0)							//索引一索引二有效
						 &&(Object_table[ID_temp-0X81].Index1 != 0)	 && (Object_table[ID_temp-0X81].Index2 != 0))//双索引表          索引一方式
			{
 				Flag_addr_temp += p_Taget_set->index[0]*Object_table[ID_temp-0X81].Index2;
 				Data_addr_temp += 2;
				if( ID_temp == 0x92)
				{
					Data_addr_temp -= 1;
        }
 				Data_addr_temp += (p_Taget_set->index[0]-1)*Object_table[ID_temp-0x81].Line_leng;
				for(j=0; j<Object_table[ID_temp-0X81].Index2; j++)
				{
					if(Rd_u8(Flag_addr_temp++) == LINE_OK_FLAG)
					{
							for(i=0; i<(line_bytes_temp * (Object_table[ID_temp-0X81].Index2)); i++)
							{
								p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++);
							}
					}
				}
			
			}
//----------------- 双索引表 索引二方式 （双索引方式）查询 -------------------------------------------------
			else if((p_Taget_set->count_index == 2)																														//两个索引
							 &&(p_Taget_set->index[0] != 0) 						 && (p_Taget_set->index[1] != 0)							//索引一索引二有效
							 &&(Object_table[ID_temp-0X81].Index1 != 0)	 && (Object_table[ID_temp-0X81].Index2 != 0))//双索引表          双索引方式
			{	
				Flag_addr_temp += (p_Taget_set->index[0]-1)*Object_table[ID_temp-0X81].Index2 + p_Taget_set->index[1];
				Data_addr_temp += 2;
				if( ID_temp == 0x92)
				{
					Data_addr_temp -= 1;
        }
				Data_addr_temp += ((p_Taget_set->index[0]-1)*Object_table[ID_temp-0X81].Index2 + p_Taget_set->index[1] - 1 ) *Object_table[ID_temp-0x81].Line_leng;
				if(Rd_u8(Flag_addr_temp) == LINE_OK_FLAG)
				{				
						for(i=0; i<line_bytes_temp; i++)
						{
							p_Taget_set->sub_buf[read_count++] = Rd_u8(Data_addr_temp++);
						}	
				}						
			}
			
			
		}
	}
	return(read_count);
}


//=======to lxb end==============================================================================



	
