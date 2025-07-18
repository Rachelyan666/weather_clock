#ifndef __USART_H
#define __USART_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef void (*usart_send_finish_callback_t)(void);
typedef void (*usart_receive_callback_t)(uint8_t data);

void usart_init(void);
void usart_write_data(uint8_t *data, uint16_t length, bool async);
void usart_write_string(const char *str, bool async);
void usart_send_finish_register(usart_send_finish_callback_t callback);
void usart_receive_register(usart_receive_callback_t callback);

#endif /*__USART_H*/
