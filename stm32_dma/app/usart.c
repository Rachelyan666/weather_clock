#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "usart.h"
#include "stm32f10x.h"


#define LED_PORT GPIOC
#define LED_PIN GPIO_Pin_13

#define USART_PORT GPIOA
#define TX_PIN GPIO_Pin_9
#define RX_PIN GPIO_Pin_10

static usart_receive_callback_t usart_receive_callback;


void usart_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable USARTy Receive and Transmit interrupts */
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* Enable USARTy DMA TX request */
  	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);	

}

void usart_write_async(uint8_t *data, uint16_t length){
	//dma
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel4); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = length;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	/* Enable USART1 DMA TX Channel */
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void usart_write_data(uint8_t *data, uint16_t length){
	for (uint16_t i = 0; i < length; i++){
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, data[i]);
	}
}

void usart_write_string(const char *str){
	uint16_t len = strlen(str);
	usart_write_data((uint8_t *)str, len);
}

void usart_receive_register(usart_receive_callback_t callback){
	usart_receive_callback = callback;
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		/* Read one byte from the receive data register */
		uint8_t data = USART_ReceiveData(USART1);

		if (usart_receive_callback){
			usart_receive_callback(data);
		}
	}
}
