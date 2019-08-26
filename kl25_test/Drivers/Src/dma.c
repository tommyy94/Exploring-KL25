/**
 * dma.c
 * This compilation unit handles Direct Memory Access.
 */

#include "dma.h"


/* Function descriptions */

/**
 * @brief   Initialize DMA for UART0 RX.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_vInit(void)
{
    /* Turn on clock to DMA0 & DMAMUX */
    SIM->SCGC7 |= SIM_SCGC7_DMA(1);
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX(1);

    /* Disable DMA channel to configure it */
    DMAMUX0->CHCFG[0] = 0;
	
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
    
    /* Enable DMA0 MUX channel with SPI1 TX as trigger */
    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(DMAMUX_CHCFG_SOURCE_SPI1_TX) & (~DMAMUX_CHCFG_TRIG_MASK);
    
    /* Set NVIC for DMA ISR */
    NVIC_SetPriority(DMA0_IRQn, 2);
    NVIC_ClearPendingIRQ(DMA0_IRQn); 
    NVIC_EnableIRQ(DMA0_IRQn);
}


/**
 * @brief   Set source and destination pointers and enable DMA0.
 * 
 * @param   pulSrcAddr      TX data adress
 * @param   ulByteCount     Number of bytes to receive.
 * 
 * @return  None
 */
void DMA0_vStart(uint32_t *const pulSrcAddr)
{
    /* Initialize src & dst pointers */
    DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)pulSrcAddr + 0x01); /* First byte sent by placing to register */
    DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)(&(SPI1->D)));
    
    /* Number of bytes to transmit */
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_BCR(strlen((const char *)pulSrcAddr) - 1); /* Subtract first byte sent  from count */
    
    /* Set enable flag */
    BME_OR32(&DMAMUX0->CHCFG[0], DMAMUX_CHCFG_ENBL(1));
}


/**
 * @brief   Stop DMA0 transfers.
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
 * @brief   DMA0 IRQ Handler for SPI1 TX complete.
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
