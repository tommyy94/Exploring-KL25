/**
 * dma.c
 * This compilation unit handles Direct Memory Access.
 */

#include "dma.h"

/* Local defines */
#define BCR_MAX_LEN                 (0x00F0FFFFUL)

/* Validate DMA address register as specified in the datasheet */
#define VALIDATE_ADDR_REG(x)        ((((x) >= 0x00000000) && ((x) < 0x00100000)) |   \
                                     (((x) >= 0x1FF00000) && ((x) < 0x20000000)) |   \
                                     (((x) >= 0x20000000) && ((x) < 0x20100000)) |   \
                                     (((x) >= 0x40000000) && ((x) < 0x40100000)))    


/* Function descriptions */

/**
 * @brief   Initialize DMAMUX0.
 * 
 * @param   ulChannel       DMA channel.
 * @param   ucSource        DMA0 trigger source.
 * 
 * @return  None
 */
    void DMAMUX0_vInit(uint32_t const ulChannel, uint32_t const ulSource)
{
    configASSERT(ulChannel < DMAMUX_CHCFG_COUNT);
    configASSERT(ulSource <= DMAMUX0_CHCFG_SOURCE_TSI);

    /* Disable DMA channels to configure it */
    DMAMUX0->CHCFG[ulChannel] = 0;
    
    /* Enable DMA0 MUX channels with SPI1 TX & RX as triggers */
    DMAMUX0->CHCFG[ulChannel] = DMAMUX_CHCFG_SOURCE(ulSource) & (~DMAMUX_CHCFG_TRIG_MASK);
}


/**
 * @brief   Link DMA0 channels.
 * 
 * @param   ucSrcCh     Source channel
 * @param   ucDstCh     Destination channel
 * 
 * @return  None
 */
void DMA0_vLinkChannel(const uint32_t ulSrcCh, const uint32_t ulDstCh)
{
    configASSERT(ulSrcCh < DMAMUX_CHCFG_COUNT);
    configASSERT(ulDstCh < DMAMUX_CHCFG_COUNT);

    /* Perform a link to channel LCH1 after each cycle-steal transfer */
    DMA0->DMA[ulSrcCh].DCR |= DMA_DCR_LINKCC(2) | DMA_DCR_LCH1(ulDstCh);
}


/**
 * @brief   Initialize DMA for byte transfers on peripheral request.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_vInit(void)
{
    /**
     * Configure channel 0:
     * Increment source address
     * Transfer bytes
     * Enable peripheral request
     * Cycle stealing mode
     */
    DMA0->DMA[DMA_CHANNEL0].DCR |= DMA_DCR_ERQ(1) | DMA_DCR_CS(1) | DMA_DCR_SSIZE(1) | DMA_DCR_DSIZE(1) | DMA_DCR_SINC(1);

    /**
     * Configure channel 1:
     * Increment destination address
     * Transfer bytes
     * Enable peripheral request
     * Cycle stealing mode
     */
    DMA0->DMA[DMA_CHANNEL1].DCR |= DMA_DCR_ERQ(1) | DMA_DCR_CS(1) | DMA_DCR_SSIZE(1) | DMA_DCR_DSIZE(1) | DMA_DCR_DINC(1);
    
    /* Clear done flags */
    DMA0->DMA[DMA_CHANNEL0].DSR_BCR &= ~DMA_DSR_BCR_DONE(1);
    DMA0->DMA[DMA_CHANNEL1].DSR_BCR &= ~DMA_DSR_BCR_DONE(1);
}


/**
 * @brief   Initialize DMA addresses and transfer size.
 * 
 * @param   ulChannel       DMA channel.
 * 
 * @param   pulSrcAddr      Source address
 * 
 * @param   pulDstAddr      Destination address
 * 
 * @param   ulLength        Transaction length
 * 
 * @return  None
 */
void DMA0_vInitTransaction(uint32_t ulChannel, uint32_t *const pulSrcAddr, uint32_t *const pulDstAddr, const uint32_t ulLength)
{
    configASSERT(ulChannel < DMAMUX_CHCFG_COUNT);
    configASSERT(VALIDATE_ADDR_REG((uint32_t)pulSrcAddr));
    configASSERT(VALIDATE_ADDR_REG((uint32_t)pulDstAddr));
    configASSERT(ulLength < BCR_MAX_LEN);
    
    /* Initialize source & destination pointers */
    DMA0->DMA[ulChannel].SAR = DMA_SAR_SAR((uint32_t)pulSrcAddr);
    DMA0->DMA[ulChannel].DAR = DMA_DAR_DAR((uint32_t)pulDstAddr);
    
    /* Number of bytes to transmit */
    DMA0->DMA[ulChannel].DSR_BCR |= DMA_DSR_BCR_BCR(ulLength);
}


/**
 * @brief   Enable DMA0 on specified channel.
 * 
 * @param   ulChannel       DMA channel.
 * 
 * @return  None
 */
void DMA0_vStart(const uint32_t ulChannel)
{
    configASSERT(ulChannel < DMAMUX_CHCFG_COUNT);
    
    /* Set enable flag */
    BME_OR32(&DMAMUX0->CHCFG[ulChannel], DMAMUX_CHCFG_ENBL(1));
}


/**
 * @brief   Disable DMA0.
 * 
 * @param   ulChannel       DMA channel.
 * 
 * @return  None
 */
void DMA0_vStop(const uint32_t ulChannel)
{
    configASSERT(ulChannel < DMAMUX_CHCFG_COUNT);

    BME_AND32(&DMAMUX0->CHCFG[ulChannel], ~DMAMUX_CHCFG_ENBL(1));
}
