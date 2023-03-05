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
 * @file      driver_adxl362.h
 * @brief     driver adxl362 header file
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

#ifndef DRIVER_ADXL362_H
#define DRIVER_ADXL362_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup adxl362_driver adxl362 driver function
 * @brief    adxl362 driver modules
 * @{
 */

/**
 * @addtogroup adxl362_basic_driver
 * @{
 */

/**
 * @brief adxl362 bool enumeration definition
 */
typedef enum
{
    ADXL362_BOOL_FALSE = 0x00,        /**< false */
    ADXL362_BOOL_TRUE  = 0x01,        /**< true */
} adxl362_bool_t;

/**
 * @brief adxl362 interrupt pin level enumeration definition
 */
typedef enum
{
    ADXL362_INTERRUPT_PIN_HIGH = 0x00,        /**< active high */
    ADXL362_INTERRUPT_PIN_LOW  = 0x01,        /**< active low */
} adxl362_interrupt_pin_level_t;

/**
 * @brief adxl362 range enumeration definition
 */
typedef enum
{
    ADXL362_RANGE_2G = 0x00,        /**< 2g */
    ADXL362_RANGE_4G = 0x01,        /**< 4g */
    ADXL362_RANGE_8G = 0x02,        /**< 8g */
} adxl362_range_t;

/**
 * @brief adxl362 bandwidth enumeration definition
 */
typedef enum
{
    ADXL362_BANDWIDTH_ODR_DIV_2 = 0x00,        /**< 1/2 odr */
    ADXL362_BANDWIDTH_ODE_DIV_4 = 0x01,        /**< 1/4 odr */
} adxl362_bandwidth_t;

/**
 * @brief adxl362 odr enumeration definition
 */
typedef enum
{
    ADXL362_ODR_12P5HZ = 0x00,        /**< 12.5Hz */
    ADXL362_ODR_25HZ   = 0x01,        /**< 25Hz */
    ADXL362_ODR_50HZ   = 0x02,        /**< 50Hz */
    ADXL362_ODR_100HZ  = 0x03,        /**< 100Hz */
    ADXL362_ODR_200HZ  = 0x04,        /**< 200Hz */
    ADXL362_ODR_400HZ  = 0x05,        /**< 400Hz */
} adxl362_odr_t;

/**
 * @brief adxl362 noise mode enumeration definition
 */
typedef enum
{
    ADXL362_NOISE_MODE_NORMAL          = 0x00,         /**< normal operation mode */
    ADXL362_NOISE_MODE_LOW_NOISE       = 0x00,         /**< low noise mode */
    ADXL362_NOISE_MODE_ULTRA_LOW_NOISE = 0x00,        /**< ultra low noise mode */
} adxl362_noise_mode_t;

/**
 * @brief adxl362 mode enumeration definition
 */
typedef enum
{
    ADXL362_MODE_STANDBY     = 0x00,        /**< standby mode */
    ADXL362_MODE_MEASUREMENT = 0x02,        /**< measurement mode */
} adxl362_mode_t;

/**
 * @brief adxl362 detect mode enumeration definition
 */
typedef enum
{
    ADXL362_DETECT_MODE_BOTH = 0x00,        /**< activity and inactivity detection both enabled mode */
    ADXL362_DETECT_MODE_LINK = 0x01,        /**< activity and inactivity detection link mode */
    ADXL362_DETECT_MODE_LOOP = 0x03,        /**< activity and inactivity detection loop mode */
} adxl362_detect_mode_t;

/**
 * @brief adxl362 detect trigger enumeration definition
 */
typedef enum
{
    ADXL362_DETECT_TRIGGER_ABSOLUTE   = 0x00,        /**< operate in absolute mode */
    ADXL362_DETECT_TRIGGER_REFERENCED = 0x01,        /**< operate in referenced mode */
} adxl362_detect_trigger_t;

/**
 * @brief adxl362 interrupt map enumeration definition
 */
typedef enum
{
    ADXL362_INTERRUPT_MAP_AWAKE          = 6,        /**< awake status map */
    ADXL362_INTERRUPT_MAP_INACT          = 5,        /**< inactivity status map */
    ADXL362_INTERRUPT_MAP_ACT            = 4,        /**< activity status map */
    ADXL362_INTERRUPT_MAP_FIFO_OVERRUN   = 3,        /**< fifo overrun status map */
    ADXL362_INTERRUPT_MAP_FIFO_WATERMARK = 2,        /**< fifo watermark status map */
    ADXL362_INTERRUPT_MAP_FIFO_READY     = 1,        /**< fifo ready status map */
    ADXL362_INTERRUPT_MAP_DATA_READY     = 0,        /**< data ready status map */
} adxl362_interrupt_map_t;

/**
 * @brief adxl362 status enumeration definition
 */
typedef enum
{
    ADXL362_STATUS_ERR_USER_REGS  = (1 << 7),        /**< seu error detect */
    ADXL362_STATUS_AWAKE          = (1 << 6),        /**< indicates whether the accelerometer is in an active or inactive */
    ADXL362_STATUS_INACT          = (1 << 5),        /**< inactivity */
    ADXL362_STATUS_ACT            = (1 << 4),        /**< activity */
    ADXL362_STATUS_FIFO_OVERRUN   = (1 << 3),        /**< fifo overrun */
    ADXL362_STATUS_FIFO_WATERMARK = (1 << 2),        /**< fifo watermark */
    ADXL362_STATUS_FIFO_READY     = (1 << 1),        /**< fifo ready */
    ADXL362_STATUS_DATA_READY     = (1 << 0),        /**< data ready */
} adxl362_status_t;

/**
 * @brief adxl362 fifo mode enumeration definition
 */
typedef enum
{
    ADXL362_FIFO_MODE_DISABLE      = 0x00,        /**< disabled */
    ADXL362_FIFO_MODE_OLDEST_SAVED = 0x01,        /**< oldest saved mode */
    ADXL362_FIFO_MODE_STREAM       = 0x02,        /**< stream mode */
    ADXL362_FIFO_MODE_TRIGGERED    = 0x03,        /**< triggered mode */
} adxl362_fifo_mode_t;

/**
 * @brief adxl362 frame type enumeration definition
 */
typedef enum
{
    ADXL362_FRAME_TYPE_X    = 0x00,        /**< x axis frame type */
    ADXL362_FRAME_TYPE_Y    = 0x01,        /**< y axis frame type */
    ADXL362_FRAME_TYPE_Z    = 0x02,        /**< z axis frame type */
    ADXL362_FRAME_TYPE_TEMP = 0x03,        /**< temperature frame type */
} adxl362_frame_type_t;

/**
 * @brief adxl362 frame structure definition
 */
typedef struct adxl362_frame_s
{
    adxl362_frame_type_t type;       /**< frame type */
    int16_t raw;                     /**< raw data */
    float data;                      /**< converted data */
} adxl362_frame_t;

/**
 * @brief adxl362 handle structure definition
 */
typedef struct adxl362_handle_s
{
    uint8_t (*spi_init)(void);                                                        /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                      /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t addr, uint8_t *buf, uint16_t len);                    /**< point to a spi_read function address */
    uint8_t (*spi_read_address16)(uint16_t addr, uint8_t *buf, uint16_t len);         /**< point to a spi_read_address16 function address */
    uint8_t (*spi_write_address16)(uint16_t addr, uint8_t *buf, uint16_t len);        /**< point to a spi_write_address16 function address */
    void (*receive_callback)(uint8_t type);                                           /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                    /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                  /**< point to a debug_print function address */
    uint8_t inited;                                                                   /**< inited flag */
    uint8_t buf[512 * 2];                                                             /**< inner buffer */
} adxl362_handle_t;

/**
 * @brief adxl362 information structure definition
 */
typedef struct adxl362_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} adxl362_info_t;

/**
 * @}
 */

/**
 * @defgroup adxl362_link_driver adxl362 link driver function
 * @brief    adxl362 link driver modules
 * @ingroup  adxl362_driver
 * @{
 */

/**
 * @brief     initialize adxl362_handle_t structure
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] STRUCTURE is adxl362_handle_t
 * @note      none
 */
#define DRIVER_ADXL362_LINK_INIT(HANDLE, STRUCTURE)                  memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_SPI_INIT(HANDLE, FUC)                   (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_SPI_DEINIT(HANDLE, FUC)                 (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_SPI_READ(HANDLE, FUC)                   (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_read_address16 function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a spi_read_address16 function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_SPI_READ_ADDRESS16(HANDLE, FUC)         (HANDLE)->spi_read_address16 = FUC

/**
 * @brief     link spi_write_address16 function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a spi_write_address16 function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_SPI_WRITE_ADDRESS16(HANDLE, FUC)        (HANDLE)->spi_write_address16 = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_DELAY_MS(HANDLE, FUC)                   (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_DEBUG_PRINT(HANDLE, FUC)                (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an adxl362 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_ADXL362_LINK_RECEIVE_CALLBACK(HANDLE, FUC)           (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup adxl362_basic_driver adxl362 basic driver function
 * @brief    adxl362 basic driver modules
 * @ingroup  adxl362_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an adxl362 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl362_info(adxl362_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to an adxl362 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_irq_handler(adxl362_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an adxl362 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 id is invalid
 *            - 5 soft reset failed
 * @note      none
 */
uint8_t adxl362_init(adxl362_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an adxl362 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t adxl362_deinit(adxl362_handle_t *handle);

/**
 * @brief      read the data
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *g points to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_read(adxl362_handle_t *handle, int16_t raw[3], float g[3]);

/**
 * @brief         read data from the fifo
 * @param[in]     *handle points to an adxl362 handle structure
 * @param[out]    *frame points to a frame buffer
 * @param[in,out] *frame_len points to a frame length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read fifo failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 * @note          none
 */
uint8_t adxl362_read_fifo(adxl362_handle_t *handle, adxl362_frame_t *frame, uint16_t *frame_len);

/**
 * @brief      read the data with eight most significant bits
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *g points to a converted data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read 8msb failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_read_8msb(adxl362_handle_t *handle, int8_t raw[3], float g[3]);

/**
 * @brief      read the temperature
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *raw points to a raw data buffer
 * @param[out] *temp points to a converted temperature data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_read_temperature(adxl362_handle_t *handle, int16_t *raw, float *temp);

/**
 * @brief      get the chip revision
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get revision failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_revision(adxl362_handle_t *handle, uint8_t *id);

/**
 * @brief      get the chip status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_status(adxl362_handle_t *handle, uint8_t *status);

/**
 * @brief      get the fifo counter
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *counter points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_fifo_counter(adxl362_handle_t *handle, uint16_t *counter);

/**
 * @brief     soft reset
 * @param[in] *handle points to an adxl362 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_soft_reset(adxl362_handle_t *handle);

/**
 * @brief     enable or disable saving temperature data to fifo
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fifo temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_fifo_temperature(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the fifo temperature status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_fifo_temperature(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     set the fifo mode
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] mode is the fifo mode
 * @return    status code
 *            - 0 success
 *            - 1 set fifo mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_fifo_mode(adxl362_handle_t *handle, adxl362_fifo_mode_t mode);

/**
 * @brief      get the fifo mode
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *mode points to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_fifo_mode(adxl362_handle_t *handle, adxl362_fifo_mode_t *mode);

/**
 * @brief     set the fifo sample
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] sample is the number of samples to store in the fifo
 * @return    status code
 *            - 0 success
 *            - 1 set fifo sample failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 sample > 511
 * @note      0 <= sample <= 511
 */
uint8_t adxl362_set_fifo_sample(adxl362_handle_t *handle, uint16_t sample);

/**
 * @brief      get the fifo sample
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *sample points to a sample buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fifo sample failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_fifo_sample(adxl362_handle_t *handle, uint16_t *sample);

/**
 * @brief     set the interrupt pin1 active level
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] level is the interrupt pin active level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_interrupt_pin1_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t level);

/**
 * @brief      get the interrupt pin1 active level
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *level points to an interrupt pin active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_interrupt_pin1_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t *level);

/**
 * @brief     set the interrupt pin1 map
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] map is the interrupt function map
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin map failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_interrupt_pin1_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t enable);

/**
 * @brief      get the interrupt pin1 map
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  map is the interrupt function map
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin map failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_interrupt_pin1_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t *enable);

/**
 * @brief     set the interrupt pin2 active level
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] level is the interrupt pin active level
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin active level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_interrupt_pin2_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t level);

/**
 * @brief      get the interrupt pin2 active level
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *level points to an interrupt pin active level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin active level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_interrupt_pin2_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t *level);

/**
 * @brief     set the interrupt pin2 map
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] map is the interrupt function map
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin map failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_interrupt_pin2_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t enable);

/**
 * @brief      get the interrupt pin2 map
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  map is the interrupt function map
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin map failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_interrupt_pin2_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t *enable);

/**
 * @brief     set the measurement range
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] range is the measurement range
 * @return    status code
 *            - 0 success
 *            - 1 set range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_range(adxl362_handle_t *handle, adxl362_range_t range);

/**
 * @brief      get the measurement range
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *range points to a measurement range buffer
 * @return     status code
 *             - 0 success
 *             - 1 get range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_range(adxl362_handle_t *handle, adxl362_range_t *range);

/**
 * @brief     set the filter bandwidth
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] bandwidth is the filter bandwidth
 * @return    status code
 *            - 0 success
 *            - 1 set bandwidth failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_bandwidth(adxl362_handle_t *handle, adxl362_bandwidth_t bandwidth);

/**
 * @brief      get the filter bandwidth
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *bandwidth points to a filter bandwidth buffer
 * @return     status code
 *             - 0 success
 *             - 1 get bandwidth failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_bandwidth(adxl362_handle_t *handle, adxl362_bandwidth_t *bandwidth);

/**
 * @brief     enable or disable interrupt pin2 as the external sampling trigger
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin2 as external sampling trigger failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_interrupt_pin2_as_external_sampling_trigger(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the interrupt pin2 as the external sampling trigger status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin2 as external sampling trigger failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_interrupt_pin2_as_external_sampling_trigger(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     set the output data rate
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] odr is the output data rate
 * @return    status code
 *            - 0 success
 *            - 1 set odr failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_odr(adxl362_handle_t *handle, adxl362_odr_t odr);

/**
 * @brief      get the output data rate
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *odr points to an output data rate buffer
 * @return     status code
 *             - 0 success
 *             - 1 get odr failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_odr(adxl362_handle_t *handle, adxl362_odr_t *odr);

/**
 * @brief     enable or disable interrupt pin1 as the external clock
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt pin1 as external clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_interrupt_pin1_as_external_clock(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the interrupt pin1 as the external clock status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt pin1 as external clock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_interrupt_pin1_as_external_clock(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     set the noise mode
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] mode is the noise mode
 * @return    status code
 *            - 0 success
 *            - 1 set noise mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_noise_mode(adxl362_handle_t *handle, adxl362_noise_mode_t mode);

/**
 * @brief      get the noise mode
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *mode points to a noise mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get noise mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_noise_mode(adxl362_handle_t *handle, adxl362_noise_mode_t *mode);

/**
 * @brief     enable or disable chip wake up
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_wake_up(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the chip wake up status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get wake up failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_wake_up(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     enable or disable auto sleep
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_auto_sleep(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the auto sleep status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto sleep failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_auto_sleep(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     set the chip mode
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_mode(adxl362_handle_t *handle, adxl362_mode_t mode);

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_mode(adxl362_handle_t *handle, adxl362_mode_t *mode);

/**
 * @brief     set the activity threshold
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] threshold is the set threshold
 * @return    status code
 *            - 0 success
 *            - 1 set activity threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold is invalid
 * @note      0 <= threshold <= 0x07FF
 */
uint8_t adxl362_set_activity_threshold(adxl362_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the activity threshold
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get activity threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_activity_threshold(adxl362_handle_t *handle, uint16_t *threshold);

/**
 * @brief      convert the activity threshold to the register raw data
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  g is the activity threshold
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 g is invalid
 * @note       g >= 0.0f
 */
uint8_t adxl362_activity_threshold_convert_to_register(adxl362_handle_t *handle, float g, uint16_t *reg);

/**
 * @brief      convert the register raw data to the activity threshold
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *g points to an activity threshold buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_activity_threshold_convert_to_data(adxl362_handle_t *handle, uint16_t reg, float *g);

/**
 * @brief     set the activity time
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] tim is the activity time
 * @return    status code
 *            - 0 success
 *            - 1 set activity time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_activity_time(adxl362_handle_t *handle, uint8_t tim);

/**
 * @brief      convert the register raw data to the activity time
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *ms points to an activity time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_activity_time_convert_to_data(adxl362_handle_t *handle, uint8_t reg, float *ms);

/**
 * @brief      convert the activity time to the register raw data
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  ms is the activity time in ms
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 ms is invalid
 * @note       ms >= 0.0f
 */
uint8_t adxl362_activity_time_convert_to_register(adxl362_handle_t *handle, float ms, uint8_t *reg);

/**
 * @brief      get the activity time
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *tim points to an activity time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get activity time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_activity_time(adxl362_handle_t *handle, uint8_t *tim);

/**
 * @brief     set the inactivity threshold
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] threshold is the set threshold
 * @return    status code
 *            - 0 success
 *            - 1 set inactivity threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold is invalid
 * @note      0 <= threshold <= 0x07FF
 */
uint8_t adxl362_set_inactivity_threshold(adxl362_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the inactivity threshold
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *threshold points to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inactivity threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_inactivity_threshold(adxl362_handle_t *handle, uint16_t *threshold);

/**
 * @brief      convert the inactivity threshold to the register raw data
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  g is the inactivity threshold
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 g is invalid
 * @note       g >= 0.0f
 */
uint8_t adxl362_inactivity_threshold_convert_to_register(adxl362_handle_t *handle, float g, uint16_t *reg);

/**
 * @brief      convert the register raw data to the inactivity threshold
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *g points to an inactivity threshold buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_inactivity_threshold_convert_to_data(adxl362_handle_t *handle, uint16_t reg, float *g);

/**
 * @brief     set the inactivity time
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] tim is the inactivity time
 * @return    status code
 *            - 0 success
 *            - 1 set inactivity time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_inactivity_time(adxl362_handle_t *handle, uint16_t tim);

/**
 * @brief      get the inactivity time
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *tim points to an inactivity time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inactivity time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_inactivity_time(adxl362_handle_t *handle, uint16_t *tim);

/**
 * @brief      convert the inactivity time to the register raw data
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  ms is the inactivity time in ms
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 ms is invalid
 * @note       ms >= 0.0f
 */
uint8_t adxl362_inactivity_time_convert_to_register(adxl362_handle_t *handle, float ms, uint16_t *reg);

/**
 * @brief      convert the register raw data to the inactivity time
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *ms points to an inactivity time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_inactivity_time_convert_to_data(adxl362_handle_t *handle, uint16_t reg, float *ms);

/**
 * @brief     set the detect mode
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] mode is the set detect mode
 * @return    status code
 *            - 0 success
 *            - 1 set detect mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_detect_mode(adxl362_handle_t *handle, adxl362_detect_mode_t mode);

/**
 * @brief      get the detect mode
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *mode points to a detect mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get detect mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_detect_mode(adxl362_handle_t *handle, adxl362_detect_mode_t *mode);

/**
 * @brief     set the inactivity detect trigger mode
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] trigger is the detect trigger mode
 * @return    status code
 *            - 0 success
 *            - 1 set inactivity detect trigger failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_inactivity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t trigger);

/**
 * @brief      get the inactivity detect trigger mode
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *trigger points to a detect trigger mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inactivity detect trigger failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_inactivity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t *trigger);

/**
 * @brief     set the activity detect trigger mode
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] trigger is the detect trigger mode
 * @return    status code
 *            - 0 success
 *            - 1 set activity detect trigger failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_activity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t trigger);

/**
 * @brief      get the activity detect trigger mode
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *trigger points to a detect trigger mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get activity detect trigger failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_activity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t *trigger);

/**
 * @brief     enable or disable inactivity
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set inactivity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_inactivity(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the inactivity status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get inactivity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_inactivity(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     enable or disable activity
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set activity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_activity(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the activity status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get activity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_activity(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @brief     enable or disable the self test
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set self test failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_self_test(adxl362_handle_t *handle, adxl362_bool_t enable);

/**
 * @brief      get the self test status
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get self test failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_self_test(adxl362_handle_t *handle, adxl362_bool_t *enable);

/**
 * @}
 */

/**
 * @defgroup adxl362_extern_driver adxl362 extern driver function
 * @brief    adxl362 extern driver modules
 * @ingroup  adxl362_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t adxl362_set_reg(adxl362_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_reg(adxl362_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      read from fifo
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t adxl362_get_fifo(adxl362_handle_t *handle, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
