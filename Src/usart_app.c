/*
 * usart_app.c
 */
#include "usart_app.h"

/*
 * SendSerialData: Sends serial data to the RX device
 * @param: usart:  pointer to USART structure
 * @param: format: format of the TX data
 * @param: ...   : data to display
 * @return void
 */
void SendSerialData(USART_Handle_t *usart, const char *format, ...)
{
	size_t size = strlen(format);
//	char serialBuffer[strlen(format)] = {0};
	char serialBuffer[100] = {0};
	va_list args;
	va_start(args, format);
	vsprintf(serialBuffer, format, args);

	usart->txBuffer = serialBuffer;
	usart->txLength = strlen(serialBuffer);

	USART_State expectedState = usart->session ? USART_RX_BUSY : USART_READY;
	while (USART_TransmitData() != expectedState);

//	USART_DMA_Transmit(usart, sizeof(serialBuffer));
//	USART_DMA_Transmit(usartHandle, dmaHandle); //
	va_end(args);						// clean memory reserved for valist
//	usart->USART_State = USART_INIT;
}

/*
 * ReceiveSerialData: Receive serial data from the slave
 * @param: usart:  pointer to USART structure
 * @return void
 */
void ReceiveSerialData(USART_Handle_t *usart)
{
	while (USART_RxData(USART_RX_BUSY) != USART_READY);
}

/*
 * ParseSerialData: Parses user input
 * @param: tempBuffer: 		temporary buffer that stores the parsed data
 * @param: usart_rxBuffer:  pointer to rx buffer that stores the user input
 * @return void
 */
void ParseSerialData(USART_Handle_t *usart, char *tempBuffer) // hello\r\world\r
{
	char *start = usart->rxBuffer;
	char *end = strstr(start, "\r");
	uint8_t bytes = end - start;
	memcpy(tempBuffer, start, bytes);
}

/*
 * ExecuteSerialData: Executes certain operations based on the user input
 * @param: usart: pointer to usart structure
 * @param: str1:  hardcoded string to compare the user input against
 * @param: I2C_Handle_t: pointer to I2C_Handle_t struct
 * @return false if "q" (quit) is entered, otherwise true
 */
bool ExecuteSerialData(USART_Handle_t *usart, const char *str1, I2C_Handle_t *I2C_Handle)
{
	unsigned char payload[10] = {0};
	if (!strcmp(str1, "temp"))
	{
		uint16_t temp = GetTemperature(SET, I2C_Handle);
		SendSerialData(usart, "Current temperature: %d\n", temp);
		sprintf (payload, "%u", temp);							// todo - verify signedness
	    nRF24_TransmitPayload(&nrfRadio, payload, 2);

	}
	else if (!strcmp(str1, "led"))
	{
		SendSerialData(usart, "Toggling LED\n");
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	else if (!strcmp(str1, "q"))
	{
		SendSerialData(usart, "No longer accepting serial data...\n");
		return false;
	}
	else if (!strcmp(str1, "stop"))
	{
		sprintf (payload, "%s", str1);
	    nRF24_TransmitPayload(&nrfRadio, payload, strlen(str1)+1);
	}
	return true;
}

/*
 * StartSerialSession: starts USART transaction until "q" is entered: reads in user input, parses,
 *                     and then execute accordingly
 * @param: usart: 			pointer to usart structure
 * @param: usart_rxBuffer:  pointer to rx buffer that stores user inputs
 * @param: rxBufferSize: 	size of the rx buffer
 * @return void
 */
void StartSerialSession (USART_Handle_t *usart, uint8_t rxBufferSize, I2C_Handle_t *I2C_Handle)
{
	char tempBuffer[rxBufferSize];
	memset(tempBuffer, 0, rxBufferSize);
	while(true)
	{
		ReceiveSerialData(usart);
		ParseSerialData(usart, tempBuffer);
		bool status = ExecuteSerialData(usart, tempBuffer, I2C_Handle);
		if (!status)		// break if "q" is entered
		{
			break;
		}

		// clear out/reset the buffers
//		usart->rxBuffer = usart_rxBuffer;
//		memset(usart_rxBuffer, 0, sizeof(rxBufferSize));
		memset(tempBuffer, 0, sizeof(tempBuffer));

		// reset the USART state
		usart->USART_State = USART_INIT;
	}
}

void USART_EnableRxInterrupts()
{
	USART_RxData(USART_RX_BUSY);
}

/*
 * @SerialRead: reads the FIFO that contains the meaningful user input (containing \r)
 */
void SerialRead(USART_Handle_t *usart, I2C_Handle_t *I2C_Handle)
{
	char token[usart->rxSize];
	memset(token, 0, usart->rxSize);

	// parse the data
	char *dataStart = usart->rxBuffer + usart->rxIdx;
	char *dataEnd = strstr(dataStart, "\r");
	uint8_t bytes;

	if (dataEnd == NULL) // wrap-around
	{
		char *lastChar = strchr(dataStart, '\0');
		bytes = lastChar - dataStart;
		memcpy(token, dataStart, bytes);
		dataEnd = strstr(usart->rxBuffer, "\r");
		memcpy(token + bytes, usart->rxBuffer, dataEnd - usart->rxBuffer);
	}
	else
	{
		bytes = dataEnd - dataStart;
		memcpy(token, dataStart, bytes);
	}

	// execute the user input request
	ExecuteSerialData(usart, token, I2C_Handle);

	usart->RxEndOfLineIdx++;
	usart->rxIdx = usart->txIdx;
}



//void DMA_StartSerialSession(USART_Handle_t *usartHandle, byte bufferSize)
//{
//	/*
//	 * get user input
//	 * process it
//	 * execute it
//	 */
////	while(true)
////	{
////		ReceiveSerialData(usartHandle);
//		uint16_t temp = GetTemperature(SET);
//		SendSerialData(usartHandle, "Current temperature: %d\n", temp);
////		USART_DMA_Transmit(usartHandle);
//		USART_DMA_Receive(usartHandle, bufferSize);
////	}
//}

