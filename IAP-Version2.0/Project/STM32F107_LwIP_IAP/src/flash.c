
#include "flash.h"
#include "flash_if.h"
#include "stm32f10x_bkp.h"
#include "string.h"
#include "spi.h"
ETHERNET_PARAMETER   Ethernet_Parameter;


//======================================================================
// 函数名称：
// Flash_ReadEthernetPara
//			 
// 说明：读以太网参数
//======================================================================
void Flash_ReadEthernetPara(void)
{ 
    u8 j;
    
    Flash_Read(ETHERNET_LOCAL_IP, &Ethernet_Parameter.Local_ip);    
    Flash_Read(ETHERNET_NET_MASK, &Ethernet_Parameter.Net_mask); 
    Flash_Read(ETHERNET_GATEWAY, &Ethernet_Parameter.Gate_way); 
    Flash_Read(ETHERNET_REMOTE_IP, &Ethernet_Parameter.Remote_ip); 
    Flash_Read(ETHERNET_PORT, &Ethernet_Parameter.port); 
    Flash_Read(ETHERNET_DNS, &Ethernet_Parameter.DNS); 

    for (j=0; j<6; j++)
    {
        Flash_Read(ETHERNET_MAC+4*j, &Ethernet_Parameter.MAC[j]); 
    }	

	BKP_WriteBackupRegister(BKP_DR1,((Ethernet_Parameter.Local_ip&0x000000FF)<<8) | ((Ethernet_Parameter.Local_ip&0x0000FF00)>>8));
	BKP_WriteBackupRegister(BKP_DR2,((Ethernet_Parameter.Local_ip&0x00FF0000)>>8) | ((Ethernet_Parameter.Local_ip&0xFF000000)>>24));
	BKP_WriteBackupRegister(BKP_DR3,((Ethernet_Parameter.Net_mask&0x000000FF)<<8) | ((Ethernet_Parameter.Net_mask&0x0000FF00)>>8));
	BKP_WriteBackupRegister(BKP_DR4,((Ethernet_Parameter.Net_mask&0x00FF0000)>>8) | ((Ethernet_Parameter.Net_mask&0xFF000000)>>24));
	BKP_WriteBackupRegister(BKP_DR5,((Ethernet_Parameter.Gate_way&0x000000FF)<<8) | ((Ethernet_Parameter.Gate_way&0x0000FF00)>>8));
	BKP_WriteBackupRegister(BKP_DR6,((Ethernet_Parameter.Gate_way&0x00FF0000)>>8) | ((Ethernet_Parameter.Gate_way&0xFF000000)>>24));
	BKP_WriteBackupRegister(BKP_DR7,((Ethernet_Parameter.Remote_ip&0x000000FF)<<8) | ((Ethernet_Parameter.Remote_ip&0x0000FF00)>>8));
	BKP_WriteBackupRegister(BKP_DR8,((Ethernet_Parameter.Remote_ip&0x00FF0000)>>8) | ((Ethernet_Parameter.Remote_ip&0xFF000000)>>24));
	BKP_WriteBackupRegister(BKP_DR9, Ethernet_Parameter.port);	
	//BKP_WriteBackupRegister(BKP_DR10, DHCP);    
}

//======================================================================
// 函数名称：
// Flash_Read
//			 
// 说明：读FLASH
//======================================================================
void Flash_Read(u32 StartAddress, void* Data)
{
    u8 i, data[4] = {0};
    for (i=0; i<4; i++)
    {
        SPI_EEPROM_ReadOut(StartAddress+i, &data[i]);
    }

    *(u32*)Data = 0x00000000 | (u32)data[0] | ((u32)data[1])<<8 | ((u32)data[2])<<16 | ((u32)data[3])<<24;
}

//======================================================================
// 函数名称：
// Flash_Read
//			 
// 说明：读FLASH
//======================================================================
void Flash_Init(void)
{ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef GPIO_init;       
    GPIO_init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;    
    
    //MOSI SCK
    GPIO_init.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_init.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_Init(GPIOA, &GPIO_init);
    
    //MISO
    GPIO_init.GPIO_Pin = GPIO_Pin_6;
    GPIO_init.GPIO_Mode = GPIO_Mode_IPU;     
    GPIO_Init(GPIOA, &GPIO_init);

    //NSS
    GPIO_init.GPIO_Pin = GPIO_Pin_11;
    GPIO_init.GPIO_Mode = GPIO_Mode_Out_PP;     
    GPIO_Init(GPIOA, &GPIO_init);  
    
    SPI_configuration();
}

//======================================================================
// 函数名称：
// SPI1_configuration(void)
//			 
// 说明：SPI1配置
//======================================================================
void SPI_configuration(void)
{
    SPI_InitTypeDef spi1_init;

    SPI_I2S_DeInit(SPI1);

    spi1_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //双线全双工 
    spi1_init.SPI_Mode = SPI_Mode_Master;                        //主模式
    spi1_init.SPI_NSS = SPI_NSS_Soft;                            //硬件配置
    spi1_init.SPI_FirstBit = SPI_FirstBit_MSB;                   //低位在先
    spi1_init.SPI_DataSize = SPI_DataSize_8b;                    //字节发送
    spi1_init.SPI_CPOL = SPI_CPOL_Low;                           //低电平空闲
    spi1_init.SPI_CPHA = SPI_CPHA_1Edge;                         //第一个沿采样
    spi1_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //预分频
    spi1_init.SPI_CRCPolynomial = 7;                             //多项式次数

    SPI_Init(SPI1, &spi1_init);                                  //初始化

    SPI_SSOutputCmd(SPI1, ENABLE);                               //NSS引脚设置，正常I/O口 

    GPIO_SetBits(GPIOA, GPIO_Pin_11);                            //NSS初始化为高电平

    SPI_Cmd(SPI1, ENABLE);                                       //使能
}
