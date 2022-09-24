#include "main.h"

/**/
void UART_RCC_INIT()
{
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
/**/GPIO_InitTypeDef GPIO_InitStructure;
void UART_GPIO_INIT()
{
    	/* Configure PA2 for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = SERIAL_Tx_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure PA3 for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = SERIAL_Rx_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
USART_InitTypeDef USART_InitStructure;
/**/
void UART_INIT()
{
     /*--------------------------------------------------------*/
     
     USART_DeInit(SERIAL);
     UART_RCC_INIT();
     UART_GPIO_INIT();
     
     //USART_StructInit(&USART_InitStructure);
     
	USART_InitStructure.USART_BaudRate = SERIAL_Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(SERIAL, &USART_InitStructure);
	
	/* Enable USART Rx interupt */
	USART_ITConfig(SERIAL, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(SERIAL, ENABLE);	
     /*--------------------------------------------------------*/
}

/**/
uint8_t UART_GetChar()
{
  uint8_t Data;
	while(USART_GetFlagStatus(SERIAL, USART_FLAG_RXNE) == RESET);
	Data = (uint8_t)USART_ReceiveData(SERIAL);
	return Data;
}

void UART_GetString(char *input)
{
	uint8_t i = 0;	
	uint8_t temp;
	
	while(1)
	{
		temp = UART_GetChar();
          if(temp == '\n' && i==0)
          {
               ;
          }
		else if(temp == 0xD) //enter character
			break;
		else
		{
			input[i] = temp;
			i++;	
		}
	}
	//i = 0;
}
void UART_SendChar(uint16_t Data)
{
    USART_SendData(SERIAL,Data);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(SERIAL, USART_FLAG_TC) == RESET)
  {}
}

void UART_SendString(char *Data)
{
	int i=0;
	while((Data[i])!='\0')
	{
		UART_SendChar(Data[i]);
		i++;
	}
}

void UART_SendInt(int Number)
{
		char Data,i=0,j=0;
		uint8_t buffer[10];
		if (Number==0)
		{
				UART_SendChar('0');
		}
		else
		{
				for (i = 10; i > 0 && Number > 0; i--) 
				{
						buffer[i] = (Number%10)+'0';
						Number /= 10;
				}

				for(j = i+1; j <= 10; j++) 
				{
						UART_SendChar(buffer[j]);	
				}
		}
}

GETCHAR_PROTOTYPE
{
  return UART_GetChar();
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
   USART_SendData(USART2,ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
