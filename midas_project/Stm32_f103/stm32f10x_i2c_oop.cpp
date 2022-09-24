#include "stm32f10x_i2c_oop.h"

/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C ENGC mask */
#define CR1_ENGC_Set            ((uint16_t)0x0040)
#define CR1_ENGC_Reset          ((uint16_t)0xFFBF)

/* I2C SWRST mask */
#define CR1_SWRST_Set           ((uint16_t)0x8000)
#define CR1_SWRST_Reset         ((uint16_t)0x7FFF)

/* I2C PEC mask */
#define CR1_PEC_Set             ((uint16_t)0x1000)
#define CR1_PEC_Reset           ((uint16_t)0xEFFF)

/* I2C ENPEC mask */
#define CR1_ENPEC_Set           ((uint16_t)0x0020)
#define CR1_ENPEC_Reset         ((uint16_t)0xFFDF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((uint16_t)0x0010)
#define CR1_ENARP_Reset         ((uint16_t)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((uint16_t)0x0080)
#define CR1_NOSTRETCH_Reset     ((uint16_t)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((uint16_t)0x0800)
#define CR2_DMAEN_Reset         ((uint16_t)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((uint16_t)0x1000)
#define CR2_LAST_Reset          ((uint16_t)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((uint16_t)0x0001)
#define OAR1_ADD0_Reset         ((uint16_t)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((uint16_t)0x0001)
#define OAR2_ENDUAL_Reset       ((uint16_t)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((uint16_t)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((uint16_t)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((uint32_t)0x07000000)

I2C::I2C(uint32_t I2C_ClockSpeed, uint16_t I2C_Mode, uint16_t I2C_DutyCycle,
				uint16_t I2C_OwnAddress1, uint16_t I2C_Ack, uint16_t I2C_AcknowledgedAddress, I2C_TypeDef* I2Cx)
{
	/*get values*/
	this->I2C_ClockSpeed = I2C_ClockSpeed;
	this->I2C_Mode = I2C_Mode;
	this->I2C_DutyCycle = I2C_DutyCycle;
	this->I2C_OwnAddress1 = I2C_OwnAddress1;
	this->I2C_Ack = I2C_Ack;
	this->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress;
	this->I2Cx = I2Cx;
	
	uint16_t tmpreg = 0, freqrange = 0;
  uint16_t result = 0x04;
  uint32_t pclk1 = 8000000;
  RCC_ClocksTypeDef  rcc_clocks;
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_I2C_CLOCK_SPEED(this->I2C_ClockSpeed));
  assert_param(IS_I2C_MODE(this->I2C_Mode));
  assert_param(IS_I2C_DUTY_CYCLE(this->I2C_DutyCycle));
  assert_param(IS_I2C_OWN_ADDRESS1(this->I2C_OwnAddress1));
  assert_param(IS_I2C_ACK_STATE(this->I2C_Ack));
  assert_param(IS_I2C_ACKNOWLEDGE_ADDRESS(this->I2C_AcknowledgedAddress));
	
	/*---------------------------- I2Cx CR2 Configuration ------------------------*/
  /* Get the I2Cx CR2 value */
  tmpreg = this->I2Cx->CR2;
  /* Clear frequency FREQ[5:0] bits */
  tmpreg &= CR2_FREQ_Reset;
  /* Get pclk1 frequency value */
  RCC_GetClocksFreq(&rcc_clocks);
  pclk1 = rcc_clocks.PCLK1_Frequency;
  /* Set frequency bits depending on pclk1 value */
  freqrange = (uint16_t)(pclk1 / 1000000);
  tmpreg |= freqrange;
  /* Write to I2Cx CR2 */
  this->I2Cx->CR2 = tmpreg;

/*---------------------------- I2Cx CCR Configuration ------------------------*/
  /* Disable the selected I2C peripheral to configure TRISE */
  this->I2Cx->CR1 &= CR1_PE_Reset;
  /* Reset tmpreg value */
  /* Clear F/S, DUTY and CCR[11:0] bits */
  tmpreg = 0;

  /* Configure speed in standard mode */
  if (this->I2C_ClockSpeed <= 100000)
  {
    /* Standard mode speed calculate */
    result = (uint16_t)(pclk1 / (this->I2C_ClockSpeed << 1));
    /* Test if CCR value is under 0x4*/
    if (result < 0x04)
    {
      /* Set minimum allowed value */
      result = 0x04;  
    }
    /* Set speed value for standard mode */
    tmpreg |= result;	  
    /* Set Maximum Rise Time for standard mode */
    this->I2Cx->TRISE = freqrange + 1; 
  }
  /* Configure speed in fast mode */
  else /*(I2C_InitStruct->I2C_ClockSpeed <= 400000)*/
  {
    if (this->I2C_DutyCycle == I2C_DutyCycle_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (uint16_t)(pclk1 / (this->I2C_ClockSpeed * 3));
    }
    else /*I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_16_9*/
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (uint16_t)(pclk1 / (this->I2C_ClockSpeed * 25));
      /* Set DUTY bit */
      result |= I2C_DutyCycle_16_9;
    }

    /* Test if CCR value is under 0x1*/
    if ((result & CCR_CCR_Set) == 0)
    {
      /* Set minimum allowed value */
      result |= (uint16_t)0x0001;  
    }
    /* Set speed value and set F/S bit for fast mode */
    tmpreg |= (uint16_t)(result | CCR_FS_Set);
    /* Set Maximum Rise Time for fast mode */
    this->I2Cx->TRISE = (uint16_t)(((freqrange * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);  
  }

  /* Write to I2Cx CCR */
  this->I2Cx->CCR = tmpreg;
  /* Enable the selected I2C peripheral */
  this->I2Cx->CR1 |= CR1_PE_Set;

/*---------------------------- I2Cx CR1 Configuration ------------------------*/
  /* Get the I2Cx CR1 value */
  tmpreg = this->I2Cx->CR1;
  /* Clear ACK, SMBTYPE and  SMBUS bits */
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure I2Cx: mode and acknowledgement */
  /* Set SMBTYPE and SMBUS bits according to I2C_Mode value */
  /* Set ACK bit according to I2C_Ack value */
  tmpreg |= (uint16_t)((uint32_t)this->I2C_Mode | this->I2C_Ack);
  /* Write to I2Cx CR1 */
  this->I2Cx->CR1 = tmpreg;

/*---------------------------- I2Cx OAR1 Configuration -----------------------*/
  /* Set I2Cx Own Address1 and acknowledged address */
  this->I2Cx->OAR1 = (this->I2C_AcknowledgedAddress | this->I2C_OwnAddress1);
}

void I2C::Cmd(FunctionalState NewState)
{
	/* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral */
    this->I2Cx->CR1 |= CR1_PE_Set;
  }
  else
  {
    /* Disable the selected I2C peripheral */
    this->I2Cx->CR1 &= CR1_PE_Reset;
  }
}

FlagStatus I2C::GetFlagStatus(uint32_t I2C_FLAG)
{
	FlagStatus bitstatus = RESET;
  __IO uint32_t i2creg = 0, i2cxbase = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

  /* Get the I2Cx peripheral base address */
  i2cxbase = (uint32_t)this->I2Cx;
  
  /* Read flag register index */
  i2creg = I2C_FLAG >> 28;
  
  /* Get bit[23:0] of the flag */
  I2C_FLAG &= FLAG_Mask;
  
  if(i2creg != 0)
  {
    /* Get the I2Cx SR1 register address */
    i2cxbase += 0x14;
  }
  else
  {
    /* Flag in I2Cx SR2 Register */
    I2C_FLAG = (uint32_t)(I2C_FLAG >> 16);
    /* Get the I2Cx SR2 register address */
    i2cxbase += 0x18;
  }
  
  if(((*(__IO uint32_t *)i2cxbase) & I2C_FLAG) != (uint32_t)RESET)
  {
    /* I2C_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset */
    bitstatus = RESET;
  }
  
  /* Return the I2C_FLAG status */
  return  bitstatus;
}

void I2C::GenerateSTART(FunctionalState NewState)
{
	 /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Generate a START condition */
    this->I2Cx->CR1 |= CR1_START_Set;
  }
  else
  {
    /* Disable the START condition generation */
    this->I2Cx->CR1 &= CR1_START_Reset;
  }
}

ErrorStatus I2C::CheckEvent(uint32_t I2C_EVENT)
{
	uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* Read the I2Cx status register */
  flag1 = this->I2Cx->SR1;
  flag2 = this->I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }
  /* Return status */
  return status;
}

void I2C::GenerateSTOP(FunctionalState NewState)
{
	/* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Generate a STOP condition */
    this->I2Cx->CR1 |= CR1_STOP_Set;
  }
  else
  {
    /* Disable the STOP condition generation */
    this->I2Cx->CR1 &= CR1_STOP_Reset;
  }
}

void I2C::Send7bitAddress(uint8_t Address, uint8_t I2C_Direction)
{
	/* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_I2C_DIRECTION(I2C_Direction));
  /* Test on the direction to set/reset the read/write bit */
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* Set the address bit0 for read */
    Address |= OAR1_ADD0_Set;
  }
  else
  {
    /* Reset the address bit0 for write */
    Address &= OAR1_ADD0_Reset;
  }
  /* Send the address */
  this->I2Cx->DR = Address;
}

void I2C::SendData(uint8_t Data)
{
	/* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  /* Write in the DR register the data to be sent */
  this->I2Cx->DR = Data;
}

uint8_t I2C::ReceiveData()
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  /* Return the data in the DR register */
  return (uint8_t)this->I2Cx->DR;
}

void I2C::AcknowledgeConfig(FunctionalState NewState)
{
	/* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(this->I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the acknowledgement */
    this->I2Cx->CR1 |= CR1_ACK_Set;
  }
  else
  {
    /* Disable the acknowledgement */
    this->I2Cx->CR1 &= CR1_ACK_Reset;
  }
}

I2C::~I2C()
{
}
