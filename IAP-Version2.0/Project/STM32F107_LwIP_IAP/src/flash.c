
#include "flash.h"
#include "flash_if.h"
#include "stm32f10x_bkp.h"
#include "string.h"
#include "spi.h"
ETHERNET_PARAMETER   Ethernet_Parameter;


//======================================================================
// �������ƣ�
// Flash_ReadEthernetPara
//			 
// ˵��������̫������
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
// �������ƣ�
// Flash_Read
//			 
// ˵������FLASH
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
// �������ƣ�
// Flash_Read
//			 
// ˵������FLASH
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
// �������ƣ�
// SPI1_configuration(void)
//			 
// ˵����SPI1����
//======================================================================
void SPI_configuration(void)
{
    SPI_InitTypeDef spi1_init;

    SPI_I2S_DeInit(SPI1);

    spi1_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //˫��ȫ˫�� 
    spi1_init.SPI_Mode = SPI_Mode_Master;                        //��ģʽ
    spi1_init.SPI_NSS = SPI_NSS_Soft;                            //Ӳ������
    spi1_init.SPI_FirstBit = SPI_FirstBit_MSB;                   //��λ����
    spi1_init.SPI_DataSize = SPI_DataSize_8b;                    //�ֽڷ���
    spi1_init.SPI_CPOL = SPI_CPOL_Low;                           //�͵�ƽ����
    spi1_init.SPI_CPHA = SPI_CPHA_1Edge;                         //��һ���ز���
    spi1_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //Ԥ��Ƶ
    spi1_init.SPI_CRCPolynomial = 7;                             //����ʽ����

    SPI_Init(SPI1, &spi1_init);                                  //��ʼ��

    SPI_SSOutputCmd(SPI1, ENABLE);                               //NSS�������ã�����I/O�� 

    GPIO_SetBits(GPIOA, GPIO_Pin_11);                            //NSS��ʼ��Ϊ�ߵ�ƽ

    SPI_Cmd(SPI1, ENABLE);                                       //ʹ��
}
