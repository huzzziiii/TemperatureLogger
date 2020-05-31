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
	char serialBuffer[strlen(format)];
	va_list args;
	va_start(args, format);
	vsprintf(serialBuffer, format, args);

	usart->txBuffer = serialBuffer;
	usart->txLength = strlen(serialBuffer);

	while (USART_TransmitData() != USART_READY);

//	USART_DMA_Transmit(usart, sizeof(serialBuffer));
//	USART_DMA_Transmit(usartHandle, dmaHandle); //
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
//	char *start = rxBuffer;
//	char *end = strstr(rxBuffer, "\r");

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
	if (!strcmp(str1, "temp"))
	{
		uint16_t temp = GetTemperature(SET, I2C_Handle);
		SendSerialData(usart, "Current temperature: %d\n", temp);
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

static I2C_Handle_t *i2cHandle;
void USART_ReceiveData(USART_Handle_t *usart, I2C_Handle_t *i2c)
{
	i2cHandle = i2c;
	USART_RxData(USART_RX_BUSY);
}

void USART_ProcessingRxData(USART_Handle_t *usart)
{
	char tempBuffer[usart->rxSize];
	memset(tempBuffer, 0, usart->rxSize);

	ParseSerialData(usart, tempBuffer);
	ExecuteSerialData(usart, tempBuffer, i2cHandle);

	// clear out/reset the buffers
//	usart->rxBuffer = usart_rxBuffer;
//	memset(usart_rxBuffer, 0, sizeof(rxBufferSize));
//	memset(tempBuffer, 0, sizeof(tempBuffer));

	// reset the USART state
	usart->USART_State = USART_INIT;
}

void USART_ApplicationCallback(USART_Handle_t *usart)
{
	 USART_ProcessingRxData(usart);
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

