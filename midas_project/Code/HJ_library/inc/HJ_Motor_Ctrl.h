#ifndef HJ_Motor_Ctrl_H
#define HJ_Motor_Ctrl_H
//#include "PMI_Hcode.h"
/**/

/*GPIO Pin define */
#define GPIO_PWM_PORTA    GPIOA
#define GPIO_PWM_PORTB		GPIOB

#define GPIO_PIN_MOTORA_PWM  GPIO_Pin_8
#define GPIO_PIN_MOTORB_PWM GPIO_Pin_9 
#define GPIO_PIN_MOTORC_PWM GPIO_Pin_10
#define GPIO_PIN_MOTORD_PWM GPIO_Pin_11
#define GPIO_PIN_MOTORE_PWM GPIO_Pin_15

 
#define Motor_Timer TIM1

/*-----------------------------------*/

void HJ_RCC_Init(void);
void HJ_GPIO_Init(void);
void HJ_PWM_Init(void);
/*-----------------------------------*/


#endif /*PMI_Motor_Ctrl_H*/
