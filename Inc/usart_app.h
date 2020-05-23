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
void ParseSerialData(USART_Handle_t *usart, char *tempBuffer, char *tempPtr);
void StartSerialSession (USART_Handle_t *usart, char *usart_rxBuffer, uint8_t rxBufferSize, I2C_Handle_t *I2C_Handle, uint8_t txSize, uint8_t rxSize);
void USART_Init(void);


#endif
