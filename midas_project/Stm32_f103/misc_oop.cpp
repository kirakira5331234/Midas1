#include "misc_oop.h"

NVIC_Class::NVIC_Class(uint8_t NVIC_IRQChannel, uint8_t NVIC_IRQChannelPreemptionPriority, 
								uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd)
{
	/* set values*/
	this->NVIC_IRQChannel = NVIC_IRQChannel;
	this->NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
	this->NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
	this->NVIC_IRQChannelCmd = NVIC_IRQChannelCmd;
	
	uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(this->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(this->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(this->NVIC_IRQChannelSubPriority));
    
  if (this->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority --------------------------------*/    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)this->NVIC_IRQChannelPreemptionPriority << tmppre;
    tmppriority |=  this->NVIC_IRQChannelSubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[this->NVIC_IRQChannel] = tmppriority;
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[this->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (this->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[this->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (this->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}

NVIC_Class::~NVIC_Class()
{
}
