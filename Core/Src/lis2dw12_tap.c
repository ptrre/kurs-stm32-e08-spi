#include "lis2dw12_tap.h"

static lis2dw12_reg_t int_route;

void lis2dw12_tap_init(void)
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
	     * Set full scale
	     */
	    lis2dw12_full_scale_set(&dev_ctx, LIS2DW12_2g);

	    /*
	     * Configure power mode
	     */
	    lis2dw12_power_mode_set(&dev_ctx, LIS2DW12_CONT_LOW_PWR_LOW_NOISE_12bit);

	    /*
	     * Set Output Data Rate
	     */
	    lis2dw12_data_rate_set(&dev_ctx, LIS2DW12_XL_ODR_400Hz);

	    /*
	     * Enable Tap detection on X, Y, Z
	     */
	    lis2dw12_tap_detection_on_z_set(&dev_ctx, PROPERTY_ENABLE);
	    lis2dw12_tap_detection_on_y_set(&dev_ctx, PROPERTY_ENABLE);
	    lis2dw12_tap_detection_on_x_set(&dev_ctx, PROPERTY_ENABLE);

	    /*
	     * Set Tap threshold on all axis
	     */
	    lis2dw12_tap_threshold_x_set(&dev_ctx, 12);
	    lis2dw12_tap_threshold_y_set(&dev_ctx, 12);
	    lis2dw12_tap_threshold_z_set(&dev_ctx, 12);

	    /*
	     * Configure Double Tap parameter
	     */
	    lis2dw12_tap_dur_set(&dev_ctx, 7);
	    lis2dw12_tap_quiet_set(&dev_ctx, 3);
	    lis2dw12_tap_shock_set(&dev_ctx, 3);

	    /*
	     * Enable Double Tap detection
	     */
	    lis2dw12_tap_mode_set(&dev_ctx, LIS2DW12_BOTH_SINGLE_DOUBLE);

	    /*
	     * Enable single tap detection interrupt
	     */
	    lis2dw12_pin_int1_route_get(&dev_ctx, &int_route.ctrl4_int1_pad_ctrl);
	    int_route.ctrl4_int1_pad_ctrl.int1_tap = PROPERTY_ENABLE;
	    lis2dw12_pin_int1_route_set(&dev_ctx, &int_route.ctrl4_int1_pad_ctrl);

	    sprintf((char*)tx_buffer, "Inicjalizacja OK\n");
	    tx_com(tx_buffer, strlen((char const*)tx_buffer));
}

void lis2dw12_tap_check(void)
{
	lis2dw12_all_sources_t all_source;

	    lis2dw12_all_sources_get(&dev_ctx, &all_source);


	    /*
	     * Check Single Tap events
	     */
	    if (all_source.tap_src.single_tap)
	    {
	      sprintf((char*)tx_buffer, "Tap Detected: Sign %s",
	              all_source.tap_src.tap_sign ? "positive" : "negative");
	      if (all_source.tap_src.x_tap)
	        sprintf((char*)tx_buffer, "%s on X", tx_buffer);
	      if (all_source.tap_src.y_tap)
	        sprintf((char*)tx_buffer, "%s on Y", tx_buffer);
	      if (all_source.tap_src.z_tap)
	        sprintf((char*)tx_buffer, "%s on Z", tx_buffer);
	      sprintf((char*)tx_buffer, "%s axis\r\n", tx_buffer);
	      tx_com(tx_buffer, strlen((char const*)tx_buffer));
	    }
}
