#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal.h"

#define MCP9808_ADDR 					0x18
#define MCP9808_REG_AMBIENT_TEMP_REG	0x5

/* Function definitions ------------------------------------------------------------------*/
void ReadTemperature(I2C_Handle_t *I2C_handle, const uint8_t bytesToRead);
uint16_t ReadTemperatureInterrupt(I2C_Handle_t *I2C_handle);
