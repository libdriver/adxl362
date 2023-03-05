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
 * @file      driver_adxl362.c
 * @brief     driver adxl362 source file
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

#include "driver_adxl362.h"

/**
 * @brief chip register definition
 */
#define CHIP_NAME                 "Analog Devices ADXL362"        /**< chip name */
#define MANUFACTURER_NAME         "Analog Devices"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.6f                            /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.5f                            /**< chip max supply voltage */
#define MAX_CURRENT               0.013f                          /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                          /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                           /**< chip max operating temperature */
#define DRIVER_VERSION            1000                            /**< driver version */

/**
 * @brief chip register definition
 */
#define ADXL362_REG_DEVID_AD              0x00        /**< device id register */
#define ADXL362_REG_DEVID_MST             0x01        /**< device id register */
#define ADXL362_REG_PARTID                0x02        /**< part id register */
#define ADXL362_REG_REVID                 0x03        /**< revision id register */
#define ADXL362_REG_XDATA                 0x08        /**< x axis data register */
#define ADXL362_REG_YDATA                 0x09        /**< y axis data register */
#define ADXL362_REG_ZDATA                 0x0A        /**< z axis data register */
#define ADXL362_REG_STATUS                0x0B        /**< status register */
#define ADXL362_REG_FIFO_ENTRIES_L        0x0C        /**< fifo entries low register */
#define ADXL362_REG_FIFO_ENTRIES_H        0x0D        /**< fifo entries high register */
#define ADXL362_REG_XDATA_L               0x0E        /**< x axis data low register */
#define ADXL362_REG_XDATA_H               0x0F        /**< x axis data high register */
#define ADXL362_REG_YDATA_L               0x10        /**< y axis data low register */
#define ADXL362_REG_YDATA_H               0x11        /**< y axis data high register */
#define ADXL362_REG_ZDATA_L               0x12        /**< z axis data low register */
#define ADXL362_REG_ZDATA_H               0x13        /**< z axis data high register */
#define ADXL362_REG_TEMP_L                0x14        /**< temperature low register */
#define ADXL362_REG_TEMP_H                0x15        /**< temperature high register */
#define ADXL362_REG_SOFT_RESET            0x1F        /**< soft reset register */
#define ADXL362_REG_THRESH_ACT_L          0x20        /**< threshold action low register */
#define ADXL362_REG_THRESH_ACT_H          0x21        /**< threshold action high register */
#define ADXL362_REG_TIME_ACT              0x22        /**< time action register */
#define ADXL362_REG_THRESH_INACT_L        0x23        /**< threshold inaction low register */
#define ADXL362_REG_THRESH_INACT_H        0x24        /**< threshold inaction high register */
#define ADXL362_REG_TIME_INACT_L          0x25        /**< time inaction low register */
#define ADXL362_REG_TIME_INACT_H          0x26        /**< time inaction high register */
#define ADXL362_REG_ACT_INACT_CTL         0x27        /**< action inaction control register */
#define ADXL362_REG_FIFO_CONTROL          0x28        /**< fifo control register */
#define ADXL362_REG_FIFO_SAMPLES          0x29        /**< fifo samples register */
#define ADXL362_REG_INTMAP1               0x2A        /**< interrupt1 map register */
#define ADXL362_REG_INTMAP2               0x2B        /**< interrupt2 map register */
#define ADXL362_REG_FILTER_CTL            0x2C        /**< filter register */
#define ADXL362_REG_POWER_CTL             0x2D        /**< power register */
#define ADXL362_REG_SELF_TEST             0x2E        /**< self register */

/**
 * @brief      read bytes
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_adxl362_read(adxl362_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_read_address16(((uint16_t)(0x0B) << 8) | reg, buf, len) != 0)        /* read data */
    {
        return 1;                                                                        /* return error */
    }
    else
    {
        return 0;                                                                        /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an adxl362 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_adxl362_write(adxl362_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_write_address16(((uint16_t)(0x0A) << 8) | reg, buf, len) != 0)        /* write data */
    {
        return 1;                                                                         /* return error */
    }
    else
    {
        return 0;                                                                         /* success return 0 */
    }
}

/**
 * @brief      read fifo
 * @param[in]  *handle points to an adxl362 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_adxl362_read_fifo(adxl362_handle_t *handle, uint8_t *buf, uint16_t len)
{
    if (handle->spi_read(0x0D, buf, len) != 0)        /* read data */
    {
        return 1;                                     /* return error */
    }
    else
    {
        return 0;                                     /* success return 0 */
    }
}

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
uint8_t adxl362_get_revision(adxl362_handle_t *handle, uint8_t *id)
{
    uint8_t res;
    
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_REVID, id, 1);               /* read revision id */
    if (res != 0)                                                         /* check the result */
    {
        handle->debug_print("adxl362: read revision id failed.\n");       /* read revision id failed */
       
        return 1;                                                         /* return error */
    }
    
    return 0;                                                             /* success return 0 */
}

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
uint8_t adxl362_init(adxl362_handle_t *handle)
{
    uint8_t id;
    uint8_t cmd;
  
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->debug_print == NULL)                                                  /* check debug_print */
    {
        return 3;                                                                     /* return error */
    }
    if (handle->spi_init == NULL)                                                     /* check spi_init */
    {
        handle->debug_print("adxl362: spi_init is null.\n");                          /* spi_init is null */
       
        return 3;                                                                     /* return error */
    }
    if (handle->spi_deinit == NULL)                                                   /* check spi_deinit */
    {
        handle->debug_print("adxl362: spi_deinit is null.\n");                        /* spi_deinit is null */
       
        return 3;                                                                     /* return error */
    }
    if (handle->spi_read == NULL)                                                     /* check spi_read */
    {
        handle->debug_print("adxl362: spi_read is null.\n");                          /* spi_read is null */
       
        return 3;                                                                     /* return error */
    }
    if (handle->spi_read_address16 == NULL)                                           /* check spi_read_address16 */
    {
        handle->debug_print("adxl362: spi_read_address16 is null.\n");                /* spi_read_address16 is null */
       
        return 3;                                                                     /* return error */
    }
    if (handle->spi_write_address16 == NULL)                                          /* check spi_write_address16 */
    {
        handle->debug_print("adxl362: spi_write_address16 is null.\n");               /* spi_write_address16 is null */
       
        return 3;                                                                     /* return error */
    }
    if (handle->delay_ms == NULL)                                                     /* check delay_ms */
    {
        handle->debug_print("adxl362: delay_ms is null.\n");                          /* delay_ms is null */
       
        return 3;                                                                     /* return error */
    }
    if (handle->receive_callback == NULL)                                             /* check receive_callback */
    {
        handle->debug_print("adxl362: receive_callback is null.\n");                  /* receive_callback is null */
       
        return 3;                                                                     /* return error */
    }

    if (handle->spi_init() != 0)                                                      /* initialize spi bus */
    {
        handle->debug_print("adxl362: spi init failed.\n");                           /* spi init failed */
       
        return 1;                                                                     /* return error */
    }
    
    if (a_adxl362_read(handle, ADXL362_REG_DEVID_AD, (uint8_t *)&id, 1) != 0)         /* read id */
    {
        handle->debug_print("adxl362: read failed.\n");                               /* read failed */
        (void)handle->spi_deinit();                                                   /* close */
        
        return 4;                                                                     /* return error */
    }
    if (id != 0xAD)                                                                   /* check id */
    {
        handle->debug_print("adxl362: id is invalid.\n");                             /* id is invalid */
        (void)handle->spi_deinit();                                                   /* close */
       
        return 4;                                                                     /* return error */
    }
    if (a_adxl362_read(handle, ADXL362_REG_DEVID_MST, (uint8_t *)&id, 1) != 0)        /* read id */
    {
        handle->debug_print("adxl362: read failed.\n");                               /* read failed */
        (void)handle->spi_deinit();                                                   /* close */
        
        return 4;                                                                     /* return error */
    }
    if (id != 0x1D)                                                                   /* check id */
    {
        handle->debug_print("adxl362: id is invalid.\n");                             /* id is invalid */
        (void)handle->spi_deinit();                                                   /* close */
       
        return 4;                                                                     /* return error */
    }
    if (a_adxl362_read(handle, ADXL362_REG_PARTID, (uint8_t *)&id, 1) != 0)           /* read part id */
    {
        handle->debug_print("adxl362: read failed.\n");                               /* read failed */
        (void)handle->spi_deinit();                                                   /* close */
        
        return 4;                                                                     /* return error */
    }
    if (id != 0xF2)                                                                   /* check id */
    {
        handle->debug_print("adxl362: id is invalid.\n");                             /* id is invalid */
        (void)handle->spi_deinit();                                                   /* close */
       
        return 4;                                                                     /* return error */
    }
    
    cmd = 0x52;                                                                       /* set the reset command */
    if (a_adxl362_write(handle, ADXL362_REG_SOFT_RESET, &cmd, 1) != 0)                /* soft reset */
    {
        handle->debug_print("adxl362: soft reset failed.\n");                         /* soft reset failed */
        (void)handle->spi_deinit();                                                   /* close */
       
        return 5;                                                                     /* return error */
    }
    handle->delay_ms(2);                                                              /* delay 2ms */
    handle->inited = 1;                                                               /* flag finish initialization */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t adxl362_deinit(adxl362_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, (uint8_t *)&prev, 1);         /* read config */
    if (res != 0)                                                                     /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");                 /* read power control failed */
       
        return 4;                                                                     /* return error */
    }
    prev &= ~(3 << 0);                                                                /* standby mode */
    res = a_adxl362_write(handle, ADXL362_REG_POWER_CTL, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("adxl362: write power control failed.\n");                /* write power control failed */
       
        return 4;                                                                     /* return error */
    }

    res = handle->spi_deinit();                                                       /* spi deinit */
    if (res != 0)                                                                     /* check result */
    {
        return 1;                                                                     /* return error */
    }
    else
    {
        handle->inited = 0;                                                           /* flag close */

        return 0;                                                                     /* success return 0 */
    }
}

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
uint8_t adxl362_read_8msb(adxl362_handle_t *handle, int8_t raw[3], float g[3])
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    uint8_t buf[3];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    res = a_adxl362_read(handle, ADXL362_REG_XDATA, buf, 3);               /* read data */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read data failed.\n");               /* read data failed */
       
        return 1;                                                          /* return error */
    }
    raw[0] = (int8_t)buf[0];                                               /* copy x */
    raw[1] = (int8_t)buf[1];                                               /* copy y */
    raw[2] = (int8_t)buf[2];                                               /* copy z */
    range = (prev >> 6) & 0x3;                                             /* get the range */
    if (range == 0)                                                        /* 2g */
    {
        g[0] = (float)raw[0] * 16.0f / 1000.0f;                            /* convert x */
        g[1] = (float)raw[1] * 16.0f / 1000.0f;                            /* convert y */
        g[2] = (float)raw[2] * 16.0f / 1000.0f;                            /* convert z */
    }
    else if (range == 1)                                                   /* 4g */
    {
        g[0] = (float)raw[0] * 16.0f / 500.0f;                             /* convert x */
        g[1] = (float)raw[1] * 16.0f / 500.0f;                             /* convert y */
        g[2] = (float)raw[2] * 16.0f / 500.0f;                             /* convert z */
    }
    else                                                                   /* 8g */
    {
        g[0] = (float)raw[0] * 16.0f / 235.0f;                             /* convert x */
        g[1] = (float)raw[1] * 16.0f / 235.0f;                             /* convert y */
        g[2] = (float)raw[2] * 16.0f / 235.0f;                             /* convert z */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_read_temperature(adxl362_handle_t *handle, int16_t *raw, float *temp)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_TEMP_L, buf, 2);                /* read temperature */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("adxl362: read temperature failed.\n");          /* read temperature failed */
       
        return 1;                                                            /* return error */
    }
    *raw = (int16_t)((uint16_t)((((uint16_t)buf[1]) << 8) | buf[0]));        /* get the raw data */
    *temp = (float)(*raw) * 0.065f;                                          /* convert the temperature */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t adxl362_get_status(adxl362_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_STATUS, status, 1);        /* read status */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("adxl362: read status failed.\n");          /* read status failed */
       
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t adxl362_get_fifo_counter(adxl362_handle_t *handle, uint16_t *counter)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_ENTRIES_L, buf, 2);        /* read fifo entries */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("adxl362: read fifo entries failed.\n");         /* read fifo entries failed */
       
        return 1;                                                            /* return error */
    }
    *counter = (uint16_t)(((uint16_t)buf[1]) << 8) | buf[0];                 /* set the counter */
    *counter = (*counter) & 0x200;                                           /* get the valid part */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t adxl362_soft_reset(adxl362_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    prev = 0x52;                                                            /* soft reset command */
    res = a_adxl362_write(handle, ADXL362_REG_SOFT_RESET, &prev, 1);        /* write soft reset */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("adxl362: write soft reset failed.\n");         /* write soft reset failed */
       
        return 1;                                                           /* return error */
    }
    handle->delay_ms(2);                                                    /* delay 2ms */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t adxl362_set_fifo_temperature(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);         /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");          /* read fifo control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(1 << 2);                                                        /* clear settings */
    prev |= (enable << 2);                                                    /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);        /* write fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write fifo control failed.\n");         /* write fifo control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_fifo_temperature(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);         /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");          /* read fifo control failed */
       
        return 1;                                                             /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 2) & 0x01);                           /* get the bool */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_fifo_mode(adxl362_handle_t *handle, adxl362_fifo_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);         /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");          /* read fifo control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(3 << 0);                                                        /* clear settings */
    prev |= (mode << 0);                                                      /* set the mode */
    res = a_adxl362_write(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);        /* write fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write fifo control failed.\n");         /* write fifo control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_fifo_mode(adxl362_handle_t *handle, adxl362_fifo_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);         /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");          /* read fifo control failed */
       
        return 1;                                                             /* return error */
    }
    *mode = (adxl362_fifo_mode_t)(prev & 0x03);                               /* get the mode */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_fifo_sample(adxl362_handle_t *handle, uint16_t sample)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    if (sample > 511)                                                         /* check sample */
    {
        handle->debug_print("adxl362: sample > 511.\n");                      /* sample > 511 */
       
        return 4;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);         /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");          /* read fifo control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(1 << 3);                                                        /* clear settings */
    prev |= (((sample >> 8) & 0x01) << 3);                                    /* set the bit 8 */
    res = a_adxl362_write(handle, ADXL362_REG_FIFO_SAMPLES, &prev, 1);        /* write fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write fifo samples failed.\n");         /* write fifo samples failed */
       
        return 1;                                                             /* return error */
    }
    
    prev = sample & 0xFF;                                                     /* get the bit7 - bit0 */
    res = a_adxl362_write(handle, ADXL362_REG_FIFO_SAMPLES, &prev, 1);        /* write fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write fifo samples failed.\n");         /* write fifo samples failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_fifo_sample(adxl362_handle_t *handle, uint16_t *sample)
{
    uint8_t res;
    uint8_t prev;
    uint8_t prev2;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);         /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");          /* read fifo control failed */
       
        return 1;                                                             /* return error */
    }
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_SAMPLES, &prev2, 1);        /* read fifo control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read fifo samples failed.\n");          /* read fifo samples failed */
       
        return 1;                                                             /* return error */
    }
    *sample = (uint16_t)((uint16_t)((prev >> 3) & 0x01) << 8) | prev2;        /* get the sample */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_interrupt_pin1_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP1, &prev, 1);         /* read int1 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int1 map failed.\n");         /* read int1 map failed */
       
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 7);                                                   /* clear settings */
    prev |= (level << 7);                                                /* set the level */
    res = a_adxl362_write(handle, ADXL362_REG_INTMAP1, &prev, 1);        /* write int1 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: write int1 map failed.\n");        /* write int1 map failed */
       
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_get_interrupt_pin1_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP1, &prev, 1);         /* read int1 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int1 map failed.\n");         /* read int1 map failed */
       
        return 1;                                                        /* return error */
    }
    *level = (adxl362_interrupt_pin_level_t)((prev >> 7) & 0x01);        /* get the level */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_set_interrupt_pin1_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP1, &prev, 1);         /* read int1 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int1 map failed.\n");         /* read int1 map failed */
       
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << map);                                                 /* clear settings */
    prev |= (enable << map);                                             /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_INTMAP1, &prev, 1);        /* write int1 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: write int1 map failed.\n");        /* write int1 map failed */
       
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_get_interrupt_pin1_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP1, &prev, 1);         /* read int1 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int1 map failed.\n");         /* read int1 map failed */
       
        return 1;                                                        /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> map) & 0x01);                    /* get the bool */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_set_interrupt_pin2_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP2, &prev, 1);         /* read int2 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int2 map failed.\n");         /* read int2 map failed */
       
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << 7);                                                   /* clear settings */
    prev |= (level << 7);                                                /* set the level */
    res = a_adxl362_write(handle, ADXL362_REG_INTMAP2, &prev, 1);        /* write int2 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: write int2 map failed.\n");        /* write int2 map failed */
       
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_get_interrupt_pin2_active_level(adxl362_handle_t *handle, adxl362_interrupt_pin_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP2, &prev, 1);         /* read int2 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int2 map failed.\n");         /* read int2 map failed */
       
        return 1;                                                        /* return error */
    }
    *level = (adxl362_interrupt_pin_level_t)((prev >> 7) & 0x01);        /* get the level */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_set_interrupt_pin2_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP2, &prev, 1);         /* read int2 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int2 map failed.\n");         /* read int2 map failed */
       
        return 1;                                                        /* return error */
    }
    prev &= ~(1 << map);                                                 /* clear settings */
    prev |= (enable << map);                                             /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_INTMAP2, &prev, 1);        /* write int2 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: write int2 map failed.\n");        /* write int2 map failed */
       
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_get_interrupt_pin2_map(adxl362_handle_t *handle, adxl362_interrupt_map_t map, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_INTMAP2, &prev, 1);         /* read int2 map */
    if (res != 0)                                                        /* check the result */
    {
        handle->debug_print("adxl362: read int2 map failed.\n");         /* read int2 map failed */
       
        return 1;                                                        /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> map) & 0x01);                    /* get the bool */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t adxl362_set_range(adxl362_handle_t *handle, adxl362_range_t range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(3 << 6);                                                         /* clear settings */
    prev |= (range << 6);                                                      /* set the range */
    res = a_adxl362_write(handle, ADXL362_REG_FILTER_CTL, &prev, 1);           /* write filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: write filter control failed.\n");        /* write filter control failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_get_range(adxl362_handle_t *handle, adxl362_range_t *range)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    *range = (adxl362_range_t)((prev >> 6) & 0x03);                            /* set the range */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_set_bandwidth(adxl362_handle_t *handle, adxl362_bandwidth_t bandwidth)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 4);                                                         /* clear settings */
    prev |= (bandwidth << 4);                                                  /* set the bandwidth */
    res = a_adxl362_write(handle, ADXL362_REG_FILTER_CTL, &prev, 1);           /* write filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: write filter control failed.\n");        /* write filter control failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_get_bandwidth(adxl362_handle_t *handle, adxl362_bandwidth_t *bandwidth)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    *bandwidth = (adxl362_bandwidth_t)((prev >> 4) & 0x01);                    /* get the bandwidth */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_set_interrupt_pin2_as_external_sampling_trigger(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 3);                                                         /* clear settings */
    prev |= (enable << 3);                                                     /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_FILTER_CTL, &prev, 1);           /* write filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: write filter control failed.\n");        /* write filter control failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_get_interrupt_pin2_as_external_sampling_trigger(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 3) & 0x01);                            /* get the bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_set_odr(adxl362_handle_t *handle, adxl362_odr_t odr)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    prev &= ~(7 << 0);                                                         /* clear settings */
    prev |= (odr << 0);                                                        /* set the odr */
    res = a_adxl362_write(handle, ADXL362_REG_FILTER_CTL, &prev, 1);           /* write filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: write filter control failed.\n");        /* write filter control failed */
       
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_get_odr(adxl362_handle_t *handle, adxl362_odr_t *odr)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);            /* read filter control */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read filter control failed.\n");         /* read filter control failed */
       
        return 1;                                                              /* return error */
    }
    *odr = (adxl362_odr_t)(prev & 0x7);                                        /* set the odr */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_set_interrupt_pin1_as_external_clock(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(1 << 6);                                                        /* clear settings */
    prev |= (enable << 6);                                                    /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_POWER_CTL, &prev, 1);           /* write power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write power control failed.\n");        /* write power control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_interrupt_pin1_as_external_clock(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 6) & 0x01);                           /* get the bool */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_noise_mode(adxl362_handle_t *handle, adxl362_noise_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(3 << 4);                                                        /* clear settings */
    prev |= (mode << 4);                                                      /* set the mode */
    res = a_adxl362_write(handle, ADXL362_REG_POWER_CTL, &prev, 1);           /* write power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write power control failed.\n");        /* write power control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_noise_mode(adxl362_handle_t *handle, adxl362_noise_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    *mode = (adxl362_noise_mode_t)((prev >> 4) & 0x03);                       /* get the mode */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_wake_up(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(1 << 3);                                                        /* clear settings */
    prev |= (enable << 3);                                                    /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_POWER_CTL, &prev, 1);           /* write power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write power control failed.\n");        /* write power control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_wake_up(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 3) & 0x01);                           /* set the bool */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_auto_sleep(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(1 << 2);                                                        /* clear settings */
    prev |= (enable << 2);                                                    /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_POWER_CTL, &prev, 1);           /* write power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write power control failed.\n");        /* write power control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_auto_sleep(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 2) & 0x01);                           /* get the bool */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_mode(adxl362_handle_t *handle, adxl362_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    prev &= ~(3 << 0);                                                        /* clear settings */
    prev |= (mode << 0);                                                      /* set the mode */
    res = a_adxl362_write(handle, ADXL362_REG_POWER_CTL, &prev, 1);           /* write power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write power control failed.\n");        /* write power control failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_mode(adxl362_handle_t *handle, adxl362_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_POWER_CTL, &prev, 1);            /* read power control */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: read power control failed.\n");         /* read power control failed */
       
        return 1;                                                             /* return error */
    }
    *mode = (adxl362_mode_t)((prev >> 0) & 0x03);                             /* set the mode */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_set_self_test(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_SELF_TEST, &prev, 1);         /* read self test */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read self test failed.\n");          /* read self test failed */
       
        return 1;                                                          /* return error */
    }
    prev &= ~(1 << 0);                                                     /* clear settings */
    prev |= (enable << 0);                                                 /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_SELF_TEST, &prev, 1);        /* write self test */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: write self test failed.\n");         /* write self test failed */
       
        return 1;                                                          /* return error */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_get_self_test(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_SELF_TEST, &prev, 1);         /* read self test */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read self test failed.\n");          /* read self test failed */
       
        return 1;                                                          /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 0) & 0x01);                        /* get the bool */
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_set_activity_threshold(adxl362_handle_t *handle, uint16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (threshold > 0x07FF)                                                        /* check the threshold */
    {
        handle->debug_print("adxl362: threshold is invalid.\n");                   /* threshold is invalid */
       
        return 4;                                                                  /* return error */
    }
    
    buf[0] = threshold & 0xFF;                                                     /* set lsb */
    buf[1] = (threshold >> 8) & 0xFF;                                              /* set msb */
    res = a_adxl362_write(handle, ADXL362_REG_THRESH_ACT_L, buf, 2);               /* write activity threshold */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("adxl362: write activity threshold failed.\n");        /* write activity threshold failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t adxl362_get_activity_threshold(adxl362_handle_t *handle, uint16_t *threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_THRESH_ACT_L, buf, 2);               /* read activity threshold */
    if (res != 0)                                                                 /* check the result */
    {
        handle->debug_print("adxl362: read activity threshold failed.\n");        /* read activity threshold failed */
       
        return 1;                                                                 /* return error */
    }
    *threshold = (uint16_t)((uint16_t)buf[1] << 8) | buf[0];                      /* get the threshold */
    *threshold &= 0x07FF;                                                         /* get the valid part */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t adxl362_activity_threshold_convert_to_register(adxl362_handle_t *handle, float g, uint16_t *reg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    if (g - 0.0f < 1e-6)                                                   /* check g > 0 */
    {
        handle->debug_print("adxl362: g is invalid.\n");                   /* g is invalid */
       
        return 4;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 6) & 0x3;                                             /* get the range */
    if (range == 0)                                                        /* 2g */
    {
        *reg = (uint16_t)(g * 1000.0f);                                    /* convert data */
    }
    else if (range == 1)                                                   /* 4g */
    {
        *reg = (uint16_t)(g * 500.0f);                                     /* convert data */
    }
    else                                                                   /* 8g */
    {
        *reg = (uint16_t)(g * 235.0f);                                     /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_activity_threshold_convert_to_data(adxl362_handle_t *handle, uint16_t reg, float *g)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 6) & 0x3;                                             /* get the range */
    if (range == 0)                                                        /* 2g */
    {
        *g = (float)((float)reg / 1000.0f);                                /* convert data */
    }
    else if (range == 1)                                                   /* 4g */
    {
        *g = (float)((float)reg / 500.0f);                                 /* convert data */
    }
    else                                                                   /* 8g */
    {
        *g = (float)((float)reg / 235.0f);                                 /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_set_activity_time(adxl362_handle_t *handle, uint8_t tim)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    prev = tim;                                                               /* set the activity time */
    res = a_adxl362_write(handle, ADXL362_REG_TIME_ACT, &prev, 1);            /* write activity time */
    if (res != 0)                                                             /* check the result */
    {
        handle->debug_print("adxl362: write activity time failed.\n");        /* write activity time failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t adxl362_get_activity_time(adxl362_handle_t *handle, uint8_t *tim)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_TIME_ACT, &prev, 1);            /* read activity time */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("adxl362: read activity time failed.\n");        /* read activity time failed */
       
        return 1;                                                            /* return error */
    }
    *tim = prev;                                                             /* get the activity time */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t adxl362_activity_time_convert_to_register(adxl362_handle_t *handle, float ms, uint8_t *reg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    if (ms - 0.0f < 1e-6)                                                  /* check ms > 0 */
    {
        handle->debug_print("adxl362: ms is invalid.\n");                  /* ms is invalid */
       
        return 4;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 0) & 0x7;                                             /* get the range */
    if (range == 0)                                                        /* 12.5Hz */
    {
        *reg = (uint8_t)(ms / 1000.0f * 12.5f);                            /* convert data */
    }
    else if (range == 1)                                                   /* 25Hz */
    {
        *reg = (uint8_t)(ms / 1000.0f * 25.0f);                            /* convert data */
    }
    else if (range == 2)                                                   /* 50Hz */
    {
        *reg = (uint8_t)(ms / 1000.0f * 50.0f);                            /* convert data */
    }
    else if (range == 3)                                                   /* 100Hz */
    {
        *reg = (uint8_t)(ms / 1000.0f * 100.0f);                           /* convert data */
    }
    else if (range == 4)                                                   /* 200Hz */
    {
        *reg = (uint8_t)(ms / 1000.0f * 200.0f);                           /* convert data */
    }
    else                                                                   /* 400Hz */
    {
        *reg = (uint8_t)(ms / 1000.0f * 400.0f);                           /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_activity_time_convert_to_data(adxl362_handle_t *handle, uint8_t reg, float *ms)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 0) & 0x7;                                             /* get the range */
    if (range == 0)                                                        /* 12.5Hz */
    {
        *ms = (float)((float)reg / 12.5f * 1000.0f);                       /* convert data */
    }
    else if (range == 1)                                                   /* 25Hz */
    {
        *ms = (float)((float)reg / 25.0f * 1000.0f);                       /* convert data */
    }
    else if (range == 2)                                                   /* 50Hz */
    {
        *ms = (float)((float)reg / 50.0f * 1000.0f);                       /* convert data */
    }
    else if (range == 3)                                                   /* 100Hz */
    {
        *ms = (float)((float)reg / 100.0f * 1000.0f);                      /* convert data */
    }
    else if (range == 4)                                                   /* 200Hz */
    {
        *ms = (float)((float)reg / 200.0f * 1000.0f);                      /* convert data */
    }
    else                                                                   /* 400Hz */
    {
        *ms = (float)((float)reg / 400.0f * 1000.0f);                      /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_set_inactivity_threshold(adxl362_handle_t *handle, uint16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    if (threshold > 0x07FF)                                                          /* check the threshold */
    {
        handle->debug_print("adxl362: threshold is invalid.\n");                     /* threshold is invalid */
       
        return 4;                                                                    /* return error */
    }
    
    buf[0] = threshold & 0xFF;                                                       /* set lsb */
    buf[1] = (threshold >> 8) & 0xFF;                                                /* set msb */
    res = a_adxl362_write(handle, ADXL362_REG_THRESH_INACT_L, buf, 2);               /* write inactivity threshold */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("adxl362: write inactivity threshold failed.\n");        /* write inactivity threshold failed */
       
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t adxl362_get_inactivity_threshold(adxl362_handle_t *handle, uint16_t *threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_THRESH_INACT_L, buf, 2);               /* read inactivity threshold */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("adxl362: read inactivity threshold failed.\n");        /* read inactivity threshold failed */
       
        return 1;                                                                   /* return error */
    }
    *threshold = (uint16_t)((uint16_t)buf[1] << 8) | buf[0];                        /* get the threshold */
    *threshold &= 0x07FF;                                                           /* get the valid part */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t adxl362_inactivity_threshold_convert_to_register(adxl362_handle_t *handle, float g, uint16_t *reg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    if (g - 0.0f < 1e-6)                                                   /* check g > 0 */
    {
        handle->debug_print("adxl362: g is invalid.\n");                   /* g is invalid */
       
        return 4;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 6) & 0x3;                                             /* get the range */
    if (range == 0)                                                        /* 2g */
    {
        *reg = (uint16_t)(g * 1000.0f);                                    /* convert data */
    }
    else if (range == 1)                                                   /* 4g */
    {
        *reg = (uint16_t)(g * 500.0f);                                     /* convert data */
    }
    else                                                                   /* 8g */
    {
        *reg = (uint16_t)(g * 235.0f);                                     /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_inactivity_threshold_convert_to_data(adxl362_handle_t *handle, uint16_t reg, float *g)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 6) & 0x3;                                             /* get the range */
    if (range == 0)                                                        /* 2g */
    {
        *g = (float)((float)reg / 1000.0f);                                /* convert data */
    }
    else if (range == 1)                                                   /* 4g */
    {
        *g = (float)((float)reg / 500.0f);                                 /* convert data */
    }
    else                                                                   /* 8g */
    {
        *g = (float)((float)reg / 235.0f);                                 /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_set_inactivity_time(adxl362_handle_t *handle, uint16_t tim)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    buf[0] = tim & 0xFF;                                                        /* set lsb */
    buf[1] = (tim >> 8) & 0xFF;                                                 /* set msb */
    res = a_adxl362_write(handle, ADXL362_REG_TIME_INACT_L, buf, 2);            /* write inactivity time */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("adxl362: write inactivity time failed.\n");        /* write inactivity time failed */
       
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t adxl362_get_inactivity_time(adxl362_handle_t *handle, uint16_t *tim)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_TIME_INACT_L, buf, 2);            /* read inactivity time */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("adxl362: read inactivity time failed.\n");        /* read inactivity time failed */
       
        return 1;                                                              /* return error */
    }
    *tim = (uint16_t)((uint16_t)buf[1] << 8) | buf[0];                         /* get the threshold */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t adxl362_inactivity_time_convert_to_register(adxl362_handle_t *handle, float ms, uint16_t *reg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    if (ms - 0.0f < 1e-6)                                                  /* check ms > 0 */
    {
        handle->debug_print("adxl362: ms is invalid.\n");                  /* ms is invalid */
       
        return 4;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 0) & 0x7;                                             /* get the range */
    if (range == 0)                                                        /* 12.5Hz */
    {
        *reg = (uint16_t)(ms / 1000.0f * 12.5f);                           /* convert data */
    }
    else if (range == 1)                                                   /* 25Hz */
    {
        *reg = (uint16_t)(ms / 1000.0f * 25.0f);                           /* convert data */
    }
    else if (range == 2)                                                   /* 50Hz */
    {
        *reg = (uint16_t)(ms / 1000.0f * 50.0f);                           /* convert data */
    }
    else if (range == 3)                                                   /* 100Hz */
    {
        *reg = (uint16_t)(ms / 1000.0f * 100.0f);                          /* convert data */
    }
    else if (range == 4)                                                   /* 200Hz */
    {
        *reg = (uint16_t)(ms / 1000.0f * 200.0f);                          /* convert data */
    }
    else                                                                   /* 400Hz */
    {
        *reg = (uint16_t)(ms / 1000.0f * 400.0f);                          /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_inactivity_time_convert_to_data(adxl362_handle_t *handle, uint16_t reg, float *ms)
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 0) & 0x7;                                             /* get the range */
    if (range == 0)                                                        /* 12.5Hz */
    {
        *ms = (float)((float)reg / 12.5f * 1000.0f);                       /* convert data */
    }
    else if (range == 1)                                                   /* 25Hz */
    {
        *ms = (float)((float)reg / 25.0f * 1000.0f);                       /* convert data */
    }
    else if (range == 2)                                                   /* 50Hz */
    {
        *ms = (float)((float)reg / 50.0f * 1000.0f);                       /* convert data */
    }
    else if (range == 3)                                                   /* 100Hz */
    {
        *ms = (float)((float)reg / 100.0f * 1000.0f);                      /* convert data */
    }
    else if (range == 4)                                                   /* 200Hz */
    {
        *ms = (float)((float)reg / 200.0f * 1000.0f);                      /* convert data */
    }
    else                                                                   /* 400Hz */
    {
        *ms = (float)((float)reg / 400.0f * 1000.0f);                      /* convert data */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_set_detect_mode(adxl362_handle_t *handle, adxl362_detect_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(3 << 4);                                                                  /* clear settings */
    prev |= (mode << 4);                                                                /* set the mode */
    res = a_adxl362_write(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                 /* write action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: write action inaction control failed.\n");        /* write action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_get_detect_mode(adxl362_handle_t *handle, adxl362_detect_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    *mode = (adxl362_detect_mode_t)((prev >> 4) & 0x03);                                /* get the detect mode */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_set_inactivity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t trigger)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 3);                                                                  /* clear settings */
    prev |= (trigger << 3);                                                             /* set the trigger */
    res = a_adxl362_write(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                 /* write action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: write action inaction control failed.\n");        /* write action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_get_inactivity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t *trigger)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    *trigger = (adxl362_detect_trigger_t)((prev >> 3) & 0x01);                          /* get the trigger */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_set_activity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t trigger)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 1);                                                                  /* clear settings */
    prev |= (trigger << 1);                                                             /* set the trigger */
    res = a_adxl362_write(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                 /* write action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: write action inaction control failed.\n");        /* write action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_get_activity_detect_trigger(adxl362_handle_t *handle, adxl362_detect_trigger_t *trigger)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    *trigger = (adxl362_detect_trigger_t)((prev >> 1) & 0x01);                          /* get the trigger */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_set_inactivity(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 2);                                                                  /* clear settings */
    prev |= (enable << 2);                                                              /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                 /* write action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: write action inaction control failed.\n");        /* write action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_get_inactivity(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 2) & 0x01);                                     /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_set_activity(adxl362_handle_t *handle, adxl362_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    prev &= ~(1 << 0);                                                                  /* clear settings */
    prev |= (enable << 0);                                                              /* set the bool */
    res = a_adxl362_write(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                 /* write action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: write action inaction control failed.\n");        /* write action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_get_activity(adxl362_handle_t *handle, adxl362_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_ACT_INACT_CTL, &prev, 1);                  /* read action inaction control */
    if (res != 0)                                                                       /* check the result */
    {
        handle->debug_print("adxl362: read action inaction control failed.\n");         /* read action inaction control failed */
       
        return 1;                                                                       /* return error */
    }
    *enable = (adxl362_bool_t)((prev >> 0) & 0x01);                                     /* get the bool */
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t adxl362_read(adxl362_handle_t *handle, int16_t raw[3], float g[3])
{
    uint8_t res;
    uint8_t prev;
    uint8_t range;
    uint8_t buf[6];
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_XDATA_L, buf, 6);             /* read x, y and z */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read x, y and z failed.\n");         /* read x, y and z failed */
       
        return 1;                                                          /* return error */
    }
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 6) & 0x3;                                             /* get the range */
    if (range == 0)                                                        /* 2g */
    {
        raw[0] = (int16_t)((uint16_t)(buf[1]) << 8 | buf[0]);              /* set x */
        raw[1] = (int16_t)((uint16_t)(buf[3]) << 8 | buf[2]);              /* set y */
        raw[2] = (int16_t)((uint16_t)(buf[5]) << 8 | buf[4]);              /* set z */
        g[0] = (float)raw[0] / 1000.0f;                                    /* convert x */
        g[1] = (float)raw[1] / 1000.0f;                                    /* convert y */
        g[2] = (float)raw[2] / 1000.0f;                                    /* convert z */
    }
    else if (range == 1)                                                   /* 4g */
    {
        raw[0] = (int16_t)((uint16_t)(buf[1]) << 8 | buf[0]);              /* set x */
        raw[1] = (int16_t)((uint16_t)(buf[3]) << 8 | buf[2]);              /* set y */
        raw[2] = (int16_t)((uint16_t)(buf[5]) << 8 | buf[4]);              /* set z */
        g[0] = (float)raw[0] / 500.0f;                                     /* convert x */
        g[1] = (float)raw[1] / 500.0f;                                     /* convert y */
        g[2] = (float)raw[2] / 500.0f;                                     /* convert z */
    }
    else                                                                   /* 8g */
    {
        raw[0] = (int16_t)((uint16_t)(buf[1]) << 8 | buf[0]);              /* set x */
        raw[1] = (int16_t)((uint16_t)(buf[3]) << 8 | buf[2]);              /* set y */
        raw[2] = (int16_t)((uint16_t)(buf[5]) << 8 | buf[4]);              /* set z */
        g[0] = (float)raw[0] / 235.0f;                                     /* convert x */
        g[1] = (float)raw[1] / 235.0f;                                     /* convert y */
        g[2] = (float)raw[2] / 235.0f;                                     /* convert z */
    }
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_read_fifo(adxl362_handle_t *handle, adxl362_frame_t *frame, uint16_t *frame_len)
{
    uint8_t res;
    uint8_t type;
    uint8_t prev;
    uint8_t range;
    uint8_t buf[2];
    uint16_t i;
    uint16_t len;
    
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->inited != 1)                                               /* check handle initialization */
    {
        return 3;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_ENTRIES_L, buf, 2);      /* read fifo entries */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read fifo entries failed.\n");       /* read fifo entries failed */
       
        return 1;                                                          /* return error */
    }
    len = (uint16_t)((uint16_t)buf[1] << 8) | buf[0];                      /* set the length */
    if (len > 512)                                                         /* check the length */
    {
        len = 512;                                                         /* set limit to 512 */
    }
    res = a_adxl362_read(handle, ADXL362_REG_FIFO_CONTROL, &prev, 1);      /* read fifo control */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read fifo control failed.\n");       /* read fifo control failed */
       
        return 1;                                                          /* return error */
    }
    len = len < (*frame_len) ? len : (*frame_len);                         /* adjust to buffer */
    if (((prev >> 2) & 0x01) != 0)                                         /* fifo has temperature */
    {
        len = (len / 4) * 4;                                               /* data alignment */
    }
    else                                                                   /* no temperature */
    {
        len = (len / 3) * 3;                                               /* data alignment */
    }
    res = a_adxl362_read_fifo(handle, handle->buf, len * 2);               /* read from fifo */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read fifo failed.\n");               /* read fifo failed */
       
        return 1;                                                          /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_FILTER_CTL, &prev, 1);        /* read filter */
    if (res != 0)                                                          /* check the result */
    {
        handle->debug_print("adxl362: read filter failed.\n");             /* read filter failed */
       
        return 1;                                                          /* return error */
    }
    range = (prev >> 6) & 0x3;                                             /* get the range */
    for (i = 0; i < len; i++)                                              /* copy data */
    {
        frame[i].raw = (uint16_t)(handle->buf[i * 2 + 1]) << 8 | 
                        handle->buf[i * 2];                                /* get the raw data */
        type = (frame[i].raw >> 14) & 0x03;                                /* get the type */
        frame[i].raw &= 0x3FFF;                                            /* get the valid part */
        if (((frame[i].raw >> 12) & 0x01) != 0)                            /* check the sign extension */
        {
            frame[i].raw |= (int16_t)((uint16_t)0x3 << 14);                /* set the sign extension */
        }
        frame[i].type = (adxl362_frame_type_t)(type);                      /* get the type */
        if (frame[i].type == ADXL362_FRAME_TYPE_TEMP)                      /* if temperature */
        {
            frame[i].data = (float)frame[i].raw * 0.065f;                  /* convert data */
        }
        else
        {
            if (range == 0)                                                /* 2g */
            {
                frame[i].data = (float)frame[i].raw / 1000.0f;             /* convert data */
            }
            else if (range == 1)                                           /* 4g */
            {
                frame[i].data = (float)frame[i].raw / 500.0f;              /* convert data */
            }
            else                                                           /* 8g */
            {
                frame[i].data = (float)frame[i].raw / 235.0f;              /* convert data */
            }
        }
    }
    *frame_len = len;                                                      /* set frame length */
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t adxl362_irq_handler(adxl362_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = a_adxl362_read(handle, ADXL362_REG_STATUS, (uint8_t *)&prev, 1);       /* read config */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("adxl362: read failed.\n");                          /* read failed */
        
        return 1;                                                                /* return error */
    }
    if ((prev & ADXL362_STATUS_ERR_USER_REGS) != 0)                              /* if seu error detect */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_ERR_USER_REGS);              /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_AWAKE) != 0)                                      /* if awake */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_AWAKE);                      /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_INACT) != 0)                                      /* if inactivity */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_INACT);                      /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_ACT) != 0)                                        /* if activity */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_ACT);                        /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_FIFO_OVERRUN) != 0)                               /* if fifo overrun */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            (void)a_adxl362_read_fifo(handle, handle->buf, 512 * 2);             /* clear the fifo */
            handle->receive_callback(ADXL362_STATUS_FIFO_OVERRUN);               /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_FIFO_WATERMARK) != 0)                             /* if fifo watermark */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_FIFO_WATERMARK);             /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_FIFO_READY) != 0)                                 /* if fifo ready */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_FIFO_READY);                 /* run callback */
        }
    }
    if ((prev & ADXL362_STATUS_DATA_READY) != 0)                                 /* if data ready */
    {
        if (handle->receive_callback != NULL)                                    /* if receive callback */
        {
            handle->receive_callback(ADXL362_STATUS_DATA_READY);                 /* run callback */
        }
    }
    
    return 0;                                                                    /* success return 0 */
}

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
uint8_t adxl362_set_reg(adxl362_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    }
    
    return a_adxl362_write(handle, reg, buf, len);        /* write data */
}

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
uint8_t adxl362_get_reg(adxl362_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    return a_adxl362_read(handle, reg, buf, len);       /* read data */
}

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
uint8_t adxl362_get_fifo(adxl362_handle_t *handle, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    return a_adxl362_read_fifo(handle, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an adxl362 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t adxl362_info(adxl362_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(adxl362_info_t));                        /* initialize adxl362 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
