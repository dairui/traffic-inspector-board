#include "include.h"
#include <stdio.h>

// /*	ad采集过滤窗口0，小于该值则清0*/
// #define LAMP_CURRENT_MIN				50

// /*	二次过滤窗口1，不过滤黄灯*/
// #define WINDOW_RYG							(720/ADC_CYCLE_TIME)
// /*	二次过滤窗口2，过滤掉黄灯*/
// #define WINDOW_RG								(2400/ADC_CYCLE_TIME)

// /*  以下用于二次滤波FIFO: 二次过滤窗口总长度 */
// #define PASSAGE_FILTER_SIZE     (2400/ADC_CYCLE_TIME+10)   
/*  二次过滤窗口Fifo  */
vu32 passage_filter_fifo[PASSAGE_FILTER_SIZE] = {0};
/*  二次过滤窗口写、读、计数值  */
vu8 passage_filter_wr_index,passage_filter_rd_index,passage_filter_counter;

/*==============================================================================================
* 语法格式：    uint16_t ADC_filter_one(uint16_t *pData,uint8_t len)
* 实现功能：    采集后的初始过滤
* 参数1：         
* 参数2：         
* 返回值：       
===============================================================================================*/
#define MIN_NUM   4
#define MAX_NUM   4
uint16_t ADC_filter_one(uint16_t *pData,uint8_t len)
{
	uint16_t value_return=0;
	uint16_t temp=0;
	uint16_t sum=0;

	uint32_t i,j;
	
	for(i=0;i<len-1;i++) 
	{
		for(j=0;j<len-i-1;j++) 
		{
			if(*(pData+j) > *(pData+j+1))
			{
				temp	=*(pData+j);
				*(pData+j)=*(pData+j+1);
				*(pData+j+1)=temp;
			}
		}
	}
	
	for(i=MIN_NUM;i<(len-MAX_NUM);i++) 
	{
		 sum +=*(pData+i);
	}
	value_return =sum/(len-MIN_NUM-MAX_NUM);
	if(value_return<LAMP_CURRENT_MIN)
		value_return = 0;
	return (value_return);
}

/*==============================================================================================
* 语法格式：    u32 first_filtering_binaryzation(u16  filter_one_value[],u8 reset_learn)
* 实现功能：    一次过滤二值化 
* 参数1：         u16  filter_one_value[]：第一次过滤得到的当前ad值
* 参数2：         
* 返回值：         first_all_passage：二值化后的通道状态
===============================================================================================*/
u16	last_filter_ADC[LAMP_NUMBER]	=  
{
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,
	LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN
};				//上一次的adc过滤值
u32 first_filtering_binaryzation(u16  filter_one_value_in[],u8 reset_checking)
{
	u32  	first_all_passage_in 		  = 0;						//二值化后的通道状态

// 	static	u16	last_filter_ADC[LAMP_NUMBER]	=  {LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN,LAMP_CURRENT_MIN};				//上一次的adc过滤值

	u8 		i = 0;
		
	/*	当前状态写入二次过滤窗口FIFO	*/
	for(i=0;i<LAMP_NUMBER;i++)
	{		
		if(filter_one_value_in[i] > last_filter_ADC[i]*BINARYZATION_WEIGHT)
		{
			last_filter_ADC[i] = (last_filter_ADC[i] + filter_one_value_in[i])/2;
			first_all_passage_in |= (1<<i);
// 			cur_filter_ADC[i] = last_filter_ADC[i];
		}
// 		printf("%x ", last_filter_ADC[i] );
	}
// 	printf("\n ");
	return first_all_passage_in;
}


/*==============================================================================================
* 语法格式：    void second_Filtering(u32  first_all_passage)
* 实现功能：    二次滤波
* 参数1：          u32  first_all_passage：第一次过滤得到的当前通道状态
* 参数2：       
* 返回值：         当前红绿状态 *cur_RG_passage,当前红黄绿状态 *cur_RYG_passage；
							有效红(黄)绿灯状态,valid_RG_passage、valid_RYG_passage;
===============================================================================================*/
void second_Filtering(u32  first_all_passage,u32  *p_state)
{
	u8 		i = 0, j = 0;
	u8 		rd_index_temp			  = 0;						//保存读指针的临时变量
	u8		windows_count_temp = 0;						//窗口中某路通道为1的个数
	u32      cur_RG_passage		  = 0;						//当前红绿灯状态
	u32 		 cur_RYG_passage	  = 0;						//当前红黄绿灯状态
// 	u32 status;
	/*	当前状态写入二次过滤窗口FIFO	*/
	passage_filter_fifo[passage_filter_wr_index] = first_all_passage;
	passage_filter_rd_index		   =	passage_filter_wr_index;
	if (++passage_filter_wr_index == PASSAGE_FILTER_SIZE) 
		passage_filter_wr_index=0;
	
	rd_index_temp	= 	passage_filter_rd_index;			//保存初始读指针位置		
	for(j=0;j < LAMP_NUMBER;j++)								//通道循环判断
	{
		windows_count_temp = 0;									//每个通道循环前，清零计数值
		rd_index_temp	   =	passage_filter_rd_index;
		for(i = 0;i<WINDOW_RG;i++)			//过滤窗口判断
		{
			
			if((passage_filter_fifo[rd_index_temp] & (u32)(1<<j)) == (u32)(1<<j))
			{
				windows_count_temp++;								//窗口里状态值当前路状态为1，则计数加一
			}
			
			/*从当前状态往前读取FIFO状态值*/
			if (rd_index_temp == 0)
			{
				rd_index_temp=PASSAGE_FILTER_SIZE-1;
			}
			else
			{
				rd_index_temp--;							//从当前状态往前读取状态值
			}
			if(i==WINDOW_RYG-1)										//过滤窗口1，较小，不过滤黄灯								
			{
				if(windows_count_temp == i+1)						//若窗口1下该路通道均为1，则置当前状态为1
				{
					cur_RYG_passage	|= (u32)(1<<j);
				}
			}
			if(i==WINDOW_RG-1)  										//过滤窗口2，较大，过滤掉黄灯		
			{
				if(windows_count_temp == i+1)						//若窗口2下该路通道均为1，则置当前状态为1
				{
					cur_RG_passage	|= (u32)(1<<j);
				}
			}
		}
	}
	/*	赋值指针，用于返回四个状态值	*/
	 *p_state 	   = cur_RG_passage;
	 *(++p_state) = cur_RYG_passage;
// // // printf("%s\n ", "11111111111111111111111111111111111111111111111111");
// // // 	for(j=0;j < WINDOW_RG;j++)								//通道循环判断
// // // 	{	
// // // 		status = passage_filter_fifo[rd_index_temp];
// // // 		/*从当前状态往前读取FIFO状态值*/
// // // 		printf("%d\n ", status);
// // // 			if (rd_index_temp == 0)
// // // 			{
// // // 				rd_index_temp=PASSAGE_FILTER_SIZE-1;
// // // 			}
// // // 			else
// // // 			{
// // // 				rd_index_temp--;							//从当前状态往前读取状态值
// // // 			}
// // // 		  for (i=0; i<12; i++)
// // // 			{        
// // // 				if ((status&0x01) == 0x01)
// // // 				{
// // // 					printf("%d", 1);
// // // 				}
// // // 				else
// // // 				{
// // // 					printf("%d", 0);
// // // 				} 

// // // 	//             if (((i-2)%3) == 0)
// // // 	//             {
// // // 	//                 printf("%d	 ", 1);
// // // 	//             }
// // // 				status >>= 1;
// // // 			}
// // // 		printf("\n");			
// // // 		}
}

/*==============================================================================================
* 语法格式：   u32* passage_RG_learn(u32 valid_RG_passage,u32 passage_get_RG,u8 passage_start)
* 实现功能：    分离红、绿灯，得到红灯状态表和绿灯状态表
* 参数1：       valid_RG_passage：有效红绿灯通道
* 参数2：       passage_get_RG为c0000000或者A0000000，用于红绿黄或者红黄绿判断
* 参数3：       passage_start第一个红灯起始位置
* 返回值：      有效红灯通道passage_red,有效绿灯通道 passage_green
===============================================================================================*/
void passage_RG_learn(u32 valid_RG_passage,u32 passage_get_RG,u8 passage_start,u32	*p_state)
{
	u8		i;
	u32		passage_human 	= 0;						//行人通道
	u32 		passage_red 			= 0;						//有效红灯通道
	u32		passage_green 	= 0;						//有效绿灯通道
	
	for (i=passage_start;i<LAMP_NUMBER-2;i=i+3)													//分离获得机动车有效红、绿灯通道
	{
		/*若当前有效红绿通道状态从第一个红灯开始为110或者101，则给有效红、绿赋值 */
		if((valid_RG_passage & (passage_get_RG>>i)) == (passage_get_RG>>i))		
		{
			passage_red 		|= (0x80000000>>i);
			passage_green |= (~passage_red & (passage_get_RG>>i));
		}
	}
	
	/*获得行人通道 */
	passage_human = valid_RG_passage & (~(passage_red | passage_green));
	
	/*获得行人通道的红、绿灯状态 */
	if(passage_human > 0)
	{
		for(i=0;i<LAMP_NUMBER-2;i++)
		{
			if((passage_human & (0x80000000>>i)) == (0x80000000>>i))
			{
				passage_red 		|= (0x80000000>>i);
				passage_green	|= (0x80000000>>(i+1));				
			}
		}
	}
	/*	赋值指针，用于返回四个状态值	*/
	 *p_state 	   = passage_red;
	 *(p_state+1) = passage_green;
}

/*==============================================================================================
* 语法格式：    u32* passage_RYG_learn(u32 valid_RG_passage,u32 valid_RYG_passage)
* 实现功能：    分离红、黄、绿灯的有效通道状态
* 参数1：         有效红(黄)绿灯状态,valid_RG_passage、valid_RYG_passage;
* 参数2：       
* 返回值：      分离的有效红灯通道passage_red,有效绿灯通道 passage_green，有效黄灯通道passage_yellow
===============================================================================================*/
void passage_RYG_learn(u32 valid_RG_passage,u32 valid_RYG_passage,u32	*p_state)
{
	u8		i;
	u8		passage_start		=		0;					//第一个红灯起始位置
	u32		passage_get_RG	=		0;					//为c0000000或者A0000000，用于红绿黄或者红黄绿判断
	
	u32 		passage_red 			= 		0;					//分离得到的红灯状态
	u32 		passage_yellow 	= 		0;					//分离得到的黄灯状态
	u32 		passage_green 	= 		0;					//分离得到的绿灯状态
	
	passage_yellow	 =	(~valid_RG_passage) & valid_RYG_passage;						 //获得通道黄灯
	for(i=0;i<LAMP_NUMBER;i++)																					 //获得第一个红灯起始位置
	{
		if((valid_RG_passage & (0x80000000>>i)) == (0x80000000>>i) )
		{
			passage_start = i;
			break;
		}
	}
	if ( ((valid_RG_passage & ( 0x80000000 >>(passage_start+1))) ==0x80000000 >>(passage_start+1))	
	&& ((valid_RG_passage & ( 0x80000000 >>(passage_start+2))) ==0)) 			 //顺序：红绿黄
	{
		passage_get_RG	=	0xC0000000;
		passage_RG_learn(valid_RG_passage,passage_get_RG,passage_start,p_state); 	 //获得通道红绿
		passage_red     = *p_state;
		passage_green = *(p_state+1);
	}
	
	else if (((valid_RG_passage & 0x80000000 >>(passage_start+2)) ==0x80000000 >>(passage_start+2))
		  && (valid_RG_passage & 0x80000000 >>(passage_start+1)) ==0)  	 	 //顺序：红黄绿
	{
		passage_get_RG	=	0xA0000000;
		passage_RG_learn(valid_RG_passage,passage_get_RG,passage_start,p_state); 	 //获得通道红绿
		passage_red     = *p_state;
		passage_green = *(p_state+1);
	}	
	/*	赋值指针，用于返回四个状态值	*/
	 *p_state 	   = passage_red;
	 *(++p_state) = passage_yellow;
	 *(++p_state) = passage_green;
	
}

/*==============================================================================================
* 语法格式：    void get_passage_table(u32 passage_red,u32 passage_yellow,u32 passage_green)
* 实现功能：    由分离的红绿黄有效通道得到各相位有效通道表passage_table[12]	
* 参数1：       	u32 passage_red,u32 passage_yellow,u32 passage_green
* 参数2：       
* 返回值：      	得到passage_table[12]
===============================================================================================*/
void get_passage_table(u32 passage_red,u32 passage_yellow,u32 passage_green,u32 passage_table[])
{
	u8		i;
	u8		j	=	0;
// 	u8		k	=	0;
// 	u8		l	=	0;
	u32 	flag_lampuse_temp=0;
	
	for(i=0;i<LAMP_NUMBER;i++)
	{
		if((passage_red & (u32)(1<<i)) ==  (u32)(1<<i) )					//红灯放入有效通道数组
		{
			passage_table[j]	|=		(u32)(1<<i);
			flag_lampuse_temp			|=		(u32)(1<<i);
			j++;
		}
		if((passage_yellow & (u32)(1<<i)) ==  (u32)(1<<i) )			//黄灯放入有效通道数组
		{
			passage_table[j]	|=		(u32)(1<<i);
			flag_lampuse_temp			|=		(u32)(1<<i);
			//k++;
		}
		if((passage_green & (u32)(1<<i)) ==  (u32)(1<<i) )			//绿灯放入有效通道数组
		{
			passage_table[j]	|=		(u32)(1<<i);
			flag_lampuse_temp			|=		(u32)(1<<i);
			//l++;
		}
	}
	Flag_LampUse = flag_lampuse_temp;
}

/*==============================================================================================
* 语法格式：    u8 get_phase_clash_table(u32 cur_passage_green)
* 实现功能：    学习与得到相位冲突表passage_clash_table[CLASH_STATE_NUMBER]
* 参数1：       	cur_passage_green当前绿灯状态
* 参数2：       	reset_learn：用于重新学习重置缓存
* 返回值：      	learning_flag=1表示完成学习；learning_flag=0未完成学习
===============================================================================================*/
u8 get_phase_clash_table(u32 cur_passage_green,u32 *phase_clash_table,u8 reset_learning)
{
	u8		i,j;
	u8		learning_flag	=	0;										//学习标志，成功为1
	static 	u32		last_passage_green	=	0;		    	//上一次的绿灯状态
	static 	u8		stage_number			=	0;				//有效阶段数
	static 	u8		checkout_count			=	0;				//学习校验次数
	/*校验时，当前绿灯状态与相位冲突表中不同的次数，
		若该绿灯值与相位冲突表中的所有状态都不同，则学习失败，重新学习*/
	u8		differ_clashTb_count			=	0;			
	
	/*	重新学习时重置静态变量	*/
	if(reset_learning == _TRUE)
	{
		last_passage_green=0;	
		stage_number=0;	
		checkout_count=0;	
	}
	
	if(	  (cur_passage_green > 0) 
	&& (last_passage_green != cur_passage_green)	)									//在绿灯放行阶段,并且和前一次不相同
	{
		last_passage_green = cur_passage_green;  						   				 //保存当前状态		
		for(i = 0;i<PHASE_CLASH_STATE_NUMBER;i++)
		{
			if((*(phase_clash_table+i)== 0) && (stage_number <= i))					//新的状态放入相位冲突表
			{
				*(phase_clash_table+i)= (~cur_passage_green); 							//保存当前阶段相位冲突
				stage_number++;																				//保存阶段数
				return 0;
			}
			else if ((*(phase_clash_table+i)== (~cur_passage_green)) && (stage_number > i))
			{																												//进入校验阶段
				if(stage_number == i+1)																	//一次校验完成，计数++
				{
					checkout_count++;
				}
				
				if (checkout_count == 2)																	//校验三次成功，则学习完成
				{
					learning_flag = 1;
					return learning_flag;
				}
				else																										//不到三次，继续校验
				{
					return 0;
				}
			}
			else if((*(phase_clash_table+i) != (~cur_passage_green)) 
				&&(*(phase_clash_table+i)   != 0) && (stage_number > i))
			{
				differ_clashTb_count++;
				if(differ_clashTb_count == stage_number && checkout_count>0)
				{//校验阶段，若当前阶段相位与所有保存的相位不同，则学习失败，重新学习
					stage_number = 0;
					checkout_count = 0;
					for(j=0;j<PHASE_CLASH_STATE_NUMBER;j++)
					{
						*(phase_clash_table+j) = 0;
					}
					printf("*************8重新学习*********");
					return 2;
				}
				else
				{//非校验阶段，则为第一轮学习阶段的写入相位冲突表，continue
					continue;	
				}
			}
			else
			{
			}
		}
	}
	return 0;
}
/*==============================================================================================
* 语法格式：    u32 green_phase_clash_check(u32 cur_passage_green)
* 实现功能：    相位冲突检测，若相位冲突表passage_clash_table[]中一个相与为0，则无冲突，否则冲突
* 参数1：       	cur_passage_green当前绿灯状态
* 参数2：       
* 返回值：      	冲突则返回cur_passage_green，否则返回0
===============================================================================================*/
u32 green_phase_clash_check(u32 cur_passage_green,u32 *phase_clash_table)
{
	u8		i;
	
	if(cur_passage_green >=1)  											//当前状态有绿灯放行相位
	{
		for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)    //最大12个相位，这个设备通道=相位
		{		
			if(*(phase_clash_table+i) == 0)									//若已经判断完有效相位冲突表
			{
				break;
			}
			else 
			{
				if( (*(phase_clash_table+i) & cur_passage_green)==0) 
				{
					return 0;																	//没有冲突返回0
				}
				else
				{
					continue;
				}
			}
		}
		return cur_passage_green;											//有冲突返回冲特表
	}
	return 0;																	//没有冲突返回0
}
/*==============================================================================================
* 语法格式：    u32 passage_clash_check(u32 cur_RYG_passage)
* 实现功能：    通道冲突检测
* 参数1：       	cur_RYG_passage当前灯状态
* 参数2：       
* 返回值：      	冲突则返回passage_clash，否则返回0
===============================================================================================*/
u32 passage_clash_check(u32 cur_RYG_passage,u32 *passage_table)
{
	u8		i;
	u8		j;
	u32		passage_state_temp = 0;			//保存通道状态临时变量
	u8		count_temp			   	 = 0;
	u32		passage_clash			 = 0;			//通道冲突表
	
	for(i=0;i<PASSAGE_NUMBER;i++)    														//通道同时大于一个灯色亮泽冲突
	{		
		count_temp = 0;		
		passage_state_temp = cur_RYG_passage & *(passage_table+i); 		//获取一个通道状态放入临时变量
		for (j=0;j<LAMP_NUMBER;j++)
		{
			if((passage_state_temp>>j & 1) ==1) 
				count_temp++;
		}
		if(count_temp>1)
		{
			passage_clash |= passage_state_temp;										//保存通道冲突表
		}
	}
	if(passage_clash>0)																					//有通道冲突
	{
		return passage_clash;
	}
	else
	{
		return 0;
	}
}
/*==============================================================================================
* 语法格式：    u8 is_period_end(u32 cur_RYG_passage)
* 实现功能：    是否周期结束
* 参数1：       	cur_RYG_passage当前灯状态
* 参数2：       
* 返回值：      	返回1表示周期结束，0则为未结束
===============================================================================================*/
u8 is_period_end(u32 cur_passage_green,u8 reset_checking)
{
	static u8	 lamp_on_count[LAMP_NUMBER]	=	{0};		//各个灯亮的次数
	static u32 last_green_passage = 0;									//上一次各个灯状态
	u8		i,j;
	
// 	if(reset_checking == _TRUE)
// 	{
// 		period_green_passage = 0;	
// 	}
// 	period_green_passage |= cur_passage_green;
// 	if(period_green_passage == passage_green && period_green_passage!=0)
// 	{
// 		return (_TRUE);														//周期结束
// 	}
// 	else
// 	{
// 		return (_FALSE);													//周期没有结束
// 	}

// // 	if((last_green_passage != cur_passage_green) && (cur_passage_green>0))					
// // 	{																								//lamp state和前一次不相同，且大于0
// // 		
// // 	}
	if(reset_checking == _TRUE)
	{
		last_green_passage = 0;	
		for(j = 0;j<LAMP_NUMBER;j++)
		{
			lamp_on_count[j]=0;
		}
	}
	if((last_green_passage != cur_passage_green) && (cur_passage_green>0))					
	{																								//lamp state和前一次不相同，且大于0
		for(i = 0;i<LAMP_NUMBER;i++)
		{
			if(((cur_passage_green>>i& 1) == 1) && ((last_green_passage>>i& 1) != 1))	
			{																						//新的状态放入相位冲突表
				lamp_on_count[i]++;
				if(lamp_on_count[i]>1)
				{
					printf("一个周期结束了啦gggggggg\n");
					for(j = 0;j<LAMP_NUMBER;j++)
					{
						lamp_on_count[j]=0;
					}
					return (_TRUE);																	//周期结束
				}
			}
		}
		last_green_passage = cur_passage_green;  				//保存当前状态		
	}
	return (_FALSE);																				//周期没有结束
}
/*==============================================================================================
* 语法格式：    u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage)
* 实现功能：    灯故障检测，设定路口循环周期，一个周期内通道灯色必须出现一个独立的状态，否则故障
* 参数1：          cur_RYG_passage当前灯状态
* 参数2：          valid_RG_passage有效灯
* 返回值：        灯故障，若大于0，返回状态值；无故障，返回0
===============================================================================================*/
u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage,u32 cur_passage_green,u8* period_completed_flag,u8 reset_checking)
{
	static u32 period_lamp_state = 0;							//一个周期的灯状态之和
	u32 state_temp=0;
	u8 i;
	
	/*灯故障计数数组*/
	static u8 lamp_fault_count[LAMP_NUMBER]= {0};		
	/*多个周期（2个）灯故障状态值*/
	u32		lamp_fault_value 	 	  = 0;	
	
	if(reset_checking==1)
	{
		period_lamp_state = 0;
		for(i=0;i<LAMP_NUMBER;i++)
		{
			lamp_fault_count[i]=0;
		}
	}
	
	if(is_period_end(cur_passage_green,reset_checking) == 0)				//若周期没有结束，则把当前灯状态或入period_lamp_state
	{
		period_lamp_state |= cur_RYG_passage;  			//保存当前状态到period_lamp_state		
		return 0;																		//通道无故障
	}
	else
	{
		state_temp = (~period_lamp_state & valid_RYG_passage);
		period_lamp_state = 0;
		
		*period_completed_flag = __TRUE;
		
		//连续检测到2次灯故障，才报警
		lamp_fault_value = 0;
		for(i=0;i<LAMP_NUMBER;i++)
		{
			if((state_temp>>i & 1) == 1)
			{
				lamp_fault_count[i]++;
				if(lamp_fault_count[i]>=2)		//连续检测到2次灯故障，则报警
				{
					lamp_fault_count[i] = 2;		//防止overflow
					lamp_fault_value |= (1<<i);
				}
			}
			else
			{
				if(lamp_fault_count[i]>0)
				{
					lamp_fault_count[i]--;
				}
			}
			
		}
		
		return lamp_fault_value;														//通道有无故障
	}
}
/*==============================================================================================
* 语法格式：    u32 lamp_phyLevel_detect(u32 cur_lamp_status,u16 standard_current[],u16 now_current[],u8 reset_checking)
* 实现功能：    灯物理电流故障检测，电流若超出范围，则灯有故障
* 参数1：          cur_lamp_status当前灯状态
* 参数2：          u16 standard_current[]学习得到的标准灯电流,u16 now_current[]当前灯电流,u8 reset_checking
* 返回值：        灯故障，若大于0，返回故障灯lamp_phy_error；无故障，返回0
===============================================================================================*/
u32 lamp_phyLevel_detect(u8 *phyerr_level,u32 cur_lamp_status,u16 standard_current[],u16 now_current[],u8 reset_checking)
{
	static u8 lamp_phyerr_count[LAMP_NUMBER] = {0};							//一个周期的灯状态之和
	u8 i;
	u32 lamp_phy_error = 0;

	if(reset_checking==1)
	{
		for(i=0;i<LAMP_NUMBER;i++)
		{
			lamp_phyerr_count[i] = 0;
		}
	}
	
	for(i=0;i<LAMP_NUMBER;i++)
	{
		if((standard_current[i] > 0) && (now_current[i] > 0))
		{
			//超出判定范围now_current[i]>(u16)(standard_current[i]*1.2) || 				
			if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(4/4)))//第1等级
			{
				lamp_phyerr_count[i]++;
				//错误状态计数,连续累积到达2次上报故障
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//防止溢出
					lamp_phy_error|=(u32)(1<<i);														//通道有故障
					*phyerr_level = 1;//第1等级
				}
			}
			else if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(3/4)))//第2等级
			{
				lamp_phyerr_count[i]++;
				//错误状态计数,连续累积到达2次上报故障
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//防止溢出
					lamp_phy_error|=(u32)(1<<i);														//通道有故障
					*phyerr_level = 2;//第2等级
				}
			}
			else if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(2/4)))//第3等级
			{
				lamp_phyerr_count[i]++;
				//错误状态计数,连续累积到达2次上报故障
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//防止溢出
					lamp_phy_error|=(u32)(1<<i);														//通道有故障
					*phyerr_level = 3;//第3等级
				}
			}
			else if(now_current[i]<(u16)(standard_current[i]*cur_min/100*(1/4)))//第4等级
			{
				lamp_phyerr_count[i]++;
				//错误状态计数,连续累积到达2次上报故障
				if(lamp_phyerr_count[i] >= 1)
				{
					lamp_phyerr_count[i] =1;//防止溢出
					lamp_phy_error|=(u32)(1<<i);														//通道有故障
					*phyerr_level = 4;//第4等级
				}
			}
			else
			{
				if(lamp_phyerr_count[i]>0)
				{
					lamp_phyerr_count[i]--;
				}
			}
		}
	}
	
	return lamp_phy_error;														//通道无故障

}
/*==============================================================================================
* 语法格式：    u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage)
* 实现功能：    灯故障检测，设定路口循环周期，一个周期内通道灯色必须出现一个独立的状态，否则故障
* 参数1：          cur_RYG_passage当前灯状态
* 参数2：          valid_RG_passage有效灯
* 返回值：        灯故障，若大于0，返回状态值；无故障，返回0
===============================================================================================*/
void save_learned_data(u32 passage_table[],u32 phase_clash_table[],u32 p_state_task[])
{
//	uint8_t i=0;
	/*
		存储phase_clash_table[]到eeprom
	*/
	save_param(PHASE_CONFLICT_LIST,	(uint8_t *)phase_clash_table,sizeof(u32)*PHASE_CLASH_STATE_NUMBER);
	/*
		存储passage_table[]到eeprom
	*/
	save_param(PASSAGE_STATUS_LIST,	(uint8_t *)passage_table,sizeof(u32)*PASSAGE_NUMBER);
	/*
		存储灯使用状态到eeprom
	*/	
	save_param(LAMP_USE,	(uint8_t *)&Flag_LampUse,sizeof(Flag_LampUse));
	/*
		存储红黄绿状态到eeprom
	*/	
	save_param(LEARNED_RED_STATUS,	(uint8_t *)&p_state_task[0],sizeof(u32));
	save_param(LEARNED_YELLOW_STATUS,	(uint8_t *)&p_state_task[1],sizeof(u32));
	save_param(LEARNED_GREEN_STATUS,	(uint8_t *)&p_state_task[2],sizeof(u32));	
}
/*==============================================================================================
* 语法格式：    u32 lamp_fault_check(u32 cur_RYG_passage,u32 valid_RYG_passage)
* 实现功能：    灯故障检测，设定路口循环周期，一个周期内通道灯色必须出现一个独立的状态，否则故障
* 参数1：          cur_RYG_passage当前灯状态
* 参数2：          valid_RG_passage有效灯
* 返回值：        灯故障，若大于0，返回状态值；无故障，返回0
===============================================================================================*/
void read_learned_data(u32 passage_table[],u32 phase_clash_table[],u32 p_state_task[])
{
//	uint8_t i=0;
	/*
		存储phase_clash_table[]到eeprom
	*/
	read_param(PHASE_CONFLICT_LIST,	(uint8_t *)phase_clash_table,sizeof(u32)*PHASE_CLASH_STATE_NUMBER);
	/*
		存储passage_table[]到eeprom
	*/	
	read_param(PASSAGE_STATUS_LIST,	(uint8_t *)passage_table,sizeof(u32)*PASSAGE_NUMBER);
	/*
		存储灯使用状态到eeprom
	*/	
	read_param(LAMP_USE,	(uint8_t *)&Flag_LampUse,sizeof(Flag_LampUse));
	/*
		存储红黄绿状态到eeprom
	*/	
	read_param(LEARNED_RED_STATUS,	(uint8_t *)&p_state_task[0],sizeof(u32));
	read_param(LEARNED_YELLOW_STATUS,	(uint8_t *)&p_state_task[1],sizeof(u32));
	read_param(LEARNED_GREEN_STATUS,	(uint8_t *)&p_state_task[2],sizeof(u32));

}
/*
*	
	u8   Phase_ParaOutputList[2][4],				//通道输出表
	u32  *realtime_table,						//当前实时值
*	function:			计算通道输出状态表（实时显示）	
*/
void channel_output_table(vu8 Passage_ParaOutputList_temp[][4],u32 realtime_table)
{
	u8 red,yellow,green;
	u8 i;
	u32 rt_std = realtime_table;

	Passage_ParaOutputList_temp[0][0] = 1;

	red = 0;
	yellow = 0;
	green = 0;
	for(i = 0; i < 8; i++)
	{
		if(rt_std & (1<<(3*i+2)))red |= (1<<i);
		if(rt_std & (1<<(3*i+1)))yellow |= (1<<i);
		if(rt_std & (1<<(3*i)))green |= (1<<i);
	}
	Passage_ParaOutputList_temp[0][1] = red;
	Passage_ParaOutputList_temp[0][2] = yellow;
	Passage_ParaOutputList_temp[0][3] = green;

	Passage_ParaOutputList_temp[1][0] = 2;
	red = 0;
	yellow = 0;
	green = 0;

	rt_std = rt_std >> 24;
	
	for(i = 0; i < 3; i++)
	{
		if(rt_std & (1<<(3*i)))red |= (1<<i);
		if(rt_std & (1<<(3*i+1)))yellow |= (1<<i);
		if(rt_std & (1<<(3*i+2)))green |= (1<<i);
	}
	Passage_ParaOutputList_temp[1][1] = red;
	Passage_ParaOutputList_temp[1][2] = yellow;
	Passage_ParaOutputList_temp[1][3] = green;	
}

void lxbprint_status(u32 this_status)
{
	u8 i;
	for (i=0; i<12; i++)
		  {    
				if ((this_status&0x01) == 0x01)
				{
					printf("%d", 1);
				}
				else
				{
					printf("%d", 0);
				} 
				this_status >>= 1;
        }   
		printf("\n");
}
/*==============================================================================================
* 语法格式：    uint16_t ADC_lamp_current(uint16_t *pData,uint8_t len)
* 实现功能：    对电流ad值进行过滤计算，得到当前灯有效电流
* 参数1：         
* 参数2：         
* 返回值：       
===============================================================================================*/
uint16_t ADC_lamp_current(uint16_t *pData,uint8_t len)
{
	uint16_t value_return=0;
	uint16_t temp=0;
	uint16_t sum=0;

	uint32_t i,j;
	
	for(i=0;i<len-1;i++) 
	{
		for(j=0;j<len-i-1;j++) 
		{
			if(*(pData+j) > *(pData+j+1))
			{
				temp	=*(pData+j);
				*(pData+j)=*(pData+j+1);
				*(pData+j+1)=temp;
			}
		}
	}
	
	if(len>8)
	{
		for(i=len/3;i<(len-len/3);i++) 
		{
			 sum +=*(pData+i);
		}
		value_return =sum/(len-len*2/3);
	}
	else
	{
		for(i=0;i<len;i++) 
		{
			 sum +=*(pData+i);
		}
		value_return =sum/len;
	}
	return (value_return);
}
/*==============================================================================================
* 语法格式：    ADC_PwrVolProc(void)
* 实现功能：    交流电电压检测
* 参数1：       learning_transformer_flag	web修改电压值后该flag为真，学习匝数比
* 参数2：       float web_change_vol web发送的标准电压值
* 返回值：      
===============================================================================================*/
//求差值绝对值
#define ABS(x, y)        ((x)>(y)?(x-y):(y-x))
float bbb =0;
u8 ADC_PwrVolProc(u8* learning_transformer_flag,float web_change_vol)
{
	u8    i                 = 0;
	u8    error_flag        = 0;
// static	u8	Pwr_Vol_Index_temp= 0;
	float Pwr_Volatage      = 0;
	float Freq_temp         = 0;
	float Vol_temp          = 0;
		
	uint16_t		aaa;
	uint16_t		time;
	static uint8_t		Pwr_Freq_Index_temp = 0;
	static u16  Pwr_Freq_Record_temp[10] = {0};          //交通灯电压频率缓冲
	static float  Pwr_Vol_Record_temp[10] = {0};           //交通灯电压电压缓冲  
	static u8		first_start_learning_flag = 0;					//第一次开始学习

	if((*learning_transformer_flag == __TRUE)
			&& (first_start_learning_flag == __FALSE))//第一次开始学习，清除缓存，重新采集计算 
	{
		first_start_learning_flag = __TRUE;
		
		Pwr_Freq_Index_temp = 0;
		for(i=0;i<10;i++)
		{
			Pwr_Freq_Record_temp[i] = 0;
			Pwr_Vol_Record_temp[i] = 0;
		}
	}

	TIM_Cmd(TIM1, DISABLE);
	time = TIM1->CNT;
	TIM1->CNT = 0;
	TIM_Cmd(TIM1, ENABLE);
	Pwr_Freq_Record_temp[Pwr_Freq_Index_temp] = (u16)((time) / 6 / 2);       //脉冲数/30秒 
				
	for (i=0; i<10; i++)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		os_dly_wait(5);
		aaa = ADC_GetConversionValue(ADC2);
// 		bbb = (float) aaa/ 4095 * 3.3;
// 		printf("%f	",bbb);
		Pwr_Volatage += (float) aaa/ 4095 * 3.3;
	}
// 	printf("\n");
	Pwr_Volatage = Pwr_Volatage / 10;
	Pwr_Volatage = Pwr_Volatage * (270+100)*100/(100*(10+20));    //U=VOL*(R27+R26)*R24/(R26*(R17+R18))
	Pwr_Volatage = Pwr_Volatage / 1.2;  
	
	if(*learning_transformer_flag == __FALSE)//若不处于学习才转化
	{
		Pwr_Volatage = Pwr_Volatage * (turn_num_lxb/100);            //变压比
	}
	else
	{
		printf("交流电匝数比学习\n");
	}

	Pwr_Vol_Record_temp[Pwr_Freq_Index_temp++] = Pwr_Volatage;

	if (Pwr_Freq_Index_temp >= 10)
	{		
			Pwr_Freq_Index_temp = 0;
			printf("交流电检测1min\n");  
			printf("%f\n",turn_num_lxb);        
			for (i=0; i<10; i++)                            //10次数据处理
			{
          Freq_temp += Pwr_Freq_Record_temp[i];
					Vol_temp  += Pwr_Vol_Record_temp[i];
			}

			Freq_temp /= 10;
			Vol_temp  /= 10;

			if(*learning_transformer_flag == __TRUE)
			{
				if(Vol_temp>0)//学习成功
				{
					turn_num_lxb = web_change_vol*100/Vol_temp;
					Vol_temp = web_change_vol;
					*learning_transformer_flag = __FALSE;
					first_start_learning_flag  = __FALSE;
					
					save_param(REAL_TURN_NUM_ADDR,	(uint8_t *)&turn_num_lxb,		sizeof(turn_num_lxb));
					
					printf("匝数比学习成功\n");  
					printf("%f\n",turn_num_lxb); 
				}
				else//继续学习匝数比
				{
					*learning_transformer_flag = __TRUE;
				}
			}
			else
			{
					if (Freq_temp < FREQ_VALVE_COUNT)//正常工作范围外开始报警 //频率变化超出阈值 
					{
						error_flag = 1;  
						printf("交流电电源频率问题\n");    
						printf("%f\n",Freq_temp);        
					}
					else if(Vol_temp < 180 || Vol_temp > 260)//正常工作范围外开始报警 //频率变化超出阈值 
					{
						error_flag = 2;  
						printf("交流电电源频率问题\n"); 
						printf("%f\n",Vol_temp);         
					}
			}			
			os_mut_wait(table_global_mut,0xFFFF);
			Pwr_Freq = (u16)Freq_temp;
			Pwr_Vol  = (u16)Vol_temp;   				
			os_mut_release(table_global_mut);		
	}
	return error_flag;
}
/*======================================================================
函数名称：
u8 Power3G_Detect(void)
//			 
说明：3G电源检测，有电返回1，掉电返回0
=====================================================================*/
u8  Power3G_Detect(void)
{
 	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
		return (1);
	else
	{
		os_dly_wait(5);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
			return(1);
		 else
			return(0);
	}
}
/*======================================================================
函数名称：
void Report_Lamp_PhyLevel(void)
//			 
说明上报电流
=====================================================================*/
u8 lamp_report_table[77];
void Report_Lamp_PhyLevel()
{
    u32 temp = 0,i;
		u16 lamp_current_temp[LAMP_NUMBER]={0}; 
		u8	count=0;

		/*绿黄红转化为红黄绿*//*全局电流大小数组，以便查询上报*/
		os_mut_wait(table_global_mut,0xFFFF);
		if(1)
		{
			lamp_current_temp[0]=Lamp_Broken_Level[2];
			lamp_current_temp[1]=Lamp_Broken_Level[1];
			lamp_current_temp[2]=Lamp_Broken_Level[0];
			lamp_current_temp[3]=Lamp_Broken_Level[5];
			lamp_current_temp[4]=Lamp_Broken_Level[4];
			lamp_current_temp[5]=Lamp_Broken_Level[3];
			lamp_current_temp[6]=Lamp_Broken_Level[8];
			lamp_current_temp[7]=Lamp_Broken_Level[7];
			lamp_current_temp[8]=Lamp_Broken_Level[6];
			lamp_current_temp[9]=Lamp_Broken_Level[11];
			lamp_current_temp[10]=Lamp_Broken_Level[10];
			lamp_current_temp[11]=Lamp_Broken_Level[9];
			
			lamp_current_temp[12]=Lamp_Broken_Level[14];
			lamp_current_temp[13]=Lamp_Broken_Level[13];
			lamp_current_temp[14]=Lamp_Broken_Level[12];
			lamp_current_temp[15]=Lamp_Broken_Level[17];
			lamp_current_temp[16]=Lamp_Broken_Level[16];
			lamp_current_temp[17]=Lamp_Broken_Level[15];
			lamp_current_temp[18]=Lamp_Broken_Level[20];
			lamp_current_temp[19]=Lamp_Broken_Level[19];
			lamp_current_temp[20]=Lamp_Broken_Level[18];
			lamp_current_temp[21]=Lamp_Broken_Level[23];
			lamp_current_temp[22]=Lamp_Broken_Level[22];
			lamp_current_temp[23]=Lamp_Broken_Level[21];
			lamp_current_temp[24]=Lamp_Broken_Level[26];
			lamp_current_temp[25]=Lamp_Broken_Level[25];
			lamp_current_temp[26]=Lamp_Broken_Level[24];
			lamp_current_temp[27]=Lamp_Broken_Level[29];
			lamp_current_temp[28]=Lamp_Broken_Level[28];
			lamp_current_temp[29]=Lamp_Broken_Level[27];
			lamp_current_temp[30]=Lamp_Broken_Level[31];
			lamp_current_temp[31]=Lamp_Broken_Level[30];
		}
		os_mut_release(table_global_mut);
	
		for (i=0; i<LAMP_NUMBER; i++)
		{   
			if ((i%3) == 0)
			{   
				temp = (i / 3) + 1;
				lamp_report_table[count++] = (u8)temp;    //通道数           
				temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
				lamp_report_table[count++] = (u8)temp;    //灯电流数据  高字节在前
				temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
				lamp_report_table[count++] = (u8)temp;    //灯电流数据                
			}
			else
			{
				temp = (u8)((lamp_current_temp[i]&0xFF00) >> 8);
				lamp_report_table[count++]= (u8)temp;    //灯电流数据
				temp = (u8)((lamp_current_temp[i]&0x00FF) >> 0);
				lamp_report_table[count++] = (u8)temp;    //灯电流数据                
			}
		}
}

/*==============================================================================================
* 语法格式：    get_stage_table(void)
* 实现功能：    得到阶段表
* 参数1：       
* 参数2：       
* 返回值：      
===============================================================================================*/
u16 lamp_stage_count[LAMP_NUMBER] = {0};//阶段灯亮的计数值
STAGE_TIMING_TABLE_t stage_time_table_temp[PHASE_CLASH_STATE_NUMBER] = {0};//阶段配时表
void get_stage_table(u8* period_completed_flag, u32 current_lamp_state,u32 learned_passage_green,u32 learned_passage_red,u8 reset_flag)
{
	static u8 bPeriod_Start = __FALSE;//阶段计算的周期开始标志
	u8 i,j;
	static u32 last_green_state=0;//上一次的绿灯状态，用于判断阶段结束
	u32 current_green_state = 0;//当前绿灯状态
	static u8  stage_count=0;//阶段计数
	static u16 period_count = 0;//周期计数
	
	if(reset_flag == __TRUE)
	{
		for(i=0;i<LAMP_NUMBER;i++)			
		{
			lamp_stage_count[i]=0;
		}
		bPeriod_Start = 0;
		last_green_state=0;
		stage_count=0;
		period_count = 0;
	}
	
	current_green_state = (current_lamp_state&learned_passage_green);

	if(bPeriod_Start == __TRUE)
	{
		if((current_green_state!=0) && (current_green_state!=last_green_state))
		{//说明一个阶段结束
			stage_time_table_temp[stage_count].stageId = stage_count;
			stage_time_table_temp[stage_count].relPhase = 0;
			for(i=0;i<LAMP_NUMBER;i++)			//当前灯状态为1，则计数值++
			{
				if(((last_green_state>>i) & 0x01) == 0x01)
				{
					stage_time_table_temp[stage_count].relPhase |= (1<<(i/3));
				}
			}
			/*保存该阶段的时间与相位*/
			for(i=0;i<LAMP_NUMBER;i++)			//当前灯状态为1，则计数值++
			{
				if((((learned_passage_green>>i) & 0x01) == 0x01)&&(lamp_stage_count[i]>0))
				{
					if((current_green_state&last_green_state)!=0)
					{//若当前相位与上个相位为包含关系，则两个绿灯间无过度时间4800ms
						stage_time_table_temp[stage_count].stGreenTime = (lamp_stage_count[i])*(ADC_CYCLE_TIME+5)/1000;//green
					}
					else
					{
						stage_time_table_temp[stage_count].stGreenTime = (lamp_stage_count[i]+(4800/ADC_CYCLE_TIME))*(ADC_CYCLE_TIME+5)/1000;//green
					}
					if((learned_passage_red>>(i+1)& 0x01) == 0x01)//红绿黄的通道顺序
					{
						stage_time_table_temp[stage_count].stRedTime = lamp_stage_count[i+1]*(ADC_CYCLE_TIME+5)/1000;//red
						stage_time_table_temp[stage_count].stYellowTime = (lamp_stage_count[i-1]+(720/ADC_CYCLE_TIME))*(ADC_CYCLE_TIME+5)/1000;//yellow
					}
					else //if((learned_passage_red>>(i+2)& 0x01) == 0x01)//红黄绿的通道顺序
					{
						stage_time_table_temp[stage_count].stYellowTime = (lamp_stage_count[i+1]+(720/ADC_CYCLE_TIME))*(ADC_CYCLE_TIME+5)/1000;//yellow
						stage_time_table_temp[stage_count].stRedTime = lamp_stage_count[i+2]*(ADC_CYCLE_TIME+5)/1000;//red
					}
				}
				lamp_stage_count[i] = 0;
			}
			last_green_state = current_green_state;
			
			if(stage_time_table_temp[stage_count].stGreenTime >0)//绿灯时间大于0才阶段有效
			{
				stage_count++;
			}
			if(stage_count == PHASE_CLASH_STATE_NUMBER)
			{
				stage_count=0;
			}
		}
		else
		{
			for(i=0;i<LAMP_NUMBER;i++)			//当前灯状态为1，则计数值++
			{
				if((current_lamp_state>>i & 0x01) == 0x01)
				{
					lamp_stage_count[i]++;
				}
			}
		}
		period_count++;
	}	
	if(*period_completed_flag == __TRUE)//以第一个周期结束为起点进行阶段计算
	{
		*period_completed_flag = __FALSE;//以便判断下一个周期结束
		if(stage_count>0)
		{
			//阶段已计算完毕
			
			os_mut_wait(table_global_mut,0xFFFF);
			current_stage_num = stage_count;//保存阶段数
			for(i=0;i<current_stage_num;i++)
			{
// // 				if((stage_time_table_temp[i].stRedTime == 1)&&(stage_time_table_temp[i+1].stRedTime != 0)&&i<PHASE_CLASH_STATE_NUMBER-2)
// // 				{
// // 					stage_time_table_temp[i].stRedTime = stage_time_table_temp[i+1].stRedTime;
// // 					stage_time_table_temp[i].stYellowTime = stage_time_table_temp[i+1].stYellowTime;
// // 				}
				stage_time_table[i] = stage_time_table_temp[i];
			}
			os_mut_release(table_global_mut);		
			
			for(i=0;i<PHASE_CLASH_STATE_NUMBER;i++)
			{
				stage_time_table_temp[i].stRedTime = 0;
				stage_time_table_temp[i].stGreenTime = 0;
				stage_time_table_temp[i].stYellowTime = 0;
				stage_time_table_temp[i].relPhase = 0;
				stage_time_table_temp[i].stageId = 0;				
			}
		}
		stage_count = 0;
		
		if(period_count>0)
		{
			period_time = period_count*(ADC_CYCLE_TIME+5)/1000;
		}
		period_count = 0;
		
		last_green_state = current_green_state;//重新开始新阶段计算
	
		bPeriod_Start = __TRUE;
	}	
	
}

void tsk_watchdog_reload()
{
	/*1.对于非定时任务，即日志、以太网应答、tcp任务，当进入任务时给运行信号xx_xx_run，
			当完成任务（任务出口处给完成信号xx_xx_over，若这两个信号相等，说明任务正常，可喂狗。
			若这些任务没运行，则run与over信号均为0，亦可喂狗
		2.对于ADC任务，学习任务，电源检测任务power_detect_task这三个定时启动的任务，
			只需要运行信号即可
	*/
	if(	((tsk_tcp_run^tsk_tcp_over) ==(__FALSE))//tcp_tsk
		&&((tsk_log_run^tsk_log_over) ==(__FALSE))//log_write_task
		&&((tsk_eth_run^tsk_eth_over) ==(__FALSE))//eth_usart_communication_task
	
		&&(tsk_ADC_run ==(__TRUE))	//ADC_task开始运行
		&&(tsk_learn_run ==(__TRUE))//learn_detect_task
		&&(tsk_power_run ==(__TRUE))	)//power_detect_task
	{
		IWDG_ReloadCounter();
		
		tsk_tcp_run=(__FALSE);
		tsk_tcp_over=(__FALSE);
		tsk_log_run=(__FALSE);
		tsk_log_over=(__FALSE);
		tsk_eth_run=(__FALSE);
		tsk_eth_over=(__FALSE);
		tsk_ADC_run=(__FALSE);
		tsk_learn_run=(__FALSE);
		tsk_power_run=(__FALSE);
		
 		TIM_Cmd(TIM3, DISABLE);/*关闭定时器喂狗*/
		GPIO_PinReverse(GPIOB, GPIO_Pin_15);//喂狗硬件看门狗
	}
}
/*******************************************************************************
* Function Name  : GPIO_PinReverse
* Description    : GPIO???
* Input          : GPIO?:GPIO_TypeDef* GPIOx
* Output         : GPIO?:uint16_t GPIO_Pin
* Return         : None
*******************************************************************************/
void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    
    GPIOx->ODR ^= GPIO_Pin;
}
