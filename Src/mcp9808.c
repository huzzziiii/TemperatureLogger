
#include <mcp9808.h>
 /* Function definitions ------------------------------------------------------------------*/
static void _ReadTemperature(I2C_Handle_t *I2C_handle);
static uint16_t _ReadTemperature_IT(I2C_Handle_t *I2C_handle);

// static variables
static uint8_t txBuffer[1] = {MCP9808_REG_AMBIENT_TEMP_REG}; // the register that contains ambient temperature values
static uint8_t rxBuffer[BYTES_TO_READ];
static uint8_t txSize = sizeof(txBuffer)/sizeof(txBuffer[0]);
static uint8_t rxSize = BYTES_PER_TRANSACTION;
/*
 * @ProcessData: Converts raw bytes into readable data
 */
uint16_t ProcessData (uint8_t *rxBuffer) {
	uint8_t startIndex = 0;
	//printf ("\nUpper byte: %d, Lower byte: %d\n", rxBuffer[startIndex], rxBuffer[startIndex+1]);
	uint16_t temperatureInDegrees;

	// process data
	uint8_t upperByte = rxBuffer[startIndex] & 0x1F; // mask out the 3 bits
	uint8_t signBit = upperByte & 0x10;

	if (signBit)
	{
		upperByte = upperByte & 0xF; 	// clear out the sign bit
		temperatureInDegrees = 256 - (upperByte << 4 | rxBuffer[startIndex+1] >> 4);
	}
	else
	{
		temperatureInDegrees = upperByte << 4 | rxBuffer[startIndex+1] >> 4;
	}
//	printf ("Temperature value: %d\n\n", temperatureInDegrees);
	return temperatureInDegrees;
}

/*
 * @ReadTemperatureInterrupt: Reads temperature values via I2C using interrupts
 */
uint16_t _ReadTemperature_IT(I2C_Handle_t *I2C_handle)
{
	I2C_handle->txBuffer = txBuffer;
	I2C_handle->pRxBuffer = rxBuffer;
	I2C_handle->rxBufferSize = rxSize;

	// Start I2C transaction
	while (HAL_I2C_StartInterrupt(I2C_TX_BUSY, txSize, rxSize) != I2C_READY);

	I2C_handle->I2C_State = I2C_INIT;

	// request the data from the sensor
	for (int i = 0; i < I2C_handle->rxBufferSize/2; i++)
	{
		I2C_handle->I2C_State = I2C_INIT;
		while (HAL_I2C_StartInterrupt(I2C_RX_BUSY, txSize, rxSize) != I2C_READY);
	}

	uint16_t temperature = ProcessData(I2C_handle->pRxBuffer);
	return temperature;
}

/*
 * @ReadTemperature: Read temperature via I2C using polling approach
 */
void _ReadTemperature(I2C_Handle_t *I2C_handle)
{
	// todo - maybe use the following data straight from I2C_handle
	uint8_t txBuffer[1] = {MCP9808_REG_AMBIENT_TEMP_REG};
	uint8_t rxBuffer[BYTES_TO_READ];
	uint8_t startRxIndex = 0;

//	uint8_t txSize = sizeof(txBuffer)/sizeof(txBuffer[0]);

	// specify the register address where temperature values will be read from
	HAL_I2C_Master_Transmit(I2C_handle, txBuffer, txSize);

	// request the data from the sensor
	for (int i = 0; i < BYTES_TO_READ/2; i++, startRxIndex+=2)
	{
		HAL_I2C_Master_Receive (I2C_handle, rxBuffer, BYTES_PER_TRANSACTION, startRxIndex);
	}

//	printf ("Printing raw bytes:\n");
	for (int i = 0; i < BYTES_TO_READ; i+=2)
	{
		ProcessData(rxBuffer);
	}
}

uint16_t GetTemperature(uint8_t interrupt, I2C_Handle_t *I2C_Handle)
{
	uint16_t temperature;

	if (interrupt == SET)
	{
		temperature = _ReadTemperature_IT(I2C_Handle);
	}
	else
	{
		_ReadTemperature(I2C_Handle);
	}
	return temperature;
}
