/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_adxl362_fifo.h
 * @brief     driver adxl362 fifo header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-02-28
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/02/28  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_ADXL362_FIFO_H
#define DRIVER_ADXL362_FIFO_H

#include "driver_adxl362_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup adxl362_example_driver
 * @{
 */

/**
 * @brief adxl362 fifo example default definition
 */
#define ADXL362_FIFO_DEFAULT_FIFO_TEMPERATURE           ADXL362_BOOL_FALSE                   /**<disable fifo temperature */
#define ADXL362_FIFO_DEFAULT_FIFO_MODE                  ADXL362_FIFO_MODE_STREAM             /**< stream mode */
#define ADXL362_FIFO_DEFAULT_FIFO_SAMPLE                128                                  /**< 128 */
#define ADXL362_FIFO_DEFAULT_INTERRUPT_PIN_LEVEL        ADXL362_INTERRUPT_PIN_LOW            /**< activity low */
#define ADXL362_FIFO_DEFAULT_BANDWIDTH_ODR              ADXL362_BANDWIDTH_ODR_DIV_2          /**< bandwidth odr 1/2 */
#define ADXL362_FIFO_DEFAULT_NOISE_MODE                 ADXL362_NOISE_MODE_NORMAL            /**< normal noise mode */
#define ADXL362_FIFO_DEFAULT_ODR                        ADXL362_ODR_12P5HZ                   /**< 12.5Hz */
#define ADXL362_FIFO_DEFAULT_RANGE                      ADXL362_RANGE_2G                     /**< 2g */

/**
 * @brief  fifo irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl362_fifo_irq_handler(void);

/**
 * @brief     fifo example init
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t adxl362_fifo_init(void (*callback)(adxl362_frame_t *frame, uint16_t frame_len));

/**
 * @brief  fifo example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adxl362_fifo_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
