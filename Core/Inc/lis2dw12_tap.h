/*
 * lis2dw12_tap.h
 *
 *  Created on: Mar 15, 2020
 *      Author: piotr
 */

#ifndef INC_LIS2DW12_TAP_H_
#define INC_LIS2DW12_TAP_H_

#include "lis2dw12_reg.h"
#include "lis2dw12_add.h"

extern stmdev_ctx_t dev_ctx;
extern uint8_t whoamI, rst;
extern uint8_t tx_buffer[1000];

void lis2dw12_tap_init(void);
void lis2dw12_tap_check(void);

#endif /* INC_LIS2DW12_TAP_H_ */
