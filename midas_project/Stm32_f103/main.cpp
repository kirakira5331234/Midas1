#include "main.h"
#include <string.h>

void ADC_INIT()
{
	// Chon Clock ADC
	RCC->CFGR |= RCC_CFGR_ADCPRE_1;	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	// GPIOA, PA0, ADC12_IN0 ; PA1, ADC12_IN1.; PA2, ADC12_IN2
	// GPIOA, PA3, ADC12_IN3 ; PA4, ADC12_IN4;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(GPIO_CRL_MODE0_0);
	GPIOA->CRL &= ~(GPIO_CRL_MODE0_1);
	GPIOA->CRL &= ~(GPIO_CRL_CNF0_0);
	GPIOA->CRL &= ~(GPIO_CRL_CNF0_1);
	
	/*GPIOA->CRL &= ~(GPIO_CRL_MODE1_0);
	GPIOA->CRL &= ~(GPIO_CRL_MODE1_1);
	GPIOA->CRL &= ~(GPIO_CRL_CNF1_0);
	GPIOA->CRL &= ~(GPIO_CRL_CNF1_1);
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE2_0);
	GPIOA->CRL &= ~(GPIO_CRL_MODE2_1);
	GPIOA->CRL &= ~(GPIO_CRL_CNF2_0);
	GPIOA->CRL &= ~(GPIO_CRL_CNF2_1);
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE3_0);
	GPIOA->CRL &= ~(GPIO_CRL_MODE3_1);
	GPIOA->CRL &= ~(GPIO_CRL_CNF3_0);
	GPIOA->CRL &= ~(GPIO_CRL_CNF3_1);
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE4_0);
	GPIOA->CRL &= ~(GPIO_CRL_MODE4_1);
	GPIOA->CRL &= ~(GPIO_CRL_CNF4_0);
	GPIOA->CRL &= ~(GPIO_CRL_CNF4_1);*/
	
	// Chon SEQUENCE va CHANNEL
	//ADC1->SQR1 |=  ADC_SQR1_L_2 | ADC_SQR1_L_0;
	ADC1->SQR3 &= ~(ADC_SQR3_SQ1_0);
	//ADC1->SQR3 |= ADC_SQR3_SQ2_0;
	//ADC1->SQR3 |= ADC_SQR3_SQ3_1;
	//ADC1->SQR3 |= ADC_SQR3_SQ4_1 | ADC_SQR3_SQ4_0;
	//ADC1->SQR3 |= ADC_SQR3_SQ5_2;
	// ADC Config
	//ADC1->CR1 |= ADC_CR1_DISCEN;
	//ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR2 |= ADC_CR2_CONT;
	// DMA Enable
	//ADC1->CR2 |= ADC_CR2_DMA;
	// ADC1 EXternal trigger
	//ADC1->CR2 |= ADC_CR2_EXTSEL_2 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_0;
	//ADC1->CR2 |= ADC_CR2_EXTTRIG;
	// ADC1 CAL
	ADC1->CR2 |= ADC_CR2_CAL;
	// ADC1 ON
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_ADON;
}	 
uint32_t ADC_READ()
{
	while (!(ADC1->SR & ADC_SR_EOC));
	return ADC1->DR;
}

void DMA_PTM_INIT(uint32_t temp)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CCR &= ~(DMA_CCR1_EN);
	DMA1_Channel1->CCR &= ~(DMA_CCR1_MEM2MEM);
	DMA1_Channel1->CCR |= DMA_CCR1_PL_0 | DMA_CCR1_PL_1;
	DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR1_MINC;
	DMA1_Channel1->CCR |= DMA_CCR1_CIRC;
	DMA1_Channel1->CCR &= ~(DMA_CCR1_DIR);
	DMA1_Channel1->CNDTR = 5;
	DMA1_Channel1->CPAR = 0x4001244C;
	DMA1_Channel1->CMAR = temp;
	temp = DMA1_Channel1->CMAR;
	DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

void Blink()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH |= GPIO_CRH_MODE13_0;
	GPIOC->CRH |= GPIO_CRH_CNF13_0;
	
	DELAY_INIT();
	
	while(1)
	{
		GPIOC->BSRR |= GPIO_BSRR_BR13;
		DELAY_MS(1000);
		GPIOC->BSRR |= GPIO_BSRR_BS13;
		DELAY_MS(1000);
	}
}
int main()
{
	char str1[20];
	//char str2[20];
	//char str3[20];
	//char str4[20];
	//char str5[20];
	
	//uint16_t *q;
	//uint16_t data[5]={5,0,0,0,0};
	//q=&data[0];
	//uint32_t temp = (intptr_t) q;
	uint32_t adc_value;
	
	DELAY_INIT();
	UART_INIT();
	ADC_INIT();
	DELAY_MS(10);
	//DMA_PTM_INIT(temp);
	while(1)
	{
		/*ADC1->CR2 |= ADC_CR2_SWSTART;
		DELAY_MS(200);
		ADC1->CR2 |= ADC_CR2_SWSTART;
		DELAY_MS(200);
		ADC1->CR2 |= ADC_CR2_SWSTART;
		DELAY_MS(200);
		ADC1->CR2 |= ADC_CR2_SWSTART;
		DELAY_MS(200);
		ADC1->CR2 |= ADC_CR2_SWSTART;
		DELAY_MS(200);*/
		adc_value = ADC_READ();
		sprintf(str1,"Ch1: %d",adc_value);
		//sprintf(str2,"Ch2: %d",data[1]);
		//sprintf(str3,"Ch3: %d",data[2]);
		//sprintf(str4,"Ch4: %d",data[3]);
		//sprintf(str5,"Ch5: %d",data[4]);

		UART_SendString(str1);
		//UART_SendString(str2);
		//UART_SendString(str3);
		//UART_SendString(str4);
		//UART_SendString(str5);
		UART_SendString("-------------------------------");
		DELAY_MS(1000);
		
	
	}
}