#include "stm32f1xx_hal.h"
#include "nrf24l01.h"
#include "config.h"

extern SPI_HandleTypeDef hspi1;

static void error() {
    while (true) {
        HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
        HAL_Delay(100);
    }
}

static const uint8_t rx_address[5] = {1, 2, 3, 4, 5};
static const uint8_t tx_address[5] = {1, 2, 3, 4, 6};

void example() {
    nrf24l01 nrf;
    uint32_t rx_data;

    {
        nrf24l01_config config;
        config.data_rate        = NRF_DATA_RATE_1MBPS;
        config.tx_power         = NRF_TX_PWR_0dBm;
        config.crc_width        = NRF_CRC_WIDTH_1B;
        config.addr_width       = NRF_ADDR_WIDTH_5;
        config.payload_length   = 4;    // maximum is 32 bytes
        config.retransmit_count = 15;   // maximum is 15 times
        config.retransmit_delay = 0x0F; // 4000us, LSB:250us
        config.rf_channel       = 0;
        config.rx_address       = rx_address;
        config.tx_address       = tx_address;
        config.rx_buffer        = (uint8_t*)&rx_data;

        config.spi         = SPI1;
        config.spi_timeout = 10; // milliseconds
        config.ce_port     = NRF_CE_GPIO_Port;
        config.ce_pin      = NRF_CE_Pin;
        config.csn_port    = NRF_CSN_GPIO_Port;
        config.csn_pin     = NRF_CSN_Pin;
        config.irq_port    = NRF_IRQ_GPIO_Port;
        config.irq_pin     = NRF_IRQ_Pin;

        nrf_init(&nrf, &config);
    }

    uint32_t tx_data = 0xDEADBEEF;

    nrf_send_packet(&nrf, (uint8_t*)&tx_data);

    while (true) {
        nrf_receive_packet(&nrf);
        if (tx_data != rx_data) error();
        HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
    }
}
