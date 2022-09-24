#include "my_serial.h"
#include "main.h"


/**/
USART_InitTypeDef USART_InitStructure; 
GPIO_InitTypeDef GPIO_InitStructure;

void UART_RCC_INIT()
{
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}
/**/
void UART_GPIO_INIT()
{
    	/* Configure PA2 for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure PA10 for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/**/
void UART_INIT()
{
     /*--------------------------------------------------------*/
     
     USART_DeInit(SERIAL);
     UART_RCC_INIT();
     UART_GPIO_INIT();
     
     //USART_StructInit(&USART_InitStructure);
     
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);	
     /*--------------------------------------------------------*/
}

/**/
uint8_t UART_GetChar()
{
     uint8_t Data;
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	Data = (uint8_t)USART_ReceiveData(USART2);
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
    USART_SendData(USART2,Data);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

}



//GETCHAR_PROTOTYPE
//{
//  return UART_GetChar();
//}

//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//   USART_SendData(USART2,ch);

//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
//  {}

//  return ch;
//}