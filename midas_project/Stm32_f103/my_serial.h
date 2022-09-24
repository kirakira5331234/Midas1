#ifndef HJ_Serial_H
#define HJ_Serial_H

/*--------------------------------------------------*/
#include "main.h"

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
void UART_SendChar(uint16_t Data);
  
void UART_GetString(char *input);
/**/
uint8_t UART_GetChar();

#endif /*HJ_Serial_H*/