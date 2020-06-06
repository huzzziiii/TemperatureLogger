/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

USART_Handle_t USART2_handle;
I2C_Handle_t I2C1_handle;
DMA_Handle_t dma_usart_rx, dma_usart_tx;

/* defining buffer for USART */
char usart_rxBuffer[9] = {0};
char *tempPtr = usart_rxBuffer;
uint8_t usart_rxLength = sizeof(usart_rxBuffer)/sizeof(usart_rxBuffer[0]);

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/*
 * Initializing user-defined variables for I2C struct
 */
void I2C_Initilization(I2C_Config_t *I2C_Config, I2C_TypeDef *i2cPeripheral)
{
	I2C1_handle.pI2Cx = i2cPeripheral;
	I2C1_handle.I2C_Config = *I2C_Config;
	I2C_Init(&I2C1_handle);
}

void USART_Init (void)
{
	USART2_handle.pUSARTx = USART2;
	USART2_handle.USART_Config.USART_baudRate = USART_BAUD_9600;
	USART2_handle.USART_Config.USART_mode = USART_MODE_TXRX;
	USART2_handle.USART_Config.USART_parityControl = USART_PARITY_DISABLED;
	USART2_handle.USART_Config.USART_stopBits = USART_STOP;
	USART2_handle.USART_Config.USART_wordLength = USART_8_DATA_BITS;
	USART2_handle.rxBuffer = usart_rxBuffer;
	USART2_handle.rxLength = usart_rxLength -1;
	USART2_handle.rxSize = usart_rxLength - 1;
	USART2_handle.bitMask = usart_rxLength - 2;
	USART2_handle.txIdx = 0;
	USART2_handle.rxIdx = 0;
	USART2_handle.dmaTransfer = DMA_TX_DISABLE;
	USART2_handle.dmaReception = DMA_RX_DISABLE;
	USART2_handle.session = SET;
	USART_Initization(&USART2_handle);
}

void DMA_Init(DMA_Handle_t *dmaHandle, DMA_Stream_TypeDef *stream, byte transferDirection)
{
	dmaHandle->DMAx = DMA1;
//	DMA_handle.DMA_Stream = DMA1_Stream5;
	dmaHandle->DMA_Stream = stream;
	dmaHandle->DMA_Config.channelSelection = 4;
	dmaHandle->DMA_Config.transferDirection = transferDirection;
//	dmaHandle->DMA_Config.dataLength = bufferSize; // todo - double check (bytes usage)
	dmaHandle->DMA_Config.memoryWordSize = BYTE;
	dmaHandle->DMA_Config.peripheralWordSize = BYTE;
	dmaHandle->DMA_Config.directMode = DIRECT_MODE_DISABLE;
	dmaHandle->DMA_Config.circularMode = CIRCULAR_MODE_DISABLE;
	dmaHandle->DMA_Config.fifoThreshold = FULL_FIFO;
	dmaHandle->DMA_Config.stream = STREAM_ENABLE;
	DMA_Initialization(dmaHandle);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();

	/* Initialize I2C struct */
	I2C_Config_t i2c_config = {
		I2C_AckControl: I2C_ACK_ENABLE,
		I2C_SCLSpeed: I2C_SCL_SPEED_SM,
		I2C_DeviceAddress: MCP9808_ADDR,
		I2C_FMDutyCycle: I2C_FM_DUTY_2
	};
    I2C_Initilization(&i2c_config, I2C1);

    /* Initialize USART struct */
    USART_Init();


    /* Initialize DMA struct */
//    DMA_Init(&dma_usart_tx, DMA_UART_TX_STREAM, MEMORY_TO_PERIPHERAL);
//    DMA_Init(&dma_usart_rx, DMA_UART_RX_STREAM, PERIPHERAL_TO_MEMORY);
//
//    __HAL_LINKDMA(&usart, dmaRx, dma_usart_rx);
//    __HAL_LINKDMA(&usart, dmaTx, dma_usart_tx);

//    DMA_Start_IT(&dma_usart_tx, (uint32_t) tx_buff, &usart.pUSARTx->DR);
//    DMA_Start_IT(&dma_usart_rx, &usart.pUSARTx->DR, (uint32_t) rx_buff);

//    StartSerialSession (&USART2_handle, usart_rxLength, &I2C1_handle);


    USART_EnableRxInterrupts();

    while(1) {
    	SerialRead(&USART2_handle, &I2C1_handle);
    	HAL_Delay(2000);
    }
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef gpio;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();			// Push button (PC13)
  __HAL_RCC_GPIOA_CLK_ENABLE();			// LED (PA5)
  __HAL_RCC_GPIOB_CLK_ENABLE();			// I2C1

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

	/* USART config */
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio.Pull = GPIO_PULLUP;
	gpio.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio);

	HAL_NVIC_EnableIRQ(USART2_IRQn);

	/* LED/Push button config */

	// Configure GPIO pin : PC13 // PUSH BUTTON
	gpio.Pin = GPIO_PIN_13;
	gpio.Mode = GPIO_MODE_IT_RISING;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &gpio);

	// Configure GPIO pin : PA5  // LED
	gpio.Pin = GPIO_PIN_5;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio);

	// Configure I2C
	gpio.Mode = GPIO_MODE_AF_OD;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Alternate = GPIO_AF4_I2C1;


	// scl
	gpio.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &gpio);

	// sdata
	gpio.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &gpio);

	/* EXTI GPIO init */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* EXTI I2C Interrupts */
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

//	/* EXTI DMA interrupt */ -- todo remove
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
