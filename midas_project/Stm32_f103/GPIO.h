#include <stdint.h>
#include "stm32f10x.h"

class	GPIO
{
	private :
		
	public	:
		GPIO();
		GPIO(uint16_t PORT);
		GPIO(uint16_t PORT, uint16_t PIN);
		~GPIO();
};