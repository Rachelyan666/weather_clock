#ifndef __ESP_USART__
#define __ESP_USART__


#include <stdbool.h>
#include <stdint.h>


typedef void (*esp_usart_send_finish_callback_t)(void);
typedef void (*esp_usart_receive_callback_t)(uint8_t data);


void esp_usart_init(void);
void esp_usart_write_data(uint8_t *data, uint16_t length, bool async);
void esp_usart_write_string(const char *str, bool async);
void esp_usart_send_finish_register(esp_usart_send_finish_callback_t callback);
void esp_usart_receive_register(esp_usart_receive_callback_t callback);


#endif /* __ESP_USART__ */
