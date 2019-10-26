#pragma once
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void spi_init(const int fd, struct spi_ioc_transfer *transfer, const unsigned long mode);
void spi_readwrite(const int fd, struct spi_ioc_transfer *transfer, unsigned char *const tx_data, unsigned char *const rx_data, const unsigned long len);
