#ifndef LIS2DW12_ADD_H_
#define LIS2DW12_ADD_H_
#include <string.h>
#include <stdio.h>
#include "stm32f3xx_hal.h"
#include "lis2dw12_reg.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"

#define SENSOR_BUS hspi2
#define CS_up_GPIO_Port LIS_SPI_GPIO_Port
#define CS_up_Pin LIS_SPI_Pin


stmdev_ctx_t dev_ctx;
uint8_t whoamI, rst;
uint8_t tx_buffer[1000];

int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp,
                              uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);
void tx_com( uint8_t *tx_buffer, uint16_t len );

void lis2dw12_init(void);
void lis2dw12_read_data(void);

#endif /* INC_LIS2DW12_ADD_H_ */
