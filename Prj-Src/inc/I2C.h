#ifndef __I2C_H
#define __I2C_H

#include "include.h"

//======================================================================
//                          I2C MACRO DEFINE
//======================================================================
#define I2C_SDA1_1        (GPIOB->BSRR = GPIO_Pin_7)
#define I2C_SDA1_0        (GPIOB->BRR = GPIO_Pin_7)

#define I2C_SCL1_1        (GPIOB->BSRR = GPIO_Pin_6)
#define I2C_SCL1_0        (GPIOB->BRR = GPIO_Pin_6)

#define I2C_SDA1_Read     ((GPIOB->IDR & GPIO_Pin_7) >> 7)

//======================================================================
//                         FUNCTION PROTOTYPE
//======================================================================
u8 I2C_Write_DS3231(u8 *tempdata);

u8 I2C_Read_DS3231(u8 *tempdata);

u8 I2C_Write_Alarm(u8 *tempdata);		//设定闹钟的秒、分、时、日期

void I2C_Write_Alarm_Register(u8 data);

void Compare_Time(void);

void I2C_Clear_Alarm_Register(void);

void I2C_Write_FlickerSecond(void);

void I2C_Write_AllredSecond(void);

u8 I2C_Read_DS3231_Reg(u8 reg);

float I2C_Get_Temperature(void);

#endif

