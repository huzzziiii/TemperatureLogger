#ifndef __MCP9808_H
#define __MCP9808_H

#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal.h"

uint16_t GetTemperature(uint8_t interrupt, I2C_Handle_t *I2C_Handle);


#define MCP9808_ADDR 					0x18
#define MCP9808_REG_AMBIENT_TEMP_REG	0x5

#define BYTES_TO_READ 2
#define TEMPERATURE_REGISTER 			MCP9808_REG_AMBIENT_TEMP_REG
#define BYTES_PER_TRANSACTION			2

//uint8_t txBuffer[1] = {MCP9808_REG_AMBIENT_TEMP_REG}; // the register that contains ambient temperature values
//uint8_t rxBuffer[BYTES_TO_READ];
//uint8_t txSize = sizeof(txBuffer)/sizeof(txBuffer[0]);
//uint8_t rxSize = BYTES_PER_TRANSACTION;


#endif
