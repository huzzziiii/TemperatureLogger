/*
 * stm32f4xx_dma.c
 *
 */
#include "stm32f4xx_dma.h"

static DMA_Handle_t *pxDmaHandle;
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
	pxDmaHandle = dmaHandle;
	DMA_EnablePeripheralClk(dmaHandle->DMAx);

	/* setting the transfer direction */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.transferDirection << DMA_SxCR_DIR_Pos;

	if (dmaHandle->DMA_Config.transferDirection == PERIPHERAL_TO_MEMORY)
	{
		dmaHandle->DMA_Stream->PAR = (uint32_t) dmaHandle->DMA_Config.sourceAddress;
		dmaHandle->DMA_Stream->M0AR = (uint32_t) dmaHandle->DMA_Config.destinationAddress;
	}
	else if (dmaHandle->DMA_Config.transferDirection == MEMORY_TO_PERIPHERAL)
	{
		dmaHandle->DMA_Stream->PAR = (uint32_t) dmaHandle->DMA_Config.destinationAddress;
		dmaHandle->DMA_Stream->M0AR = (uint32_t) dmaHandle->DMA_Config.sourceAddress;
	}

	/* Set the data length to be transferred */
	dmaHandle->DMA_Stream->NDTR = dmaHandle->DMA_Config.dataLength;

	/* FIFO mode / direct mode */
	dmaHandle->DMA_Stream->FCR |= dmaHandle->DMA_Config.directMode << DMA_SxFCR_DMDIS_Pos;

	/* set the FIFO threshold */
	if (dmaHandle->DMA_Config.directMode == DIRECT_MODE_DISABLE)
	{
		dmaHandle->DMA_Stream->FCR |= dmaHandle->DMA_Config.fifoThreshold;
	}

	/* set the circular mode if circularMode is set */
	if (dmaHandle->DMA_Config.circularMode == CIRCULAR_MODE_ENABLE)
	{
		dmaHandle->DMA_Stream->CR |= DMA_SxCR_CIRC;
	}

	/* Set the channel */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.channelSelection << DMA_SxCR_CHSEL_Pos;

	/* Set the memory and peripheral data size */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.memoryWordSize << DMA_SxCR_MSIZE_Pos|
								 dmaHandle->DMA_Config.peripheralWordSize << DMA_SxCR_PSIZE_Pos;

	/* Set word size */
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.peripheralWordSize << DMA_SxCR_PSIZE_Pos;
	dmaHandle->DMA_Stream->CR |= dmaHandle->DMA_Config.memoryWordSize << DMA_SxCR_MSIZE_Pos;

	/* memory increment mode */
	dmaHandle->DMA_Stream->CR |= DMA_SxCR_MINC;
//	dmaHandle->DMA_Stream->CR |= DMA_SxCR_PINC;

	/* Configuring the stream */
	dmaHandle->DMA_Stream->CR |= DMA_SxCR_EN;

	DMA_EnableInterrupts(dmaHandle);
}

void DMA_EnableInterrupts(DMA_Handle_t *dmaHandle)
{
	// half transfer
	_HAL_DMA_ENABLE_IT(dmaHandle->DMA_Stream,DMA_IT_TC  |
											 DMA_IT_HT|
											 DMA_IT_TE|
											 DMA_IT_DME|
											 DMA_IT_FEIE);

//	dmaHandle->DMA_Stream->CR |= DMA_IT_TC |
//								 DMA_IT_TCEI|
//								 DMA_IT_TEIE|
//								 DMA_IT_FEIE|
//								 DMA_IT_DMEIE
}

void DMA1_Stream6_IRQHandler(void)
{
	// clear DMAT

	_HAL_DMA_DISABLE_IT(pxDmaHandle->DMA_Stream, DMA_IT_TC);
//	_HAL_DMA_DISABLE_IT(pxDmaHandle->DMA_Stream-?)


}

