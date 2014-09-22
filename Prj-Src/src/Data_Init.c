
#include "include.h"
//========================================================
void save_flag(uint16_t write_addr, uint8_t flag_value,  uint8_t bit)
{
	uint8_t write_temp=0;

	write_temp =Rd_u8(write_addr);	
	if(flag_value !=0)
		write_temp |= (0x01<<bit);
	else 
		write_temp &= (~(0x01<<bit));

	Wr_u8(write_addr,write_temp) ;	
}
//-----------------------------------------
uint8_t read_flag(uint16_t read_addr, uint8_t bit)
{
	uint8_t read_temp=0;

	read_temp=Rd_u8(read_addr);	
	
	if((read_temp & (0x01<<bit)) !=0)
		return 1;
	else 
		return 0;
}
//========================================================
/*
*  
*	以下这两个函数可以搞定大部分的参数save和read。
*
*/
//========================================================
void save_param(uint16_t write_addr, uint8_t *pTemp,  uint8_t len)
{
	uint16_t i;

	for(i=0;i<len;i++)
	{
		Wr_u8(write_addr+i,*(pTemp+i)) ;	
	}	
// 	return 0;
}
//--------------------------
void read_param(uint16_t read_addr, uint8_t *pTemp,  uint8_t len)
{
	int i;

	for(i=0;i<len;i++)
	{
		*(pTemp+i)=Rd_u8(read_addr+i) ;	
	}		
}
//========================================================
/**
  * @brief  
  * @param  
  * @retval 
  */
void save_default_parameter(void)
{
	uint32_t i=0;
		/*
		读蓄电池放电月
	*/	
	save_param(DATE_MONTH,	(uint8_t *)&Dtae_Month,sizeof(Dtae_Month));
	save_param(CHARGE_TIME,	(uint8_t *)&Para_Charge,sizeof(Para_Charge));
	/*
	
		
	/*
		说明，以下的参数，xxx是参变量，而 r_xxx 是只读的变量, 即出厂化设置的值。
	*/	
	localm[NETIF_ETH] 	= r_ip_config;  			//   拷贝参数
	local_port 	= r_local_port;		
	for(i=0;i<6;i++)
		mac_adr[i] = r_mac_adr[i];	
	for(i=0;i<4;i++)
		server_ip[i] = r_server_ip[i];
	server_port=r_server_port;
	
	for(i=0;i<4;i++)
		myself_server_ip[i] = r_myself_server_ip[i];
	myself_server_port=r_myself_server_port;
	
	save_param(LOCAL_IP_ADDR,				LocM.IpAdr,				sizeof(LocM.IpAdr));
	save_param(LOCAL_MASK_ADDR,			LocM.NetMask,			sizeof(LocM.NetMask));
	save_param(LOCAL_GATEWAY_ADDR,	LocM.DefGW,				sizeof(LocM.DefGW));
	save_param(LOCAL_PORT_ADDR,			(uint8_t *)&local_port,	sizeof(local_port));
	save_param(LOCAL_MAC_ADDR,			mac_adr,								sizeof(mac_adr));
	save_param(LOCAL_DNS_ADDR,			LocM.PriDNS,			sizeof(LocM.PriDNS));
	save_param(SERVER_IP_ADDR,			server_ip,							sizeof(server_ip));
	save_param(SERVER_PORT_ADDR,		(uint8_t *)&server_port,	sizeof(server_port));	
	save_param(SERVER_IP2_ADDR,			myself_server_ip,			sizeof(myself_server_ip));
	save_param(SERVER_PORT2_ADDR,		(uint8_t *)&myself_server_port,		sizeof(myself_server_port));
	
	//-------------------------------------------		
	for(i=0;i<sizeof(password);i++)
		password[i]	=	r_password[i];
	alarm_restart_hour 		= r_alarm_restart_hour;
	alarm_restart_minute 	= r_alarm_restart_minute;
	
	save_param(PASSWORD_ADDR,				password,								sizeof(password));
	save_param(ALARM_RE_HOUR_ADDR,	&alarm_restart_hour,		sizeof(alarm_restart_hour));
	save_param(ALARM_RE_minu_ADDR,	&alarm_restart_minute,	sizeof(alarm_restart_minute));
	//-------------------------------------------		
	turn_num	= r_turn_num;
	turn_num_lxb	= r_turn_num_lxb;
	bat_min		= r_bat_min;
	cur_min		= r_cur_min;
	send_time	= r_send_time;

	save_param(TURN_NUM_ADDR,				(uint8_t *)&turn_num,		sizeof(turn_num));
	save_param(REAL_TURN_NUM_ADDR,	(uint8_t *)&turn_num_lxb,		sizeof(turn_num_lxb));
	save_param(BAT_MIN_ADDR,				(uint8_t *)&bat_min,		sizeof(bat_min));
	save_param(CUR_MIN_ADDR,				(uint8_t *)&cur_min,		sizeof(cur_min));
 	save_param(SEND_TIME_ADDR,			(uint8_t *)&send_time,	sizeof(send_time));
	//-------------------------------------------		
	machine_addr_len = r_machine_addr_len;	
	machine_type_len = r_machine_type_len;
	for(i=0;i<sizeof(machine_addr);i++)
		machine_addr[i] = r_machine_addr[i];
	for(i=0;i<sizeof(machine_type);i++)
		machine_type[i] = r_machine_type[i];
		
 	save_param(MACHINE_ADDR_ADDR,		&machine_addr_len,						sizeof(machine_addr_len));
 	save_param(MACHINE_ADDR_ADDR+1,		machine_addr,						sizeof(machine_addr));
 	save_param(MACHINE_TYPE_ADDR,		&machine_type_len,						sizeof(machine_type_len));
 	save_param(MACHINE_TYPE_ADDR+1,		machine_type,						sizeof(machine_type));
	//-------------------------------------------	
}
//========================================================
/**
  * @brief  
  * @param  
  * @retval 
  */
void read_all_parameter(void)
{
	
	/*
		读蓄电池放电月
	*/	
	read_param(DATE_MONTH,	(uint8_t *)&Dtae_Month,sizeof(Dtae_Month));
	read_param(CHARGE_TIME,	(uint8_t *)&Para_Charge,sizeof(Para_Charge));
	
	/*
		存储有效通道组合表
	*/	
	read_param(VALID_PASSAGE_COMBINE,	(uint8_t *)&combine_valid_passage,sizeof(combine_valid_passage));
	
	alarm_restart_flag  =read_flag(ALARM_RE_FLAG_ADDR,  		ALARM_RE_FLAG_BIT);
	DHCP_flag 					=read_flag(DHCP_FLAG_ADDR,  				DHCP_FLAG_BIT);
	
	
	read_param(LOCAL_IP_ADDR,				LocM.IpAdr,				sizeof(LocM.IpAdr));
	read_param(LOCAL_MASK_ADDR,			LocM.NetMask,			sizeof(LocM.NetMask));
	read_param(LOCAL_GATEWAY_ADDR,	LocM.DefGW,				sizeof(LocM.DefGW));
	read_param(LOCAL_PORT_ADDR,			(uint8_t *)&local_port,	sizeof(local_port));
	read_param(LOCAL_MAC_ADDR,			mac_adr,								sizeof(mac_adr));
	read_param(LOCAL_DNS_ADDR,			LocM.PriDNS,			sizeof(LocM.PriDNS));
	read_param(SERVER_IP_ADDR,			server_ip,							sizeof(server_ip));
	read_param(SERVER_PORT_ADDR,		(uint8_t *)&server_port,	sizeof(server_port));	
	read_param(SERVER_IP2_ADDR,			myself_server_ip,			sizeof(myself_server_ip));
	read_param(SERVER_PORT2_ADDR,		(uint8_t *)&myself_server_port,		sizeof(myself_server_port));
	//-------------------------------------------		
	read_param(PASSWORD_ADDR,				password,								sizeof(password));
	read_param(ALARM_RE_HOUR_ADDR,	&alarm_restart_hour,		sizeof(alarm_restart_hour));
	read_param(ALARM_RE_minu_ADDR,	&alarm_restart_minute,	sizeof(alarm_restart_minute));
	//-------------------------------------------		
	read_param(TURN_NUM_ADDR,				(uint8_t *)&turn_num,		sizeof(turn_num));
	read_param(REAL_TURN_NUM_ADDR,	(uint8_t *)&turn_num_lxb,	sizeof(turn_num_lxb));
	read_param(BAT_MIN_ADDR,				(uint8_t *)&bat_min,		sizeof(bat_min));
	read_param(CUR_MIN_ADDR,				(uint8_t *)&cur_min,		sizeof(cur_min));
 	read_param(SEND_TIME_ADDR,			(uint8_t *)&send_time,	sizeof(send_time));
	//-------------------------------------------		
 	read_param(MACHINE_ADDR_ADDR,		&machine_addr_len,						sizeof(machine_addr_len));
 	read_param(MACHINE_ADDR_ADDR+1,		machine_addr,						sizeof(machine_addr));
 	read_param(MACHINE_TYPE_ADDR,		&machine_type_len,						sizeof(machine_type_len));
 	read_param(MACHINE_TYPE_ADDR+1,		machine_type,						sizeof(machine_type));
	//-------------------------------------------	
	
	BKP_WriteBackupRegister(BKP_DR1,((LocM.IpAdr[0]&0x000000FF)<<8) | ((LocM.IpAdr[1]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR2,((LocM.IpAdr[2]&0x000000FF)<<8) | ((LocM.IpAdr[3]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR3,((LocM.NetMask[0]&0x000000FF)<<8) | ((LocM.NetMask[1]&0x0000FF00)));
	BKP_WriteBackupRegister(BKP_DR4,((LocM.NetMask[2]&0x000000FF)<<8) | ((LocM.NetMask[3]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR5,((LocM.DefGW[0]&0x000000FF)<<8) | ((LocM.DefGW[1]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR6,((LocM.DefGW[2]&0x000000FF)<<8) | ((LocM.DefGW[3]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR7,((server_ip[0]&0x000000FF)<<8) | ((server_ip[1]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR8,((server_ip[2]&0x000000FF)<<8) | ((server_ip[3]&0x000000FF)));
	BKP_WriteBackupRegister(BKP_DR9, local_port);
	BKP_WriteBackupRegister(BKP_DR10, 69);
}
//======================================================
