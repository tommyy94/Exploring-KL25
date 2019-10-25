#include "includes.h"


#define MSG_LEN     (8UL)


int main(void)
{
    struct spi_ioc_transfer xSpi;
    uint64_t ullTxData[MSG_LEN] = { 5, 6, 5 };
    uint64_t ullRxData[MSG_LEN];
    int32_t *plFile = (int32_t *)malloc(sizeof(int32_t));
    
    /* Clock idle low, data clock in rising edge, output on falling edge, 1000 MHz, 8 bits per word. */
    if (SPI_lInit(&xSpi, plFile, SPI_MODE_0, 1000000UL, 8) > 0)
    {
        exit(EXIT_FAILURE);
    }
    
    if (SPI_lReadWrite(&xSpi, plFile, ullTxData, ullRxData, MSG_LEN))
    {
        exit(EXIT_FAILURE);
    }
    
    if (SPI_lClose(plFile) > 0)
    {
        exit(EXIT_FAILURE);
    }
    
    free(plFile);
	return 0;
}
