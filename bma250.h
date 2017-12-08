#ifndef BMA250_H_
#define BMA250_H_

#include "common.h"

#define BMA250_Addr          (0x18<<1)    //SD0 Low  0x18;    SD0 High 0x19;

#define BMP_CHIPID           0x00
#define BMP_ACC_X_LSB        0x02
#define BMP_ACC_X_MSB        0x03
#define BMP_ACC_Y_LSB        0x04
#define BMP_ACC_Y_MSB        0x05
#define BMP_ACC_Z_LSB        0x06
#define BMP_ACC_Z_MSB        0x07

#define BMP_GRANGE           0x0F	   // g Range
#define BMP_BWD              0x10	   // Bandwidth
#define BMP_PM               0x11	   // Power modes
#define BMP_SCR              0x13	   // Special Control Register
#define BMP_RESET            0x14	   // Soft reset register writing 0xB6 causes reset
#define BMP_ISR1             0x16	   // Interrupt settings register 1
#define BMP_ISR2             0x17	   // Interrupt settings register 2
#define BMP_IMR1             0x19	   // Interrupt mapping register 1
#define BMP_IMR2             0x1A	   // Interrupt mapping register 2
#define BMP_IMR3             0x1B	   // Interrupt mapping register 3

void BMA250_Init(void);
uint8 BMA250_WriteByte(uint8 I2C_addr,uint8 Reg_addr,uint8 data);
uint8 BMA250_ReadByte(uint8 I2C_addr, uint8 Reg_addr);
uint8 BMA250_IIC_Read_Byte(uint8 ack);
uint8 BMA250_Read_LenBytes(uint8 I2C_addr, uint8 Reg_addr, uint8 len, uint8 *buff);
uint8 Check_BMA250_ID(void);
void BMA250_Get_Acceleration(short *gx, short *gy, short *gz);

#endif


