#ifndef __NTC_H
#define __NTC_H

#include <stdbool.h>
#include <stdint.h>

void ntc_init(void);
uint16_t ntc_read(void);
int adc_to_temp(uint16_t adc_value);

#endif /*__NTC_H*/
