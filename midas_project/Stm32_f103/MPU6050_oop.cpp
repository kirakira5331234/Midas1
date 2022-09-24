#include "MPU6050_oop.h"

MPU6050::MPU6050(I2C_TypeDef* I2Cx, uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	this->I2Cx = I2Cx;
	this->SCL_Pin = SCL_Pin;
	this->SDA_Pin = SDA_Pin;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	if (I2Cx == I2C1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	}
	else if (I2Cx == I2C2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	}
	
	I2C MPU6050_I2C(100000, I2C_Mode_I2C, I2C_DutyCycle_2, MPU6050_DEFAULT_ADDRESS, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit, I2Cx);
	GPIO GPIO_I2C_SCL(SCL_Pin,GPIO_Speed_50MHz,GPIO_Mode_AF_OD,GPIOB);
	GPIO GPIO_I2C_SDA(SDA_Pin,GPIO_Speed_50MHz,GPIO_Mode_AF_OD,GPIOB);
	MPU6050_I2C.Cmd(ENABLE);
}

MPU6050::~MPU6050()
{
}

void MPU6050::TestConnection()
{
	
}

void MPU6050::Read_Data(uint8_t Addr,uint8_t *data)
{
	I2C MPU6050_I2C(100000, I2C_Mode_I2C, I2C_DutyCycle_2, MPU6050_DEFAULT_ADDRESS, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit, this->I2Cx);
	/* While the bus is busy */
    while (MPU6050_I2C.GetFlagStatus(I2C_FLAG_BUSY));
		MPU6050_I2C.GenerateSTART(ENABLE);
	/* Send MPU6050 address for write */
    MPU6050_I2C.Send7bitAddress(Addr, I2C_Direction_Transmitter);
	
}
