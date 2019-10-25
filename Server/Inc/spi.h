#pragma once
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int32_t SPI_lInit(struct spi_ioc_transfer *pxSpi, int32_t *const plFile, const uint32_t ulMode, const uint32_t ulSpeed, const uint8_t ucBits);
int32_t SPI_lReadWrite(struct spi_ioc_transfer *pxSpi, int32_t *const plFile, uint64_t *const pullTxData, uint64_t *const pullRxData, const uint32_t ulLength);
int32_t SPI_lClose(int32_t *const plFile);
