/*
 * stm32f4fxx_hal_usart.c
 */
#include "../Inc/stm32f4xx_hal_usart.h"

/*
 * Static variables
 */
static USART_Handle_t *ptrUSARTx;
bool endLine = false;


uint32_t GetPCLK(USART_TypeDef *pUSARTx)
{
	uint32_t pclk;
	if (pUSARTx == USART1 || USART6)
	{
		pclk = HAL_RCC_GetPCLK2Freq();
	}
	else if (pUSARTx == USART2)
	{
		pclk = HAL_RCC_GetPCLK1Freq();
	}
	return pclk;
}

/*
 * USART initialization and deinitialization
 */
static void USART_Deinit(USART_Handle_t *pUSART); // todo maybe remove

/*
 * Function declarations
 */
static void USART_EnablePeripheralClk(USART_TypeDef *pUSARTx);
static void USART_EnableCtrlBits(void);
static void USART_ClearCtrlBits(void);
static void USART_CloseTransmission(void);
static void USART_ClearCtrlBits(void);
static void USART_TXE(void);

/*
 * Interrupt handlers --- TODO ***
 */

/*
 * @USART_EnablePeripheralClk: Enables USARTx clock
 */
static void USART_EnablePeripheralClk(USART_TypeDef *pUSARTx)
{
	if (pUSARTx == USART1)
	{
		RCC->APB1ENR |= RCC_APB2ENR_USART1EN;
	}
	else if (pUSARTx == USART2)
	{
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	else if (pUSARTx == USART6)
	{
		RCC->APB1ENR |= RCC_APB2ENR_USART6EN;
	}
}


/*
 * @USART_TransmitData: Enables USART control bits
 */
USART_State USART_TransmitData(void)
{
	if (ptrUSARTx->USART_State != USART_TX_BUSY)
	{
		ptrUSARTx->USART_State = USART_TX_BUSY;
		ptrUSARTx->pUSARTx->CR1 |= (USART_CR1_TXEIE | USART_CR1_TCIE); // set Transmit Data Empty & Transmission Complete control bits
	}
	return ptrUSARTx->USART_State;
}

USART_State USART_RxData(USART_State desiredState)
{
	if (ptrUSARTx->USART_State != desiredState && ptrUSARTx->USART_State != USART_READY)
	{
		ptrUSARTx->USART_State = desiredState;
		ptrUSARTx->pUSARTx->CR1 |= (USART_CR1_RXNEIE | USART_CR1_RE);
	}
	return ptrUSARTx->USART_State;
}

/*
 * @USART2_IRQHandler: IRQ handler for servicing USART2 interrupts
 */
void USART2_IRQHandler(void)
{
	endLine = false;

	// Transmit buffer empty
	if (ptrUSARTx->pUSARTx->SR & USART_SR_TXE)
	{
		if (ptrUSARTx->USART_State == USART_TX_BUSY)
		{
			// TXE
			if (ptrUSARTx->txLength)
			{
				ptrUSARTx->pUSARTx->DR = *ptrUSARTx->txBuffer++; // send to serial
				ptrUSARTx->txLength--;
			}
		}
	}

	// RXNE
	if (ptrUSARTx->pUSARTx->SR & USART_SR_RXNE && ptrUSARTx->rxLength)
	{
		*ptrUSARTx->rxBuffer = ptrUSARTx->pUSARTx->DR;
		if (*ptrUSARTx->rxBuffer == '\r')
		{
			endLine = true;
		}
		ptrUSARTx->rxBuffer++;
		ptrUSARTx->rxLength--;
	}

	if (ptrUSARTx->pUSARTx->SR & USART_SR_TC) // transmission complete
	{
		if (ptrUSARTx->USART_State == USART_TX_BUSY)
		{
			if (!ptrUSARTx->txLength)		 // if no more bytes to send
			{
				USART_CloseTransmission();
			}
		}
		else if (ptrUSARTx->USART_State == USART_RX_BUSY)
		{
			if (!ptrUSARTx->rxLength || endLine)		// if no more bytes to receive OR user keyed in enter
			{
				USART_CloseTransmission();
			}
		}
	}
}

/*
 * @USART_Init: Populates USART struct
 */
void USART_Initization(USART_Handle_t *pUSART)
{
	ptrUSARTx = pUSART;

	// initializing peripheral clock
	USART_EnablePeripheralClk(pUSART->pUSARTx);

	// enable UE bit
	pUSART->pUSARTx->CR1 |= USART_CR1_UE;

	// word length
	pUSART->pUSARTx->CR1 |= pUSART->USART_Config.USART_wordLength << USART_CR1_M_Pos;

	// parity control
	if (pUSART->USART_Config.USART_parityControl)
	{
		pUSART->pUSARTx->CR1 |= USART_CR1_PS;
	}

	// USART mode
	if (pUSART->USART_Config.USART_mode == USART_MODE_TX)
	{
		pUSART->pUSARTx->CR1 |= USART_CR1_TE;
	}
	else if (pUSART->USART_Config.USART_mode == USART_MODE_RX)
	{
		pUSART->pUSARTx->CR1 |= USART_CR1_RE;
	}
	else if (pUSART->USART_Config.USART_mode == USART_MODE_TXRX)
	{
		pUSART->pUSARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
	}

	if (pUSART->dmaTransfer == DMA_TX_ENABLE)
	{
		pUSART->pUSARTx->CR3 |= USART_CR3_DMAT;
	}

	if (pUSART->dmaReception == DMA_RX_ENABLE)
	{
		pUSART->pUSARTx->CR3 |= USART_CR3_DMAR;
	}
	// STOP bit
	pUSART->pUSARTx->CR2 |= pUSART->USART_Config.USART_stopBits << USART_CR2_STOP_Pos;

	// Hardware control
						// todo ....!

	// configuring baud rate
	uint8_t oversamplingValue = 16, divisor = 100;
	uint32_t tempUsartDiv, tempFraction;

	uint8_t over8 = pUSART->pUSARTx->CR1 & USART_CR1_OVER8; 	// by default, over8=0 = 16x oversampling
	tempUsartDiv = (GetPCLK(pUSART->pUSARTx) * divisor) / (8 * (2 - over8) * pUSART->USART_Config.USART_baudRate);

	uint8_t divMantissa = tempUsartDiv / divisor;
	tempFraction = (tempUsartDiv - divMantissa * divisor) * oversamplingValue;

	uint8_t divFraction = (tempFraction + 50)/divisor;

	// writing to BRR register
	pUSART->pUSARTx->BRR |= (divMantissa << USART_BRR_DIV_Mantissa_Pos | divFraction);

}


static void USART_CloseTransmission()
{

	if (ptrUSARTx->USART_State == USART_RX_BUSY)
	{
		ptrUSARTx->pUSARTx->CR1 &= ~USART_CR1_RXNEIE;
	}
	else if (ptrUSARTx->USART_State == USART_TX_BUSY)
	{
		ptrUSARTx->pUSARTx->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);
	}
	ptrUSARTx->USART_State = USART_READY;
	ptrUSARTx->rxLength = ptrUSARTx->rxSize;	// restoring the original RX size
}

static void USART_EnableCtrlBits(void)
{
	// set Transmit Data Empty & Transmission Complete control bits
	ptrUSARTx->pUSARTx->CR1 |= (USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE);
}

static void USART_ClearCtrlBits(void)
{
	ptrUSARTx->pUSARTx->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE);
}
