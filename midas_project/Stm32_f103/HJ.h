#ifndef    HANDJOB_H
#define    HANDJOB_H

#define   NUMB_OF_MOTOR  4

typedef enum
{
     motor_a = 0,
     motor_b,
     motor_c,
     motor_d
}handjob_motor_id;

typedef struct
{
     char motor_angle[4];
     char motor_id;
}handjob_type;

void HJ_Init(handjob_type *hj);
handjob_type HJ_StringToHJ(char* input);

#endif    /*HANDJOB_H*/