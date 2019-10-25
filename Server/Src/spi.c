#include "spi.h"


/**
 * @brief   Initialize SPI peripheral.
 * 
 * @param   pxSpi       Pointer to SPI structure.
 * 
 * @param   plFile      Pointer to SPI file descriptor.
 * 
 * @param   ulMode      Mask for CPOl (Clock Polarity) and CPHA (Clock Phase).
 * 
 * @param   ulSpeed     Maximum transfer speed.
 * 
 * @param   ulBits      Bits per word.
 * 
 * @return  lStatus     Success/Failure.
 */
int32_t SPI_lInit(struct spi_ioc_transfer *pxSpi, int32_t *const plFile, const uint32_t ulMode, const uint32_t ulSpeed, const uint8_t ucBits)
{
    int32_t lStatus = 0;
    
    pxSpi->speed_hz = ulSpeed;
    pxSpi->bits_per_word = ucBits;
    pxSpi->delay_usecs = 0;
    pxSpi->cs_change = 0;
    pxSpi->delay_usecs = 0;
    pxSpi->pad = 0;
    
    *plFile = open("/dev/spidev0.1", O_RDWR);
    if (*plFile < 0)
    {
        perror("Error opening SPI device!\n");
        lStatus = *plFile;
        goto Cleanup;
    }
    
    lStatus = ioctl(*plFile, SPI_IOC_WR_MODE, &ulMode);
    if (lStatus < 0)
    {
        perror("Error setting SPI WR mode!\n");
        goto Cleanup;
    }
    
    lStatus = ioctl(*plFile, SPI_IOC_RD_MODE, &ulMode);
    if (lStatus < 0)
    {
        perror("Error setting SPI RD mode!\n");
        goto Cleanup;
    }
    
    lStatus = ioctl(*plFile, SPI_IOC_WR_BITS_PER_WORD, &pxSpi->bits_per_word);
    if (lStatus < 0)
    {
        perror("Error setting SPI WR bits!\n");
        goto Cleanup;
    }
    
    lStatus = ioctl(*plFile, SPI_IOC_RD_BITS_PER_WORD, &pxSpi->bits_per_word);
    if (lStatus < 0)
    {
        perror("Error setting SPI RD bits!\n");
        goto Cleanup;
    }
    
    lStatus = ioctl(*plFile, SPI_IOC_WR_MAX_SPEED_HZ, &pxSpi->speed_hz);
    if (lStatus < 0)
    {
        perror("Error setting SPI WR max speed!\n");
        goto Cleanup;
    }
    
    lStatus = ioctl(*plFile, SPI_IOC_RD_MAX_SPEED_HZ, &pxSpi->speed_hz);
    if (lStatus < 0)
    {
        perror("Error setting SPI RD max speed!\n");
        goto Cleanup;
    }
    
Cleanup:
    return (lStatus);
}


/**
 * @brief  Write full-duplex SPI message.
 * 
 * @param   pxSpi       Pointer to SPI structure.
 * 
 * @param   plFile      Pointer to SPI file descriptor.
 *
 * @param   pulTxData   Pointer to TX message.
 *
 * @param   pulTxData   Pointer to RX message.
 
 * @param   ulLength    Message length.
 * 
 * @return  lStatus     Success/Failure
 */
int32_t SPI_lReadWrite(struct spi_ioc_transfer *pxSpi, int32_t *const plFile, uint64_t *const pullTxData, uint64_t *const pullRxData, const uint32_t ulLength)
{
    int32_t lStatus;
    
    pxSpi->tx_buf = (uint64_t)pullTxData;
    pxSpi->rx_buf = (uint64_t)pullRxData;
    pxSpi->len = ulLength;

    lStatus = ioctl(*plFile, SPI_IOC_MESSAGE(ulLength), &pxSpi);

    if (lStatus < 0)
    {
        perror("Error transmitting SPI data!\n");
    }

    return (lStatus);
}


/**
 * @brief   Close SPI peripheral file descriptor.
 * 
 * @param   plFile      Pointer to SPI file descriptor.
 * 
 * @return  lStatus     Success/Failure
 */
int32_t SPI_lClose(int32_t *const plFile)
{
    int32_t lStatus = 0;

    lStatus = close(*plFile);
    if (lStatus < 0)
    {
        perror("Error closing SPI device!\n");
    }
    
    return (lStatus);
}
