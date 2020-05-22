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
void ParseSerialData(USART_Handle_t *usart, char *tempBuffer, char *usart_rxBuffer) // hello\r\world\r
{
//	char *start = *tempPtr;
//	char *end = strstr(*tempPtr , "\r");
//	uint8_t bytes = end - start;
//	memcpy(tempBuffer, start, bytes);
//	*tempPtr = usart->rxBuffer;

	char *start = usart_rxBuffer;
	char *end = strstr(usart_rxBuffer, "\r");
	uint8_t bytes = end - start;
	memcpy(tempBuffer, start, bytes);
}

/*
 * ExecuteSerialData: Executes certain operations based on the user input
 * @param: usart: pointer to usart structure
 * @param: str1:  hardcoded string to compare the user input against
 * @return false if "q" (quit) is entered, otherwise true
 */
bool ExecuteSerialData(USART_Handle_t *usart, const char *str1)
{
	if (!strcmp(str1, "temp"))
	{
		uint16_t temp = GetTemperature(SET);
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
void StartSerialSession (USART_Handle_t *usart, char *usart_rxBuffer, uint8_t rxBufferSize)
{
	char tempBuffer[20] = {0};
	while(true)
	{
		ReceiveSerialData(usart);
		ParseSerialData(usart, tempBuffer, usart_rxBuffer);
		bool status = ExecuteSerialData(usart, tempBuffer);
		if (!status)		// break if "q" is entered
		{
			break;
		}

		// clear out/reset the buffers
		usart->rxBuffer = usart_rxBuffer;
		memset(usart_rxBuffer, 0, sizeof(rxBufferSize));
		memset(tempBuffer, 0, sizeof(tempBuffer));

		// reset the USART state
		usart->USART_State = USART_INIT;
	}
}
