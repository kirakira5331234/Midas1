#include "stm32f10x_exti_oop.h"

EXTI_Class::EXTI_Class(uint32_t EXTI_Line, EXTIMode_TypeDef EXTI_Mode, EXTITrigger_TypeDef EXTI_Trigger, FunctionalState EXTI_LineCmd)
{
	/*set values*/
	this->EXTI_Line = EXTI_Line;
	this->EXTI_Mode = EXTI_Mode;
	this->EXTI_Trigger = EXTI_Trigger;
	this->EXTI_LineCmd = EXTI_LineCmd;
	
	uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_EXTI_MODE(this->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(this->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(this->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(this->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;
     
  if (this->EXTI_LineCmd != DISABLE)
  {
    /* Clear EXTI line configuration */
    EXTI->IMR &= ~this->EXTI_Line;
    EXTI->EMR &= ~this->EXTI_Line;
    
    tmp += this->EXTI_Mode;

    *(__IO uint32_t *) tmp |= this->EXTI_Line;

    /* Clear Rising Falling edge configuration */
    EXTI->RTSR &= ~this->EXTI_Line;
    EXTI->FTSR &= ~this->EXTI_Line;
    
    /* Select the trigger for the selected external interrupts */
    if (this->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* Rising Falling edge */
      EXTI->RTSR |= this->EXTI_Line;
      EXTI->FTSR |= this->EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += this->EXTI_Trigger;

      *(__IO uint32_t *) tmp |= this->EXTI_Line;
    }
  }
  else
  {
    tmp += this->EXTI_Mode;

    /* Disable the selected external lines */
    *(__IO uint32_t *) tmp &= ~this->EXTI_Line;
  }
}

ITStatus EXTI_Class::GetITStatus()
{
	ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* Check the parameters */
  assert_param(IS_GET_EXTI_LINE(this->EXTI_Line));
  
  enablestatus =  EXTI->IMR & EXTI_Line;
  if (((EXTI->PR & this->EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

void EXTI_Class::ClearITPendingBit()
{
	/* Check the parameters */
  assert_param(IS_EXTI_LINE(this->EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

EXTI_Class::~EXTI_Class()
{
}
