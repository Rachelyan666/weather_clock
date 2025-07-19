#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include "stm32f10x.h"

void delay(uint32_t x);
void board_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif  /*__LED_H*/
