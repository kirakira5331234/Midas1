#ifndef HJ_Serial_H
#define HJ_Serial_H

/*--------------------------------------------------*/
#include "stm32f10x_usart.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
  
  
  
#define SERIAL_Rx_Pin    GPIO_Pin_3
#define SERIAL_Tx_Pin    GPIO_Pin_2
#define SERIAL_GPIO GPIOA

#define SERIAL_Baud 9600
#define SERIAL USART2

/**/
void UART_RCC_INIT();

/**/
void UART_GPIO_INIT();

/**/
void UART_INIT();
uint8_t UART_GetChar();
void UART_GetString(char *input);
void UART_SendChar(uint16_t Data);
void UART_SendString(char *Data);
void UART_SendInt(int Number);
/**/

#endif /*HJ_Serial_H*/
