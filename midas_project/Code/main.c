#include "main.h"

/***************************************
DA BO SYSTICK_HANDLER TRONG stm32f10x_it
***************************************/






#define RUN_MOTOR(id,value)   TIM1->CCR##id = value*10 + 900

/**************************************/
uint32_t time=0;
int flag = 0;
int a_deadband = 100;
int b_deadband = 50;
int c_deadband = 50;
int d_deadband = 50;
int a_ADC = 0;
int b_ADC = 0;
int c_ADC = 0;
int d_ADC = 0;
uint16_t ADCValue[4]={0};												// Mang gia tri ADC do duoc
volatile char HJValue[12]={'*','a','f','b','f','c','f','d','f','e','f','?'};	// Mang chuyen doi ADC toc do servo
volatile char RecValue[12]={0,0,0,0,0,0,0,0,0,0,0,0};						// Mang nhan du lieu tu gang tay sang canh tay
char transfer[2];
int id,vl,i;
int temp = 0;
handjob_type b;
uint32_t usTicks;
  
/***************************************/

void DELAY_INIT()
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000000);
}

void DELAY_US(uint32_t us)
{
	usTicks = us;
	while (usTicks);
}

void DELAY_MS(uint32_t ms)
{
	while (ms--)
	{
		DELAY_US(1000);
	}
}

void TIMbase_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock)/1000000)-1;     // frequency = 1000000
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void GPIO_INIT(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	/*RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void ADC_INIT()
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                 
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1, &ADC_InitStructure);
 
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
  
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*ADC1 Clock*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	/* ADC Channel Regular config */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	
	
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
 
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
     
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}	
                                       

void DMA_PTM_INIT() // truyen temp
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* Disable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                     
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

uint32_t ADC_READ()
{
	while (!(ADC1->SR & ADC_SR_EOC));
	return ADC1->DR;
}

int filter(int value, int *n_ADC, int n_deadband)
{
	if (abs(*n_ADC - value) > n_deadband)
	{
		*n_ADC = value;
	}
	return *n_ADC;
}

char NgonNhan(int value)
{
	/*if (value <= 976)
		return '8';
	else if (value >= 977 && value <= 1049)
		return '7';
	else if (value >= 1050 && value <= 1122)
		return '6';
	else if (value >= 1123 && value <= 1195)
		return '5';
	else if (value >= 1196 && value <= 1268)
		return '4';
	else if (value >= 1269 && value <= 1341)
		return '3';
	else if (value >= 1342 && value <= 1414)
		return '2';
	else if (value >= 1415 && value <= 1487)
		return '1';
	else if (value >= 1488 && value <= 1560)
		return '0';
	else return '0';*/
	
	if (value > 800 && value < 1000)
		return '7';
	if (value < 800 )
		return '0';
	return '0';
}

char NgonGiua(int value)
{
	/*if (value <= 1237)
		return '8';
	else if (value >= 1238 && value <= 1306)
		return '7';
	else if (value >= 1307 && value <= 1375)
		return '6';
	else if (value >= 1376 && value <= 1444)
		return '5';
	else if (value >= 1445 && value <= 1513)
		return '4';
	else if (value >= 1514 && value <= 1583)
		return '3';
	else if (value >= 1584 && value <= 1652)
		return '2';
	else if (value >= 1653 && value <= 1721)
		return '1';
	else if (value >= 1722 && value <= 1790)
		return '0';
	else return '0';*/
		
	if (value > 1000 && value < 1300)
		return '6';
	if (value < 1000 )
		return '0';
	return '0';
}
char NgonTro(int value)
{
	/*if (value <= 804)
		return '8';
	else if (value >= 805 && value <= 881)
		return '7';
	else if (value >= 882 && value <= 958)
		return '6';
	else if (value >= 959 && value <= 1035)
		return '5';
	else if (value >= 1036 && value <= 1112)
		return '4';
	else if (value >= 1113 && value <= 1189)
		return '3';
	else if (value >= 1190 && value <= 1266)
		return '2';
	else if (value >= 1267 && value <= 1343)
		return '1';
	else if (value >= 1344)
		return '0';
	else return '0';*/
	
	if (value > 800 && value < 1000)
		return '6';
	if (value < 530 )
		return '0';
	return '0';
}

char NgonCai (int value)
{
	/*if (value >= 840)
		return '0';
	else
		return '2';*/
	
	if (value > 620)
		return 0;
	if (value > 395 && value < 450)
		return 0;
}
void Glove()
{
	GPIO_INIT();
	ADC_INIT();
	DMA_PTM_INIT();
	UART_INIT();
	DELAY_INIT();
	TIMbase_Configuration();
		while(1)
		{
			if (flag == 0)
			{
				TIM_Cmd(TIM4, DISABLE);
				HJValue[2] = '3';	//e 
				HJValue[4] = NgonTro(filter(ADCValue[1],&d_ADC,d_deadband));	//d				
				HJValue[6] = NgonGiua(filter(ADCValue[2],&c_ADC,c_deadband)); //c
				HJValue[8] = HJValue[6]; //NgonNhan(filter(ADCValue[3],&b_ADC,b_deadband)); //b
				HJValue[10] = HJValue[6]; //NgonNhan(filter(ADCValue[3],&a_ADC,a_deadband));//a
//				UART_SendChar(HJValue[0]);
//				UART_SendChar(HJValue[1]);
//				UART_SendChar(HJValue[2]);
//				UART_SendChar(HJValue[3]);
//				UART_SendChar(HJValue[4]);
//				UART_SendChar(HJValue[5]);
//				UART_SendChar(HJValue[6]);
//				UART_SendChar(HJValue[7]);
//				UART_SendChar(HJValue[8]);
//				UART_SendChar(HJValue[9]);
//				UART_SendChar(HJValue[10]);
//				UART_SendChar(HJValue[11]);
				flag = 1;
				TIM_Cmd(TIM4, ENABLE);
			}
		}
}

void Hand()
{
	char transfer[2];
  int id,vl,i;
	int temp = 0;
  handjob_type b;
  /*Initialization*/
	UART_INIT();
	DELAY_INIT();
  HJ_PWM_Init();
  //HJ_Init(&b);
  while(1)
  {
		while(1)
		{
 			RecValue[temp] = UART_GetChar();
			if (RecValue[temp] == '*')
				temp = 0;
			if (RecValue[temp] == '?' && temp == 11)
			{
				break;
			}
			temp++;
		}
		temp = 0;
		RecValue[11] = 'z';
		UART_SendChar('0');
          /*
          formula:     <motor_name> + <value> + "\n"
                        params:ma
          <motor_name>: a,b,c or d
          <value> 0 -> 9 (0 degree -> ~180 degree)
          example: "a0\n"    motor_a 0 degree
                   "b5\n"    motor_b 90 degree    
          */
 
		//HJ_Init(&b);
		for( i =1; i<11; i=i+2)
		{
			transfer[0]= RecValue[i];
			transfer[1]= RecValue[i+1];
			//b = HJ_StringToHJ(transfer); /*transfer string to hanjob types*/
          
			//vl = b.motor_angle[id];
      transfer[0] = transfer[0] - 'a';
			transfer[1] = transfer[1] - '0';
			
			switch(transfer[0])
				{
					case 0: 
					{
						RUN_MOTOR(1,transfer[1]);
						//UART_SendString("Done 1");
						break;
					}
					case 1: 
					{
						RUN_MOTOR(2,transfer[1]);
						//UART_SendString("Done 2");					
						break;
					}
					case 2: 
					{
						RUN_MOTOR(4,transfer[1]);
						//UART_SendString("Done 3");						
						break;
					}
					case 3: 
					{
						RUN_MOTOR(3,transfer[1]);
						//UART_SendString("Done 4");
						break;
					}
					case 4:
					{
						RUN_MOTOR(3,transfer[1]);
						//UART_SendString("Done 5");
						break;
					}
					default:
						break;
				}
				DELAY_MS(700);
		}
		UART_SendChar('1');
	}
}
void ADCSample()
	{
	GPIO_INIT();
	ADC_INIT();
	DMA_PTM_INIT();
	UART_INIT();
	DELAY_INIT();
	while(1)
	{
		UART_SendInt(filter(ADCValue[0],&d_ADC,d_deadband));
		UART_SendChar('\n');
		UART_SendInt(filter(ADCValue[1],&c_ADC,c_deadband));
		UART_SendChar('\n');
		UART_SendInt(filter(ADCValue[2],&b_ADC,b_deadband));
		UART_SendChar('\n');
		UART_SendInt(filter(ADCValue[3],&a_ADC,a_deadband));
		UART_SendChar('\n');
		UART_SendString("------------");
		UART_SendChar('\n');
		
		HJValue[2] = '3';	//e 
		HJValue[4] = NgonTro(filter(ADCValue[1],&d_ADC,d_deadband));	//d				
		HJValue[6] = NgonGiua(filter(ADCValue[2],&c_ADC,c_deadband)); //c
		HJValue[8] = HJValue[6]; //NgonNhan(filter(ADCValue[3],&b_ADC,b_deadband)); //b
		HJValue[10] = HJValue[6]; //NgonNhan(filter(ADCValue[3],&a_ADC,a_deadband));//a
		
		
		UART_SendChar(HJValue[0]);
		UART_SendChar(HJValue[1]);
		UART_SendChar(HJValue[2]);
		UART_SendChar(HJValue[3]);
		UART_SendChar(HJValue[4]);
		UART_SendChar(HJValue[5]);
		UART_SendChar(HJValue[6]);
		UART_SendChar(HJValue[7]);
		UART_SendChar(HJValue[8]);
		UART_SendChar(HJValue[9]);
		UART_SendChar(HJValue[10]);
		UART_SendChar(HJValue[11]);
		UART_SendChar('\n');
		UART_SendString("------------");
		UART_SendChar('\n');
		DELAY_MS(500);	
	}
}
int main ()
{
	Glove();
	//ADCSample();
	//Hand();
}
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		if(++time>100)
		{
			UART_SendChar(HJValue[0]);
			UART_SendChar(HJValue[1]);
			UART_SendChar(HJValue[2]);
			UART_SendChar(HJValue[3]);
			UART_SendChar(HJValue[4]);
			UART_SendChar(HJValue[5]);
			UART_SendChar(HJValue[6]);
			UART_SendChar(HJValue[7]);
			UART_SendChar(HJValue[8]);
			UART_SendChar(HJValue[9]);
			UART_SendChar(HJValue[10]);
			UART_SendChar(HJValue[11]);
			time = 0;
		}
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
	}
}

void USART2_IRQHandler(void)
{
	uint16_t rx; 
  if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET) // check RX interrupt
  {
		rx = USART_ReceiveData(USART2);
		if (rx == '0')
		{
			TIM_Cmd(TIM4, DISABLE);
		}
		else if (rx == '1')
			flag = 0;
			else
				TIM_Cmd(TIM4, ENABLE);
  }
}

void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}
