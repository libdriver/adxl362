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
 * @file      driver_adxl362_motion.c
 * @brief     driver adxl362 motion source file
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

#include "driver_adxl362_motion.h"

static adxl362_handle_t gs_handle;        /**< adxl362 handle */

/**
 * @brief  motion irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl362_motion_irq_handler(void)
{
    if (adxl362_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     motion example init
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t adxl362_motion_init(void (*callback)(uint8_t type))
{
    uint8_t res;
    uint8_t reg_short;
    uint16_t reg;
    
    /* link interface function */
    DRIVER_ADXL362_LINK_INIT(&gs_handle, adxl362_handle_t);
    DRIVER_ADXL362_LINK_SPI_INIT(&gs_handle, adxl362_interface_spi_init);
    DRIVER_ADXL362_LINK_SPI_DEINIT(&gs_handle, adxl362_interface_spi_deinit);
    DRIVER_ADXL362_LINK_SPI_READ(&gs_handle, adxl362_interface_spi_read);
    DRIVER_ADXL362_LINK_SPI_READ_ADDRESS16(&gs_handle, adxl362_interface_spi_read_address16);
    DRIVER_ADXL362_LINK_SPI_WRITE_ADDRESS16(&gs_handle, adxl362_interface_spi_write_address16);
    DRIVER_ADXL362_LINK_DELAY_MS(&gs_handle, adxl362_interface_delay_ms);
    DRIVER_ADXL362_LINK_DEBUG_PRINT(&gs_handle, adxl362_interface_debug_print);
    DRIVER_ADXL362_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
    /* init the adxl362 */
    res = adxl362_init(&gs_handle);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: init failed.\n");
       
        return 1;
    }
    
    /* disable fifo temperature */
    res = adxl362_set_fifo_temperature(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo temperature failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo */
    res = adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_DISABLE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo sample 0 */
    res = adxl362_set_fifo_sample(&gs_handle, 0);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo sample failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt pin1 default active level */
    res = adxl362_set_interrupt_pin1_active_level(&gs_handle, ADXL362_MOTION_DEFAULT_INTERRUPT_PIN_LEVEL);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 active level failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable awake map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable inact map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable act map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo overrun map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo watermark map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo ready map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable data ready map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt pin2 active level low */
    res = adxl362_set_interrupt_pin2_active_level(&gs_handle, ADXL362_INTERRUPT_PIN_LOW);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 active level failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable awake map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable inact map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable act map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo overrun map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo watermark map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo ready map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable data ready map */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default bandwidth */
    res = adxl362_set_bandwidth(&gs_handle, ADXL362_MOTION_DEFAULT_BANDWIDTH_ODR);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set bandwidth failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default noise mode */
    res = adxl362_set_noise_mode(&gs_handle, ADXL362_MOTION_DEFAULT_NOISE_MODE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable wake up */
    res = adxl362_set_wake_up(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set wake up failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable auto sleep */
    res = adxl362_set_auto_sleep(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set auto sleep failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable interrupt pin2 as external sampling trigger */
    res = adxl362_set_interrupt_pin2_as_external_sampling_trigger(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 as external sampling trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable interrupt pin1 as external clock */
    res = adxl362_set_interrupt_pin1_as_external_clock(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 as external clock failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default odr */
    res = adxl362_set_odr(&gs_handle, ADXL362_MOTION_DEFAULT_ODR);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default range */
    res = adxl362_set_range(&gs_handle, ADXL362_MOTION_DEFAULT_RANGE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default activity threshold */
    res = adxl362_activity_threshold_convert_to_register(&gs_handle, ADXL362_MOTION_DEFAULT_ACTIVITY_THRESHOLD, &reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: activity threshold convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set activity threshold */
    res = adxl362_set_activity_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default activity time */
    res = adxl362_activity_time_convert_to_register(&gs_handle, ADXL362_MOTION_DEFAULT_ACTIVITY_TIME, &reg_short);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: activity time convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set activity time */
    res = adxl362_set_activity_time(&gs_handle, reg_short);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity time failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inactivity threshold */
    res = adxl362_inactivity_threshold_convert_to_register(&gs_handle, ADXL362_MOTION_DEFAULT_INACTIVITY_THRESHOLD, &reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: inactivity threshold convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inactivity threshold */
    res = adxl362_set_inactivity_threshold(&gs_handle, reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inactivity time */
    res = adxl362_inactivity_time_convert_to_register(&gs_handle, ADXL362_MOTION_DEFAULT_INACTIVITY_TIME, &reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: inactivity time convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set inactivity time */
    res = adxl362_set_inactivity_time(&gs_handle, reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity time failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default detect mode */
    res = adxl362_set_detect_mode(&gs_handle, ADXL362_MOTION_DEFAULT_DETECT_MODE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default inactivity detect trigger */
    res = adxl362_set_inactivity_detect_trigger(&gs_handle, ADXL362_MOTION_DEFAULT_INACTIVITY_DETECT_TRIGGER);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default activity detect trigger */
    res = adxl362_set_activity_detect_trigger(&gs_handle, ADXL362_MOTION_DEFAULT_ACTIVITY_DETECT_TRIGGER);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable inactivity */
    res = adxl362_set_inactivity(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable activity */
    res = adxl362_set_activity(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable self test */
    res = adxl362_set_self_test(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set self test failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start measurement */
    res = adxl362_set_mode(&gs_handle, ADXL362_MODE_MEASUREMENT);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  motion example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t adxl362_motion_deinit(void)
{
    /* deinit */
    if (adxl362_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}
