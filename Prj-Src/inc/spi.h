#ifndef __SPI_H
#define __SPI_H
//8K Byte容量
#define EEP_READ     (0x03)
#define EEP_WRITE    (0x02)
#define EEP_WREN     (0x06)
#define EEP_WRDI     (0x04)
#define EEP_RDSR     (0x05)
#define EEP_WRSR     (0x01)

// // void EEP_ss (u32 ss);
// // u8   EEP_send (u8 outb);
// // u8   EEP_ReadStatusReg(void);
// // void EEP_WriteEnable(void);
// // void EEP_Program(u16 Addr,u32 size,u8* buf);
// // void EEP_ReadOut(u16 Addr, u8* buf);

extern void Wr_u8(u16 Addr,u8 dat);		  //写一个字节数据到指定地址
extern u8 Rd_u8(u16 Addr);		   //从指定地址读一个字节
void EEP_erase_ALL(void);	
#endif
