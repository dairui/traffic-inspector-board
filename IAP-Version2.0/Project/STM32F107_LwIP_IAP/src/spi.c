
#include "flash.h"
#include "flash_if.h"
#include "stm32f10x_bkp.h"
#include "string.h"
#include "spi.h"

//======================================================================
// 函数名称：
// spi_ss(void)
//			 
// 说明：NSS控制
//======================================================================
void SPI_EEPROM_ss (u32 ss) 
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
u8 SPI_EEPROM_send (u8 outb) 
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
u8 SPI_EEPROM_ReadStatusReg(void)
 {  
    u8 status=0;

    SPI_EEPROM_ss(0);
                                     // 片选变低
    SPI_EEPROM_send(EEPROM_RDSR);

	status = SPI_EEPROM_send(EEPROM_RDSR);

	SPI_EEPROM_ss(1);

	return status;
   
 }
//======================================================================
// 函数名称：
// WriteEnable(void)
//			 
// 说明：
//======================================================================
void SPI_EEPROM_WriteEnable(void)
 { 
     SPI_EEPROM_ss(0);
                                     // 片选变低
     SPI_EEPROM_send(EEPROM_WREN);
 
     SPI_EEPROM_ss(1);   
 }

//======================================================================
// 函数名称：
// PageProgram(void)
//			 
// 说明：最大写入32字节
//====================================================================== 
void SPI_EEPROM_Program(u16 Addr,u32 size,u8* buf)
{
     u16 i = 0;

     while((SPI_EEPROM_ReadStatusReg()&0x01 == 1) && (i <= 65530))
     {
        i++;
     }     

     SPI_EEPROM_WriteEnable();      //写允许
     
     SPI_EEPROM_ss(0);
                                     
     SPI_EEPROM_send(EEPROM_WRITE);

     SPI_EEPROM_send((Addr&0xFF00)>>8);

     SPI_EEPROM_send((Addr&0x00FF)>>0);

     for (i=0; i<size; i++)
     {
        SPI_EEPROM_send(buf[i]);
     }
 
     SPI_EEPROM_ss(1);  
}

//======================================================================
// 函数名称：
// ReadOut(void)
//			 
// 说明：读一个字节
//====================================================================== 
void SPI_EEPROM_ReadOut(u16 Addr, u8* buf)
{
     SPI_EEPROM_ss(0);
                                     
     SPI_EEPROM_send(EEPROM_READ);

     SPI_EEPROM_send((Addr&0xFF00)>>8);

     SPI_EEPROM_send((Addr&0x00FF)>>0);

     *buf = SPI_EEPROM_send(0x00);
 
     SPI_EEPROM_ss(1); 
}






