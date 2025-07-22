#ifndef __PWM_H__
#define __PWM_H__


#include <stdbool.h>
#include <stdint.h>


void pwm_init(uint32_t period_us);
void pwm_set_duty(uint32_t ch, uint32_t duty_us);
void pwm_start(void);
void pwm_stop(void);


#endif /* __PWM_H__ */
