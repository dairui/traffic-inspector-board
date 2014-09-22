
#include "include.h"

//======================================================================
// 函数名称：
// spi_ss(void)
//			 
// 说明：NSS控制
//======================================================================
void EEP_ss (u32 ss) 
{
   /* Enable/Disable SPI Chip Select (drive it high or low). */

   if (ss == 1)
   {
        GPIO_SetBits(GPIOA, GPIO_Pin_11);
   }
   else
   {
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
   }
}


//======================================================================
// 函数名称：
// spi_send(void)
//			 
// 说明：发送
//======================================================================
u8 EEP_send (u8 outb) 
{
   /* Write and Read a byte on SPI interface. */

   /* Wait if TXE cleared, Tx FIFO is full. */
   while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
   SPI_I2S_SendData(SPI1, outb);

   /* Wait if RNE cleared, Rx FIFO is empty. */    
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); 
    return SPI_I2S_ReceiveData(SPI1);   
}

//======================================================================
// 函数名称：
// ReadStatusReg(void)
//			 
// 说明：
//======================================================================
u8 EEP_ReadStatusReg(void)
 {  
	u8 status=0;

	EEP_ss(0);
																	 // 片选变低
	EEP_send(EEP_RDSR);

	status = EEP_send(EEP_RDSR);

	EEP_ss(1);

	return status;
   
 }
//======================================================================
// 函数名称：
// WriteEnable(void)
//			 
// 说明：
//======================================================================
void EEP_WriteEnable(void)
 { 
     EEP_ss(0);
                                     // 片选变低
     EEP_send(EEP_WREN);
 
     EEP_ss(1);   
 }
//================================
//数据库用驱动
//=================================
void Wr_u8(u16 Addr,u8 dat)		  //写一个字节数据到指定地址
{   
	 u16 i = 0;

	 while((EEP_ReadStatusReg()&0x01 == 1) && (i <= 65530))
	 {
			i++;
	 }     

	 EEP_WriteEnable();      //写允许
	 
	 EEP_ss(0);
																	 
	 EEP_send(EEP_WRITE);
	 EEP_send((Addr&0xFF00)>>8);
	 EEP_send((Addr&0x00FF)>>0);
	 EEP_send(dat); 

	 EEP_ss(1);  
}


//=====================================
//数据库用驱动
//=====================================
u8 Rd_u8(u16 Addr)		   //从指定地址读一个字节
{ 
  u8 data = 0;	
	u16 i = 0;
	
	while((EEP_ReadStatusReg()&0x01 == 1) && (i <= 65530))
	{
		 i++;
	}     	
	
	EEP_ss(0);
																 
	EEP_send(EEP_READ);
	EEP_send((Addr&0xFF00)>>8);
	EEP_send((Addr&0x00FF)>>0);
	data = EEP_send(0x00);
	
	EEP_ss(1); 
	
  return data;
}

void EEP_erase_ALL(void)		  //把EEPROM填补0
{   
	u16 i = 0;
	u16 j = 0;
	u16 Addr;

  
	for(j=0;j<0x2000;j+=32)
	{
		while((EEP_ReadStatusReg()&0x01 == 1) && (i <= 65530))
		{
			i++;
		} 
		Addr =j;
		
		EEP_WriteEnable();      //写允许 
		
		EEP_ss(0);

		EEP_send(EEP_WRITE);
		EEP_send((Addr&0xFF00)>>8);
		EEP_send((Addr&0x00FF)>>0);

		for(i=0;i<32;i++) EEP_send(0);

		EEP_ss(1); 
	}		 
}




