#ifndef __SPI0_H__
#define __SPI0_H__

#include "SPI_config.h"

#if USE_SPI_0

void SPI0_init();

#if SPI0_MOSI_ENABLE
void SPI0_write(uint8_t data);
#endif

#if SPI0_MISO_ENABLE
uint8_t SPI0_read();
#endif

#if SPI0_MOSI_ENABLE && SPI0_MISO_ENABLE
uint8_t SPI0_write_read(uint8_t dat);
#endif

#endif


#endif