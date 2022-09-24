#include "stm32f10x_GPIO_oop.h"

GPIO::GPIO()
{
}

GPIO::GPIO(uint16_t GPIO_Pin, GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode,GPIO_TypeDef* GPIOx)
{
	this->GPIO_Pin = GPIO_Pin;
	this->GPIO_Speed = GPIO_Speed;
	this->GPIO_Mode = GPIO_Mode;
	this->GPIOx = GPIOx;
	uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(this->GPIOx));
  assert_param(IS_GPIO_MODE(this->GPIO_Mode));
  assert_param(IS_GPIO_PIN(this->GPIO_Pin));  
  
/*---------------------------- GPIO Mode Configuration -----------------------*/
  currentmode = ((uint32_t)this->GPIO_Mode) & ((uint32_t)0x0F);
  if ((((uint32_t)this->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
  { 
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(this->GPIO_Speed));
    /* Output mode */
    currentmode |= (uint32_t)this->GPIO_Speed;
  }
/*---------------------------- GPIO CRL Configuration ------------------------*/
  /* Configure the eight low port pins */
  if (((uint32_t)this->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = this->GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* Get the port pins position */
      currentpin = (this->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding low control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (this->GPIO_Mode == GPIO_Mode_IPD)
        {
          this->GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        }
        else
        {
          /* Set the corresponding ODR bit */
          if (this->GPIO_Mode == GPIO_Mode_IPU)
          {
            this->GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
        }
      }
    }
    this->GPIOx->CRL = tmpreg;
  }
/*---------------------------- GPIO CRH Configuration ------------------------*/
  /* Configure the eight high port pins */
  if (this->GPIO_Pin > 0x00FF)
  {
    tmpreg = this->GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* Get the port pins position */
      currentpin = ((this->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (this->GPIO_Mode == GPIO_Mode_IPD)
        {
          this->GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* Set the corresponding ODR bit */
        if (this->GPIO_Mode == GPIO_Mode_IPU)
        {
          this->GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    this->GPIOx->CRH = tmpreg;
  }
}

GPIO::~GPIO()
{
}

void GPIO::Set_Bit()
{
	this->GPIOx->ODR |= this->GPIO_Pin ;
}

void GPIO::Clear_Bit()
{
	this->GPIOx->ODR &= this->GPIO_Pin ;
}

void GPIO::Toggle()
{
	this->GPIOx->ODR ^= this->GPIO_Pin;
}
	
void GPIO::EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
	uint32_t tmp = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}

uint8_t GPIO::ReadInputDataBit()
{
	uint8_t bitstatus = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(this->GPIOx));
  assert_param(IS_GET_GPIO_PIN(this->GPIO_Pin)); 
  
  if ((this->GPIOx->IDR & this->GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

void GPIO::GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
	uint32_t tmp = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}

uint8_t GPIO::ReadOutputDataBit()
{
	uint8_t bitstatus = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(this->GPIOx));
  assert_param(IS_GET_GPIO_PIN(this->GPIO_Pin)); 
  
  if ((this->GPIOx->ODR & this->GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

uint16_t GPIO::ReadInputData()
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(this->GPIOx));
  
  return ((uint16_t)this->GPIOx->IDR);
}

void GPIO::ResetBits()
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(this->GPIOx));
  assert_param(IS_GPIO_PIN(this->GPIO_Pin));
  
  this->GPIOx->BRR = this->GPIO_Pin;
}
