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
 * @file      driver_adxl362_register_test.c
 * @brief     driver adxl362 register test source file
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

#include "driver_adxl362_register_test.h"
#include <stdlib.h>

static adxl362_handle_t gs_handle;        /**< adxl362 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t adxl362_register_test(void)
{
    uint8_t res;
    uint8_t id;
    uint8_t status;
    uint8_t tim_short;
    uint8_t tim_short_check;
    uint16_t tim;
    uint16_t tim_check;
    uint16_t reg;
    uint16_t counter;
    uint16_t sample;
    uint16_t sample_check;
    uint16_t threshold;
    uint16_t threshold_check;
    float g;
    float g_check;
    float ms;
    float ms_check;
    adxl362_bool_t enable;
    adxl362_info_t info;
    adxl362_fifo_mode_t fifo_mode;
    adxl362_interrupt_pin_level_t level;
    adxl362_range_t range;
    adxl362_bandwidth_t bandwidth;
    adxl362_odr_t odr;
    adxl362_noise_mode_t noise_mode;
    adxl362_mode_t mode;
    adxl362_detect_mode_t detect_mode;
    adxl362_detect_trigger_t trigger;

    /* link interface function */
    DRIVER_ADXL362_LINK_INIT(&gs_handle, adxl362_handle_t);
    DRIVER_ADXL362_LINK_SPI_INIT(&gs_handle, adxl362_interface_spi_init);
    DRIVER_ADXL362_LINK_SPI_DEINIT(&gs_handle, adxl362_interface_spi_deinit);
    DRIVER_ADXL362_LINK_SPI_READ(&gs_handle, adxl362_interface_spi_read);
    DRIVER_ADXL362_LINK_SPI_READ_ADDRESS16(&gs_handle, adxl362_interface_spi_read_address16);
    DRIVER_ADXL362_LINK_SPI_WRITE_ADDRESS16(&gs_handle, adxl362_interface_spi_write_address16);
    DRIVER_ADXL362_LINK_DELAY_MS(&gs_handle, adxl362_interface_delay_ms);
    DRIVER_ADXL362_LINK_DEBUG_PRINT(&gs_handle, adxl362_interface_debug_print);
    DRIVER_ADXL362_LINK_RECEIVE_CALLBACK(&gs_handle, adxl362_interface_receive_callback);
    
    /* get information */
    res = adxl362_info(&info);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        adxl362_interface_debug_print("adxl362: chip is %s.\n", info.chip_name);
        adxl362_interface_debug_print("adxl362: manufacturer is %s.\n", info.manufacturer_name);
        adxl362_interface_debug_print("adxl362: interface is %s.\n", info.interface);
        adxl362_interface_debug_print("adxl362: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        adxl362_interface_debug_print("adxl362: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        adxl362_interface_debug_print("adxl362: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        adxl362_interface_debug_print("adxl362: max current is %0.2fmA.\n", info.max_current_ma);
        adxl362_interface_debug_print("adxl362: max temperature is %0.1fC.\n", info.temperature_max);
        adxl362_interface_debug_print("adxl362: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    adxl362_interface_debug_print("adxl362: start register test.\n");
    
    /* init the adxl362 */
    res = adxl362_init(&gs_handle);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: init failed.\n");
       
        return 1;
    }
    
    /* adxl362_get_revision test */
    adxl362_interface_debug_print("adxl362: adxl362_get_revision test.\n");
    
    /* get revision */
    res = adxl362_get_revision(&gs_handle, &id);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get revision failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: revision is 0x%02X.\n", id);
    
    /* adxl362_get_status test */
    adxl362_interface_debug_print("adxl362: adxl362_get_status test.\n");
    
    /* get status */
    res = adxl362_get_status(&gs_handle, &status);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get status failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: status is 0x%02X.\n", status);
    
    /* adxl362_get_fifo_counter test */
    adxl362_interface_debug_print("adxl362: adxl362_get_fifo_counter test.\n");
    
    /* get fifo counter */
    res = adxl362_get_fifo_counter(&gs_handle, &counter);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: fifo counter %d.\n", counter);
    
    /* adxl362_soft_reset test */
    adxl362_interface_debug_print("adxl362: adxl362_soft_reset test.\n");
    
    /* soft reset */
    res = adxl362_soft_reset(&gs_handle);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: soft reset failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check reset %s.\n", (res == 0) ? "ok" : "error");
    
    /* adxl362_set_fifo_temperature/adxl362_get_fifo_temperature test */
    adxl362_interface_debug_print("adxl362: adxl362_set_fifo_temperature/adxl362_get_fifo_temperature test.\n");
    
    /* enable fifo temperature */
    res = adxl362_set_fifo_temperature(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo temperature failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable fifo temperature */
    adxl362_interface_debug_print("adxl362: enable fifo temperature.\n");
    
    /* get fifo temperature */
    res = adxl362_get_fifo_temperature(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo temperature failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo temperature %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* disable fifo temperature */
    res = adxl362_set_fifo_temperature(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo temperature failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable fifo temperature */
    adxl362_interface_debug_print("adxl362: disable fifo temperature.\n");
    
    /* get fifo temperature */
    res = adxl362_get_fifo_temperature(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo temperature failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo temperature %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_fifo_temperature/adxl362_get_fifo_temperature test */
    adxl362_interface_debug_print("adxl362: adxl362_set_fifo_temperature/adxl362_get_fifo_temperature test.\n");
    
    /* set fifo triggered mode */
    res = adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_TRIGGERED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo triggered mode */
    adxl362_interface_debug_print("adxl362: set fifo triggered mode.\n");
    
    /* get fifo mode */
    res = adxl362_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo mode %s.\n", (fifo_mode == ADXL362_FIFO_MODE_TRIGGERED) ? "ok" : "error");
    
    /* set fifo stream mode */
    res = adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_STREAM);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fifo stream mode */
    adxl362_interface_debug_print("adxl362: set fifo stream mode.\n");
    
    /* get fifo mode */
    res = adxl362_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo mode %s.\n", (fifo_mode == ADXL362_FIFO_MODE_STREAM) ? "ok" : "error");
    
    /* set oldest saved mode */
    res = adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_OLDEST_SAVED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set oldest saved mode */
    adxl362_interface_debug_print("adxl362: set oldest saved mode.\n");
    
    /* get fifo mode */
    res = adxl362_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo mode %s.\n", (fifo_mode == ADXL362_FIFO_MODE_OLDEST_SAVED) ? "ok" : "error");
    
    /* set disable mode */
    res = adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_DISABLE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set disable mode */
    adxl362_interface_debug_print("adxl362: set disable mode.\n");
    
    /* get fifo mode */
    res = adxl362_get_fifo_mode(&gs_handle, &fifo_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo mode %s.\n", (fifo_mode == ADXL362_FIFO_MODE_DISABLE) ? "ok" : "error");
    
    /* adxl362_set_fifo_sample/adxl362_get_fifo_sample test */
    adxl362_interface_debug_print("adxl362: adxl362_set_fifo_sample/adxl362_get_fifo_sample test.\n");
    
    sample = rand() % 512;
    
    /* set fifo sample */
    res = adxl362_set_fifo_sample(&gs_handle, sample);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo sample failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set fifo sample %d.\n", sample);
    
    /* get fifo sample */
    res = adxl362_get_fifo_sample(&gs_handle, &sample_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get fifo sample failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check fifo sample %s.\n", (sample == sample_check) ? "ok" : "error");
    
    /* adxl362_set_interrupt_pin1_active_level/adxl362_get_interrupt_pin1_active_level test */
    adxl362_interface_debug_print("adxl362: adxl362_set_interrupt_pin1_active_level/adxl362_get_interrupt_pin1_active_level test.\n");
    
    /* high level */
    res = adxl362_set_interrupt_pin1_active_level(&gs_handle, ADXL362_INTERRUPT_PIN_HIGH);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 active level failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* high level */
    adxl362_interface_debug_print("adxl362: set interrupt pin1 active level high.\n");
    
    /* get interrupt pin1 active level */
    res = adxl362_get_interrupt_pin1_active_level(&gs_handle, &level);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 active level failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 active level %s.\n", (level == ADXL362_INTERRUPT_PIN_HIGH) ? "ok" : "error");
    
    /* low level */
    res = adxl362_set_interrupt_pin1_active_level(&gs_handle, ADXL362_INTERRUPT_PIN_LOW);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 active level failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    
    /* low level */
    adxl362_interface_debug_print("adxl362: set interrupt pin1 active level low.\n");
    
    /* get interrupt pin1 active level */
    res = adxl362_get_interrupt_pin1_active_level(&gs_handle, &level);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 active level failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 active level %s.\n", (level == ADXL362_INTERRUPT_PIN_LOW) ? "ok" : "error");
    
    /* adxl362_set_interrupt_pin1_map/adxl362_get_interrupt_pin1_map test */
    adxl362_interface_debug_print("adxl362: adxl362_set_interrupt_pin1_map/adxl362_get_interrupt_pin1_map test.\n");
    
    /* awake enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map awake enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* awake disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map awake disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* inact enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map inact enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* inact disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map inact disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* act enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map act enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* act disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map act disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* fifo overrun enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map fifo overrun enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* fifo overrun disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map fifo overrun disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* fifo watermark enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map fifo watermark enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* fifo watermark disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map fifo watermark disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* fifo ready enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map fifo ready enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* fifo ready disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map fifo ready disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* data ready enable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map data ready enable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* data ready disable */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 map data ready disable.\n");
    
    /* get interrupt pin1 map */
    res = adxl362_get_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_interrupt_pin2_map/adxl362_get_interrupt_pin2_map test */
    adxl362_interface_debug_print("adxl362: adxl362_set_interrupt_pin2_map/adxl362_get_interrupt_pin2_map test.\n");
    
    /* awake enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map awake enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* awake disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map awake disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_AWAKE, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* inact enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map inact enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* inact disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map inact disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* act enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map act enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* act disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map act disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* fifo overrun enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map fifo overrun enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* fifo overrun disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map fifo overrun disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* fifo watermark enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map fifo watermark enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* fifo watermark disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map fifo watermark disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* fifo ready enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map fifo ready enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* fifo ready disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map fifo ready disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* data ready enable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map data ready enable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* data ready disable */
    res = adxl362_set_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 map data ready disable.\n");
    
    /* get interrupt pin2 map */
    res = adxl362_get_interrupt_pin2_map(&gs_handle, ADXL362_INTERRUPT_MAP_DATA_READY, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 map failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 map %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_range/adxl362_get_range test */
    adxl362_interface_debug_print("adxl362: adxl362_set_range/adxl362_get_range test.\n");
    
    /* 2g */
    res = adxl362_set_range(&gs_handle, ADXL362_RANGE_2G);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set range 2g.\n");
    
    /* get range */
    res = adxl362_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check range %s.\n", (range == ADXL362_RANGE_2G) ? "ok" : "error");
    
    /* 4g */
    res = adxl362_set_range(&gs_handle, ADXL362_RANGE_4G);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set range 4g.\n");
    
    /* get range */
    res = adxl362_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check range %s.\n", (range == ADXL362_RANGE_4G) ? "ok" : "error");
    
    /* 8g */
    res = adxl362_set_range(&gs_handle, ADXL362_RANGE_8G);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set range 8g.\n");
    
    /* get range */
    res = adxl362_get_range(&gs_handle, &range);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get range failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check range %s.\n", (range == ADXL362_RANGE_8G) ? "ok" : "error");
    
    /* adxl362_set_bandwidth/adxl362_get_bandwidth test */
    adxl362_interface_debug_print("adxl362: adxl362_set_bandwidth/adxl362_get_bandwidth test.\n");
    
    /* 1/2 odr */
    res = adxl362_set_bandwidth(&gs_handle, ADXL362_BANDWIDTH_ODR_DIV_2);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set bandwidth failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set bandwidth 1/2 odr.\n");
    
    /* get bandwidth */
    res = adxl362_get_bandwidth(&gs_handle, &bandwidth);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get bandwidth failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check bandwidth %s.\n", (bandwidth == ADXL362_BANDWIDTH_ODR_DIV_2) ? "ok" : "error");
    
    /* 1/4 odr */
    res = adxl362_set_bandwidth(&gs_handle, ADXL362_BANDWIDTH_ODE_DIV_4);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set bandwidth failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set bandwidth 1/4 odr.\n");
    
    /* get bandwidth */
    res = adxl362_get_bandwidth(&gs_handle, &bandwidth);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get bandwidth failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check bandwidth %s.\n", (bandwidth == ADXL362_BANDWIDTH_ODE_DIV_4) ? "ok" : "error");
    
    /* adxl362_set_interrupt_pin2_as_external_sampling_trigger/adxl362_get_interrupt_pin2_as_external_sampling_trigger test */
    adxl362_interface_debug_print("adxl362: adxl362_set_interrupt_pin2_as_external_sampling_trigger/adxl362_get_interrupt_pin2_as_external_sampling_trigger test.\n");
    
    /* enable */
    res = adxl362_set_interrupt_pin2_as_external_sampling_trigger(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 as external sampling trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 as external sampling trigger enable.\n");
    
    /* get interrupt pin2 as external sampling trigger */
    res = adxl362_get_interrupt_pin2_as_external_sampling_trigger(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 as external sampling trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 as external sampling trigger %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = adxl362_set_interrupt_pin2_as_external_sampling_trigger(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin2 as external sampling trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin2 as external sampling trigger disable.\n");
    
    /* get interrupt pin2 as external sampling trigger */
    res = adxl362_get_interrupt_pin2_as_external_sampling_trigger(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin2 as external sampling trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin2 as external sampling trigger %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_odr/adxl362_get_odr test */
    adxl362_interface_debug_print("adxl362: adxl362_set_odr/adxl362_get_odr test.\n");
    
    /* 12.5Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_12P5HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set odr 12.5Hz.\n");
    
    /* get odr */
    res = adxl362_get_odr(&gs_handle, &odr);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check odr %s.\n", (odr == ADXL362_ODR_12P5HZ) ? "ok" : "error");
    
    /* 25Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_25HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set odr 25Hz.\n");
    
    /* get odr */
    res = adxl362_get_odr(&gs_handle, &odr);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check odr %s.\n", (odr == ADXL362_ODR_25HZ) ? "ok" : "error");
    
    /* 50Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_50HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set odr 50Hz.\n");
    
    /* get odr */
    res = adxl362_get_odr(&gs_handle, &odr);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check odr %s.\n", (odr == ADXL362_ODR_50HZ) ? "ok" : "error");
    
    /* 100Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_100HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set odr 100Hz.\n");
    
    /* get odr */
    res = adxl362_get_odr(&gs_handle, &odr);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check odr %s.\n", (odr == ADXL362_ODR_100HZ) ? "ok" : "error");
    
    /* 200Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_200HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set odr 200Hz.\n");
    
    /* get odr */
    res = adxl362_get_odr(&gs_handle, &odr);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check odr %s.\n", (odr == ADXL362_ODR_200HZ) ? "ok" : "error");
    
    /* 400Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_400HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set odr 400Hz.\n");
    
    /* get odr */
    res = adxl362_get_odr(&gs_handle, &odr);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get odr failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check odr %s.\n", (odr == ADXL362_ODR_400HZ) ? "ok" : "error");
    
    /* adxl362_set_interrupt_pin1_as_external_clock/adxl362_get_interrupt_pin1_as_external_clock test */
    adxl362_interface_debug_print("adxl362: adxl362_set_interrupt_pin1_as_external_clock/adxl362_get_interrupt_pin1_as_external_clock test.\n");
    
    /* enable */
    res = adxl362_set_interrupt_pin1_as_external_clock(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 as external clock failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 as external clock enable.\n");
    
    /* get interrupt pin1 as external clock */
    res = adxl362_get_interrupt_pin1_as_external_clock(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 as external clock failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 as external clock %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = adxl362_set_interrupt_pin1_as_external_clock(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 as external clock failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set interrupt pin1 as external clock disable.\n");
    
    /* get interrupt pin1 as external clock */
    res = adxl362_get_interrupt_pin1_as_external_clock(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get interrupt pin1 as external clock failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check interrupt pin1 as external clock %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_noise_mode/adxl362_get_noise_mode test */
    adxl362_interface_debug_print("adxl362: adxl362_set_noise_mode/adxl362_get_noise_mode test.\n");
    
    /* set noise mode */
    res = adxl362_set_noise_mode(&gs_handle, ADXL362_NOISE_MODE_NORMAL);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set noise mode normal.\n");
    
    /* get noise mode */
    res = adxl362_get_noise_mode(&gs_handle, &noise_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check noise mode %s.\n", (noise_mode == ADXL362_NOISE_MODE_NORMAL) ? "ok" : "error");
    
    /* set low noise mode */
    res = adxl362_set_noise_mode(&gs_handle, ADXL362_NOISE_MODE_LOW_NOISE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set noise mode low noise.\n");
    
    /* get noise mode */
    res = adxl362_get_noise_mode(&gs_handle, &noise_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check noise mode %s.\n", (noise_mode == ADXL362_NOISE_MODE_LOW_NOISE) ? "ok" : "error");
    
    /* set ultra low noise mode */
    res = adxl362_set_noise_mode(&gs_handle, ADXL362_NOISE_MODE_ULTRA_LOW_NOISE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set noise mode ultra low noise.\n");
    
    /* get noise mode */
    res = adxl362_get_noise_mode(&gs_handle, &noise_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get noise mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check noise mode %s.\n", (noise_mode == ADXL362_NOISE_MODE_ULTRA_LOW_NOISE) ? "ok" : "error");
    
    /* adxl362_set_wake_up/adxl362_get_wake_up test */
    adxl362_interface_debug_print("adxl362: adxl362_set_wake_up/adxl362_get_wake_up test.\n");
    
    /* enable */
    res = adxl362_set_wake_up(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set wake up failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set wake up enable.\n");
    
    /* get wake up */
    res = adxl362_get_wake_up(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get wake up failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check wake up %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = adxl362_set_wake_up(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set wake up failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set wake up disable.\n");
    
    /* get wake up */
    res = adxl362_get_wake_up(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get wake up failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check wake up %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_auto_sleep/adxl362_get_auto_sleep test */
    adxl362_interface_debug_print("adxl362: adxl362_set_auto_sleep/adxl362_get_auto_sleep test.\n");
    
    /* enable */
    res = adxl362_set_auto_sleep(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set auto sleep failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set auto sleep enable.\n");
    
    /* get auto sleep */
    res = adxl362_get_auto_sleep(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get auto sleep failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check sleep %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = adxl362_set_auto_sleep(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set auto sleep failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set auto sleep disable.\n");
    
    /* get auto sleep */
    res = adxl362_get_auto_sleep(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get auto sleep failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check sleep %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* adxl362_set_mode/adxl362_get_mode test */
    adxl362_interface_debug_print("adxl362: adxl362_set_mode/adxl362_get_mode test.\n");
    
    /* measurement */
    res = adxl362_set_mode(&gs_handle, ADXL362_MODE_MEASUREMENT);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set mode measurement.\n");
    
    /* get mode */
    res = adxl362_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check mode %s.\n", (mode == ADXL362_MODE_MEASUREMENT) ? "ok" : "error");
    
    /* standby */
    res = adxl362_set_mode(&gs_handle, ADXL362_MODE_STANDBY);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set mode standby.\n");
    
    /* get mode */
    res = adxl362_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check mode %s.\n", (mode == ADXL362_MODE_STANDBY) ? "ok" : "error");
    
    /* adxl362_set_activity_threshold/adxl362_get_activity_threshold test */
    adxl362_interface_debug_print("adxl362: adxl362_set_activity_threshold/adxl362_get_activity_threshold test.\n");
    
    /* set activity threshold */
    threshold = rand() % 0x7FF;
    res = adxl362_set_activity_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set activity threshold 0x%04X.\n", threshold);
    
    /* get activity threshold */
    res = adxl362_get_activity_threshold(&gs_handle, &threshold_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get activity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity threshold %s.\n", (threshold_check == threshold) ? "ok" : "error");
    
    /* adxl362_activity_threshold_convert_to_register/adxl362_activity_threshold_convert_to_data test */
    adxl362_interface_debug_print("adxl362: adxl362_activity_threshold_convert_to_register/adxl362_activity_threshold_convert_to_data test.\n");
    
    /* activity threshold convert to register */
    g = (float)(rand() % 100) / 1000.0f;
    res = adxl362_activity_threshold_convert_to_register(&gs_handle, g, &reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: activity threshold convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: activity threshold convert to register %0.2fg.\n", g);
    
    /* activity threshold convert to data */
    res = adxl362_activity_threshold_convert_to_data(&gs_handle, reg, &g_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: activity threshold convert to data failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity threshold %0.2fg.\n", g_check);
    
    /* adxl362_set_activity_time/adxl362_get_activity_time test */
    adxl362_interface_debug_print("adxl362: adxl362_set_activity_time/adxl362_get_activity_time test.\n");
    
    /* set activity time */
    tim_short = rand() % 256;
    res = adxl362_set_activity_time(&gs_handle, tim_short);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity time failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set activity time 0x%02X.\n", tim_short);
    
    /* get activity time */
    res = adxl362_get_activity_time(&gs_handle, &tim_short_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get activity time failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity time %s.\n", (tim_short_check == tim_short) ? "ok" : "error");
    
    /* adxl362_activity_time_convert_to_register/adxl362_activity_time_convert_to_data test */
    adxl362_interface_debug_print("adxl362: adxl362_activity_time_convert_to_register/adxl362_activity_time_convert_to_data test.\n");
    
    /* activity time convert to register */
    ms = (float)(rand() % 1000) / 10.0f;
    res = adxl362_activity_time_convert_to_register(&gs_handle, ms, &status);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: activity time convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: activity time convert to register %0.2fms.\n", ms);
    
    /* activity time convert to data */
    res = adxl362_activity_time_convert_to_data(&gs_handle, status, &ms_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: activity time convert to data failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity time %0.2fms.\n", ms_check);
    
    /* adxl362_set_inactivity_threshold/adxl362_get_inactivity_threshold test */
    adxl362_interface_debug_print("adxl362: adxl362_set_inactivity_threshold/adxl362_get_inactivity_threshold test.\n");
    
    /* set inactivity threshold */
    threshold = rand() % 0x3FF;
    res = adxl362_set_inactivity_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set inactivity threshold 0x%04X.\n", threshold);
    
    /* get inactivity threshold */
    res = adxl362_get_inactivity_threshold(&gs_handle, &threshold_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get inactivity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity threshold %s.\n", (threshold_check == threshold) ? "ok" : "error");
    
    /* adxl362_inactivity_threshold_convert_to_register/adxl362_inactivity_threshold_convert_to_data test */
    adxl362_interface_debug_print("adxl362: adxl362_inactivity_threshold_convert_to_register/adxl362_inactivity_threshold_convert_to_data test.\n");
    
    /* inactivity threshold convert to register */
    g = (float)(rand() % 100) / 1000.0f;
    res = adxl362_inactivity_threshold_convert_to_register(&gs_handle, g, &reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: inactivity threshold convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: inactivity threshold convert to register %0.2fg.\n", g);
    
    /* inactivity threshold convert to data */
    res = adxl362_inactivity_threshold_convert_to_data(&gs_handle, reg, &g_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: inactivity threshold convert to data failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity threshold %0.2fg.\n", g_check);
    
    /* adxl362_set_inactivity_time/adxl362_get_inactivity_time test */
    adxl362_interface_debug_print("adxl362: adxl362_set_inactivity_time/adxl362_get_inactivity_time test.\n");
    
    /* set inactivity time */
    tim = rand() % 0xFFFFL;
    res = adxl362_set_inactivity_time(&gs_handle, tim);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity time failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set inactivity time 0x%04X.\n", tim);
    
    /* get inactivity time */
    res = adxl362_get_inactivity_time(&gs_handle, &tim_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get inactivity time failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity time %s.\n", (tim_check == tim) ? "ok" : "error");
    
    /* adxl362_inactivity_time_convert_to_register/adxl362_inactivity_time_convert_to_data test */
    adxl362_interface_debug_print("adxl362: adxl362_inactivity_time_convert_to_register/adxl362_inactivity_time_convert_to_data test.\n");
    
    /* inactivity time convert to register */
    ms = (float)(rand() % 10000) / 10.0f;
    res = adxl362_inactivity_time_convert_to_register(&gs_handle, ms, &reg);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: inactivity time convert to register failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: inactivity time convert to register %0.2fms.\n", ms);
    
    /* inactivity time convert to data */
    res = adxl362_inactivity_time_convert_to_data(&gs_handle, reg, &ms_check);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: inactivity time convert to data failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
     adxl362_interface_debug_print("adxl362: check inactivity time %0.2fms.\n", ms_check);
    
    /* adxl362_set_detect_mode/adxl362_get_detect_mode test */
    adxl362_interface_debug_print("adxl362: adxl362_set_detect_mode/adxl362_get_detect_mode test.\n");
    
    /* both */
    res = adxl362_set_detect_mode(&gs_handle, ADXL362_DETECT_MODE_BOTH);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set detect mode both.\n");
    
    /* get detect mode */
    res = adxl362_get_detect_mode(&gs_handle, &detect_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check detect mode %s.\n", (detect_mode == ADXL362_DETECT_MODE_BOTH) ? "ok" : "error");
    
    /* link */
    res = adxl362_set_detect_mode(&gs_handle, ADXL362_DETECT_MODE_LINK);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set detect mode link.\n");
    
    /* get detect mode */
    res = adxl362_get_detect_mode(&gs_handle, &detect_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check detect mode %s.\n", (detect_mode == ADXL362_DETECT_MODE_LINK) ? "ok" : "error");
    
    /* loop */
    res = adxl362_set_detect_mode(&gs_handle, ADXL362_DETECT_MODE_LOOP);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set detect mode loop.\n");
    
    /* get detect mode */
    res = adxl362_get_detect_mode(&gs_handle, &detect_mode);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check detect mode %s.\n", (detect_mode == ADXL362_DETECT_MODE_LOOP) ? "ok" : "error");
    
    /* adxl362_set_inactivity_detect_trigger/adxl362_get_inactivity_detect_trigger test */
    adxl362_interface_debug_print("adxl362: adxl362_set_inactivity_detect_trigger/adxl362_get_inactivity_detect_trigger test.\n");
    
    /* absolute mode */
    res = adxl362_set_inactivity_detect_trigger(&gs_handle, ADXL362_DETECT_TRIGGER_ABSOLUTE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set inactivity detect trigger absolute mode.\n");
    
    /* get inactivity detect trigger */
    res = adxl362_get_inactivity_detect_trigger(&gs_handle, &trigger);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get inactivity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity detect trigger %s.\n", (trigger == ADXL362_DETECT_TRIGGER_ABSOLUTE) ? "ok" : "error");
    
    /* referenced mode */
    res = adxl362_set_inactivity_detect_trigger(&gs_handle, ADXL362_DETECT_TRIGGER_REFERENCED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set inactivity detect trigger referenced mode.\n");
    
    /* get inactivity detect trigger */
    res = adxl362_get_inactivity_detect_trigger(&gs_handle, &trigger);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get inactivity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity detect trigger %s.\n", (trigger == ADXL362_DETECT_TRIGGER_REFERENCED) ? "ok" : "error");
    
    /* adxl362_set_activity_detect_trigger/adxl362_get_activity_detect_trigger test */
    adxl362_interface_debug_print("adxl362: adxl362_set_activity_detect_trigger/adxl362_get_activity_detect_trigger test.\n");
    
    /* absolute mode */
    res = adxl362_set_activity_detect_trigger(&gs_handle, ADXL362_DETECT_TRIGGER_ABSOLUTE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set activity detect trigger absolute mode.\n");
    
    /* get activity detect trigger */
    res = adxl362_get_activity_detect_trigger(&gs_handle, &trigger);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get activity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity detect trigger %s.\n", (trigger == ADXL362_DETECT_TRIGGER_ABSOLUTE) ? "ok" : "error");
    
    /* referenced mode */
    res = adxl362_set_activity_detect_trigger(&gs_handle, ADXL362_DETECT_TRIGGER_REFERENCED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set activity detect trigger referenced mode.\n");
    
    /* get activity detect trigger */
    res = adxl362_get_activity_detect_trigger(&gs_handle, &trigger);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get activity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity detect trigger %s.\n", (trigger == ADXL362_DETECT_TRIGGER_REFERENCED) ? "ok" : "error");
    
    /* adxl362_set_inactivity/adxl362_get_inactivity test */
    adxl362_interface_debug_print("adxl362: adxl362_set_inactivity/adxl362_get_inactivity test.\n");
    
    /* disable */
    res = adxl362_set_inactivity(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set inactivity disable.\n");
    
    /* get inactivity */
    res = adxl362_get_inactivity(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get inactivity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* enable */
    res = adxl362_set_inactivity(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set inactivity enable.\n");
    
    /* get inactivity */
    res = adxl362_get_inactivity(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get inactivity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check inactivity %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* adxl362_set_activity/adxl362_get_activity test */
    adxl362_interface_debug_print("adxl362: adxl362_set_activity/adxl362_get_activity test.\n");
    
    /* disable */
    res = adxl362_set_activity(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set activity disable.\n");
    
    /* get activity */
    res = adxl362_get_activity(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get activity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* enable */
    res = adxl362_set_activity(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set activity enable.\n");
    
    /* get activity */
    res = adxl362_get_activity(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get activity failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check activity %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* adxl362_set_self_test/adxl362_get_self_test test */
    adxl362_interface_debug_print("adxl362: adxl362_set_self_test/adxl362_get_self_test test.\n");
    
    /* enable */
    res = adxl362_set_self_test(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set self test failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set self test enable.\n");
    
    /* get self test */
    res = adxl362_get_self_test(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get self test failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check self test %s.\n", (enable == ADXL362_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = adxl362_set_self_test(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set self test failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: set self test disable.\n");
    
    /* get self test */
    res = adxl362_get_self_test(&gs_handle, &enable);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: get self test failed.\n");
        (void)adxl362_deinit(&gs_handle);
        
        return 1;
    }
    adxl362_interface_debug_print("adxl362: check self test %s.\n", (enable == ADXL362_BOOL_FALSE) ? "ok" : "error");
    
    /* finish register test */
    adxl362_interface_debug_print("adxl362: finish register test.\n");
    (void)adxl362_deinit(&gs_handle);
    
    return 0;
}
