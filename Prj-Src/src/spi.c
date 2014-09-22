
#include "include.h"

//======================================================================
// �������ƣ�
// spi_ss(void)
//			 
// ˵����NSS����
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
// �������ƣ�
// spi_send(void)
//			 
// ˵��������
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
// �������ƣ�
// ReadStatusReg(void)
//			 
// ˵����
//======================================================================
u8 EEP_ReadStatusReg(void)
 {  
	u8 status=0;

	EEP_ss(0);
																	 // Ƭѡ���
	EEP_send(EEP_RDSR);

	status = EEP_send(EEP_RDSR);

	EEP_ss(1);

	return status;
   
 }
//======================================================================
// �������ƣ�
// WriteEnable(void)
//			 
// ˵����
//======================================================================
void EEP_WriteEnable(void)
 { 
     EEP_ss(0);
                                     // Ƭѡ���
     EEP_send(EEP_WREN);
 
     EEP_ss(1);   
 }
//================================
//���ݿ�������
//=================================
void Wr_u8(u16 Addr,u8 dat)		  //дһ���ֽ����ݵ�ָ����ַ
{   
	 u16 i = 0;

	 while((EEP_ReadStatusReg()&0x01 == 1) && (i <= 65530))
	 {
			i++;
	 }     

	 EEP_WriteEnable();      //д����
	 
	 EEP_ss(0);
																	 
	 EEP_send(EEP_WRITE);
	 EEP_send((Addr&0xFF00)>>8);
	 EEP_send((Addr&0x00FF)>>0);
	 EEP_send(dat); 

	 EEP_ss(1);  
}


//=====================================
//���ݿ�������
//=====================================
u8 Rd_u8(u16 Addr)		   //��ָ����ַ��һ���ֽ�
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

void EEP_erase_ALL(void)		  //��EEPROM�0
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
		
		EEP_WriteEnable();      //д���� 
		
		EEP_ss(0);

		EEP_send(EEP_WRITE);
		EEP_send((Addr&0xFF00)>>8);
		EEP_send((Addr&0x00FF)>>0);

		for(i=0;i<32;i++) EEP_send(0);

		EEP_ss(1); 
	}		 
}




