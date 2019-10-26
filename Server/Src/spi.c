#include "spi.h"


/**
 * @brief   Initialize SPI peripheral.
 * 
 * @param   fd          File descriptor.
 * 
 * @param   transfer    Pointer to SPI structure.
 * 
 * @param   mode        Mask for CPOl (Clock Polarity) and CPHA (Clock Phase).
 * 
 * @return  None
 */
void spi_init(const int fd, struct spi_ioc_transfer *transfer, const unsigned long mode)
{
    int ret;
    
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret < 0)
    {
        perror("Error setting SPI WR mode!\n");
        exit(EXIT_FAILURE);
    }
    
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (ret < 0)
    {
        perror("Error setting SPI RD mode!\n");
        exit(EXIT_FAILURE);
    }
    
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &transfer->bits_per_word);
    if (ret < 0)
    {
        perror("Error setting SPI WR bits!\n");
        exit(EXIT_FAILURE);
    }
    
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &transfer->bits_per_word);
    if (ret < 0)
    {
        perror("Error setting SPI RD bits!\n");
        exit(EXIT_FAILURE);
    }
    
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &transfer->speed_hz);
    if (ret < 0)
    {
        perror("Error setting SPI WR max speed!\n");
        exit(EXIT_FAILURE);
    }
    
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &transfer->speed_hz);
    if (ret < 0)
    {
        perror("Error setting SPI RD max speed!\n");
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief  Write full-duplex SPI message.
 * 
 * @param   fd          File descriptor.
 * 
 * @param   transfer    Pointer to SPI structure.
 *
 * @param   tx_data     Pointer to TX message.
 *
 * @param   rx_Data     Pointer to RX message.
 
 * @param   len         Message length.
 * 
 * @return  None
 */
void spi_readwrite(const int fd, struct spi_ioc_transfer *transfer, unsigned char *const tx_data, unsigned char *const rx_data, const unsigned long len)
{
    int ret;
    
    transfer->tx_buf = (unsigned long)tx_data;
    transfer->rx_buf = (unsigned long)rx_data;
    transfer->len = len;

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &*transfer);
    if (ret < 0)
    {
        perror("Error transmitting SPI data!\n");
    }
}
