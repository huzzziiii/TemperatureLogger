#ifndef __USART_APP_H
#define __USART_APP_H


#include "stm32f4xx_hal_usart.h"
#include "mcp9808.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SendSerialData(USART_Handle_t *usart, const char *format, ...);
void ReceiveSerialData(USART_Handle_t *usart);
void ParseSerialData(USART_Handle_t *usart, char *tempBuffer);
void SerialRead(USART_Handle_t *usart, I2C_Handle_t *I2C_Handle);
void StartSerialSession (USART_Handle_t *usart, uint8_t rxBufferSize, I2C_Handle_t *I2C_Handle);
void USART_ProcessingRxData(USART_Handle_t *usart);
void USART_ApplicationCallback(USART_Handle_t *usart);

void USART_Init(void);

extern USART_Handle_t USART2_handle;

#endif
