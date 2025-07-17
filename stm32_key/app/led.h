#ifndef __LED_H
#define __LED_H

#include <stdbool.h>

void led_init(void);
void led_set(bool on);
void led_on(void);
void led_off(void);

#endif  /*__LED_H*/
