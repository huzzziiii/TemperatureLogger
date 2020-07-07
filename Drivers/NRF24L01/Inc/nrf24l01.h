/*
 * nrf24l01.h
 */
#include "stm32f4xx_hal.h"
#include "usart_app.h"
#include "string.h"
#include "stdbool.h"

//#include "stm32f4xx_hal_gpio.h"  // todo - figure out why including this causes errors

#ifndef NRF24L01_INC_NRF24L01_H_
#define NRF24L01_INC_NRF24L01_H_

#define PRI_UINT64_C_Val(value) ((unsigned long) (value>>32)), ((unsigned long)value)
#define PRI_UINT64 "%lx%lx"
#define LONG_TO_STR(STR, LONG_VAL) (sprintf(STR, "%lx%lx", PRI_UINT64_C_Val(LONG_VAL)))


//extern struct nrfl2401RadioModule nrfRadio;

extern struct nrfl2401RadioModule nrfRadio;

#define enumToStr(str) 	#str

// commands
#define nRF24_CMD_R_REG				(uint8_t) 0x00
#define nRF24_CMD_W_REG 			(uint8_t) 0x20
#define nRF24_CMD_R_PAYLOAD			(uint8_t) 0x61
#define nRF24_CMD_W_PAYLOAD			(uint8_t) 0xA0
#define nRF24_CMD_R_RX_PL_WID		(uint8_t) 0x60
#define nRF24_CMD_FLUSH_TX			(uint8_t) 0xE1
#define nRF24_CMD_FLUSH_RX			(uint8_t) 0xE2
#define nRF24_CMD_NOP				(uint8_t) 0xFF

// register definitions
#define nRF24_CONFIG_REG			(uint8_t) 0x00
#define nRF24_EN_AA_REG				(uint8_t) 0x01
#define nRF24_EN_RXADDR_REG			(uint8_t) 0x02
#define nRF24_SETUP_AW_REG			(uint8_t) 0x03
#define nRF24_SETUP_RETR_REG		(uint8_t) 0x04
#define nRF24_RF_CH_REG				(uint8_t) 0x05
#define nRF24_RF_SETUP_REG			(uint8_t) 0x06
#define nRF24_STATUS_REG			(uint8_t) 0x07
#define nRF24_OBSERVE_TX_REG		(uint8_t) 0x08
#define nRF24_RPD					(uint8_t) 0x09
#define nRF24_RX_ADDR_P0_REG		(uint8_t) 0x0A
#define nRF24_RX_ADDR_P1_REG		(uint8_t) 0x0B
#define nRF24_RX_ADDR_P2_REG		(uint8_t) 0x0C
#define nRF24_RX_ADDR_P3_REG		(uint8_t) 0x0D
#define nRF24_RX_ADDR_P4_REG		(uint8_t) 0x0E
#define nRF24_RX_ADDR_P5_REG		(uint8_t) 0x0F
#define nRF24_TX_ADDR_REG			(uint8_t) 0x10

#define nRF24_RX_PW_P0_REG			(uint8_t) 0x11
#define nRF24_RX_PW_P1_REG			(uint8_t) 0x12
#define nRF24_RX_PW_P2_REG			(uint8_t) 0x13
#define nRF24_RX_PW_P3_REG			(uint8_t) 0x14
#define nRF24_RX_PW_P4_REG			(uint8_t) 0x15
#define nRF24_RX_PW_P5_REG			(uint8_t) 0x16
#define nRF24_FIFO_STATUS_REG		(uint8_t) 0x17
#define nRF24_DYNPD_REG				(uint8_t) 0x1C
#define nRF24_FEATURE_REG			(uint8_t) 0x1D

// masking bits
#define nRF24_R_W_MASK				(uint8_t) 0x1F
#define nRF24_IRQ_CLR_MASK			(uint8_t) 0x70

/* Register bits definitions */
// config register bits
//#define nRF24_CONFIG_PRIM_RX		(uint8_t) 0x01 // PRIM_RX bit in CONFIG register
//#define nRF24_CONFIG_PWR_UP        	(uint8_t) 0x02 // PWR_UP bit in CONFIG register
//#define nRF24_FLAG_RX_DR          	(uint8_t) 0x40 // RX_DR bit (data ready RX FIFO interrupt)
//#define nRF24_FLAG_TX_DS           	(uint8_t) 0x20 // TX_DS bit (data sent TX FIFO interrupt)
//#define nRF24_EN_CRC			   	(uint8_t) 0x80 // Enable CRC
//#define nRF24_FLAG_MAX_RT          	(uint8_t) 0x10 // MAX_RT bit (maximum number of TX retransmits interrupt)

#define nRF24_DEFAULT_STATUS_REG	(uint8_t) 0x0E 	// TODO - rmv
#define nRF24_MAX_FIFO_BYTES		(uint8_t) 32
// Config register bit positions
//#define nRF24_MASK_RX_DR			(uint8_t) 1 << 6
//#define nRF24_MASK_TX_DS			(uint8_t) 1 << 5
//#define nRF24_MASK_MAX_RT			(uint8_t) 1 << 4
//#define nRF24_EN_CRC				(uint8_t) 1 << 3
//#define nRF24_CRCO					(uint8_t) 1 << 2
//#define nRF24_PWR_UP				(uint8_t) 1 << 1
//#define nRF24_PRIM_RX				(uint8_t) 1

typedef enum {
	nRF24_PRIM_RX,
	nRF24_PWR_UP,
	nRF24_CRCO,
	nRF24_EN_CRC,
	nRF24_MASK_MAX_RT,
	nRF24_MASK_TX_DS,
	nRF24_MASK_RX_DR
} NRF_ConfigRegBits;

typedef enum {
	nRF24_SET_PWR_UP = 1 << nRF24_PWR_UP
} nRF24_SET_CONFIG_REG;

typedef enum {
	nRF24_TX_FULL_POS = 0,
	nRF24_RX_P_NO_POS = 1,
	nRF24_TX_DS_POS   = 5,
	nRF24_RX_DR_POS = 6,
	nRF24_MAX_RT_POS = 4,
} NRF_STATUS_REG_POS;

typedef enum {
	nRF24_EN_DYNAMIC_PAYLOAD_POS = 2,
	nRF24_EN_PAYLOAD_WITH_ACK_POS = 1,
} NRF_FEATURE_REG_POS;

typedef enum {
	nRF24_ENABLE_PAYLOAD_WITH_ACK = 1 << nRF24_EN_PAYLOAD_WITH_ACK_POS,
	nRF24_ENABLE_DYNAMIC_PAYLOAD = 1 << nRF24_EN_DYNAMIC_PAYLOAD_POS
} NRF_FEATURE_REG;

typedef enum {
	nRF24_DISABLE_MAX_RT  = 1 << nRF24_MAX_RT_POS,
	nRF24_DISABLE_TX_FULL = 1 << nRF24_TX_FULL_POS,
	nRF24_DISABLE_TX_DS   = 1 << nRF24_TX_DS_POS,
	nRF24_DISABLE_RX_DR   = 1 << nRF24_RX_DR_POS
} NRF_DISABLE_STATUS_REG;



// RF setup
#define nRF24_CONT_WAVE				(uint8_t) 1 << 7
#define nRF24_RF_DR_LOW				(uint8_t) 1 << 5
#define nRF24_RF_DR_HIGH(DATA_RATE)	(DATA_RATE << 3)
#define nRF24_RF_PWR(_PWR_)			((_PWR_) << 1)

// port related definitions
#define nRF24_GPIO_PORT			GPIOC
#define rNRF24_CE_PIN			GPIO_PIN_8
#define rNRF24_CSN_PIN			GPIO_PIN_6
#define nRF24_IRQ_PIN			GPIO_PIN_5

#define SPI_SCLK_PORT			GPIOB
#define SPI_SCLK_PIN			GPIO_PIN_13

#define SPI_MISO_PORT			GPIOB
#define SPI_MISO_PIN			GPIO_PIN_14

#define SPI_MOSI_PORT			GPIOB
#define SPI_MOSI_PIN			GPIO_PIN_15

// Address field widths
#define nRF24_THREE_BYTES		(uint8_t) 0x1
#define nRF24_FOUR_BYTES		(uint8_t) 0x2
#define nRF24_FIVE_BYTES		(uint8_t) 0x3

// data pipes
//#define DATA_PIPE_0				(uint8_t) 1
//#define DATA_PIPE_1				(uint8_t) 1 << 1
//#define DATA_PIPE_2				(uint8_t) 1 << 2
//#define DATA_PIPE_3				(uint8_t) 1 << 3
//#define DATA_PIPE_4				(uint8_t) 1 << 4
//#define DATA_PIPE_5				(uint8_t) 1 << 5

typedef enum {
	DEFAULT_STATUS_REG =  0x0E,
	DEFAULT_CONFIG_REG =  0x08,
	DEFAULT_RF_SETUP_REG= 0x0E,
	DEFAULT_RF_CH_REG	= 0x02,
	DEFAULT_EN_AA_REG = 0x3E,
	DEFAULT_FEATURE_REG = 0x00,
	DEFAULT_DPL_PX  = 0x00

} NRF_DEFAULT_REG_VALUES;

typedef enum {
	DATA_PIPE_0 = 1,
	DATA_PIPE_1 = 1 << 1,
	DATA_PIPE_2 = 1 << 2,
	DATA_PIPE_3 = 1 << 3,
	DATA_PIPE_4 = 1 << 4,
	DATA_PIPE_5 = 1 << 5
} NRF_DATA_PIPES;


// SPI pin set/reset definitions
#define SET_CE()				HAL_GPIO_WritePin(nRF24_GPIO_PORT, rNRF24_CE_PIN, GPIO_PIN_SET);
#define SET_CSN()				HAL_GPIO_WritePin(nRF24_GPIO_PORT, rNRF24_CSN_PIN, GPIO_PIN_SET);
#define RESET_CE()				HAL_GPIO_WritePin(nRF24_GPIO_PORT, rNRF24_CE_PIN, GPIO_PIN_RESET);
#define RESET_CSN()				HAL_GPIO_WritePin(nRF24_GPIO_PORT, rNRF24_CSN_PIN, GPIO_PIN_RESET);

// data dates
#define nRF24_SPEED_1MBPS				(uint8_t) 0x00
#define nRF24_SPEED_2MBPS				(uint8_t) 0x01
#define nRF24_SPEED_250KBPS				(uint8_t) 0x02

// RF output power
#define nRF24_neg18_dBm					(uint8_t) 0x00
#define nRF24_neg12_dBm					(uint8_t) 0x01
#define nRF24_neg6_dBm					(uint8_t) 0x02
#define nRF24_0_dBm						(uint8_t) 0x03

#define nRF24_MAX_DATA_PIPES			(uint8_t) 0x06

typedef enum {
	NRF_TX_BUSY,
	NRF_TX_OK,
	NRF_RX_BUSY,
	NRF_RX_OK
} NRF_STATE;

typedef enum {
	NRF_OK,
	NRF_ERROR,
} NRF_STATUS;

typedef enum {
	BIT_RESET,
	BIT_SET,
} NRF_FLAG;

typedef enum {
	TX_CTRL = BIT_RESET,
	RX_CTRL = BIT_SET,
	PWR_UP = BIT_SET,
	PWR_DOWN = BIT_RESET
} CONFIG_REG_FLAGS;



typedef struct
{
	SPI_HandleTypeDef *spiHandle;
	uint8_t *enableRxDataPipes;
	uint8_t *disableAutoAck;
	uint8_t rfChannel;
	uint8_t *rfSetup;		// todo - maybe remove
	uint8_t dataRate;
	uint8_t *rxBuffer;
	uint8_t payloadLength;
	uint8_t addressWidth;
	uint8_t addressFieldWidth;  // for SETUP_AW
	uint8_t *txAddress;
	uint8_t enableDynamicPayload;
	uint8_t enablePayloadkWithAck;
	uint8_t rxAddress;			// todo - to use or not?
	uint8_t txDevicesUsed;   	// upto nRF24_MAX_DATA_PIPES
	uint8_t (*txAddressesList)[5];	// pointer to an array containing addresses of all PTXs
	uint8_t *rxPayloadWidths; 	// pointer to an array containing a list of payload widths for each data pipe being used


	// config reg
	uint8_t configReg_maskRxDR;
	uint8_t configReg_maskTxDS;
	uint8_t configReg_maxRT;
	uint8_t configReg_enCRC;
	uint8_t configReg_crco;
	uint8_t configReg_pwrUP;
	uint8_t configReg_primRX;
} nrfl2401_Config;

typedef struct nrfl2401RadioModule
{
	NRF_STATE state;
	nrfl2401_Config config;
} nrfl2401;



// function declarations
void nRF24_GPIO_Init(GPIO_InitTypeDef *gpioPort);
NRF_STATUS nRF24_Initialization(nrfl2401 *nrf);
NRF_STATUS nRF24_TransmitPayload(nrfl2401 *nrf, uint8_t *data, uint8_t size);
NRF_STATUS nRF24_WriteRegister(nrfl2401 *nrf, uint8_t reg, uint8_t *data, const uint8_t txSize, char *text);

void nRF24_DisableAutoAck(nrfl2401 *nrf);
void nRF24_ReadFifo(nrfl2401 *nrf, uint8_t readBuffer[]);
void nRF24_TxCallback(nrfl2401 *nrfRadio);
NRF_STATUS nRF24_ReadPayload(nrfl2401 *nrf);
void nRF24_EnablePayloadWithAck(nrfl2401 *nrf);
void nRF24_SetConfigureRegister(nrfl2401 *nrf, NRF_ConfigRegBits configRegBit, NRF_FLAG setFlag);





//NRF_STATUS nRF24_TransmitPayload(nrfl2401 *nrf);
//NRF_STATUS nRF24_ReceivePayload(nrfl2401 *nrf);

#endif /* NRF24L01_INC_NRF24L01_H_ */
