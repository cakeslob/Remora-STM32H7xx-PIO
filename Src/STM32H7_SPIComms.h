#ifndef STM32H7_SPIComms_H
#define STM32H7_SPIComms_H

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_dma.h"

#include <memory>
#include <algorithm>

#include "remora/remora.h"
#include "remora/comms/commsInterface.h"
#include "remora/modules/moduleInterrupt.h"

typedef struct
{
  __IO uint32_t ISR;   /*!< DMA interrupt status register */
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
} DMA_Base_Registers;

typedef enum {
    DMA_HALF_TRANSFER = 1,   // Half-transfer completed
    DMA_TRANSFER_COMPLETE = 2, // Full transfer completed
    DMA_OTHER = 3        // Other or error status
} DMA_TransferStatus_t;

class STM32H7_SPIComms : public CommsInterface {
private:
    volatile rxData_t*  		ptrRxData;
    volatile txData_t*  		ptrTxData;
    volatile DMA_RxBuffer_t* 	ptrRxDMABuffer;
    SPI_TypeDef*        		spiType;

    uint8_t						RxDMAmemoryIdx;
    uint8_t						RXbufferIdx;
    bool						copyRXbuffer;

	ModuleInterrupt<STM32H7_SPIComms>*	NssInterrupt;
    ModuleInterrupt<STM32H7_SPIComms>*	dmaTxInterrupt;
	ModuleInterrupt<STM32H7_SPIComms>*	dmaRxInterrupt;
	IRQn_Type					irqNss;
	IRQn_Type					irqDMArx;
	IRQn_Type					irqDMAtx;

    SPI_HandleTypeDef   		spiHandle;
    DMA_HandleTypeDef   		hdma_spi_tx;
    DMA_HandleTypeDef   		hdma_spi_rx;
    DMA_HandleTypeDef   		hdma_memtomem;
    HAL_StatusTypeDef   		dmaStatus;

    uint8_t						interruptType;

    bool						newWriteData;

	HAL_StatusTypeDef startMultiBufferDMASPI(uint8_t*, uint8_t*, uint8_t*, uint8_t*, uint16_t);
	int getActiveDMAmemory(DMA_HandleTypeDef*);

	int DMA_IRQHandler(DMA_HandleTypeDef *);
	void handleRxInterrupt(void);
	void handleTxInterrupt(void);
	void handleNssInterrupt(void);

public:
	STM32H7_SPIComms(volatile rxData_t*, volatile txData_t*, SPI_TypeDef*);
	virtual ~STM32H7_SPIComms();

    void init(void);
    void start(void);
    void tasks(void);
};

#endif
