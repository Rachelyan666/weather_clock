#ifndef __USART_H
#define __USART_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef void (*usart_receive_callback_t)(uint8_t data);

void usart_init(void);
void usart_write_async(uint8_t *data, uint16_t length);
void usart_write_data(uint8_t *data, uint16_t length);
void usart_write_string(const char *str);
void usart_receive_register(usart_receive_callback_t callback);

#endif  /*__USART_H*/
