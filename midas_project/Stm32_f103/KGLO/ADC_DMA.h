#ifndef __ADC_DMA_H
#define __ADC_DMA_H

/*
Ver 1.0
*/

/* Standard includes. */
#include "stdint.h"

/* Library includes. */
#include "stm32f10x_adc.h"

#define ADC_DMA_Chanel_Number			5
#define ADC_DMA_SampleTime				ADC_SampleTime_55Cycles5

void ADC_DMA_Config(uint32_t* ADCValue);
void ADC_DMA_Start(void);

#endif /* __ADC_DMA_H */
