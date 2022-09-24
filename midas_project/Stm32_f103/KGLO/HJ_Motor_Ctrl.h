#ifndef HJ_Motor_Ctrl_H
#define HJ_Motor_Ctrl_H

#ifdef __cplusplus
extern "C" {
#endif 
	
//#include "PMI_Hcode.h"
/**/
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/*GPIO Pin define */
#define GPIO_PWM_PORT    GPIOA      

#define GPIO_PIN_MOTORA_PWM GPIO_Pin_8
#define GPIO_PIN_MOTORB_PWM GPIO_Pin_9 
#define GPIO_PIN_MOTORC_PWM GPIO_Pin_10
#define GPIO_PIN_MOTORD_PWM GPIO_Pin_11
 
#define Motor_Timer TIM1

/*-----------------------------------*/

void HJ_RCC_Init(void);
void HJ_GPIO_Init(void);
void HJ_PWM_Init(void);
/*-----------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*PMI_Motor_Ctrl_H*/
