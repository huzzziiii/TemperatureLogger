

#ifndef DMA_INC_STM32F4XX_DMA_H_
#define DMA_INC_STM32F4XX_DMA_H_

#include "stm32f401xe.h"
#include "stm32f4xx_hal.h"

typedef uint8_t byte;

typedef struct {
	byte channelSelection;
	byte stream;
	byte memoryWordSize;
	byte peripheralWordSize; 			// word, half word
	byte transferDirection; 			// m2m, p2m, ...
	byte directMode; 				    // fifo, direct
	byte circularMode;
	uint16_t dataLength;
	byte fifoThreshold;
	uint32_t sourceAddress;
	uint32_t destinationAddress;
} DMA_Config_t;


typedef struct DMA_Handle{
	DMA_TypeDef *DMAx;
	DMA_InitTypeDef *DMA_InitTypeDef;
	DMA_Stream_TypeDef *DMA_Stream;
	DMA_Config_t DMA_Config;
	HAL_DMA_StateTypeDef state;
	void *Parent;
	void (*HalfXferCplCallback) (struct DMA_Handle *dma);
	void (*XferCplCallback) (struct DMA_Handle *dma);
	void (*XferErrorCallback) (struct DMA_Handle *dma);
} DMA_Handle_t;

//typedef enum 		// todo - uncomment after removing other dma file
//{
//	HAL_DMA_STATE_RESET             = 0x00U,  /*!< DMA not yet initialized or disabled */
//	HAL_DMA_STATE_READY             = 0x01U,  /*!< DMA initialized and ready for use   */
//	HAL_DMA_STATE_BUSY              = 0x02U,  /*!< DMA process is ongoing              */
//	HAL_DMA_STATE_TIMEOUT           = 0x03U,  /*!< DMA timeout state                   */
//	HAL_DMA_STATE_ERROR             = 0x04U,  /*!< DMA error state                     */
//	HAL_DMA_STATE_ABORT             = 0x05U,  /*!< DMA Abort state                     */
//}HAL_DMA_StateTypeDef;

typedef enum
{
	PERIPHERAL_TO_MEMORY,
	MEMORY_TO_PERIPHERAL,
	MEMORY_TO_MEMORY
} MEMORY_TRANSFER;

typedef enum
{
	BYTE,
	HALF_WORD,
	WORD
} WORD_SIZE;

typedef enum {
	DIRECT_MODE_ENABLE,
	DIRECT_MODE_DISABLE
} DIRECT_MODE;

typedef enum {
	QUARTER_FULL_FIFO,
	HALF_FULL_FIFO,
	THREE_QUARTER_FULL_FIFO,
	FULL_FIFO
} FIFO_THRESHOLD;

typedef enum {
	STREAM_DISABLE,
	STREAM_ENABLE
} STREAM;

typedef enum {
	CIRCULAR_MODE_DISABLE,
	CIRCULAR_MODE_ENABLE
} CIRCULAR_MODE;


void DMA_Initialization(DMA_Handle_t *dmaHandle);
void DMA_InterruptHandler(DMA_Handle_t *dmaHandle);

/* MACRO definitions */

// interrupts
#define DMA_IT_TC                     ((uint32_t)DMA_SxCR_TCIE)
#define DMA_IT_HT                     ((uint32_t)DMA_SxCR_HTIE)
#define DMA_IT_TE                     ((uint32_t)DMA_SxCR_TEIE)
#define DMA_IT_DME                    ((uint32_t)DMA_SxCR_DMEIE)
#define DMA_IT_FEIE 				  ((uint32_t)DMA_SxFCR_FEIE)

// interrupt enable/disable
#define _HAL_DMA_DISABLE_IT(_HANDLE_, _INTERRUPT_) ( (_HANDLE_)->DMA_Stream->CR &= ~(_INTERRUPT_) )
#define _HAL_DMA_ENABLE_IT(_HANDLE_, _INTERRUPT_) ( (_HANDLE_)->DMA_Stream->CR |= (_INTERRUPT_) )
#define _HAL_DMA_DISABLE(_HANDLE_) (_HANDLE_)->DMA_Stream->CR &= ~(DMA_SxCR_EN)
#define _HAL_DMA_ENABLE(_HANDLE_) (_HANDLE_)->DMA_Stream->CR |= (DMA_SxCR_EN)

// DMA streams
#define DMA_UART_RX_STREAM 		(DMA_Stream_TypeDef*) DMA1_Stream5
#define DMA_UART_TX_STREAM 		(DMA_Stream_TypeDef*) DMA1_Stream6

#endif /* DMA_INC_STM32F4XX_DMA_H_ */
