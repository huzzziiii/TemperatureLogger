/*
 * nrf24l01.c
 */
#include "nrf24l01.h"


static uint8_t nRF24_RX_ADDR[nRF24_MAX_DATA_PIPES] = {
		nRF24_RX_ADDR_P0_REG,
		nRF24_RX_ADDR_P1_REG,
		nRF24_RX_ADDR_P2_REG,
		nRF24_RX_ADDR_P3_REG,
		nRF24_RX_ADDR_P4_REG,
		nRF24_RX_ADDR_P5_REG
};

static uint8_t nRF24_RX_PAYLOAD[nRF24_MAX_DATA_PIPES] = {
		nRF24_RX_PW_P0_REG,
		nRF24_RX_PW_P1_REG,
		nRF24_RX_PW_P2_REG,
		nRF24_RX_PW_P3_REG,
		nRF24_RX_PW_P4_REG,
		nRF24_RX_PW_P5_REG
};

unsigned long long concatData(uint8_t *arr, uint8_t size)
{
	long long unsigned value = 0;
	for (uint8_t i = 0; i < size; i++)
	{
		value <<= 8;
		value |= arr[i];
	}
	return value;
}

void nRF24_GPIO_Init(GPIO_InitTypeDef *gpioPort)
{
	// SCK, MISO, MOSI
	gpioPort->Mode = GPIO_MODE_AF_PP;
	gpioPort->Pin = SPI_SCLK_PIN | SPI_MOSI_PIN | SPI_MISO_PIN;
	gpioPort->Alternate = GPIO_AF5_SPI2;
	gpioPort->Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(SPI_SCLK_PORT, gpioPort);

	// CE and SCN
	gpioPort->Pin = rNRF24_CE_PIN | rNRF24_CSN_PIN;
	gpioPort->Mode = GPIO_MODE_OUTPUT_PP;
	gpioPort->Pull = GPIO_NOPULL;
	gpioPort->Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(nRF24_GPIO_PORT, gpioPort);

	RESET_CE();
	SET_CSN();

	// IRQ
	gpioPort->Pin = nRF24_IRQ_PIN;
	gpioPort->Mode = GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init(nRF24_GPIO_PORT, gpioPort);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

NRF_STATUS nRF24_WriteRegister(nrfl2401 *nrf, uint8_t reg, uint8_t *data, const uint8_t txSize, char *text)
{
	uint8_t txBuffer[10] = {0};
	uint8_t rxBuffer[10] = {0};
	uint8_t spiTxSize = txSize + 1;

	if (reg <= nRF24_CMD_W_REG)					// registers that need to be written to/read from
	{
		txBuffer[0] = nRF24_CMD_W_REG | (reg & nRF24_R_W_MASK);
	}
	else
	{
		txBuffer[0] = reg;
	}

	// populate the buffer with the data that needs to be sent
	for (uint8_t i = 0; i < txSize; i++)
	{
		txBuffer[i+1] = data[i];
	}

	nRF24_SendCommand(nrf, txBuffer, rxBuffer, spiTxSize);

	OutputToSerial(txBuffer, rxBuffer, spiTxSize, text);

	return NRF_OK;
}

//void nRF24_ClearIRQ(nrfl2401 *nrf)
//{
//	uint8_t data;
//	uint8_t readBuffer[2] = {0};
//	nRF24_ReadRegister(nrf, nRF24_STATUS_REG, (uint8_t) 1, readBuffer, "Reading Status Register");
//
//	// clear the IRQ bits by setting to 1
//	data = readBuffer[1] | nRF24_IRQ_CLR_MASK;
//
//	nRF24_WriteRegister(nrf, nRF24_STATUS_REG, &data, (uint8_t) 1, "Clearing IRQ bits");
//
//	nRF24_ReadRegister(nrf, nRF24_STATUS_REG, (uint8_t) 1, readBuffer, "Reading Status Register");
//}

void nRF24_SendCommand(nrfl2401 *nrf, uint8_t *txBuffer, uint8_t *rxBuffer, uint8_t size)
{
	RESET_CSN();

//	HAL_SPI_TransmitReceive_IT(nrf->config.spiHandle, txBuffer, rxBuffer, size);
	HAL_StatusTypeDef transactionStatus = HAL_SPI_TransmitReceive(nrf->config.spiHandle, txBuffer, rxBuffer, size, 1000);

	SET_CSN();

	// maybe return ... - todo!
//	return transactionStatus;
}

void nRF24_ReadRegister(nrfl2401 *nrf, uint8_t reg, const uint8_t rxSize, uint8_t *rxBuffer, char *text)
{
//	txBuffer[0] = reg < nRF24_CMD_W_REG ? (nRF24_CMD_R_REG | (reg & nRF24_R_W_MASK)) : reg;
	uint8_t txBuffer[10] = {0};
	uint8_t spiRxSize = rxSize;

	if (reg <= nRF24_CMD_W_REG)
	{
		txBuffer[0] = nRF24_CMD_R_REG | (reg & nRF24_R_W_MASK);
		spiRxSize++;
	}
	else
	{
		txBuffer[0] = reg;
	}

	nRF24_SendCommand(nrf, txBuffer, rxBuffer, spiRxSize);

	OutputToSerial(txBuffer, rxBuffer, spiRxSize, text);
}

void OutputToSerial(uint8_t *writeBuffer, uint8_t *readBuffer, uint8_t size, char *text)
{
	char strData[100] = {0}, rxStrData[100] = {0};
	long long unsigned txStr = concatData(writeBuffer, size);
	long long unsigned rxStr = concatData(readBuffer, size);
	LONG_TO_STR(strData, txStr);
	LONG_TO_STR(rxStrData, rxStr);

	char outputMsg[60] = {0};
	strcpy(outputMsg, text);
	strcat(outputMsg, ":          0x%s ----------- 0x%s\n");

//	char temp[30] = {0}; // todo - remove !!
//	strcpy(temp, outputMsg);

	printf (outputMsg, strData, rxStrData);
}

void nRF24_PrintDetails(nrfl2401 *nrf)
{
	printf ("\n ------------- PRINTING details --------\n");
	uint8_t readBuffer[10] = {0};
	nRF24_ReadRegister(nrf, nRF24_CONFIG_REG, (uint8_t) 1, readBuffer, "Config");

	nRF24_ReadRegister(nrf, nRF24_RX_ADDR_P0_REG, nrf->config.addressWidth, readBuffer, "RX_ADDR_P0");

	nRF24_ReadRegister(nrf, nRF24_EN_AA_REG, (uint8_t) 1, readBuffer, "EN_AA");

	nRF24_ReadRegister(nrf, nRF24_TX_ADDR_REG, nrf->config.addressWidth, readBuffer, "TX_ADDR_REG");

	nRF24_ReadRegister(nrf, nRF24_RF_SETUP_REG, (uint8_t) 1, readBuffer, "RF_Setup");

	nRF24_ReadRegister(nrf, nRF24_RF_CH_REG, (uint8_t) 1, readBuffer, "RF_CH Reg");

	nRF24_ReadRegister(nrf, nRF24_SETUP_AW_REG, (uint8_t) 1, readBuffer, "ADDRESS WIDTH");

	nRF24_ReadRegister(nrf, nRF24_OBSERVE_TX_REG, (uint8_t) 1, readBuffer, "OBSERVE_TX");

	nRF24_ReadRegister(nrf, nRF24_FEATURE_REG, (uint8_t) 1, readBuffer, "FEATURE_REG");

	nRF24_ReadRegister(nrf, nRF24_DYNPD_REG, (uint8_t) 1, readBuffer, "DPL_Px");
}

NRF_STATUS ReadStatusRegister(nrfl2401 *nrf, FlagStatus verifyInitState)
{
	uint8_t readBuffer[2] = {0};
	nRF24_ReadRegister(nrf, nRF24_STATUS_REG, (uint8_t) 1, readBuffer, "Read Status Reg");

	if (readBuffer[0] != nRF24_DEFAULT_STATUS_REG)
	{
		return NRF_ERROR;
	}
	return NRF_OK;
}

void nRF24_WriteStatusRegister(nrfl2401 *nrf, uint8_t data, uint8_t size)
{
	nRF24_WriteRegister(nrf, nRF24_STATUS_REG, &data, size, "Status register");
}

void nRF24_SetTxAddress(nrfl2401 *nrf, const uint8_t size)
{
	nRF24_WriteRegister(nrf, nRF24_TX_ADDR_REG, nrf->config.txAddress, size, "TX address");
}

void nRF24_SetRxAddress(nrfl2401 *nrf, uint8_t size)
{
	char printStr[10] = "RX_ADDR_P0";
	for (uint8_t i = 0; i < nrf->config.txDevicesUsed; i++)
	{
//		nRF24_WriteRegister(nrf, nRF24_RX_ADDR[i], nrf->config.txAddressesList[i], size);
		nRF24_WriteRegister(nrf, nRF24_RX_ADDR[i], nrf->config.txAddressesList[i], size, printStr);
	}
}

void nRF24_EnableRxDataPipes(nrfl2401 *nrf, const uint8_t size)
{
	nRF24_WriteRegister(nrf, nRF24_EN_RXADDR_REG, nrf->config.enableRxDataPipes, size, "RX_DATA_PIPE0");
}

void nRF24_SetAddressFieldWidth(nrfl2401 *nrf, const uint8_t size)
{
	nRF24_WriteRegister(nrf, nRF24_SETUP_AW_REG, &nrf->config.addressFieldWidth, size, "Address field width");
}

void nRF24_SetupRF(nrfl2401 *nrf, const uint8_t size)
{
	nRF24_WriteRegister(nrf, nRF24_RF_SETUP_REG, nrf->config.rfSetup, size, "Setup RF");
}

NRF_STATUS nRF24_SetDataRate(nrfl2401 *nrf)
{
	uint8_t readBuffer[10] = {0};
	uint8_t setValue;

	// reading RF setup register
	nRF24_ReadRegister(nrf, nRF24_RF_SETUP_REG, (uint8_t) 1, readBuffer, "Read RF Setup");

	setValue = readBuffer[1] | nrf->config.dataRate;
	// setting data rate
	nRF24_WriteRegister(nrf, nRF24_RF_SETUP_REG, &setValue, (uint8_t) 1, "Set data rate");

	// verifying set data
	nRF24_ReadRegister(nrf, nRF24_RF_SETUP_REG, (uint8_t) 1, readBuffer, "Verifying RF Setup");
	if (readBuffer[1] != setValue)
	{
		return NRF_ERROR;
	}
	return NRF_OK;
}

void ContinueReadingStatusReg(nrfl2401 *nrf)
{
	uint8_t rxBuffer[2] = {0};
	while (1)
	{
		nRF24_ReadRegister(nrf, nRF24_CONFIG_REG, (uint8_t) 1, rxBuffer, "Polling config register");
		HAL_Delay(2000);
	}
}

void nRF24_DisableIRQ(nrfl2401 *nrf, NRF_DISABLE_STATUS_REG bitPos)
{
	uint8_t registerRead[10] = {0};
//	uint8_t write = nRF24_DISABLE_MAX_RT | nRF24_DISABLE_TX_DS | nRF24_DISABLE_RX_DR;
	uint8_t write = bitPos;
	nRF24_ReadRegister(nrf, nRF24_STATUS_REG, (uint8_t) 1, registerRead, "Reading Status Reg");

	nRF24_WriteRegister(nrf, nRF24_STATUS_REG, (uint8_t[]) {registerRead[1] | write}, (uint8_t) 1, "Disabling IRQs");
}

void nRF24_FlushTX(nrfl2401 *nrf)
{
	nRF24_WriteRegister(nrf, nRF24_CMD_FLUSH_TX, (uint8_t[]) {nRF24_CMD_NOP}, (uint8_t) 2, "Flush TX FIFO");
}

void nRF24_FlushRX(nrfl2401 *nrf)
{
	nRF24_WriteRegister(nrf, nRF24_CMD_FLUSH_RX, (uint8_t[]) {nRF24_CMD_NOP}, (uint8_t) 2, "Flush RX FIFO");
}

void nRF24_Reset(nrfl2401 *nrf)
{
	printf (" ************* RESETTING *************\n");
//	nRF24_SetConfigureRegister(nrf, nRF24_PWR_UP, PWR_DOWN);
//
//	nRF24_WriteStatusRegister(nrf, nRF24_DEFAULT_STATUS_REG, (uint8_t) 2);
//
//	nRF24_SetConfigureRegister(nrf, nRF24_PWR_UP, PWR_UP);

//	nRF24_WriteRegister(nrf, nRF24_CONFIG_REG, 0x08, (uint8_t) 2);

//	uint8_t rxBuffer[2] = {0};
	uint8_t registerRead[2] = {0};


//	nRF24_ReadRegister(nrf, nRF24_CONFIG_REG, (uint8_t) 1, rxBuffer, "Reading Default Config Register");	// 0x0011 1111
	nRF24_ReadRegister(nrf, nRF24_CONFIG_REG, (uint8_t) 1, registerRead, "Reading Default Config");	// 0x0011 1111

	if (registerRead[1] != DEFAULT_CONFIG_REG)
	{
		nRF24_WriteRegister(nrf, nRF24_CONFIG_REG, (uint8_t[]) {DEFAULT_CONFIG_REG}, (uint8_t) 1, "Setting CONFIG");
	}

	// read status register
	nRF24_ReadRegister(nrf, nRF24_STATUS_REG, (uint8_t) 1, registerRead, "Reading Default Status");	// 0x0011 1111
	if (registerRead[1] != DEFAULT_STATUS_REG)
	{
		if (registerRead[1] & nRF24_DISABLE_MAX_RT)
		{
			nRF24_WriteRegister(nrf, nRF24_STATUS_REG, (uint8_t[]) {registerRead[1] | nRF24_DISABLE_MAX_RT}, (uint8_t) 1, "Disabling MAX_RT");
		}

		if (registerRead[1] & nRF24_DISABLE_TX_FULL)
		{
			nRF24_WriteRegister(nrf, nRF24_STATUS_REG, (uint8_t[]) {registerRead[1] | nRF24_DISABLE_TX_FULL}, (uint8_t) 1, "Disabing TX_FULL");
		}

		if (registerRead[1] & nRF24_DISABLE_TX_DS)
		{
			nRF24_WriteRegister(nrf, nRF24_STATUS_REG, (uint8_t[]) {registerRead[1] | nRF24_DISABLE_TX_DS}, (uint8_t) 1, "Disabing TX_DR");
		}

		// read en_aa register
		nRF24_ReadRegister(nrf, nRF24_EN_AA_REG, (uint8_t) 1, registerRead, "Reading EN_AA");
		if (registerRead[1] != DEFAULT_EN_AA_REG)
		{
			nRF24_WriteRegister(nrf, DEFAULT_EN_AA_REG, (uint8_t[]) {registerRead[1] | DEFAULT_EN_AA_REG}, (uint8_t) 1, "Resetting EN_AA");
		}
	}

	// read RF setup register
	nRF24_ReadRegister(nrf, nRF24_RF_SETUP_REG, (uint8_t) 1, registerRead, "Reading Default RF setup");
	if (registerRead[1] != DEFAULT_RF_SETUP_REG)
	{
		nRF24_WriteRegister(nrf, nRF24_RF_SETUP_REG, (uint8_t[]) {DEFAULT_RF_SETUP_REG}, (uint8_t) 1, "Setting default RF_SETUP");
	}

	// read RF_CH register
	nRF24_ReadRegister(nrf, nRF24_RF_CH_REG, (uint8_t) 1, registerRead, "Reading Default RF_CH");
	if (registerRead[1] != DEFAULT_RF_CH_REG)
	{
		nRF24_WriteRegister(nrf, nRF24_RF_SETUP_REG, (uint8_t[]) {DEFAULT_RF_CH_REG}, (uint8_t) 1, "Setting default RF_CH");
	}

	// read Feature register
	nRF24_ReadRegister(nrf, nRF24_FEATURE_REG, (uint8_t) 1, registerRead, "Reading FEATURE reg");
	if (registerRead[1] != DEFAULT_FEATURE_REG)
	{
		nRF24_WriteRegister(nrf, nRF24_FEATURE_REG, (uint8_t[]) {DEFAULT_FEATURE_REG}, (uint8_t) 1, "Set default FTR_REG");
	}

	// read DPL PX register
	nRF24_ReadRegister(nrf, nRF24_DYNPD_REG, (uint8_t) 1, registerRead, "Reading DPL_Px");
	if (registerRead[1] != DEFAULT_DPL_PX)
	{
		nRF24_WriteRegister(nrf, nRF24_DYNPD_REG, (uint8_t[]) {DEFAULT_DPL_PX}, (uint8_t) 1, "Reset DPL_Px");
	}


	printf ("\n");
}

void nRF24_EnableDynamicPayload(nrfl2401 *nrf)
{
	uint8_t readBuffer[10] = {0}, setValue;
	if (nrf->config.enableDynamicPayload == SET)
	{
		nRF24_ReadRegister(nrf, nRF24_FEATURE_REG, (uint8_t) 1, readBuffer, "Reading FEATURE_REG");

		setValue = readBuffer[1] | nRF24_ENABLE_DYNAMIC_PAYLOAD;
		nRF24_WriteRegister(nrf, nRF24_FEATURE_REG, &setValue, (uint8_t) 1, "Enabling dynamic payload");

		nRF24_WriteRegister(nrf, nRF24_DYNPD_REG, nrf->config.enableRxDataPipes, (uint8_t) 1, "EN_DPL data pipe");
	}
}

void nRF24_PowerUp(nrfl2401 *nrf)
{
	nRF24_WriteRegister(nrf, nRF24_CONFIG_REG, (uint8_t[]) {nRF24_SET_PWR_UP}, (uint8_t) 1, "Enabling PWR_UP");

	nRF24_ReadRegister(nrf, nRF24_CONFIG_REG, (uint8_t) 1, (uint8_t[10]) {}, "Verifying PWR_UP");
}



NRF_STATUS nRF24_Initialization(nrfl2401 *nrf)
{
	nRF24_Reset(nrf);

//	nRF24_SetConfigureRegister(nrf, nRF24_PWR_UP, PWR_DOWN);

//	// set PRIM_RX low
//	nRF24_SetConfigureRegister(nrf, nRF24_PRIM_RX, TX_CTRL);

	// switch to TX mode
	nRF24_SetConfigureRegister(nrf, nRF24_PRIM_RX, TX_CTRL);

	// set TX address
	nRF24_SetTxAddress(nrf, nrf->config.addressWidth);		// used only for PTX device

	// enable RX data pipes
	nRF24_EnableRxDataPipes(nrf, (uint8_t) 1); 				// enabling data pipe 0 to receive the ACK

	// Disable auto ACK for a respective data pipe if it needs to be
	nRF24_DisableAutoAck(nrf);

	// set address field width
	nRF24_SetAddressFieldWidth(nrf, (uint8_t) 1);

	// Data rate
	nRF24_SetDataRate(nrf);

	// set channel
	nRF24_SetChannel(nrf);

	// RF setup
//	nRF24_SetupRF(nrf, (uint8_t) 1);

	nRF24_SetRxAddress(nrf, nrf->config.addressWidth);

	if (nrf->config.enableDynamicPayload == SET)
	{
		nRF24_EnableDynamicPayload(nrf);				// Enable dynamic payload
	}
	else
	{
		nRF24_SetPayloadWidth(nrf, (uint8_t) 1);		// todo - no need in PTX
	}

	nRF24_EnablePayloadWithAck(nrf);

	ReadStatusRegister(nrf, RESET);

	nRF24_FlushTX(nrf);

	nRF24_PowerUp(nrf);
}

NRF_STATUS nRF24_VerifyRegister(uint8_t actual, uint8_t expected)
{
	if (actual != expected)
	{
		return NRF_ERROR;
	}
	return NRF_OK;
}

void nRF24_SetChannel(nrfl2401 *nrf)
{
	uint8_t readBuffer[10] = {0};

	nRF24_WriteRegister(nrf, nRF24_RF_CH_REG, &nrf->config.rfChannel, (uint8_t) 1, "Setting channel");

	nRF24_ReadRegister(nrf, nRF24_RF_CH_REG, (uint8_t) 1, readBuffer, "Reading RF_CH");

//	if (nRF24_VerifyRegister(actual, expected))
}

void nRF24_SetPayloadWidth(nrfl2401 *nrf, uint8_t size)
{
	for (uint8_t i = 0; i < nrf->config.txDevicesUsed; i++)
	{
		nRF24_WriteRegister(nrf, nRF24_RX_PAYLOAD[i], &nrf->config.rxPayloadWidths[i], size, "Payload width");
	}
}

bool nRF24_IsTxFifoFull(nrfl2401 *nrf)
{
	uint8_t readBuffer[10] = {0};
	nRF24_ReadRegister(nrf, nRF24_STATUS_REG, (uint8_t) 1, readBuffer, "Read Status register");

	if (readBuffer[1] & nRF24_DISABLE_TX_FULL)
	{
		return true;
	}
	return false;
}

/*
 * @nRF24_IRQHandler: interrupt handler for servicing nRF24 radio module's requests
 */
void nRF24_IRQHandler(nrfl2401 *nrfRadio)
{
	uint8_t regStatus[10] = {0}, fifoStatus[10] = {0};
	printf ("\nIRQ handler...\n");

	nRF24_ReadRegister(nrfRadio, nRF24_STATUS_REG, (uint8_t) 1, regStatus, "Read Status register");

	nRF24_ReadFifo(nrfRadio, fifoStatus);

	// TX FIFO full
	if (regStatus[1] & nRF24_DISABLE_TX_DS)
	{
		nRF24_TxCallback(nrfRadio);
		nrfRadio->state = NRF_TX_OK;
		nRF24_DisableIRQ(nrfRadio, nRF24_DISABLE_TX_FULL);
	}
	// RX FIFO != empty
	else if (regStatus[1] & nRF24_DISABLE_RX_DR)
	{
		nRF24_ReadPayload(nrfRadio);
		nRF24_RxCallback(nrfRadio);
		nrfRadio->state = NRF_RX_OK;
		nRF24_DisableIRQ(nrfRadio, nRF24_DISABLE_RX_DR);
	}

	// TODO - check if TX FIFO is full - if it is, call nRF24_TransmitPayload()

	// clear pending bit
	__HAL_GPIO_EXTI_CLEAR_IT(nRF24_IRQ_PIN);
}

void nRF24_ReadFifo(nrfl2401 *nrf, uint8_t readBuffer[])
{
	printf ("Reading FIFO...\n");
	nRF24_ReadRegister(nrf, nRF24_FIFO_STATUS_REG, (uint8_t) 1, readBuffer, "Reading FIFO status");

	nRF24_ReadRegister(nrf, nRF24_CMD_R_PAYLOAD, (uint8_t) 2, readBuffer, "Reading FIFO");
}

NRF_STATUS nRF24_GetDynamicPayloadSize(nrfl2401 *nrf, uint8_t readBuffer[])
{
	nRF24_ReadRegister(nrf, nRF24_CMD_R_RX_PL_WID, (uint8_t) 2, readBuffer, "Read Dyn_PayloadSize");

	// flush bytes if bytes retrieved are > nRF24_MAX_FIFO_BYTES
	if (readBuffer[1] > nRF24_MAX_FIFO_BYTES)
	{
		nRF24_FlushRX(nrf);
		return NRF_ERROR;
	}
	return NRF_OK;
}

NRF_STATUS nRF24_ReadPayload(nrfl2401 *nrf)
{
	printf ("\nReading payload...\n");
	uint8_t readBuffer[10] = {0}, payloadSize;

	// switch to RX mode
	nRF24_SetConfigureRegister(nrf, nRF24_PRIM_RX, RX_CTRL);

	if (nrf->config.enableDynamicPayload == ENABLE)
	{
		if (nRF24_GetDynamicPayloadSize(nrf, &payloadSize) != NRF_OK)
		{
			printf ("ERROR reading bytes from RX FIFO\n");
			return NRF_ERROR;
		}
	}
	else
	{
		payloadSize = nrf->config.payloadLength;
	}

	SET_CE();

	nRF24_ReadRegister(nrf, nRF24_FIFO_STATUS_REG, (uint8_t) 1, readBuffer, "Reading FIFO status");

	nRF24_ReadRegister(nrf, nRF24_CMD_R_PAYLOAD, payloadSize, nrf->config.rxBuffer, "Reading payload");

	return NRF_OK;
}

NRF_STATUS nRF24_TransmitPayload(nrfl2401 *nrf, uint8_t *data, uint8_t size)
{
	printf ("\n ********** Sending payload data %c ************\n", *data);

//	// switch to TX mode
//	nRF24_SetConfigureRegister(nrf, nRF24_PRIM_RX, TX_CTRL);		todo - switch to TX mode here!

	nrf->state = NRF_TX_BUSY;

	for (int i=0; i<1; i++)			// TODO - remove!
	{
		char txt[20] = "\nTX payload_";
		sprintf (txt, strcat(txt, "%d"), i);

		nRF24_WriteRegister(nrf, nRF24_CMD_W_PAYLOAD, &data[i], size, txt);

		nRF24_PrintDetails(&nrfRadio);
	}
	SET_CE();

//	HAL_Delay(5);
	while(nrf->state == NRF_TX_BUSY); // THIS AND the following line should be in the callback I guess

	RESET_CE();
}

void nRF24_EnablePayloadWithAck(nrfl2401 *nrf)
{
	uint8_t readBuffer[5] = {0}, setValue;
	if (nrf->config.enablePayloadkWithAck == SET)
	{
		nRF24_ReadRegister(nrf, nRF24_FEATURE_REG, 1, readBuffer, "Reading FEATURE_REG");

		setValue = readBuffer[1] | nRF24_ENABLE_PAYLOAD_WITH_ACK;
		nRF24_WriteRegister(nrf, nRF24_FEATURE_REG, &setValue, (uint8_t) 1, "Enable PAYLOAD with ACK");
	}
}

void nRF24_DisableAutoAck(nrfl2401 *nrf)
{
	uint8_t regState;
	uint8_t rxBuffer[2] = {0};

	if (nrf->config.disableAutoAck[0] > 0)		// the first element represents the size of data pipes who's ACK is to be disabled
	{
		nRF24_ReadRegister(nrf, nRF24_EN_AA_REG, 1, rxBuffer, "Reading EN_AA");

		for (uint8_t i = 1; i <= nrf->config.disableAutoAck[0]; i++)
		{
			regState = (rxBuffer[1] & ~(nrf->config.disableAutoAck[i]));
		}
		nRF24_WriteRegister(nrf, nRF24_EN_AA_REG, &regState, (uint8_t) 1, "Disable ACK");
	}
	nRF24_ReadRegister(nrf, nRF24_EN_AA_REG, 1, rxBuffer, "Reading EN_AA");
}

void nRF24_SetConfigureRegister(nrfl2401 *nrf, NRF_ConfigRegBits configRegBit, NRF_FLAG setFlag)
{
	uint8_t regState;
	uint8_t rxBuffer[2] = {0};

	// read the initial state of config register
	nRF24_ReadRegister(nrf, nRF24_CONFIG_REG, (uint8_t) 1, rxBuffer, "Read Config register");

	// set the flag that's being passed
	regState = (setFlag == BIT_SET) ? (rxBuffer[1] | 1 << configRegBit) : (rxBuffer[1] & ~(1 << configRegBit));

	nRF24_WriteRegister(nrf, nRF24_CONFIG_REG, &regState, (uint8_t) 1, "Write Config register");
}



__weak void nRF24_TxCallback(nrfl2401 *nrf)
{

}

__weak void nRF24_RxCallback(nrfl2401 *nrf)
{

}
