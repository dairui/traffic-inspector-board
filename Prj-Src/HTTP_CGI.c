/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_CGI.C
 *      Purpose: HTTP Server CGI Module
 *      Rev.:    V4.22
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <Net_Config.h>
#include <stdio.h>
#include <string.h>
#include "include.h"

/* ---------------------------------------------------------------------------
 * The HTTP server provides a small scripting language.
 *
 * The script language is simple and works as follows. Each script line starts
 * with a command character, either "i", "t", "c", "#" or ".".
 *   "i" - command tells the script interpreter to "include" a file from the
 *         virtual file system and output it to the web browser.
 *   "t" - command should be followed by a line of text that is to be output
 *         to the browser.
 *   "c" - command is used to call one of the C functions from the this file.
 *         It may be followed by the line of text. This text is passed to
 *         'cgi_func()' as a pointer to environment variable.
 *   "#' - command is a comment line and is ignored (the "#" denotes a comment)
 *   "." - denotes the last script line.
 *
 * --------------------------------------------------------------------------*/


// // /* at_System.c */
// // extern  LOCALM localm[];
// // #define LocM   localm[NETIF_ETH]

// // // /* Net_Config.c */
// // // extern struct tcp_cfg   tcp_config;
// // // extern struct http_cfg  http_config;
// // // #define tcp_NumSocks    tcp_config.NumSocks
// // // #define tcp_socket      tcp_config.Scb
// // // #define http_EnAuth     http_config.EnAuth
// // // #define http_auth_passw http_config.Passw

/* Net_Config.c */
extern struct tcp_info tcp_socket[];
extern U8 const tcp_NumSocks;
extern U8 const http_EnAuth;
extern U8       http_auth_passw[20];

extern BOOL LEDrun;
extern void LED_out (U32 val);
extern BOOL LCDupdate;
extern U8   lcd_text[2][16+1];

/* Local variables. */
//static U8 P2;
static char const state[][9] = {
  "FREE",
  "CLOSED",
  "LISTEN",
  "SYN_REC",
  "SYN_SENT",
  "FINW1",
  "FINW2",
  "CLOSING",
  "LAST_ACK",
  "TWAIT",
  "CONNECT"};

/* My structure of CGI status U32 variable. This variable is private for */
/* each HTTP Session and is not altered by HTTP Server. It is only set to  */
/* zero when the cgi_func() is called for the first time.                  */
typedef struct {
  U16 xcnt;
  U16 unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

// // 		char str_temp[0x40]="待改";
// // 		char cp_temp[40]={0};


/*----------------------------------------------------------------------------
 * HTTP Server Common Gateway Interface Functions
 *---------------------------------------------------------------------------*/
/*临时变量，以便checkbox用途*/
float 		turn_num_temp				=0.0;  	 	//  变压器匝数比==已改成标准电压值lxb
uint16_t 	bat_min_temp				=0;    		//  后备电池报警阀值(分钟)
uint16_t 	cur_min_temp				=0;    		//  信号灯电流异常报警阀值(%)
uint16_t 	send_time_temp			=0;    		//  信号灯电流上报时间(分钟)
uint16_t combin_passage_temp[COMBINE_VALID_PASSAGE_NUM] = {0};
/*--------------------------- cgi_process_var -------------------------------*/
void cgi_process_var (U8 *qs) {
  /* This function is called by HTTP server to process the Querry_String   */
  /* for the CGI Form GET method. It is called on SUBMIT from the browser. */
  /*.The Querry_String.is SPACE terminated.                                */
  U8 *var;
  int s[4];
	//char str_temp[17];

	int len;
	char ch_password[17]={0};
	char re_ch_password[17]={0};
	
	char str_temp[0x40]="待改";
	float fl_temp=0;
	float f2_temp=0;

  var = (U8 *)alloc_mem (0x40);
  do {
    /* Loop through all the parameters. */
    qs = http_get_var (qs, var, 0x40);
    /* Check the returned string, 'qs' now points to the next. */
    if (var[0] != 0) {
      /* Returned string is non 0-length. */
      if (str_scomp (var, "ip=") == __TRUE) {
        /* My IP address parameter. */
        sscanf ((const char *)&var[3], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
				if(	(LocM.IpAdr[0] != (uint8_t)s[0]) ||
						(LocM.IpAdr[1] != (uint8_t)s[1]) ||
						(LocM.IpAdr[2] != (uint8_t)s[2]) ||
						(LocM.IpAdr[3] != (uint8_t)s[3]) )
					{
						local_ip_flag		=__TRUE;
						
						LocM.IpAdr[0]   = (uint8_t)s[0];
						LocM.IpAdr[1]   = (uint8_t)s[1];
						LocM.IpAdr[2]   = (uint8_t)s[2];
						LocM.IpAdr[3]   = (uint8_t)s[3];
					}
				
      }     			
      else if (str_scomp (var, "msk=") == __TRUE) {
        /* Net mask parameter. */
        sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
				if(	(LocM.NetMask[0] != (uint8_t)s[0]) ||
						(LocM.NetMask[1] != (uint8_t)s[1]) ||
						(LocM.NetMask[2] != (uint8_t)s[2]) ||
						(LocM.NetMask[3] != (uint8_t)s[3]) )
					{
						local_mask_flag		=__TRUE;
						
						LocM.NetMask[0]   = (uint8_t)s[0];
						LocM.NetMask[1]   = (uint8_t)s[1];
						LocM.NetMask[2]   = (uint8_t)s[2];
						LocM.NetMask[3]   = (uint8_t)s[3];
					}
      }
      else if (str_scomp (var, "gw=") == __TRUE) {
        /* Default gateway parameter. */
        sscanf ((const char *)&var[3], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
				if(	(LocM.DefGW[0] != (uint8_t)s[0]) ||
						(LocM.DefGW[1] != (uint8_t)s[1]) ||
						(LocM.DefGW[2] != (uint8_t)s[2]) ||
						(LocM.DefGW[3] != (uint8_t)s[3]) )
					{
						local_gateway_flag		=__TRUE;
						
						LocM.DefGW[0]   = (uint8_t)s[0];
						LocM.DefGW[1]   = (uint8_t)s[1];
						LocM.DefGW[2]   = (uint8_t)s[2];
						LocM.DefGW[3]   = (uint8_t)s[3];
					}
				
      }
      else if (str_scomp (var, "sip=") == __TRUE) {
        /* 服务器－IP */
        sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
				if(	(server_ip[0] != (uint8_t)s[0]) ||
						(server_ip[1] != (uint8_t)s[1]) ||
						(server_ip[2] != (uint8_t)s[2]) ||
						(server_ip[3] != (uint8_t)s[3]) )
					{
						server_ip_flag		=__TRUE;
						
						server_ip[0]   = (uint8_t)s[0];
						server_ip[1]   = (uint8_t)s[1];
						server_ip[2]   = (uint8_t)s[2];
						server_ip[3]   = (uint8_t)s[3];
					}
      }
      else if (str_scomp (var, "spt=") == __TRUE) {
        /* 服务器－端口 */
        sscanf ((const char *)&var[4], "%d",&s[0]);
				if(server_port != (uint16_t)s[0])
				{					
					server_port_flag		=__TRUE;
						
					server_port   = s[0];
				}

      }
      else if (str_scomp (var, "sww=") == __TRUE) {
         /* 服务器－IP2 */
        /*sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
				if(	(myself_server_ip[0] != (uint8_t)s[0]) ||
						(myself_server_ip[1] != (uint8_t)s[1]) ||
						(myself_server_ip[2] != (uint8_t)s[2]) ||
						(myself_server_ip[3] != (uint8_t)s[3]) )
					{
						server_www_flag		=__TRUE;
						
						myself_server_ip[0]   = (uint8_t)s[0];
						myself_server_ip[1]   = (uint8_t)s[1];
						myself_server_ip[2]   = (uint8_t)s[2];
						myself_server_ip[3]   = (uint8_t)s[3];
					}			*/		
      }		
			else if (str_scomp (var, "swp=") == __TRUE) {
        /* 服务器－端口2 */
        /*sscanf ((const char *)&var[4], "%d",&s[0]);
				if(myself_server_port != (uint16_t)s[0])
				{					
					server_port_flag		=__TRUE;
						
					server_port   = s[0];
				}*/

      }			
//===============================================================================	
			else if (str_scomp (var, "userName=") == __TRUE) {
        /* My IP address parameter. */
        sscanf ((const char *)&var[9], "%s",str_temp);
				len = strlen ((const char *)str_temp);
				if (mem_comp ((char *)userName, str_temp, len) == __TRUE) {
					/* OK, both entered passwords the same, change it. */
					userName_flag=__TRUE;
				}				

      }						
      else if (str_scomp (var, "password=") == __TRUE) {
        /* My IP address parameter. */
        sscanf ((const char *)&var[9], "%s",str_temp);
				len = strlen ((const char *)str_temp);
				if (mem_comp ((char *)password, str_temp, len) == __TRUE) {
					/* OK, both entered passwords the same, change it. */
					password_flag=__TRUE;
				}		

      }
//===============================================================================				
//===============================================================================			
      else if (str_scomp (var, "radio1=") == __TRUE) {
        /* 出厂设置：是－否 */
        sscanf ((const char *)&var[7], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "yes") == __TRUE) 
					reset_all_flag 	=__TRUE;    //  恢复出厂设置标志
				else
					reset_all_flag 	=__FALSE;   		
      }
			
			else if (str_scomp (var, "radio2=") == __TRUE) {
        /* 重新学习：是－否 */
        sscanf ((const char *)&var[7], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "yes") == __TRUE) 
					restudy_flag 	=__TRUE;    //  重新学习标志
				else
					restudy_flag 	=__FALSE;   
      }
			else if (str_scomp (var, "radio3=") == __TRUE) {
        /* 重启设备：是－否 */
        sscanf ((const char *)&var[7], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "yes") == __TRUE) 
					restart_flag 	=__TRUE;    //  重启设备标志
				else
					restart_flag 	=__FALSE;   
      }
			
			else if (str_scomp (var, "radio4=") == __TRUE) {
        /* 定时重启：启用－禁止 */
        sscanf ((const char *)&var[7], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "yes") == __TRUE) 
				{
					if(alarm_restart_flag ==__FALSE)
					{
						alarm_restart_flag 	=__TRUE;    //  定时重启设备标志
						change_alarm_restart_flag =__TRUE;		
					}
				}
				else
				{
					if(alarm_restart_flag ==__TRUE) 
					{
						alarm_restart_flag 	=__FALSE;
						change_alarm_restart_flag =__TRUE;							
					}						
				}		

 				
      }
//===============================================================================	
//===============================================================================				
			else if (str_scomp (var, "radio9=") == __TRUE) {
        /* Default gateway parameter. */
        sscanf ((const char *)&var[7], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "yes") == __TRUE) 
				{
					if(DHCP_flag ==__FALSE)
					{
						DHCP_flag 	=__TRUE;    //DHCP 是否启用 状态
						change_DHCP_flag =__TRUE;		
					}
				}
				else
				{
					if(DHCP_flag ==__TRUE) 
					{
						DHCP_flag 	=__FALSE;
						change_DHCP_flag =__TRUE;							
					}						
				}					
				
      }
//===============================================================================			
			//=====================================
			else if (str_scomp (var, "restart_hour=") == __TRUE) {
        /* 系统重启参数－时. 				*/
        sscanf ((const char *)&var[13], "%d",&s[0]);
				alarm_restart_hour =s[0];
      }
			
			else if (str_scomp (var, "restart_minute=") == __TRUE) {
        /* 系统重启参数－分. 				*/
        sscanf ((const char *)&var[15], "%d",&s[0]);
				alarm_restart_minute =s[0];	

      }
			//=====================================
			//=====================================
			/*   密码修改  */
			else if (str_scomp (var, "ch_pw=") == __TRUE) {
        sscanf ((const char *)&var[6], "%s",ch_password);	
				re_ch_pw_count ++;
      }
			
			else if (str_scomp (var, "re_ch_pw=") == __TRUE) {
        sscanf ((const char *)&var[9], "%s",re_ch_password);
				re_ch_pw_count ++;
      }
			//=====================================			
		
			//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
			else if (str_scomp (var, "turn_num=") == __TRUE) {
        /* 变压器匝数比 				*/
        //sscanf ((const char *)&var[9], "%f",&turn_num);
				sscanf ((const char *)&var[9], "%f",&fl_temp);
				turn_num_temp =fl_temp;
// 				if((ABS(turn_num,fl_temp)>=1)&&(0!=fl_temp))
// 				{
// 					turn_num_flag =__TRUE;
// 					turn_num =fl_temp;
// 				}
      }				
			//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// 			else if (str_scomp (var, "turn_num_lxb=") == __TRUE) {
//         /* 变压器匝数比 				*/
//         //sscanf ((const char *)&var[9], "%f",&turn_num);
// 				sscanf ((const char *)&var[13], "%.1f",&f2_temp);
// 				if((turn_num_lxb !=f2_temp)&&(0!=f2_temp))
// 				{
// 					turn_num_lxb_flag =__TRUE;
// 					turn_num_lxb =f2_temp;
// 				}
// 			}
			else if (str_scomp (var, "bat_min=") == __TRUE) {
        /* 后备电池报警阀值(分钟) 				*/
        //sscanf ((const char *)&var[8], "%d",&bat_min);
				sscanf ((const char *)&var[8], "%d",&s[0]);
				bat_min_temp   = s[0];
// 				if(bat_min != (uint16_t)s[0])
// 				{					
// 					bat_min_flag		=__TRUE;					
// 					bat_min   = s[0];
// 				}
      }		
			else if (str_scomp (var, "cur_min=") == __TRUE) {
        /* 信号灯电流异常报警阀值(mA) 				*/
        //sscanf ((const char *)&var[8], "%d",&cur_min);
				sscanf ((const char *)&var[8], "%d",&s[0]);
				cur_min_temp   = s[0];
// 				if(cur_min != (uint16_t)s[0])
// 				{					
// 					cur_min_flag		=__TRUE;					
// 					cur_min   = s[0];
// 				}
      }		
			else if (str_scomp (var, "send_time=") == __TRUE) {
        /* 信号灯电流上报时间(分钟) 				*/
        //sscanf ((const char *)&var[10], "%d",&send_time);
				sscanf ((const char *)&var[10], "%d",&s[0]);
				send_time_temp   = s[0];
// 				if(send_time != (uint16_t)s[0])
// 				{					
// 					send_time_flag		=__TRUE;					
// 					send_time   = s[0];
// 				}
      }			
	
			else if (str_scomp (var, "appbox=") == __TRUE) {
        /* 重启设备：是－否 */
        sscanf ((const char *)&var[7], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "stand_vol") == __TRUE) //标准电压
				{
					if(turn_num !=turn_num_temp)
					{
						turn_num_flag =__TRUE;
						turn_num =turn_num_temp;
					}
				}
				else if(str_scomp ((U8*)str_temp, "bat_alarm") == __TRUE) //蓄电池异常报警
				{
					if(bat_min !=bat_min_temp)
					{
						bat_min_flag		=__TRUE;					
						bat_min   = bat_min_temp;
					}
				}	
				else if(str_scomp ((U8*)str_temp, "lamp_alarm") == __TRUE) //信号灯异常报警
				{
					if(cur_min   != cur_min_temp)
					{
						cur_min_flag		=__TRUE;					
						cur_min   = cur_min_temp;
					}
				}	
				else if(str_scomp ((U8*)str_temp, "lamp_current") == __TRUE) //信号灯电流上报周期
				{
					if(send_time   != send_time_temp)
					{
						send_time_flag		=__TRUE;					
						send_time   = send_time_temp;
					}
				}			
      }			
			//组合通道设置
			else if (str_scomp (var, "combine_pas1=") == __TRUE) {
				
				sscanf ((const char *)&var[13], "%d",&combin_passage_temp[0]);
      }			
			else if (str_scomp (var, "combine_pas2=") == __TRUE) {
				
				sscanf ((const char *)&var[13], "%d",&combin_passage_temp[1]);
      }			
			else if (str_scomp (var, "combine_pas3=") == __TRUE) {
				
				sscanf ((const char *)&var[13], "%d",&combin_passage_temp[2]);
      }			
			else if (str_scomp (var, "combine_pas4=") == __TRUE) {
				
				sscanf ((const char *)&var[13], "%d",&combin_passage_temp[3]);
      }			
			else if (str_scomp (var, "combine_pas5=") == __TRUE) {
				
				sscanf ((const char *)&var[13], "%d",&combin_passage_temp[4]);
      }			
			else if (str_scomp (var, "combine_pas6=") == __TRUE) {
				
				sscanf ((const char *)&var[13], "%d",&combin_passage_temp[5]);
      }			
	
			else if (str_scomp (var, "combbox=") == __TRUE) {
        
        sscanf ((const char *)&var[8], "%s",str_temp);
				if(str_scomp ((U8*)str_temp, "comb_pas1") == __TRUE) 
				{
					if(combin_passage_temp[0] !=combine_valid_passage[0])
					{
						bSet_Valid_Passage =__TRUE;
						combine_valid_passage[0] =combin_passage_temp[0];
					}
				}
				else if(str_scomp ((U8*)str_temp, "comb_pas2") == __TRUE)
				{
					if(combin_passage_temp[1] !=combine_valid_passage[1])
					{
						bSet_Valid_Passage =__TRUE;
						combine_valid_passage[1] =combin_passage_temp[1];
					}
				}
				else if(str_scomp ((U8*)str_temp, "comb_pas3") == __TRUE) 
				{
					if(combin_passage_temp[2] !=combine_valid_passage[2])
					{
						bSet_Valid_Passage =__TRUE;
						combine_valid_passage[2] =combin_passage_temp[2];
					}
				}
				else if(str_scomp ((U8*)str_temp, "comb_pas4") == __TRUE) 
				{
					if(combin_passage_temp[3] !=combine_valid_passage[3])
					{
						bSet_Valid_Passage =__TRUE;
						combine_valid_passage[3] =combin_passage_temp[3];
					}
				}
				else if(str_scomp ((U8*)str_temp, "comb_pas5") == __TRUE) 
				{
					if(combin_passage_temp[4] !=combine_valid_passage[4])
					{
						bSet_Valid_Passage =__TRUE;
						combine_valid_passage[4] =combin_passage_temp[4];
					}
				}
				else if(str_scomp ((U8*)str_temp, "comb_pas6") == __TRUE) 
				{
					if(combin_passage_temp[5] !=combine_valid_passage[5])
					{
						bSet_Valid_Passage =__TRUE;
						combine_valid_passage[5] =combin_passage_temp[5];
					}
				}
      }			
			
    }
  }while (qs);
  free_mem ((OS_FRAME *)var);
	
  if (re_ch_pw_count == 2) {
		re_ch_pw_count =0;
    len = strlen ((const char *)ch_password);
    if (mem_comp (ch_password, re_ch_password, len) == __TRUE) {
      /* OK, both entered passwords the same, change it. */
      str_copy ((U8*)password, (U8*)ch_password);
			 ch_pw_flag 			=__TRUE;           // 是否修改密码
    }
  }	
	
}


/*--------------------------- cgi_process_data ------------------------------*/

void cgi_process_data (U8 code, U8 *dat, U16 len) {
  /* This function is called by HTTP server to process the returned Data    */
  /* for the CGI Form POST method. It is called on SUBMIT from the browser. */
  /* Parameters:                                                            */
  /*   code  - callback context code                                        */
  /*           0 = www-url-encoded form data                                */
  /*           1 = filename for file upload (0-terminated string)           */
  /*           2 = file upload raw data                                     */
  /*           3 = end of file upload (file close requested)                */
  /*           4 = any xml encoded POST data (single or last stream)        */
  /*           5 = the same as 4, but with more xml data to follow          */
  /*               Use http_get_content_type() to check the content type    */  
  /*   dat   - pointer to POST received data                                */
  /*   len   - received data length                                         */
//  U8 passw[12],retyped[12];
  U8 *var;//,stpassw;

  switch (code) {
    case 0:
      /* Url encoded form data received. */
      break;

    default:
      /* Ignore all other codes. */
      return;
  }

//  stpassw = 0;
  var = (U8 *)alloc_mem (40);
  do {
    /* Parse all returned parameters. */
    dat = http_get_var (dat, var, 40);
    if (var[0] != 0) {

	 
    }
  }while (dat);
  free_mem ((OS_FRAME *)var);

}


/*--------------------------- cgi_func --------------------------------------*/
//char str_temp[20]="待改";
U16 cgi_func (U8 *env, U8 *buf, U16 buflen, U32 *pcgi) {
  /* This function is called by HTTP server script interpreter to make a    */
  /* formated output for 'stdout'. It returns the number of bytes written   */
  /* to the output buffer. Hi-bit of return value (len is or-ed with 0x8000)*/
  /* is a repeat flag for the system script interpreter. If this bit is set */
  /* to 1, the system will call the 'cgi_func()' again for the same script  */
  /* line with parameter 'pcgi' pointing to a 4-byte buffer. This buffer    */
  /* can be used for storing different status variables for this function.  */
  /* It is set to 0 by HTTP Server on first call and is not altered by      */
  /* HTTP server for repeated calls. This function should NEVER write more  */
  /* than 'buflen' bytes to the buffer.                                     */
  /* Parameters:                                                            */
  /*   env    - environment variable string                                 */
  /*   buf    - HTTP transmit buffer                                        */
  /*   buflen - length of this buffer (500-1400 bytes - depends on MSS)     */
  /*   pcgi   - pointer to session local buffer used for repeated loops     */
  /*            This is a U32 variable - size is 4 bytes. Value is:         */
  /*            - on 1st call = 0                                           */
  /*            - 2nd call    = as set by this function on first call       */
  TCP_INFO *tsoc;
  U32 len = 0;

	char str_temp[0x40]="待改";
	char cp_temp[40]={0};
	
	char stemp[16]="无";u8 j;u16 count;
	
  switch (env[0]) {
    /* Analyze the environment string. It is the script 'c' line starting */
    /* at position 2. What you write to the script file is returned here. */
//===========================================================		
   case 'a' :
      /* Network parameters - file 'network.cgi' */
      switch (env[2]) {
        case 'i':
          /* Write the local IP address. The format string is included */
          /* IP设置－本机IP.                 */
          if(DHCP_flag ==__FALSE) 
					len = sprintf((char *)buf,(const char *)&env[4],LocM.IpAdr[0],
                        LocM.IpAdr[1],LocM.IpAdr[2],LocM.IpAdr[3]);
          break;
        case 'm':
          /* IP设置－子网掩码.                 */
				if(DHCP_flag ==__FALSE) 
          len = sprintf((char *)buf,(const char *)&env[4],LocM.NetMask[0],
                        LocM.NetMask[1],LocM.NetMask[2],LocM.NetMask[3]);
          break;
        case 'g':
          /* IP设置－本机网关.                 */
				if(DHCP_flag ==__FALSE) 
          len = sprintf((char *)buf,(const char *)&env[4],LocM.DefGW[0],
                        LocM.DefGW[1],LocM.DefGW[2],LocM.DefGW[3]);
          break;
        case 's':
          /* IP设置－服务器IP.                 */
				if(DHCP_flag ==__FALSE) 
          len = sprintf((char *)buf,(const char *)&env[4],server_ip[0],
                        server_ip[1],server_ip[2],server_ip[3]); 
          break;
        case 'p':
          /* IP设置－端口.                 */
				if(DHCP_flag ==__FALSE) 
          len = sprintf((char *)buf,(const char *)&env[4],server_port
                        ); 
          break;
        case 'w':
          /* IP设置－服务器域名.                 */   
				myself_server_ip[0]=0;
				myself_server_ip[1]=0;
				myself_server_ip[2]=0;
				myself_server_ip[3]=0;
				if(DHCP_flag ==__FALSE) 
          len = sprintf((char *)buf,(const char *)&env[4],myself_server_ip[0],
                        myself_server_ip[1],myself_server_ip[2],myself_server_ip[3]);  
          break;
        case 'x':
          /* IP设置－端口.                 */
				myself_server_port=0;
				if(DHCP_flag ==__FALSE) 
          len = sprintf((char *)buf,(const char *)&env[4],myself_server_port
                        ); 
          break;
      }
      break;

//===========================================================
    case 'b':
				if (env[2] == '0') 
				{
					/* 重启设置－状态		 */		

					len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
				}
			else if (env[2] == '1') 
				{
					/* 重启设置－时		 */		

					len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
				}
			else if (env[2] == '2') 
				{
					/* 重启设置－分 	 */		
					len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
				}				
      else if (env[2] == 'c') {
			
				len = sprintf((char *)buf,(const char *)&env[4],0);


				break;
      }
			break;

    case 'c':
      /* TCP status - file 'tcp.cgi' */
      while ((len + 150) < buflen) {
        tsoc = &tcp_socket[MYBUF(pcgi)->xcnt];
        MYBUF(pcgi)->xcnt++;
        /* 'sprintf' format string is defined here. */
        len += sprintf((char *)(buf+len),"<tr align=\"center\">");
        if (tsoc->State <= TCP_STATE_CLOSED) {
          len += sprintf ((char *)(buf+len),
                          "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                          "<td>-</td><td>-</td></tr>\r\n",
                          MYBUF(pcgi)->xcnt,state[tsoc->State]);
        }
        else if (tsoc->State == TCP_STATE_LISTEN) {
          len += sprintf ((char *)(buf+len),
                          "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                          "<td>%d</td><td>-</td></tr>\r\n",
                          MYBUF(pcgi)->xcnt,state[tsoc->State],tsoc->LocPort);
        }
        else {
          len += sprintf ((char *)(buf+len),
                          "<td>%d</td><td>%s</td><td>%d.%d.%d.%d</td>"
                          "<td>%d</td><td>%d</td><td>%d</td></tr>\r\n",
                          MYBUF(pcgi)->xcnt,state[tsoc->State],
                          tsoc->RemIpAdr[0],tsoc->RemIpAdr[1],
                          tsoc->RemIpAdr[2],tsoc->RemIpAdr[3],
                          tsoc->RemPort,tsoc->LocPort,tsoc->AliveTimer);
        }
        /* Repeat for all TCP Sockets. */
        if (MYBUF(pcgi)->xcnt == tcp_NumSocks) {
          break;
        }
      }
      if (MYBUF(pcgi)->xcnt < tcp_NumSocks) {
        /* Hi bit is a repeat flag. */
        len |= 0x8000;
      }
      break;
 //==============================================
    case 'd':
      /* base_infomation - file 'enter.cgi','main_page.cgi' */
      switch (env[2]) {
        case '1':
					//str_temp[]=device_info;    //待改
				  /* 基本信息－设备信息. 				*/			  

          break;
        case '2':
          /* 基本信息－设备地点. 				*/        //待改
          len = sprintf((char *)buf,(const char *)&env[4],
                        machine_addr);
					break;
        case '3':
          /* 基本信息－设备型号. 				*/        //待改
          len = sprintf((char *)buf,(const char *)&env[4],
                        xjj_machine_type);				
          break;
	
        case '4':
          /* 基本信息－MAC地址. 				*/        
        sprintf(&str_temp[0],"%x",own_hw_adr[0]);  	//十六进制数据转换为字符串
				str_temp[0]=toupper(str_temp[0]);  					//  大小转换写
				str_temp[1]=toupper(str_temp[1]);
				if(own_hw_adr[0]<0x10)   										//如果是一位的数据(十六进制),前面补0
				{
					str_temp[1]=str_temp[0];
					str_temp[0]='0';
					str_temp[2]=0;
				}
				sprintf(&str_temp[4],"%x",own_hw_adr[1]);	
				str_temp[4]=toupper(str_temp[4]);  
				str_temp[5]=toupper(str_temp[5]);
				if(own_hw_adr[1]<0x10)    
				{
					str_temp[5]=str_temp[4];
					str_temp[4]='0';
					str_temp[6]=0;
				}				
        sprintf(&str_temp[8],"%x",own_hw_adr[2]);
				str_temp[8]=toupper(str_temp[8]);  
				str_temp[9]=toupper(str_temp[9]);
				if(own_hw_adr[2]<0x10)    
				{
					str_temp[9]=str_temp[8];
					str_temp[8]='0';
					str_temp[10]=0;
				}		
				sprintf(&str_temp[12],"%x",own_hw_adr[3]);
				str_temp[12]=toupper(str_temp[12]);  					
				str_temp[13]=toupper(str_temp[13]);
				if(own_hw_adr[3]<0x10)   										
				{
					str_temp[13]=str_temp[12];
					str_temp[12]='0';
					str_temp[14]=0;
				}
        sprintf(&str_temp[16],"%x",own_hw_adr[4]);
				str_temp[16]=toupper(str_temp[16]);  					
				str_temp[17]=toupper(str_temp[17]);
				if(own_hw_adr[4]<0x10)   										
				{
					str_temp[17]=str_temp[16];
					str_temp[16]='0';
					str_temp[18]=0;
				}
				sprintf(&str_temp[20],"%x",own_hw_adr[5]);
				str_temp[20]=toupper(str_temp[20]);  					
				str_temp[21]=toupper(str_temp[21]);
				if(own_hw_adr[5]<0x10)   										
				{
					str_temp[21]=str_temp[20];
					str_temp[20]='0';
					str_temp[22]=0;
				}				
          len = sprintf((char *)buf,(const char *)&env[4],
                        &str_temp[0],&str_temp[4],&str_temp[8],
												&str_temp[12],&str_temp[16],&str_temp[20]);			
          break;			
        case '5':
          /* 基本信息－本机IP. 				*/        
          len = sprintf((char *)buf,(const char *)&env[4],LocM.IpAdr[0],
                        LocM.IpAdr[1],LocM.IpAdr[2],LocM.IpAdr[3]);			
          break;		
        case '6':
          /* 基本信息－服务器IP. 				*/    
          len = sprintf((char *)buf,(const char *)&env[4],server_ip[0],
                        server_ip[1],server_ip[2],server_ip[3]);			
          break;	
        case '7':
          /* 基本信息－服务器端口. 				*/     
				sprintf (str_temp,"%d",server_port);//server_port  str_temp
          len = sprintf((char *)buf,(const char *)&env[4],str_temp);			
          break;	
				case '8':	/*  变压器匝数比  */
					len = sprintf((char *)buf,(const char *)&env[4],
										 turn_num_lxb);
						break;
				case '9':	/*  实时电压监测	  */
						len = sprintf((char *)buf,(const char *)&env[4],
													Pwr_Vol);
						break;
				case 'a':	/*  设备型号	  */
						len = sprintf((char *)buf,(const char *)&env[4],
													machine_type);
						break;
				case 'b':	/*  是否学习成功	  */
					if((start_learning_flag == __TRUE)&&(learning_success_flag == __FALSE))
					{
						len = sprintf((char *)buf,(const char *)&env[4],
													"正在学习...");
					}
					else if(learning_success_flag == __TRUE)
					{
						len = sprintf((char *)buf,(const char *)&env[4],
													"学习成功");
					}
					else
					{
						len = sprintf((char *)buf,(const char *)&env[4],
													"未学习");
					}
						break;
					case 'c':	/*  有效通道	  */
					if(learning_success_flag == __TRUE)//学习成功，则显示学习状态
					{
						count = Flag_LampUse;
							for (j=0; j<12/3; j++)
							{
								if(count & 0x07)
								{
									if(count & 0x07 != 0x07)			//行人通道
									{
										if((count&1)==1)
											stemp[j*4] = 0x31;
										else
											stemp[j*4] = 0x30;
										
										if((count>>1&1)==1)
											stemp[j*4+1] = 0x31;
										else
											stemp[j*4+1] = 0x30;
										
										if((count>>2&1)==1)
											stemp[j*4+2] = 0x31;
										else
											stemp[j*4+2] = 0x30;
									}
									else
									{
										stemp[j*4] = '1';
										stemp[j*4+1] = '1';
										stemp[j*4+2] = '1';
									}
									stemp[j*4+3] = 0x20;
								}
								else
								{
									stemp[j*4] = '0';
									stemp[j*4+1] = '0';
									stemp[j*4+2] = '0';
									stemp[j*4+3] = 0x20;
								}
								count = count>>3;
							}
						}
						len = sprintf((char *)buf,(const char *)&env[4],
												stemp);
							break;
					case 'e':	/*  通道电流	  */					
						len = sprintf((char *)buf,(const char *)&env[4],
												standard_lamp_current[2],
												standard_lamp_current[1],
												standard_lamp_current[0],
												standard_lamp_current[5],
												standard_lamp_current[4],
												standard_lamp_current[3],
												standard_lamp_current[8],
												standard_lamp_current[7],
												standard_lamp_current[6],
												standard_lamp_current[11],
												standard_lamp_current[10],
												standard_lamp_current[9]);
							break;
					case 'f':	/*  周期时长	  */					
						len = sprintf((char *)buf,(const char *)&env[4],
												period_time);
							break;
					case 'd':	/*  阶段表	  */
							len = 0;
						os_mut_wait(table_global_mut,0xFFFF);
						if(current_stage_num>0)
						{
							for(j=0;j<current_stage_num;j++)
							{
								len = len+sprintf((char *)(buf+len),(const char *)&env[4],
														"阶段",
														j+1,
														":",
														(stage_time_table[j].relPhase>>0)&1,
														(stage_time_table[j].relPhase>>1)&1,
														(stage_time_table[j].relPhase>>2)&1,
														(stage_time_table[j].relPhase>>3)&1,
														(stage_time_table[j].relPhase>>4)&1,
														(stage_time_table[j].relPhase>>5)&1,
														(stage_time_table[j].relPhase>>6)&1,
														(stage_time_table[j].relPhase>>7)&1,
														(stage_time_table[j].stGreenTime));
							}
						}
						else
						{
							len = len+sprintf((char *)(buf+len),(const char *)&env[4],
														"阶段",
														0,":",0,0,0,0,0,0,0,0,0);
						}
						os_mut_release(table_global_mut);	
						break;
      }
      break;
 //==============================================
    case 'e':
      /* change_code  - file 'enter.cgi' */
		 switch (env[2]) {
        case '1':
						len = sprintf((char *)buf,(const char *)&env[4],
                       "admin");
          break;
        case '2':
				len = sprintf((char *)buf,(const char *)&env[4],
                      "admin");
          break;				
			}
				
      break;
//===========================================================		
    case 'g':
      /* 当前定时启动状态 */
				 switch (env[2]) {
						case 'a':
							cp_temp[0]=0;  //  每次都清零
							str_temp[0]=0;
							if(alarm_restart_flag ==__TRUE)   
							{							
								sprintf(cp_temp,"%s","定时启用-");
								strcat(str_temp,cp_temp); 
								sprintf(cp_temp,"%d",alarm_restart_hour);
								strcat(str_temp,cp_temp); 
								sprintf(cp_temp,"%s","时-");
								strcat(str_temp,cp_temp); 
								sprintf(cp_temp,"%d",alarm_restart_minute);
								strcat(str_temp,cp_temp); 
								sprintf(cp_temp,"%s","分");
								strcat(str_temp,cp_temp); 
							}
							else
								sprintf(&str_temp[0],"%s","已禁用");

							len = sprintf((char *)buf,(const char *)&env[4],
												 str_temp);
							break;		
				}

      break;
//===========================================================		
    case 'v':
      /* 组合通道设置 */
				 switch (env[2]) {
						case 'a':
							len = sprintf((char *)buf,(const char *)&env[4],
												 combine_valid_passage[0]);
							break;	
						case 'b':
							len = sprintf((char *)buf,(const char *)&env[4],
												 combine_valid_passage[1]);
							break;	
						case 'c':
							len = sprintf((char *)buf,(const char *)&env[4],
												 combine_valid_passage[2]);
							break;	
						case 'd':
							len = sprintf((char *)buf,(const char *)&env[4],
												 combine_valid_passage[3]);
							break;	
						case 'e':
							len = sprintf((char *)buf,(const char *)&env[4],
												 combine_valid_passage[4]);
							break;	
						case 'f':
							len = sprintf((char *)buf,(const char *)&env[4],
												 combine_valid_passage[5]);
							break;		
				}

      break;

    case 'x':
			switch (env[2]) {
						case '1':
						/* DHCP是否启用状态 */
										str_temp[0]=0; //  每次都清零
										if(DHCP_flag ==__TRUE)   							
											sprintf(&str_temp[0],"%s","enable");
										else
											sprintf(&str_temp[0],"%s","forbidden");

										len = sprintf((char *)buf,(const char *)&env[4],
															 str_temp);
							 break;		
						}
      break;

    case 'y':
      /* Button state - xml file 'button.cgx' */
			break;
    case 't':
      /* 应用设置－参数 */
				switch (env[2]) {
						case 'a':  	/*  变压器匝数比  */
								len = sprintf((char *)buf,(const char *)&env[4],
													 turn_num);
							break;
						case 'f':  	/*  变压器匝数比  */
								len = sprintf((char *)buf,(const char *)&env[4],
													 turn_num_lxb);
							break;
						case 'b':		/*  后备电池报警阀值(分钟)  */
						len = sprintf((char *)buf,(const char *)&env[4],
													bat_min);
							break;	
						case 'c':		/*  信号灯电流异常报警阀值(mA)  */
						len = sprintf((char *)buf,(const char *)&env[4],
													cur_min);
							break;	
						case 'd':		/*  信号灯电流上报时间(分钟)	  */
						len = sprintf((char *)buf,(const char *)&env[4],
													send_time);
							break;		
						case 'e':		/*  实时电压监测	  */
						len = sprintf((char *)buf,(const char *)&env[4],
													Pwr_Vol);
							break;												
					}					
			break;		
		
		
  }
  return ((U16)len);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
