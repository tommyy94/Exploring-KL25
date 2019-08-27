/**
 * dma.c
 * This compilation unit handles Direct Memory Access.
 */

#include "dma.h"


/* Local defines */
#define BYTE_OFFSET     (0x01UL)


/* Function descriptions */


/**
 * @brief   Initialize DMAMUX0.
 * 
 * @param   ucSource        DMA0 trigger source.
 * 
 * @return  None
 */
void DMAMUX0_vInit(uint8_t const ucSource)
{
    /* Enable clock gating */
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX(1);
    
    /* Disable DMA channel to configure it */
    DMAMUX0->CHCFG[0] = 0;
    
    /* Enable DMA0 MUX channel with SPI1 TX as trigger */
    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(ucSource) & (~DMAMUX_CHCFG_TRIG_MASK);
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
    /* Enable clock gating */
    SIM->SCGC7 |= SIM_SCGC7_DMA(1);
	
    /**
     * Generate interrupt on completion
     * Increment source address
     * Transfer bytes
     * Enable peripheral request
     * Cycle stealing mode
     */
    DMA0->DMA[0].DCR = DMA_DCR_EINT(1) | DMA_DCR_ERQ(1) | DMA_DCR_CS(1) | DMA_DCR_SSIZE(1) | DMA_DCR_DSIZE(1) | DMA_DCR_SINC(1);
    
    /* Clear done flag */
    DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE(1);
    
    /* Set NVIC for DMA ISR */
    NVIC_SetPriority(DMA0_IRQn, 2);
    NVIC_ClearPendingIRQ(DMA0_IRQn); 
    NVIC_EnableIRQ(DMA0_IRQn);
}


/**
 * @brief   Initialize DMA addresses and transfer size.
 * 
 * @param   pulSrcAddr      Source address
 * @param   pulDstAddr      Destination address
 * 
 * @return  None
 */
void DMA0_vInitTransaction(uint32_t *const pulSrcAddr, uint32_t *const pulDstAddr)
{
    /* Initialize source & destination pointers */
    DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)pulSrcAddr + BYTE_OFFSET); /* First byte sent by placing to register */
    DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)pulDstAddr);
    
    /* Number of bytes to transmit */
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_BCR(strlen((const char *)pulSrcAddr) - BYTE_OFFSET); /* Subtract first byte sent from count */
}


/**
 * @brief   Enable DMA0.
 * 
 * @param   pulSrcAddr      TX data adress
 * @param   ulByteCount     Number of bytes to receive.
 * 
 * @return  None
 */
void DMA0_vStart(void)
{
    /* Set enable flag */
    BME_OR32(&DMAMUX0->CHCFG[0], DMAMUX_CHCFG_ENBL(1));
}


/**
 * @brief   Disable DMA0.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_vStop(void)
{
    BME_AND32(&DMAMUX0->CHCFG[0], ~DMAMUX_CHCFG_ENBL(1));
}


/**
 * @brief   DMA0 IRQ Handler for transaction complete.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_IRQHandler(void)
{
    DMA0_vStop();
    BME_OR32(&DMA0->DMA[0].DSR_BCR, DMA_DSR_BCR_DONE(1));
}
