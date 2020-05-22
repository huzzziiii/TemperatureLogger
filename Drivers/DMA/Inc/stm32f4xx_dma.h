/*
 * stm32f4xx_dma.h
 *
 *  Created on: May 5, 2020
 *      Author: huzaifaasif
 */

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

typedef struct {
	DMA_TypeDef *DMAx;
	DMA_InitTypeDef *DMA_InitTypeDef;
	DMA_Stream_TypeDef *DMA_Stream;
	DMA_Config_t DMA_Config;
} DMA_Handle_t;


typedef enum {
	PERIPHERAL_TO_MEMORY,
	MEMORY_TO_PERIPHERAL,
	MEMORY_TO_MEMORY
} MEMORY_TRANSFER;

typedef enum {
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

/* MACRO definitions */
#define DMA_IT_TC                     ((uint32_t)DMA_SxCR_TCIE)
#define DMA_IT_HT                     ((uint32_t)DMA_SxCR_HTIE)
#define DMA_IT_TE                     ((uint32_t)DMA_SxCR_TEIE)
#define DMA_IT_DME                    ((uint32_t)DMA_SxCR_DMEIE)
#define DMA_IT_FEIE 				  ((uint32_t)DMA_SxFCR_FEIE)


#define _HAL_DMA_DISABLE_IT(_HANDLE_, _INTERRUPT_) ( (_HANDLE_)->CR &= ~(_INTERRUPT_) )
#define _HAL_DMA_ENABLE_IT(_HANDLE_, _INTERRUPT_) ( (_HANDLE_)->CR |= (_INTERRUPT_) )

#endif /* DMA_INC_STM32F4XX_DMA_H_ */
