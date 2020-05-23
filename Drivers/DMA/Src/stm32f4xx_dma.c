/*
 * stm32f4xx_dma.c
 *
 */
#include "stm32f4xx_dma.h"

static void DMA_SetConfig(DMA_Handle_t *dmaHandle, uint32_t srcAddress, uint32_t dstAddress, byte size);

/*
 * @USART_EnablePeripheralClk: Enables DMx clock
 */
static void DMA_EnablePeripheralClk(DMA_TypeDef *pDMAx)
{
	if (pDMAx == DMA1)
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	}
	else if (pDMAx == DMA2)
	{
		RCC->AHB1ENR|= RCC_AHB1ENR_DMA2EN;
	}
}

/*
 * Initializing user-defined members for DMA sruct
 */
void DMA_Initialization(DMA_Handle_t *dmaHandle)
{
//	pxDmaHandle = dmaHandle;
	DMA_EnablePeripheralClk(dmaHandle->DMAx);

//	/* setting the transfer direction */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.transferDirection << DMA_SxCR_DIR_Pos;

	/* Set the data length to be transferred */
	dmaHandle->DMA_Stream->NDTR = dmaHandle->DMA_Config.dataLength;

	/* FIFO mode / direct mode */
	dmaHandle->DMA_Stream->FCR |= dmaHandle->DMA_Config.directMode << DMA_SxFCR_DMDIS_Pos;

	/* set the FIFO threshold */
	if (dmaHandle->DMA_Config.directMode == DIRECT_MODE_DISABLE)
	{
		dmaHandle->DMA_Stream->FCR |= dmaHandle->DMA_Config.fifoThreshold;
	}

	/* set the circular mode if set */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.circularMode << DMA_SxCR_CIRC_Pos;


	/* Set the channel */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.channelSelection << DMA_SxCR_CHSEL_Pos;

	/* Set the memory and peripheral data size */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.memoryWordSize << DMA_SxCR_MSIZE_Pos|
								 dmaHandle->DMA_Config.peripheralWordSize << DMA_SxCR_PSIZE_Pos;

	/* memory increment mode */
	dmaHandle->DMA_Stream->CR |= DMA_SxCR_MINC;
//	dmaHandle->DMA_Stream->CR |= DMA_SxCR_PINC;

	/* Configuring the stream */
//	dmaHandle->DMA_Stream->CR |= DMA_SxCR_EN;

	dmaHandle->state = HAL_DMA_STATE_READY;

}

void DMA_SetConfig(DMA_Handle_t *dmaHandle, uint32_t srcAddress, uint32_t dstAddress, byte size)
{
	dmaHandle->DMA_Stream->NDTR |= size; 	// set buffer size

	if (dmaHandle->DMA_Config.transferDirection == PERIPHERAL_TO_MEMORY)
	{
		dmaHandle->DMA_Stream->PAR = srcAddress;
		dmaHandle->DMA_Stream->M0AR = dstAddress;
	}
	else if (dmaHandle->DMA_Config.transferDirection == MEMORY_TO_PERIPHERAL)
	{
		dmaHandle->DMA_Stream->PAR = dstAddress;
		dmaHandle->DMA_Stream->M0AR = srcAddress;
	}
}

/*
 * @brief: Starts DMA transfer with interrupts enabled
 */
HAL_StatusTypeDef DMA_Start_IT(DMA_Handle_t *dmaHandle, uint32_t srcAddress, uint32_t dstAddress, byte size)
{
	if (dmaHandle->state == HAL_DMA_STATE_READY)
	{
		dmaHandle->state = HAL_DMA_STATE_BUSY;

		// set source, destination addresses
		DMA_SetConfig(dmaHandle, srcAddress, dstAddress, size);

		// enable DMA interrupts
		_HAL_DMA_ENABLE_IT(dmaHandle, DMA_IT_TC | DMA_IT_TE);
		if (dmaHandle->HalfXferCplCallback != NULL)
		{
			_HAL_DMA_ENABLE_IT(dmaHandle, DMA_IT_HT);
		}

		// enable DMA
		_HAL_DMA_ENABLE(dmaHandle);
		return HAL_OK;
	}
	return HAL_BUSY;
}

/*
 * @brief: interrupt handler for DMA
 */
void DMA_InterruptHandler(DMA_Handle_t *dmaHandle)
{
	/* half transfer interrupt handling */
	if (dmaHandle->DMA_Stream->CR & DMA_IT_HT)
	{
		// disable interrupts
		_HAL_DMA_DISABLE_IT(dmaHandle, DMA_IT_HT);

		// invoke calback for processing
		if (dmaHandle->HalfXferCplCallback != NULL)
		{
			dmaHandle->HalfXferCplCallback(dmaHandle);
		}
	}
	else if (dmaHandle->DMA_Stream->CR & DMA_IT_TC)
	{
		_HAL_DMA_DISABLE_IT(dmaHandle, DMA_IT_TC);

		if (dmaHandle->XferCplCallback != NULL)
		{
			dmaHandle->XferCplCallback(dmaHandle);
		}
	}
	else if (dmaHandle->DMA_Stream->CR & DMA_IT_TE)
	{
		_HAL_DMA_DISABLE_IT(dmaHandle, DMA_IT_TE);

		if (dmaHandle->XferErrorCallback != NULL)
		{
			dmaHandle->XferErrorCallback(dmaHandle);
		}
	}
}





