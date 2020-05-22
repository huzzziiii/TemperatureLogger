#include "stm32f4xx_hal_usart.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SendSerialData(USART_Handle_t *usart, const char *format, ...);
void ReceiveSerialData(USART_Handle_t *usart);
void ParseSerialData(USART_Handle_t *usart, char *tempBuffer, char *tempPtr);
void StartSerialSession (USART_Handle_t *usart, char *usart_rxBuffer, uint8_t rxBufferSize);
void USART_Init (void);
