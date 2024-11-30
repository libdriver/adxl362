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
 * @file      stm32f407_driver_adxl362_interface.c
 * @brief     stm32f407 driver adxl362 interface source file
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

#include "driver_adxl362_interface.h"
#include "delay.h"
#include "spi.h"
#include "uart.h"
#include <stdarg.h>

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t adxl362_interface_spi_init(void)
{
    return spi_init(SPI_MODE_0);
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t adxl362_interface_spi_deinit(void)
{
    return spi_deinit();
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t adxl362_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;

    __set_BASEPRI(1);
    res = spi_read(reg, buf, len);
    __set_BASEPRI(0);

    return res;
}

/**
 * @brief      interface spi bus read
 * @param[in]  addr spi register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t adxl362_interface_spi_read_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t res;

    __set_BASEPRI(1);
    res = spi_read_address16(addr, buf, len);
    __set_BASEPRI(0);

    return res;
}

/**
 * @brief     interface spi bus write
 * @param[in] addr spi register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t adxl362_interface_spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t res;

    __set_BASEPRI(1);
    res = spi_write_address16(addr, buf, len);
    __set_BASEPRI(0);

    return res;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void adxl362_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void adxl362_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;

    memset((char *)str, 0, sizeof(char) * 256);
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);

    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
void adxl362_interface_receive_callback(uint8_t type)
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
            adxl362_interface_debug_print("adxl362: irq awake.\n");
            
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
            adxl362_interface_debug_print("adxl362: irq fifo overrun.\n");
            
            break;
        }
        case ADXL362_STATUS_FIFO_WATERMARK :
        {
            adxl362_interface_debug_print("adxl362: irq fifo watermark.\n");
            
            break;
        }
        case ADXL362_STATUS_FIFO_READY :
        {
            adxl362_interface_debug_print("adxl362: irq fifo ready.\n");
            
            break;
        }
        case ADXL362_STATUS_DATA_READY :
        {
            adxl362_interface_debug_print("adxl362: irq data ready.\n");
            
            break;
        }
        default :
        {
            adxl362_interface_debug_print("adxl362: unknown code.\n");
            
            break;
        }
    }
}
