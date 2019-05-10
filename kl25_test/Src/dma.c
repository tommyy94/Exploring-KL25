#include "dma.h"


/* Function descriptions */

/**
 * @brief   Initialize DMA for ADC0. Unused.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_vInit(uint32_t *pulDstAddr)
{
    /* Turn on clock to DMA0 & DMAMUX */
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

    /* Disable DMA0 to configure it */
    DMAMUX0->CHCFG[0] = 0;
	
    /**
     * Generate interrupt on completion
     * Increment address
     * Transfer 16-bits
     * Enable peripheral request
     */
    DMA0->DMA[0].DCR = DMA_DCR_EINT(1) | DMA_DCR_SINC(0) | DMA_DCR_SSIZE(2) | DMA_DCR_DSIZE(2) | DMA_DCR_ERQ(1) | DMA_DCR_CS(1);
    
    /* Enable DMA0 MUX channel with ADC0 complete as trigger */
    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(DMAMUX_CHCFG_SOURCE_ADC0); 
    
    /* Initialize src & dst pointers */
    DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)(&(ADC0->R[0])));
    DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)pulDstAddr);
    
    /* Byte count */
    DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(2);
    
    /* Set NVIC for DMA ISR */
    NVIC_SetPriority(DMA0_IRQn, 2);
    NVIC_ClearPendingIRQ(DMA0_IRQn); 
    NVIC_EnableIRQ(DMA0_IRQn);
    
    /* Clear done flag */
    DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;
    
    /* Set enable flag */
    DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;	
}


/**
 * @brief   DMA0 IRQ Handler for transfer completion.
 * 
 * @param   None
 * 
 * @return  None
 */
void DMA0_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(DMA0_IRQn);
    
    /* Signal interrupt */
    FGPIOD->PTOR = MASK(SIGNAL_SHIFT);
    
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;
    DMAMUX0->CHCFG[0] &= ~DMAMUX_CHCFG_ENBL_MASK;
}
