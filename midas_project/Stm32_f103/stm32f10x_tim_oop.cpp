#include "stm32f10x_tim_oop.h"

Timer_Base::Timer_Base(TIM_TypeDef* TIMx,uint16_t TIM_Prescaler,uint16_t TIM_CounterMode,
								uint16_t TIM_Period,uint16_t TIM_ClockDivision,uint8_t TIM_RepetitionCounter)
{
	/* set value for class*/
	this->TIMx = TIMx;
	this->TIM_Prescaler = TIM_Prescaler;
	this->TIM_CounterMode = TIM_CounterMode;
	this->TIM_Period = TIM_Period;
	this->TIM_ClockDivision = TIM_ClockDivision;
	this->TIM_RepetitionCounter = TIM_RepetitionCounter;
	uint16_t tmpcr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));

  tmpcr1 = TIMx->CR1;  

  if((this->TIMx == TIM1) || (this->TIMx == TIM8)|| (this->TIMx == TIM2) || (this->TIMx == TIM3)||
     (this->TIMx == TIM4) || (this->TIMx == TIM5)) 
  {
    /* Select the Counter Mode */
    tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_CR1_DIR | TIM_CR1_CMS)));
    tmpcr1 |= (uint32_t)this->TIM_CounterMode;
  }
 
  if((this->TIMx != TIM6) && (this->TIMx != TIM7))
  {
    /* Set the clock division */
    tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CR1_CKD));
    tmpcr1 |= (uint32_t)this->TIM_ClockDivision;
  }

  this->TIMx->CR1 = tmpcr1;

  /* Set the Autoreload value */
  this->TIMx->ARR = this->TIM_Period ;
 
  /* Set the Prescaler value */
  this->TIMx->PSC = this->TIM_Prescaler;
    
  if ((this->TIMx == TIM1) || (this->TIMx == TIM8)|| (this->TIMx == TIM15)|| (this->TIMx == TIM16) || (this->TIMx == TIM17))  
  {
    /* Set the Repetition Counter value */
    this->TIMx->RCR = this->TIM_RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler and the Repetition counter
     values immediately */
  this->TIMx->EGR = TIM_PSCReloadMode_Immediate;  
}

void Timer_Base::Cmd(FunctionalState NewState)
{
	/* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(this->TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    this->TIMx->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    /* Disable the TIM Counter */
    this->TIMx->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
  }
}

uint16_t Timer_Base::GetCounter()
{
	/* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(this->TIMx));
  /* Get the Counter Register value */
  return this->TIMx->CNT;
}

void Timer_Base::IT_Config(uint16_t TIM_IT, FunctionalState NewState)
{
	/* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(this->TIMx));
  assert_param(IS_TIM_IT(this->TIM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    this->TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
    this->TIMx->DIER &= (uint16_t)~TIM_IT;
  }
}

ITStatus Timer_Base::GetITStatus(uint16_t TIM_IT)
{
	ITStatus bitstatus = RESET;  
  uint16_t itstatus = 0x0, itenable = 0x0;
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(this->TIMx));
  assert_param(IS_TIM_GET_IT(TIM_IT));
   
  itstatus = this->TIMx->SR & TIM_IT;
  
  itenable = this->TIMx->DIER & TIM_IT;
  if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

void Timer_Base::ClearITPendingBit(uint16_t TIM_IT)
{
	/* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(this->TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  /* Clear the IT pending Bit */
  this->TIMx->SR = (uint16_t)~TIM_IT;
}

Timer_Base::~Timer_Base()
{
}
