#include "dma.h"


/* Function descriptions */

/**
 * @brief   Initialize DMA for ADC0.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_vInit(void)
{
    /* Turn on clock to DMA0 & DMAMUX */
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

    /* Disable DMA0 to configure it */
    DMAMUX0->CHCFG[0] = 0;
	
    /**
     * Generate interrupt on completion
     * Increment destination address
     * Transfer bytes
     * Enable peripheral request
     */
    DMA0->DMA[0].DCR = DMA_DCR_EINT(1) | DMA_DCR_ERQ(1) | DMA_DCR_CS(1) | DMA_DCR_SSIZE(1) | DMA_DCR_DINC(1) | DMA_DCR_DSIZE(1);
    
    /* Enable DMA0 MUX channel with UART0 RX as trigger */
    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(DMAMUX_CHCFG_SOURCE_UART0_RX);
    
    /* Set NVIC for DMA ISR */
    NVIC_SetPriority(DMA0_IRQn, 2);
    NVIC_ClearPendingIRQ(DMA0_IRQn); 
    NVIC_EnableIRQ(DMA0_IRQn);
}


void DMA0_vStart(uint32_t *const pulDstAddr, uint32_t ulByteCount)
{
    /* Initialize src & dst pointers */
    DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)(&(UART0->D)));
    DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)pulDstAddr);
    
    /* Byte count */
    DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(ulByteCount);
    
    /* Clear done flag */
    DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;
    
    /* Set enable flag */
    DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;	
}


/**
 * @brief   DMA0 IRQ Handler for receive complete.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(DMA0_IRQn);
    
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
    DMAMUX0->CHCFG[0] &= ~DMAMUX_CHCFG_ENBL_MASK;
}
