//SPI algunas funciones
//voy a poner los drivers solo para que no me salga error
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/adc.h"


#define pin_num_mosi 23
#define pin_num_miso 19
#define pin_num_clk 18
#define pin_num_cs 5


//Funcion de configuración para el SPI para un sensor CP4132
static spi_device_handle_t spi_dev;


//funcion de configuración del bus SPI
void spi_bus_init() {
  spi_bus_config_t buscfg={
    .mosi_io_num=pin_num_mosi,
    .miso_io_num=pin_num_miso,
    .sclk_io_num=pin_num_clk,
    .quadwp_io_num=-1,
    .quadhd_io_num=-1,
    .max_transfer_sz=2
  };

spi_device_interface_config_t devcfg={
  .clock_speed_hz=1000000,           //Clock out at 1 MHz
  .mode=3,//porque cuando cae elreloj, que empieza en alto es que se toma la muestra
  .spics_io_num=pin_num_cs,
  .queue_size = 1 
};

spi_bus_initialize(SPI2_HOST,&buscfg, SPI_DMA_DISABLED);
spi_bus_add_device(SPI2_HOST, &devcfg, &spi_dev);
}

//Función para escribir registros
void mcp4132_read_register(uint8_t reg, uint8_t *data) {
  uint8_t buf[2];
  buf[0] = reg;
  buf[1] = 0;
  spi_transaction_t t = {
    .len = 16,
    .rx_buffer=buf,
  };
  spi_device_transmit(spi_dev, &t);
}

//funcion para leer registros
void mcp4132_write_register(uint8_t reg, uint8_t data) {
  uint8_t buf[2];
  buf[0] = reg;
  buf[1] = 0;
  spi_transaction_t t = {
    .len = 16,
    .tx_buffer = buf,
  };
  spi_device_transmit(spi_dev, &t);
}

//función mcp1432_set_cutoff_frequency configura LA RESISTENCIA
void mpc4132_set_wiper(uint8_t value) {

}


//PARTE DE UART y el ADC
void UartConfig(void) {
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_12_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_1, &uart_config);
  uart_set_pin(UART_NUM_1, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM_1, 1024 * 2, 0, 0, NULL, 0);
}

void adc (void) {
  // Configuración del ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // Configura el canal ADC1_CHANNEL_0 con atenuación de 11dB
}


void app_main() {
  spi_bus_init();
  UartConfig();
  adc();

  
}
