#include "osapi.h"
#include "user_interface.h"

#include "user_config.h"

#include "ets_sys.h"
#include "gpio.h"
#include "os_type.h"

// GPIO pin used configured by i2c_master.h
#include "driver/i2c_master.h"

static os_timer_t ptimer;
static os_timer_t ptimer_i2c_tx;
static os_timer_t ptimer_i2c_rx;

bool beginTransmission(uint8_t i2c_addr);
void endTransmission();

bool requestFrom(uint8_t i2c_addr);
bool read(uint8_t num_bytes, int8_t* data);
bool write(uint8_t i2c_data);

void sendHelloArduino();
void getRawData();

uint16 us_SECOND_ONE = 1000 * 1000 * 1;


void blink(void *arg)
{
  static uint8_t state = 0;

  if (state) {
    GPIO_OUTPUT_SET(2, 1);
  } else {
    GPIO_OUTPUT_SET(2, 0);
  }
  state ^= 1;
}

void receiveI2C(void *arg)
{
    int8_t packets[13] = {64,2,0,0,0,0,0,0,0,0,0,0,0};

    requestFrom(0x08);
    os_delay_us(1000);
    read(13, packets);
    endTransmission();

    for (int i=0; i< 13; i++)
    {
        char test_c = (char)packets[i];
        os_printf("%c", test_c);
    }
    os_printf("\r\n");

}

void sendHelloArduino(void *arg)
{
    char ascii_str[] = "Hello Arduino!";
    int len = strlen(ascii_str);

    beginTransmission(0x08);

    for(int i = 0; i < len ; i++)
    {
        int a_as_int = (int)ascii_str[i];
        write(a_as_int);
    }

    endTransmission();
    os_delay_us(100);

}

bool requestFrom(uint8_t i2c_addr)
{
    uint8_t i2c_addr_read = (i2c_addr << 1) + 1;
    
    i2c_master_start();
    i2c_master_writeByte(i2c_addr_read);
    
    return i2c_master_checkAck();
}


void ICACHE_FLASH_ATTR user_init(void)
{
    // Disable WiFi
    wifi_set_opmode(NULL_MODE);
    
    uart_init(76800);
    os_printf("SDK version:%s\n", system_get_sdk_version());

    gpio_init();

    // uart_div_modify(0, UART_CLK_FREQ/115200);
    i2c_master_gpio_init();
    i2c_master_init();
    os_delay_us(1000);

    os_printf("init done\n" );

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    os_timer_disarm(&ptimer);
    os_timer_setfn(&ptimer, (os_timer_func_t *)blink, NULL);
    os_timer_arm(&ptimer, 1000, 1);

    os_timer_disarm(&ptimer_i2c_tx);
    os_timer_setfn(&ptimer_i2c_tx, (os_timer_func_t *)sendHelloArduino, NULL);
    os_timer_arm(&ptimer_i2c_tx, 100, 1);


    os_timer_disarm(&ptimer_i2c_rx);
    os_timer_setfn(&ptimer_i2c_rx, (os_timer_func_t *)receiveI2C, NULL);
    os_timer_arm(&ptimer_i2c_rx, 100, 1);

}


bool read(uint8_t num_bytes, int8_t* data)
{
    if (num_bytes < 1 || data == NULL) return false;

    int i;
    for(i = 0; i < num_bytes - 1; i++)
    {
        data[i] = i2c_master_readByte();
        i2c_master_send_ack();
    }
    // nack the final packet so that the slave releases SDA
    data[num_bytes - 1] = i2c_master_readByte();
    i2c_master_send_nack();

    return true;
}


bool write(uint8_t i2c_data)
{
    i2c_master_writeByte(i2c_data);
    
    return i2c_master_checkAck();
}


bool beginTransmission(uint8_t i2c_addr)
{
    uint8_t i2c_addr_write = (i2c_addr << 1);
    
    i2c_master_start();
    i2c_master_writeByte(i2c_addr_write);
    
    return i2c_master_checkAck();
}

void endTransmission()
{
    i2c_master_stop();
}

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }
    return rf_cal_sec;
}
