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
 * @file      driver_adxl362_fifo_test.c
 * @brief     driver adxl362 fifo test source file
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

#include "driver_adxl362_fifo_test.h"

static adxl362_handle_t gs_handle;        /**< adxl362 handle */
static adxl362_frame_t gs_frame[512];     /**< fifo frame */
volatile static uint8_t gs_flag = 0;      /**< fifo flag */

/**
 * @brief  fifo test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t adxl362_fifo_test_irq_handler(void)
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
 * @brief     interface receive callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_adxl362_interface_receive_callback(uint8_t type)
{
    switch (type)
    {
        case ADXL362_STATUS_ERR_USER_REGS :
        {
            adxl362_interface_debug_print("adxl362: irq seu error detect.\n");

            break;
        }
        case ADXL362_STATUS_AWAKE :
        {
            break;
        }
        case ADXL362_STATUS_INACT :
        {
            adxl362_interface_debug_print("adxl362: irq inactivity.\n");

            break;
        }
        case ADXL362_STATUS_ACT :
        {
            adxl362_interface_debug_print("adxl362: irq activity.\n");

            break;
        }
        case ADXL362_STATUS_FIFO_OVERRUN :
        {
            uint8_t res;
            uint16_t frame_len;

            /* total 512 */
            frame_len = 512;

            /* read fifo */
            res = adxl362_read_fifo(&gs_handle, gs_frame, &frame_len);
            if (res != 0)
            {
                return;
            }

            /* flag happened */
            gs_flag = 1;

            /* output */
            adxl362_interface_debug_print("adxl362: irq fifo overrun with %d.\n", frame_len);

            break;
        }
        case ADXL362_STATUS_FIFO_WATERMARK :
        {
            uint8_t res;
            uint16_t i;
            uint16_t frame_len;

            /* total 512 */
            frame_len = 512;

            /* read fifo */
            res = adxl362_read_fifo(&gs_handle, gs_frame, &frame_len);
            if (res != 0)
            {
                return;
            }

            /* flag happened */
            gs_flag = 1;

            /* output */
            adxl362_interface_debug_print("adxl362: irq fifo watermark with %d.\n", frame_len);

            /* output */
            for (i = 0; i < frame_len; i++)
            {
                if (gs_frame[i].type == ADXL362_FRAME_TYPE_X)
                {
                    adxl362_interface_debug_print("adxl362: x axis is %0.2fg.\n", gs_frame[i].data);
                }
                else if (gs_frame[i].type == ADXL362_FRAME_TYPE_Y)
                {
                    adxl362_interface_debug_print("adxl362: y axis is %0.2fg.\n", gs_frame[i].data);
                }
                else if (gs_frame[i].type == ADXL362_FRAME_TYPE_Z)
                {
                    adxl362_interface_debug_print("adxl362: z axis is %0.2fg.\n", gs_frame[i].data);
                }
                else
                {
                    adxl362_interface_debug_print("adxl362: temperature is %0.2fC.\n", gs_frame[i].data);
                }
            }

            break;
        }
        case ADXL362_STATUS_FIFO_READY :
        {
            break;
        }
        case ADXL362_STATUS_DATA_READY :
        {
            break;
        }
        default :
        {
            adxl362_interface_debug_print("adxl362: unknown code.\n");

            break;
        }
    }
}

/**
 * @brief     fifo test
 * @param[in] times is the set times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t adxl362_fifo_test(uint32_t times)
{
    uint8_t res;
    uint32_t t;
    adxl362_info_t info;

    /* link interface function */
    DRIVER_ADXL362_LINK_INIT(&gs_handle, adxl362_handle_t);
    DRIVER_ADXL362_LINK_SPI_INIT(&gs_handle, adxl362_interface_spi_init);
    DRIVER_ADXL362_LINK_SPI_DEINIT(&gs_handle, adxl362_interface_spi_deinit);
    DRIVER_ADXL362_LINK_SPI_READ(&gs_handle, adxl362_interface_spi_read);
    DRIVER_ADXL362_LINK_SPI_READ_ADDRESS16(&gs_handle, adxl362_interface_spi_read_address16);
    DRIVER_ADXL362_LINK_SPI_WRITE_ADDRESS16(&gs_handle, adxl362_interface_spi_write_address16);
    DRIVER_ADXL362_LINK_DELAY_MS(&gs_handle, adxl362_interface_delay_ms);
    DRIVER_ADXL362_LINK_DEBUG_PRINT(&gs_handle, adxl362_interface_debug_print);
    DRIVER_ADXL362_LINK_RECEIVE_CALLBACK(&gs_handle, a_adxl362_interface_receive_callback);

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

    /* start fifo test */
    adxl362_interface_debug_print("adxl362: start fifo test.\n");

    /* init the adxl362 */
    res = adxl362_init(&gs_handle);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: init failed.\n");

        return 1;
    }

    /* enable fifo temperature */
    res = adxl362_set_fifo_temperature(&gs_handle, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo temperature failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* fifo oldest saved mode */
    res = adxl362_set_fifo_mode(&gs_handle, ADXL362_FIFO_MODE_OLDEST_SAVED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo mode failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set fifo sample 96 */
    res = adxl362_set_fifo_sample(&gs_handle, 96);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set fifo sample failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set interrupt pin1 active level low */
    res = adxl362_set_interrupt_pin1_active_level(&gs_handle, ADXL362_INTERRUPT_PIN_LOW);
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

    /* disable inact map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_INACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* disable act map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_ACT, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* enable fifo overrun map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_OVERRUN, ADXL362_BOOL_TRUE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set interrupt pin1 map failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* enable fifo watermark map */
    res = adxl362_set_interrupt_pin1_map(&gs_handle, ADXL362_INTERRUPT_MAP_FIFO_WATERMARK, ADXL362_BOOL_TRUE);
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

    /* odr 1/2 */
    res = adxl362_set_bandwidth(&gs_handle, ADXL362_BANDWIDTH_ODR_DIV_2);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set bandwidth failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* normal mode */
    res = adxl362_set_noise_mode(&gs_handle, ADXL362_NOISE_MODE_NORMAL);
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

    /* set activity threshold 0x0000 */
    res = adxl362_set_activity_threshold(&gs_handle, 0x0000);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set activity time 0x00 */
    res = adxl362_set_activity_time(&gs_handle, 0x00);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity time failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set inactivity threshold 0x0000 */
    res = adxl362_set_inactivity_threshold(&gs_handle, 0x0000);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity threshold failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set inactivity time 0x0000 */
    res = adxl362_set_inactivity_time(&gs_handle, 0x0000);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity time failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set detect link mode */
    res = adxl362_set_detect_mode(&gs_handle, ADXL362_DETECT_MODE_LINK);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set detect mode failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set inactivity detect trigger */
    res = adxl362_set_inactivity_detect_trigger(&gs_handle, ADXL362_DETECT_TRIGGER_REFERENCED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set activity detect trigger */
    res = adxl362_set_activity_detect_trigger(&gs_handle, ADXL362_DETECT_TRIGGER_REFERENCED);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set activity detect trigger failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* disable inactivity */
    res = adxl362_set_inactivity(&gs_handle, ADXL362_BOOL_FALSE);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set inactivity failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* disable activity */
    res = adxl362_set_activity(&gs_handle, ADXL362_BOOL_FALSE);
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

    /* set 12.5Hz */
    res = adxl362_set_odr(&gs_handle, ADXL362_ODR_12P5HZ);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set odr failed.\n");
        (void)adxl362_deinit(&gs_handle);

        return 1;
    }

    /* set 2g */
    res = adxl362_set_range(&gs_handle, ADXL362_RANGE_2G);
    if (res != 0)
    {
        adxl362_interface_debug_print("adxl362: set range failed.\n");
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

    /* flag 0 */
    gs_flag = 0;

    /* set the times */
    t = times;

    /* loop */
    while (t != 0)
    {
        /* check the flag */
        if (gs_flag != 0)
        {
            /* t-- */
            t--;

            /* restart */
            gs_flag = 0;
        }
    }

    /* finish fifo test */
    adxl362_interface_debug_print("adxl362: finish fifo test.\n");
    (void)adxl362_deinit(&gs_handle);

    return 0;
}
