
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
// �������ƣ�
// I2C_start(void)
//
// ˵����I2C�ȴ�ʱ��2us
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
// �������ƣ�
// I2C_start(void)
//
// ˵����i2c��ʼ����
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
// �������ƣ�
// I2C_Write_Byte(void)
//
// ˵��������1�ֽ�����
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
// �������ƣ�
// I2C_Read_Byte(void)
//
// ˵������ȡ1�ֽ�����
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
// �������ƣ�
// I2C_Send_Ack(u8 ack)
//
// ˵��������ȷ�ϱ�ʶ
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

    //�ͷ�����
    I2C_SDA1_1;
    I2C_Delay();
}


//======================================================================
// �������ƣ�
// I2C_Receive_Ack(void)
//
// ˵�������ȷ�ϱ�ʶ
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
// �������ƣ�
// I2C_Stop(void)
//
// ˵����I2Cֹͣ����
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
// �������ƣ�
// I2C_Write_DS3231(u8 adress, u8 *tempdata)
//
// ˵������ָ����ַд������ MSB�ꡣ������LSB
//======================================================================
u8 I2C_Write_DS3231(u8 *tempdata)
{
    u8 tx_count = 0;
    u8 flag = 0;
    u8 jump = 0;
		u8 i = 0;

    //�����ط�����
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //������ַ+д
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x00);           //���ʵ�ַ
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<7; i++)                 //7�ֽ�����
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
// �������ƣ�
// u8 I2C_Read_DS3231(u8 adress, u8 *tempdata)
//
// ˵������ָ����ַ��ȡ����   MSB�ꡣ������LSB
//======================================================================

u8 I2C_Read_DS3231(u8 *tempdata)
{
    u8 tx_count = 0;
    u8 flag = 0, i = 0;    

    //�����ط�����
    while(tx_count < 3)       
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //������ַ+д
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x00);            //�ķ��ʵ�ַָ��
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        I2C_Stop_DS3231();        
    
        I2C_start_DS3231();                     

        I2C_Write_Byte_DS3231(0xD1);            //������ַ+��
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<7; i++)                 //7�ֽ�����
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
	tempdata[6] = tempdata[6]/16 * 10 + tempdata[6] % 16;//��"16����ת��10����"
	tempdata[5] = tempdata[5]/16 * 10 + tempdata[5] % 16;//��
	tempdata[4] = tempdata[4]/16 * 10 + tempdata[4] % 16;//��
	tempdata[2] = tempdata[2]/16 * 10 + tempdata[2] % 16;//ʱ
	tempdata[1] = tempdata[1]/16 * 10 + tempdata[1] % 16;//��
	tempdata[0] = tempdata[0]/16 * 10 + tempdata[0] % 16;//��
    return flag;
}


u8 I2C_Write_Alarm(u8 *tempdata)		//�趨���ӵ��롢�֡�ʱ������
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
	
	//�����ط�����
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //������ַ+д
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x07);           //���ʵ�ַ
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i = 0; i < 4; i++)                 //7�ֽ�����
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

    //�����ط�����
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //������ַ+д
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x0E);           //���ʵ�ַ
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<1; i++)                 //1�ֽ�����
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

    //�����ط�����
    while(tx_count < 3)
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //������ַ+д
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(0x0F);           //���ʵ�ַ
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        for (i=0; i<1; i++)                 //7�ֽ�����
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
//��DS3231�¶�
//===========================================================
float I2C_Get_Temperature(void)     /*     �˺�������   */    /*   by_lm   */ 
{
	u32 temperature;
	float temp;
      
      // �ȴ����ƼĴ�����CONVλΪ0
      while( I2C_Read_DS3231_Reg(0x0E) & 0x20 );
      
      temperature = I2C_Read_DS3231_Reg(0X11);
      temperature <<= 5;
      temperature |= (I2C_Read_DS3231_Reg(0X12)>>3);
      if( temperature&0x1000 )      // �������λΪ1����ʾ��һ������
      {
            temperature |= 0xE000;  // ���з�����չ
      }
	temp = ((float)(((u32)(temperature *1000)) * 0.03125)) / 1000;

      return temp;
}

//======================================================================
// �������ƣ�
// u8 I2C_Read_DS3231(u8 adress, u8 *tempdata)
//
// ˵������ָ���Ĵ�����ȡ����  
//======================================================================

u8 I2C_Read_DS3231_Reg(u8 reg)
{
    u8 tx_count = 0;
    u8 data = 0;    

    //�����ط�����
    while(tx_count < 3)       
    {
        I2C_start_DS3231();

        I2C_Write_Byte_DS3231(0xD0);            //������ַ+д
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }        

        I2C_Write_Byte_DS3231(reg);            //�ķ��ʵ�ַָ��
        if (I2C_Receive_Ack_DS3231() == 1)           
        {   
            tx_count++;
            continue;
        }

        I2C_Stop_DS3231();        
    
        I2C_start_DS3231();                     

        I2C_Write_Byte_DS3231(0xD1);            //������ַ+��
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

