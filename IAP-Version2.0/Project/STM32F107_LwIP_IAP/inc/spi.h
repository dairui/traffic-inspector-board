#ifndef __SPI_H
#define __SPI_H
//8M Byte容量
//4KB/sector
//共有2048个sector
#define EEPROM_READ     (0x03)
#define EEPROM_WRITE    (0x02)
#define EEPROM_WREN     (0x06)
#define EEPROM_WRDI     (0x04)
#define EEPROM_RDSR     (0x05)
#define EEPROM_WRSR     (0x01)

void SPI_EEPROM_ss (u32 ss);
u8   SPI_EEPROM_send (u8 outb);
u8   SPI_EEPROM_ReadStatusReg(void);
void SPI_EEPROM_WriteEnable(void);
void SPI_EEPROM_Program(u16 Addr,u32 size,u8* buf);
void SPI_EEPROM_ReadOut(u16 Addr, u8* buf);

#endif
