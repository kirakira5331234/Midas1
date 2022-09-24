/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int32_t StepLeftMotor[3] = {0,0,0};
int32_t StepCounterMotor[3] = {0,0,0};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void TIM1_UP_IRQHandler(void)/*X axis's motor*/
{    
     //int32_t temp_stepleft = abs(StepLeftMotor[0]);
     if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
     {
          StepCounterMotor[0]++;
          if(StepCounterMotor[0] >= StepLeftMotor[0])
          {
               TIM_Cmd(TIM1, DISABLE);
          }
          TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
     }
}

void TIM2_IRQHandler(void)/*Y axis's motor*/
{
     //int32_t temp_stepleft = abs(StepLeftMotor[1]);
     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
     {
          StepCounterMotor[1]++;
          if(StepCounterMotor[1] >= StepLeftMotor[1])
          {
               TIM_Cmd(TIM2, DISABLE);
          }          
          TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
     } 
}
void TIM3_IRQHandler(void)/*Z axis's motor*/
{
     //int32_t temp_stepleft = abs(StepLeftMotor[2]);
     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
     {
          StepCounterMotor[2]++;
          if(StepCounterMotor[2] >= StepLeftMotor[2])
          {
               TIM_Cmd(TIM3, DISABLE);
          }  
          TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
          
     }
}
void set_step(int8_t motor_numb, uint32_t step)
{
     StepLeftMotor[motor_numb] = step;
//     switch(motor_numb)
//     {
//          case 0: TIM_Cmd(TIM1, ENABLE); break;
//          case 1: TIM_Cmd(TIM2, ENABLE); break;
//          case 2: TIM_Cmd(TIM3, ENABLE); break;
//     }
     
}
void reset_counter()
{
     StepCounterMotor[0] = StepCounterMotor[1] = StepCounterMotor[2] = 0;
}
int8_t left_step_equal_zeros()
{
     if(StepLeftMotor[0] <= StepCounterMotor[0] && StepLeftMotor[1] <= StepCounterMotor[1] && StepLeftMotor[2] <= StepCounterMotor[2])
          return 1;
     return 0;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
