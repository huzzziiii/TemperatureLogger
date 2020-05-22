/*
 * stm32f4xx_hal_usart.h
 */
#include <stdint.h>
#include <string.h>
#include "stdbool.h"
#include "../Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f401xe.h"
#include "../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"
#include "../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.h"


#ifndef USART__INC_STM32F4XX_HAL_USART_H_
#define USART__INC_STM32F4XX_HAL_USART_H_
/*
 * USART mode
 */
typedef enum {
	USART_MODE_TX,
	USART_MODE_RX,
	USART_MODE_TXRX
} USART_Mode;

typedef enum {
	DMA_TX_DISABLE = 0,
	DMA_RX_DISABLE = 0,
	DMA_TX_ENABLE = 1,
	DMA_RX_ENABLE = 1
} DMA_ControlBits;
/*
 * Stop bits
 */
typedef enum {
	USART_STOP,
	USART_HALF_STOP,
	USART_DOUBLE_STOPS,
	USART_ONE_HALF_STOPS
} USART_StopBits;

/*
 * Parity control
 */
typedef enum {
	USART_PARITY_DISABLED,
	USART_PARITY_ENABLED
} USART_Parity;

/*
 * Word length
 */
typedef enum {
	USART_8_DATA_BITS,
	USART_9_DATA_BITS
} USART_WordLength;
/*
 * Standard baud rates
 */
typedef enum {
	USART_BAUD_110 = 110,
	USART_BAUD_300 = 300,
	USART_BAUD_600 = 600,
	USART_BAUD_1200 = 1200,
	USART_BAUD_2400 = 2400,
	USART_BAUD_4800 = 4800,
	USART_BAUD_9600 = 9600,
	USART_BAUD_14400 = 14400,
	USART_BAUD_19200 = 19200,
	USART_BAUD_38400 = 38400,
	USART_BAUD_57600 = 57600,
	USART_BAUD_115200 = 115200,
	USART_BAUD_128000 = 128000,
	USART_BAUD_256000 = 256000
} USART_BaudRates;


/*
 * --------- USART structures ---------
 */

/*
 * @ USART states for interrupt
 */
typedef enum {
	USART_INIT,
	USART_READY,
	USART_RX_BUSY,
	USART_TX_BUSY,
	USART_ERROR
} USART_State;

// user-defined struct
typedef struct {
	uint8_t USART_parityControl;
	uint16_t USART_baudRate;
	uint8_t USART_stopBits;
	uint8_t USART_wordLength;
	uint8_t USART_mode;
	uint8_t USART_HardwareCtrl;
} USART_Config_t;


typedef struct {
	USART_TypeDef *pUSARTx;
	USART_Config_t USART_Config;
	USART_State USART_State;
	char *txBuffer;
	char *rxBuffer;
	uint8_t txLength;
	uint8_t rxLength;
	uint8_t rxSize;
	uint8_t dmaTransfer;
	uint8_t dmaReception;
} USART_Handle_t;


/*
 * --------- function prototypes ---------
 */
void USART_Initization(USART_Handle_t *pUSART);
USART_State USART_TransmitData(void);
USART_State USART_RxData(USART_State desiredState);

#endif /* USART__INC_STM32F4XX_HAL_USART_H_ */
