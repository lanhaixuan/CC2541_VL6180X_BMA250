#include <iocc2541.h>
#include <softi2c.h>
#include "vl6180x_i2c.h"
#include "bma250.h"


uint8 distance=0;
uint8 data_staus=0, data_id;
uint16 light;

/****************************************************************************
* 名    称: InitCLK()
* 功    能: 设置系统时钟源
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitCLK()
{
    CLKCONCMD &= ~0x40;             //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);        //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;             //设置系统主时钟频率为32MHZ   
}


int main( void )
{
    short aacx[2],aacy[2],aacz[2];	    //加速度传感器原始数据
    InitCLK();                      //设置系统时钟源    
    IIC_Init();   
    VL6180X_Init();
    BMA250_Init();
    while(1)
    {   
        data_id = Check_BMA250_ID();  // 0x03
        BMA250_Get_Acceleration(aacx, aacy, aacz);
        distance = RangePollingRead();
        DelayMS(5);
      
    }
}

