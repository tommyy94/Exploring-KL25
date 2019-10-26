#include "includes.h"


#define MSG_LEN     (3UL)


int main(void)
{
    struct spi_ioc_transfer transfer = {
        .speed_hz = 1000000UL,
        .bits_per_word = 8,
        .delay_usecs = 0,
        .cs_change = 0,
        .pad = 0,
    };
    
    int fd;
    unsigned char tx_data[MSG_LEN] = "abc";
    unsigned char rx_data[MSG_LEN];
    
    fd = open("/dev/spidev0.1", O_RDWR);
    if (fd < 0)
    {
        perror("Error opening SPI device!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Clock idle low, data clock in rising edge, output on falling edge */
    spi_init(fd, &transfer, SPI_MODE_0);
    
    /* Loopback test */
    spi_readwrite(fd, &transfer, tx_data, rx_data, MSG_LEN);
    printf("Main rx: %s\n", rx_data);
    
    if (close(fd) < 0)
    {
        perror("Error closing SPI device!\n");
    }
    
	return 0;
}
