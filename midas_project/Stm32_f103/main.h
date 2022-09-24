
#include "main.h"

#define RUN_MOTOR(id,value)   TIM1->CCR##id = value*10 + 900
int main(void)
{
     char a = '6';
     char string[2];
     int id,vl;
     handjob_type b;
     /*
     Initialization
     */
     HJ_PWM_Init();
     UART_INIT();
     HJ_Init(&b);
     
     while(1)
     {
          string[0] = 0x00;
          string[1] = 0x00;
          
          /*
          formula:     <motor_name> + <value> + "\n"
          params:
          <motor_name>: a,b,c or d
          <value> 0 -> 9 (0 degree -> ~180 degree)
          example: "a0\n"    motor_a 0 degree
                   "b5\n"    motor_b 90 degree    
          */
          UART_GetString(string);
          
          
          HJ_Init(&b);
          b = HJ_StringToHJ(string); /*transfer string to hanjob types*/
          
          vl = b.motor_angle[id];
          
          switch(b.motor_id)
          {
               case 0: RUN_MOTOR(1,vl); break;
               case 1: RUN_MOTOR(2,vl); break;
               case 2: RUN_MOTOR(3,vl); break;
               case 3: RUN_MOTOR(4,vl); break;
               default:break;
          }
     }
     
     return 0;
}
