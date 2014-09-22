
#include "Include.h" 

//======================================================================
//                          FUNCTION PROTOTYPE
//======================================================================
//I2C DS3231
static void I2C_start_DS3231(void);

static void I2C_Stop_DS3231(void);

static void I2C_Write_Byte_DS3231(u8 tempdata);

static u8 I2C_Read_Byte_DS3231(void);

static void I2C_Send_Ack_DS3231(u8 ack);

static u8 I2C_Receive_Ack_DS3231(void);

//======================================================================
// 函数名称：
// I2C_start(void)
//
// 说明：I2C等待时间2us
//======================================================================
static void I2C_Delay(void)
{
    u8 i = 35;
    while(i > 0)
    {
        i--; 
    }
}

//======================================================================
// 函数名称：
// I2C_start(void)
//
// 说明：i2c开始工作
//======================================================================
static void I2C_start_DS3231(void)
{
    I2C_SDA1_1;
    I2C_SCL1_1;
    I2C_Delay();

    I2C_SDA1_0;
    I2C_Delay();

    I2C_SCL1_0;
    I2C_Delay();

}

//======================================================================
// 函数名称：
// I2C_Write_Byte(void)
//
// 说明：传送1字节数据
//======================================================================

static void I2C_Write_Byte_DS3231(u8 tempdata)
{
    u8 i;
    for (i=0; i<8; i++)
    {        
        if((tempdata & 0x80) == 0x80)
        {
            I2C_SDA1_1;
        }
        else
        {
            I2C_SDA1_0;
        }
        I2C_Delay();

        I2C_SCL1_1;
        I2C_Delay();

        I2C_SCL1_0;
        I2C_Delay();

        tempdata = tempdata << 1;
    }

    I2C_SDA1_1;
    I2C_Delay();    
}




//======================================================================
// 函数名称：
// I2C_Read_Byte(void)
//
// 说明：读取1字节数据
//======================================================================
static u8 I2C_Read_Byte_DS3231(void)
{
    u8 i;
    u8 tempdata = 0;
    
    for (i=0; i<8; i++)
    {
        I2C_SCL1_1;
        I2C_Delay();

        tempdata |= I2C_SDA1_Read;
        I2C_Delay();

        I2C_SCL1_0;
        I2C_Delay();

        if (i < 7)
        {
            tempdata = tempdata << 1;
        }
        
    }

    I2C_SDA1_1;
    I2C_Delay();

    return tempdata;  
}


//======================================================================
// 函数名称：
// I2C_Send_Ack(u8 ack)
//
// 说明：发送确认标识
//======================================================================
static void I2C_Send_Ack_DS3231(u8 ack)
{
    if (ack == 1)
    {
        I2C_SDA1_1;
        I2C_Delay();

        I2C_SCL1_1;
        I2C_Delay();
    }
    else
    {
        I2C_SDA1_0;
        I2C_Delay();

        I2C_SCL1_1;
        I2C_Delay();
    }	
	
    I2C_SCL1_0;
    I2C_Delay();

    //释放总线
    I2C_SDA1_1;
    I2C_Delay();
}


//======================================================================
// 函数名称：
// I2C_Receive_Ack(void)
//
// 说明：检测确认标识
//======================================================================
static u8 I2C_Receive_Ack_DS3231(void)
{    
    u8 ack = 0;

    I2C_SCL1_1;
    I2C_Delay();
    
    ack = I2C_SDA1_Read;
    I2C_Delay();

    I2C_SCL1_0;
    I2C_Delay();

    return ack;
}

//======================================================================
// 函数名称：
// I2C_Stop(void)
//
// 说明：I2C停止发送
//======================================================================
static void I2C_Stop_DS3231(void)
{
    I2C_SDA1_0;
    I2C_Delay();
    
    I2C_SCL1_1;    
    I2C_Delay();

    I2C_SDA1_1;
    I2C_Delay();
}

//======================================================================
// 函数名称：
// I2C_Write_DS3231(u8 adress, u8 *tempdata)
//
// 说明：向指定地址写入数据 MSB年。。。秒LSB
//======================================================================
u8 I2C_Write_DS3231(u8 *tempdata)
{
    u8 tx_count = 0;
    u8 flag = 0;
    u8 jump = 0;
		u8 i = 0;

    //错误重发三次
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //器件地址+写
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x00);           //访问地址
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<7; i++)                 //7字节数据
        {
            if (i == 2)
            {
                tempdata[i] &= ~0x40;
            }

            if (i == 5)
            {
                tempdata[i] &= ~0x80;
            }            
        
            I2C_Write_Byte_DS3231(tempdata[i]);
            if (I2C_Receive_Ack_DS3231() == 1)
            {
              jump = 1;
              break;
            }
        }

        if (jump == 1)
        {
            tx_count++;
            jump = 0;
            continue;
        } 
            
        I2C_Stop_DS3231();
        
        flag = 1;
        break;       
    }
    return flag;
}

//======================================================================
// 函数名称：
// u8 I2C_Read_DS3231(u8 adress, u8 *tempdata)
//
// 说明：向指定地址读取数据   MSB年。。。秒LSB
//======================================================================

u8 I2C_Read_DS3231(u8 *tempdata)
{
    u8 tx_count = 0;
    u8 flag = 0, i = 0;    

    //错误重发三次
    while(tx_count < 3)       
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //器件地址+写
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x00);            //改访问地址指针
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        I2C_Stop_DS3231();        
    
        I2C_start_DS3231();                     

        I2C_Write_Byte_DS3231(0xD1);            //器件地址+读
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<7; i++)                 //7字节数据
        {
            tempdata[i] = I2C_Read_Byte_DS3231();
            if (i < 6)
            {
                I2C_Send_Ack_DS3231(0);
            }
            else               
            {
                I2C_Send_Ack_DS3231(1);
            }

            if (i == 2)
            {
                tempdata[i] &= ~0x40;
            }

            if (i == 5)
            {
                tempdata[i] &= ~0x80;
            }              
        }
            
        I2C_Stop_DS3231();
        
        flag = 1;
        break;
    }
	tempdata[6] = tempdata[6]/16 * 10 + tempdata[6] % 16;//年"16进制转成10进制"
	tempdata[5] = tempdata[5]/16 * 10 + tempdata[5] % 16;//月
	tempdata[4] = tempdata[4]/16 * 10 + tempdata[4] % 16;//日
	tempdata[2] = tempdata[2]/16 * 10 + tempdata[2] % 16;//时
	tempdata[1] = tempdata[1]/16 * 10 + tempdata[1] % 16;//分
	tempdata[0] = tempdata[0]/16 * 10 + tempdata[0] % 16;//秒
    return flag;
}


u8 I2C_Write_Alarm(u8 *tempdata)		//设定闹钟的秒、分、时、日期
{
	u8 tx_count = 0;
	u8 flag = 0;
	u8 jump = 0;
	u8 i = 0;

	u8 Date,Hour,Minute,Seconds,Temp_Data[4] = {0};

	Seconds = tempdata[0];
	Seconds &= 0x7F;

	Minute = tempdata[1];
	Minute &= 0x7F;

	Hour = tempdata[2];
	Hour &= 0x7F;
	Hour &= 0xBF;

	Date = tempdata[3];
	Date |= 0x80;
	Date &= 0xBF;

	Temp_Data[0] = Seconds;
	Temp_Data[1] = Minute;
	Temp_Data[2] = Hour;
	Temp_Data[3] = Date;
	
	//错误重发三次
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //器件地址+写
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x07);           //访问地址
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i = 0; i < 4; i++)                 //7字节数据
        {
            I2C_Write_Byte_DS3231(Temp_Data[i]);
            if (I2C_Receive_Ack_DS3231() == 1)
            {
              jump = 1;
              break;
            }
        }

        if (jump == 1)
        {
            tx_count++;
            jump = 0;
            continue;
        } 
            
        I2C_Stop_DS3231();
        
        flag = 1;
        break;       
    }
    return flag;
	
}

void I2C_Write_Alarm_Register(u8 data)
{
		u8 tx_count = 0;
		//u8 flag = 0;
		u8 jump = 0;
		u8 i = 0;

    //错误重发三次
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //器件地址+写
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x0E);           //访问地址
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<1; i++)                 //1字节数据
        {
            I2C_Write_Byte_DS3231(data);
            if (I2C_Receive_Ack_DS3231() == 1)
            {
              jump = 1;
              break;
            }
        }

        if (jump == 1)
        {
            tx_count++;
            jump = 0;
            continue;
        } 
            
        I2C_Stop_DS3231();
        
        //flag = 1;
        break;       
    }
}

void I2C_Clear_Alarm_Register(void)
{
    u8 tx_count = 0;
    u8 jump = 0;
    //u8 flag = 0;
		u8 i = 0;

    //错误重发三次
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //器件地址+写
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x0F);           //访问地址
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<1; i++)                 //7字节数据
        {
            I2C_Write_Byte_DS3231(0x00);
            if (I2C_Receive_Ack_DS3231() == 1)
            {
              jump = 1;
              break;
            }
        }

        if (jump == 1)
        {
            tx_count++;
            jump = 0;
            continue;
        } 
            
        I2C_Stop_DS3231();
        
        //flag = 1;
        break;       
    }
}

//===========================================================
//u32 I2C_Get_Temperature(void)
//读DS3231温度
//===========================================================
float I2C_Get_Temperature(void)     /*     此函数不用   */    /*   by_lm   */ 
{
	u32 temperature;
	float temp;
      
      // 等待控制寄存器的CONV位为0
      while( I2C_Read_DS3231_Reg(0x0E) & 0x20 );
      
      temperature = I2C_Read_DS3231_Reg(0X11);
      temperature <<= 5;
      temperature |= (I2C_Read_DS3231_Reg(0X12)>>3);
      if( temperature&0x1000 )      // 如果符号位为1，表示是一个负数
      {
            temperature |= 0xE000;  // 进行符号扩展
      }
	temp = ((float)(((u32)(temperature *1000)) * 0.03125)) / 1000;

      return temp;
}

//======================================================================
// 函数名称：
// u8 I2C_Read_DS3231(u8 adress, u8 *tempdata)
//
// 说明：向指定寄存器读取数据  
//======================================================================

u8 I2C_Read_DS3231_Reg(u8 reg)
{
    u8 tx_count = 0;
    u8 data = 0;    

    //错误重发三次
    while(tx_count < 3)       
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //器件地址+写
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(reg);            //改访问地址指针
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        I2C_Stop_DS3231();        
    
        I2C_start_DS3231();                     

        I2C_Write_Byte_DS3231(0xD1);            //器件地址+读
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        data = I2C_Read_Byte_DS3231();
            
        I2C_Stop_DS3231();
        
        break;
    }
    return data;
}

