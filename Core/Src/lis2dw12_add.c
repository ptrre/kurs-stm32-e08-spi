#include "lis2dw12_add.h"

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

static axis3bit16_t data_raw_acceleration;
static float acceleration_mg[3];

void lis2dw12_init(void)
{
  /*
   *  Initialize mems driver interface
   */

  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  dev_ctx.handle = &SENSOR_BUS;

  /*
   *  Check device ID
   */
  lis2dw12_device_id_get(&dev_ctx, &whoamI);
  if (whoamI != LIS2DW12_ID)
  {
      sprintf((char*)tx_buffer, "Blad. Nie wykryto akcelerometru! %d\n", whoamI);
      tx_com(tx_buffer, strlen((char const*)tx_buffer));
      while(1);
  }

  /*
   * Restore default configuration
   */
  lis2dw12_reset_set(&dev_ctx, PROPERTY_ENABLE);
  do {
    lis2dw12_reset_get(&dev_ctx, &rst);
  } while (rst);

  /*
   *  Enable Block Data Update
   */
  lis2dw12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

  /*
   * Set full scale
   */ 
  //lis2dw12_full_scale_set(&dev_ctx, LIS2DW12_8g);
  lis2dw12_full_scale_set(&dev_ctx, LIS2DW12_2g);

  /*
   * Configure filtering chain
   *
   * Accelerometer - filter path / bandwidth
   */ 
  lis2dw12_filter_path_set(&dev_ctx, LIS2DW12_LPF_ON_OUT);
  lis2dw12_filter_bandwidth_set(&dev_ctx, LIS2DW12_ODR_DIV_4);

  /*
   * Configure power mode
   */   
  lis2dw12_power_mode_set(&dev_ctx, LIS2DW12_HIGH_PERFORMANCE);
  //lis2dw12_power_mode_set(&dev_ctx, LIS2DW12_CONT_LOW_PWR_LOW_NOISE_12bit);

  /*
   * Set Output Data Rate
   */
  lis2dw12_data_rate_set(&dev_ctx, LIS2DW12_XL_ODR_100Hz);

  sprintf((char*)tx_buffer, "Inicjalizacja OK\n");
  tx_com(tx_buffer, strlen((char const*)tx_buffer));
}

void lis2dw12_read_data(void) {
	uint8_t reg;

	/*
	 * Read output only if new value is available
	 */
	lis2dw12_flag_data_ready_get(&dev_ctx, &reg);
	if (reg)
	{
		/*
		 * Read acceleration data
		 */
		memset(data_raw_acceleration.u8bit, 0x00, 3 * sizeof(int16_t));
	    lis2dw12_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
	    //acceleration_mg[0] = lis2dw12_from_fs8_lp1_to_mg(data_raw_acceleration.i16bit[0]);
	   // acceleration_mg[1] = lis2dw12_from_fs8_lp1_to_mg(data_raw_acceleration.i16bit[1]);
	    //acceleration_mg[2] = lis2dw12_from_fs8_lp1_to_mg(data_raw_acceleration.i16bit[2]);
	    acceleration_mg[0] = lis2dw12_from_fs2_to_mg(data_raw_acceleration.i16bit[0]);
	    acceleration_mg[1] = lis2dw12_from_fs2_to_mg(data_raw_acceleration.i16bit[1]);
	    acceleration_mg[2] = lis2dw12_from_fs2_to_mg(data_raw_acceleration.i16bit[2]);

	    sprintf((char*)tx_buffer, "Acc X, Y, Z [mg]: %4.2f %4.2f %4.2f\n",
	    		acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
	    tx_com(tx_buffer, strlen((char const*)tx_buffer));
	}
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp,
                              uint16_t len)
{
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &reg, 1, 1000);
    HAL_SPI_Transmit(handle, bufp, len, 1000);
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);

    return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
	/* Read command */
	reg |= 0x80;
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &reg, 1, 1000);
    HAL_SPI_Receive(handle, bufp, len, 1000);
    HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);

    return 0;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  tx_buffer     buffer to trasmit
 * @param  len           number of byte to send
 *
 */
void tx_com(uint8_t *tx_buffer, uint16_t len)
{
	HAL_UART_Transmit(&huart2, tx_buffer, len, 1000);
}

