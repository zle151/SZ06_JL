#include "SPI0.h"

#if USE_SPI_0

#if SPI0_SOFT


#define CLK(bit) 	gpio_bit_write(SPI0_CLK_PORT, SPI0_CLK_PIN, ((bit) == 0) ? RESET : SET)
#define MOSI(bit) 	gpio_bit_write(SPI0_MOSI_PORT,SPI0_MOSI_PIN, ((bit) == 0) ? RESET : SET)

void SPI0_init() {
    /*********** GPIO config ***************/
    // clk
    rcu_periph_clock_enable(SPI0_CLK_PORT_RCU);
    gpio_mode_set(SPI0_CLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SPI0_CLK_PIN);
    gpio_output_options_set(SPI0_CLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI0_CLK_PIN);

#if SPI0_MOSI_ENABLE
    // mosi
    rcu_periph_clock_enable(SPI0_MOSI_PORT_RCU);
    gpio_mode_set(SPI0_MOSI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SPI0_MOSI_PIN);
    gpio_output_options_set(SPI0_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI0_MOSI_PIN);
#endif
#if SPI0_MISO_ENABLE
    // miso
    rcu_periph_clock_enable(SPI0_MISO_PORT_RCU);
    gpio_mode_set(SPI0_MISO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, SPI0_MISO_PIN);
    gpio_output_options_set(SPI0_MISO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI0_MISO_PIN);
#endif
}

#if SPI0_MISO_ENABLE
#define MISO_STATE()  gpio_input_bit_get(SPI0_MISO_PORT, SPI0_MISO_PIN)
#endif


#if SPI0_MOSI_ENABLE
void SPI0_write(uint8_t dat) {
    uint8_t i;
    for(i=0; i<8; i++)
    {
        CLK(0);
        MOSI(dat & 0x80);
        dat<<=1;
        CLK(1);
    }
    CLK(0);//拉低
}
#endif
#if SPI0_MISO_ENABLE
uint8_t SPI0_read() {
    uint8_t i,read=0;
    for(i=0; i<8; i++)
    {
        CLK(0);
        read<<=1;
        if(MISO_STATE())
        {
            read++;
        }
        CLK(1);
    }
    CLK(0);//拉低
    return read;
}
#endif


#if SPI0_MOSI_ENABLE && SPI0_MISO_ENABLE
uint8_t SPI0_write_read(uint8_t dat) {
    uint8_t i,read=0;
    for(i=0; i<8; i++)
    {
        CLK(0);
        MOSI(dat&0x80);
        dat<<=1;

        read<<=1;
        if(MISO_STATE())
        {
            read++;
        }
        CLK(1);
    }
    CLK(0);//拉低
    return read;
}
#endif


#else

#define SPIx_RCU			RCU_SPI0
#define SPIx				  SPI0
#define SPI0_CLK_AF		GPIO_AF_5
#define SPI0_MOSI_AF	GPIO_AF_5
#define SPI0_MISO_AF  GPIO_AF_5

void SPI0_init() {

    /*********** GPIO config ***************/
    // clk
    rcu_periph_clock_enable(SPI0_CLK_PORT_RCU);
    gpio_mode_set(SPI0_CLK_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SPI0_CLK_PIN);
    gpio_output_options_set(SPI0_CLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI0_CLK_PIN);
    gpio_af_set(SPI0_CLK_PORT, SPI0_CLK_AF, SPI0_CLK_PIN);

#if SPI0_MOSI_ENABLE
    // mosi
    rcu_periph_clock_enable(SPI0_MOSI_PORT_RCU);
    gpio_mode_set(SPI0_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SPI0_MOSI_PIN);
    gpio_output_options_set(SPI0_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI0_MOSI_PIN);
    gpio_af_set(SPI0_MOSI_PORT, SPI0_MOSI_AF, SPI0_MOSI_PIN);
#endif
#if SPI0_MISO_ENABLE
    // miso
    rcu_periph_clock_enable(SPI0_MISO_PORT_RCU);
    gpio_mode_set(SPI0_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SPI0_MISO_PIN);
    gpio_output_options_set(SPI0_MISO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI0_MISO_PIN);
    gpio_af_set(SPI0_MISO_PORT, SPI0_MISO_AF, SPI0_MISO_PIN);
#endif
    /*************** spi config **************/
    // 时钟
    rcu_periph_clock_enable(SPIx_RCU);

    spi_parameter_struct spi_init_struct;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    // negative slave select
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.clock_polarity_phase = SPI0_PARAM_CPOL_CPHA;
    spi_init_struct.prescale             = SPI0_PARAM_PSC;
    spi_init_struct.endian               = SPI0_PARAM_ENDIAN;
    spi_init(SPIx, &spi_init_struct);

    spi_enable(SPIx);
}

#if SPI0_MOSI_ENABLE
void SPI0_write(uint8_t data) {
    while(RESET == spi_i2s_flag_get(SPIx, SPI_FLAG_TBE));
    //硬件电路，发送数据，放到寄存器
    spi_i2s_data_transmit(SPIx, data);

    while(RESET == spi_i2s_flag_get(SPIx, SPI_FLAG_RBNE));
    //硬件电路，发送数据，放到寄存器
    spi_i2s_data_receive(SPIx);
}
#endif
#if SPI0_MISO_ENABLE
uint8_t SPI0_read() {
    while(RESET == spi_i2s_flag_get(SPIx, SPI_FLAG_TBE));
    //硬件电路，发送数据，放到寄存器
    spi_i2s_data_transmit(SPIx, 0x00);

    while(RESET == spi_i2s_flag_get(SPIx, SPI_FLAG_RBNE));
    //硬件电路，发送数据，放到寄存器
    return spi_i2s_data_receive(SPIx);
}
#endif

#if SPI0_MOSI_ENABLE && SPI0_MISO_ENABLE
uint8_t SPI0_write_read(uint8_t dat) {
    while(RESET == spi_i2s_flag_get(SPIx, SPI_FLAG_TBE));
    //硬件电路，发送数据，放到寄存器
    spi_i2s_data_transmit(SPIx, dat);

    while(RESET == spi_i2s_flag_get(SPIx, SPI_FLAG_RBNE));
    //硬件电路，发送数据，放到寄存器
    return spi_i2s_data_receive(SPIx);
}
#endif

#endif

#endif