#ifndef __FLASH_H
#define __FLASH_H
#include <stdint.h>
#include "stm32f10x.h"

void Flash_ReadEthernetPara(void);
void Flash_Read(u32 StartAddress, void* Data);
void SPI_configuration(void);
void Flash_Init(void);


//以太网参数
typedef struct _ETHERNET_PARAMETER
{
     u32   Local_ip;                  //结点地址长度
     u32   Net_mask;
     u32   Gate_way;
     u32   Remote_ip;
     u32   port;
     u32   MAC[6];
     u32   DNS;
}ETHERNET_PARAMETER;


//flash使用标记 
#define FLASH_ETHERNET_PARA    ((u32)0x0000)                 //Page 124 of 127     
#define FLASH_USED_FLAG        ((u32)0x0000)                 //必须为0xBC
#define FLASH_USER_NAME        ((u32)FLASH_USED_FLAG+4)         //用户名 16Byte
#define FLASH_USER_PASSWORD    ((u32)FLASH_USER_NAME+4*16)      //密码   16Byte
#define ETHERNET_LOCAL_IP      ((u32)FLASH_USER_PASSWORD+4*16)  //本地IP
#define ETHERNET_NET_MASK      ((u32)ETHERNET_LOCAL_IP+4)       //子网掩码
#define ETHERNET_GATEWAY       ((u32)ETHERNET_NET_MASK+4)       //网关
#define ETHERNET_REMOTE_IP     ((u32)ETHERNET_GATEWAY+4)        //远程主机IP 
#define ETHERNET_PORT          ((u32)ETHERNET_REMOTE_IP+4)      //端口号
#define ETHERNET_DNS           ((u32)ETHERNET_PORT+4)           //DNS 
#define ETHERNET_MAC           ((u32)ETHERNET_DNS+4)            //MAC  

#endif