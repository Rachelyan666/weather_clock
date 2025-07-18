#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"

static volatile bool usart_send_busy;
static usart_send_finish_callback_t usart_send_finish_callback;
static usart_receive_callback_t usart_receive_callback;

static void usart_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void usart_nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void usart_lowlevel_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void usart_init(void)
{
	usart_gpio_init();
	usart_nvic_init();
	usart_lowlevel_init();
}

void usart_write_data(uint8_t *data, uint16_t length, bool async)
{
	DMA_DeInit(DMA1_Channel4);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)data;
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
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	usart_send_busy = true;
	DMA_Cmd(DMA1_Channel4, ENABLE);

	if (!async)
	{
		while (usart_send_busy)
			;
	}
}

void usart_write_string(const char *str, bool async)
{
	uint16_t len = strlen(str);
	usart_write_data((uint8_t *)str, len, async);
}

void usart_send_finish_register(usart_send_finish_callback_t callback)
{
	usart_send_finish_callback = callback;
}

void usart_receive_register(usart_receive_callback_t callback)
{
	usart_receive_callback = callback;
}

void DMA1_Channel4_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC4) == SET)
	{
		usart_send_busy = false;

		if (usart_send_finish_callback)
			usart_send_finish_callback();

		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t data = USART_ReceiveData(USART1);

		if (usart_receive_callback)
			usart_receive_callback(data);
	}
}
