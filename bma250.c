#include "bma250.h"
#include "softi2c.h"


void BMA250_Init(void)
{
    uint8 bGRange = 0;                                  // g Range;
    uint8 bBwd = 0;                                     // Bandwidth
    uint8 bSleep = 0;                                   // Sleep phase
	
// Configure sensor and start to sample data
#if (BMP_AS_RANGE == 2)
	bGRange = 0x03;
#elif (BMP_AS_RANGE == 4)
	bGRange = 0x05;
#elif (BMP_AS_RANGE == 8)
	bGRange = 0x08;
#elif (BMP_AS_RANGE == 16)
	bGRange = 0x0C;
#else
//	#error "Measurement range not supported"
#endif
  
#if (BMP_AS_BANDWIDTH == 8)
	bBwd = 0x08;
#elif (BMP_AS_BANDWIDTH == 16)
	bBwd = 0x09;
#elif (BMP_AS_BANDWIDTH == 31)
	bBwd = 0x0A;
#elif (BMP_AS_BANDWIDTH == 63)
	bBwd = 0x0B;
#elif (BMP_AS_BANDWIDTH == 125)
	bBwd = 0x0C;
#elif (BMP_AS_BANDWIDTH == 250)
	bBwd = 0x0D;
#elif (BMP_AS_BANDWIDTH == 500)
	bBwd = 0x0E;
#elif (BMP_AS_BANDWIDTH == 1000)
	bBwd = 0x0F;
#else
//	#error "Sample rate not supported"
#endif
	
#if (BMP_AS_SLEEPPHASE == 1)
	bSleep = 0x4C;
#elif (BMP_AS_SLEEPPHASE == 2)
	bSleep = 0x4E;
#elif (BMP_AS_SLEEPPHASE == 4)
	bSleep = 0x50;
#elif (BMP_AS_SLEEPPHASE == 6)
	bSleep = 0x52;
#elif (BMP_AS_SLEEPPHASE == 10)
	bSleep = 0x54;
#elif (BMP_AS_SLEEPPHASE == 25)
	bSleep = 0x56;
#elif (BMP_AS_SLEEPPHASE == 50)
	bSleep = 0x58;
#else
//	#error "Sleep phase duration not supported"
#endif

	// write sensor configuration
	BMA250_WriteByte(BMA250_Addr, BMP_GRANGE, bGRange);  // Set measurement range
	BMA250_WriteByte(BMA250_Addr, BMP_BWD, bBwd);        // Set filter bandwidth
	BMA250_WriteByte(BMA250_Addr, BMP_PM, bSleep);       // Set filter bandwidth


#ifndef BMP_AS_FILTERING
	BMA250_WriteByte(BMA250_Addr, BMP_SCR, 0x80);        // acquire unfiltered acceleration data
#endif
//	// configure sensor interrupt
//	bmp_as_write_register(BMP_IMR2, 0x01);       // map new data interrupt to INT1 pin
//	bmp_as_write_register(BMP_ISR2, 0x10);       // enable new data interrupt
//	
//	// enable CC430 interrupt pin for data read out from acceleration sensor
//	AS_INT_IFG &= ~AS_INT_PIN;                   // Reset flag
//	AS_INT_IE  |=  AS_INT_PIN;                   // Enable interrupt
}

/**
* BMA250 IIC写一个字节 
* I2C_addr  BMA250地址
* Reg_addr:寄存器地址
* data: 写入寄存器地址
* 返回值:0,正常
*        1,错误代码
*/
uint8 BMA250_WriteByte(uint8 I2C_addr,uint8 Reg_addr,uint8 data)
{
    start();
    iic_write(I2C_addr | 0X00); //WRITE i2c
    if (check_ack()==0) {
	goto err;
    }
    
    iic_write(Reg_addr);
    if (check_ack()==0) {
	goto err;
    }
    
    iic_write(data);
    if (check_ack()==0) {
        goto err;
    }
    stop();
    return 1;
err:
	stop();
	return 0;
}

/**
* BMA250 IIC读一个字节 
* I2C_addr  BMA250地址
* Reg_addr:寄存器地址
* 返回值:data,正常
*        0,错误代码
*/
uint8 BMA250_ReadByte(uint8 I2C_addr, uint8 Reg_addr)
{
    uint8 data;
    start();
    iic_write(I2C_addr | 0X00); //WRITE i2c
    if (check_ack()==0) {
	goto err;
    }
    
    iic_write(Reg_addr);
    if (check_ack()==0) {
	goto err;
    }
    stop();
    
    start();
    iic_write(I2C_addr | 0X01); //READ
    if (check_ack()==0) {
	goto err;
    }
    
    data = iic_read();
    send_noack();
    stop();
    return data;
    
err:
	stop();
	return 0;
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK 
uint8 BMA250_IIC_Read_Byte(uint8 ack)
{
    uint8 data;
    data = iic_read();
    if(ack) send_ack();
    else send_noack();
    return data;
}


/**
* BMA250 IIC连续读len个字节 
* I2C_addr:  BMA250地址
* Reg_addr:寄存器地址
* len:要读取的长度
* buff:读取到的数据存储区
* 返回值:0,正常
*        其他,错误代码
*/
uint8 BMA250_Read_LenBytes(uint8 I2C_addr, uint8 Reg_addr, uint8 len, uint8 *buff)
{
    start();
    iic_write(I2C_addr | 0X00); //WRITE i2c
    if (check_ack()==0) {
	goto err;
    }
    
    iic_write(Reg_addr);
    if (check_ack()==0) {
	goto err;
    }
    stop();
    
    start();
    iic_write(I2C_addr | 0X01); //READ
    if (check_ack()==0) {
	goto err;
    }
    while(len)
    {
            if(len==1) *buff=BMA250_IIC_Read_Byte(0);   //读数据,发送nACK 
            else *buff=BMA250_IIC_Read_Byte(1);		//读数据,发送ACK  
            len--;
            buff++; 
    }    
    stop();
    return 0;
err:
	stop();
	return 1;
}

uint8 Check_BMA250_ID(void)
{
    return (BMA250_ReadByte(BMA250_Addr, 0x00));
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
void BMA250_Get_Acceleration(short *gx, short *gy, short *gz)
{
    *(gx+0) = BMA250_ReadByte(BMA250_Addr, BMP_ACC_X_LSB);
    *(gx+1) = BMA250_ReadByte(BMA250_Addr, BMP_ACC_X_MSB);
    *(gy+0) = BMA250_ReadByte(BMA250_Addr, BMP_ACC_Y_LSB);
    *(gy+1) = BMA250_ReadByte(BMA250_Addr, BMP_ACC_Y_MSB);
    *(gz+0) = BMA250_ReadByte(BMA250_Addr, BMP_ACC_Z_LSB);
    *(gz+1) = BMA250_ReadByte(BMA250_Addr, BMP_ACC_Z_MSB);      
}

