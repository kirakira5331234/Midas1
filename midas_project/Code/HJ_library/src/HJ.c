#include "main.h"

void HJ_Init(handjob_type *hj)
{
     hj->motor_angle[motor_a] = 0;
     hj->motor_angle[motor_b] = 0;
     hj->motor_angle[motor_c] = 0;
     hj->motor_angle[motor_d] = 0;
}
handjob_type HJ_StringToHJ(char* input)
{
     handjob_type result;
     int motor_numb;
     int motor_angle;
     
     motor_numb = (int)(input[0] - 'a');
     motor_angle = (int) (input[1] - '0');
     
     result.motor_angle[motor_numb] = motor_angle;
     result.motor_id = motor_numb;
     return result;
}
