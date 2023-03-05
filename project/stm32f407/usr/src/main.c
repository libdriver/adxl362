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
 * @file      main.c
 * @brief     main source file
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

#include "driver_adxl362_basic.h"
#include "driver_adxl362_motion.h"
#include "driver_adxl362_fifo.h"
#include "driver_adxl362_register_test.h"
#include "driver_adxl362_read_test.h"
#include "driver_adxl362_motion_test.h"
#include "driver_adxl362_fifo_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
volatile uint8_t g_flag;                   /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        /* run the gpio riq */
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface fifo callback
 * @param[in] *frame points to a frame buffer
 * @param[in] frame_len is the frame length
 * @note      none
 */
static void a_fifo_callback(adxl362_frame_t *frame, uint16_t frame_len)
{
    uint16_t i;

    /* flag happened */
    g_flag = 1;

    /* output */
    adxl362_interface_debug_print("adxl362: irq fifo watermark with %d.\n", frame_len);

    /* output */
    for (i = 0; i < frame_len; i++)
    {
        if (frame[i].type == ADXL362_FRAME_TYPE_X)
        {
            adxl362_interface_debug_print("adxl362: x axis is %0.2fg.\n", frame[i].data);
        }
        else if (frame[i].type == ADXL362_FRAME_TYPE_Y)
        {
            adxl362_interface_debug_print("adxl362: y axis is %0.2fg.\n", frame[i].data);
        }
        else if (frame[i].type == ADXL362_FRAME_TYPE_Z)
        {
            adxl362_interface_debug_print("adxl362: z axis is %0.2fg.\n", frame[i].data);
        }
        else
        {
            adxl362_interface_debug_print("adxl362: temperature is %0.2fC.\n", frame[i].data);
        }
    }
}

/**
 * @brief     interface motion callback
 * @param[in] type is the irq type
 * @note      none
 */
static void a_motion_callback(uint8_t type)
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
            break;
        }
        case ADXL362_STATUS_ACT :
        {
            /* flag happened */
            g_flag = 1;

            /* output */
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
 * @brief     adxl362 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t adxl362(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"times", required_argument, NULL, 1},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* running times */
            case 1 :
            {
                /* set the times */
                times = atol(optarg);

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;

        /* run register test */
        res = adxl362_register_test();
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;

        /* read test */
        res = adxl362_read_test(times);
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_fifo", type) == 0)
    {
        uint8_t res;

        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the gpio irq */
        g_gpio_irq = adxl362_fifo_test_irq_handler;

        /* run fifo test */
        res = adxl362_fifo_test(times);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;

            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;

        return 0;
    }
    else if (strcmp("t_motion", type) == 0)
    {
        uint8_t res;

        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the gpio irq */
        g_gpio_irq = adxl362_motion_test_irq_handler;

        /* run motion test */
        res = adxl362_motion_test(times);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;

            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;

        return 0;
    }
    else if (strcmp("e_basic", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float g[3];

        /* basic init */
        res = adxl362_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* delay 1000ms */
        adxl362_interface_delay_ms(1000);

        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = adxl362_basic_read((float *)g);
            if (res != 0)
            {
                (void)adxl362_basic_deinit();

                return 1;
            }

            /* output */
            adxl362_interface_debug_print("%d/%d\n", i + 1, times);
            adxl362_interface_debug_print("adxl362: x is %0.3fg.\n", g[0]);
            adxl362_interface_debug_print("adxl362: y is %0.3fg.\n", g[1]);
            adxl362_interface_debug_print("adxl362: z is %0.3fg.\n", g[2]);

            /* delay 1000ms */
            adxl362_interface_delay_ms(1000);
        }

        /* basic deinit */
        (void)adxl362_basic_deinit();

        return 0;
    }
    else if (strcmp("e_fifo", type) == 0)
    {
        uint8_t res;
        uint32_t timeout;

        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the gpio irq */
        g_gpio_irq = adxl362_fifo_irq_handler;

        /* set 0 */
        g_flag = 0;

        /* fifo init */
        res = adxl362_fifo_init(a_fifo_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;

            return 1;
        }

        /* set timeout */
        timeout = 500;

        /* loop */
        while (times != 0)
        {
            /* check the flag */
            if (g_flag != 0)
            {
                g_flag = 0;
                timeout = 500;
                times--;
            }
            timeout--;
            /* check the timeout */
            if (timeout == 0)
            {
                (void)gpio_interrupt_deinit();
                (void)adxl362_fifo_deinit();
                g_gpio_irq = NULL;
                adxl362_interface_debug_print("adxl362: fifo read timeout.\n");

                return 1;
            }

            /* delay 10ms */
            adxl362_interface_delay_ms(10);
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();

        /* fifo deinit */
        (void)adxl362_fifo_deinit();
        g_gpio_irq = NULL;

        return 0;
    }
    else if (strcmp("e_motion", type) == 0)
    {
        uint8_t res;
        uint32_t timeout;

        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the motion irq */
        g_gpio_irq = adxl362_motion_irq_handler;

        /* motion init */
        res = adxl362_motion_init(a_motion_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;

            return 1;
        }

        /* set timeout */
        timeout = 500;

        /* loop */
        while (times != 0)
        {
            /* check the flag */
            if (g_flag != 0)
            {
                adxl362_interface_debug_print("adxl362: motion detect %d.\n", times);
                g_flag = 0;
                timeout = 500;
                times--;
            }
            timeout--;
            /* check the timeout */
            if (timeout == 0)
            {
                (void)gpio_interrupt_deinit();
                (void)adxl362_motion_deinit();
                g_gpio_irq = NULL;
                adxl362_interface_debug_print("adxl362: motion detect timeout.\n");

                return 1;
            }

            /* delay 10ms */
            adxl362_interface_delay_ms(10);
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();

        /* motion deinit */
        (void)adxl362_motion_deinit();
        g_gpio_irq = NULL;

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        adxl362_interface_debug_print("Usage:\n");
        adxl362_interface_debug_print("  adxl362 (-i | --information)\n");
        adxl362_interface_debug_print("  adxl362 (-h | --help)\n");
        adxl362_interface_debug_print("  adxl362 (-p | --port)\n");
        adxl362_interface_debug_print("  adxl362 (-t reg | --test=reg)\n");
        adxl362_interface_debug_print("  adxl362 (-t read | --test=read) [--times=<num>]\n");
        adxl362_interface_debug_print("  adxl362 (-t fifo | --test=fifo) [--times=<num>]\n");
        adxl362_interface_debug_print("  adxl362 (-t motion | --test=motion) [--times=<num>]\n");
        adxl362_interface_debug_print("  adxl362 (-e basic | --example=basic) [--times=<num>]\n");
        adxl362_interface_debug_print("  adxl362 (-e fifo | --example=fifo) [--times=<num>]\n");
        adxl362_interface_debug_print("  adxl362 (-e motion | --example=motion) [--times=<num>]\n");
        adxl362_interface_debug_print("\n");
        adxl362_interface_debug_print("Options:\n");
        adxl362_interface_debug_print("  -e <basic | fifo | motion>, --example=<basic | fifo | motion>\n");
        adxl362_interface_debug_print("                       Run the driver example.\n");
        adxl362_interface_debug_print("  -h, --help           Show the help.\n");
        adxl362_interface_debug_print("  -i, --information    Show the chip information.\n");
        adxl362_interface_debug_print("  -p, --port           Display the pin connections of the current board.\n");
        adxl362_interface_debug_print("  -t <reg | read | fifo | motion>, --test=<reg | read | fifo | motion>\n");
        adxl362_interface_debug_print("                       Run the driver test.\n");
        adxl362_interface_debug_print("      --times=<num>    Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        adxl362_info_t info;

        /* print adxl362 info */
        adxl362_info(&info);
        adxl362_interface_debug_print("adxl362: chip is %s.\n", info.chip_name);
        adxl362_interface_debug_print("adxl362: manufacturer is %s.\n", info.manufacturer_name);
        adxl362_interface_debug_print("adxl362: interface is %s.\n", info.interface);
        adxl362_interface_debug_print("adxl362: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        adxl362_interface_debug_print("adxl362: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        adxl362_interface_debug_print("adxl362: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        adxl362_interface_debug_print("adxl362: max current is %0.2fmA.\n", info.max_current_ma);
        adxl362_interface_debug_print("adxl362: max temperature is %0.1fC.\n", info.temperature_max);
        adxl362_interface_debug_print("adxl362: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        adxl362_interface_debug_print("adxl362: SPI interface SCK connected to GPIOA PIN5.\n");
        adxl362_interface_debug_print("adxl362: SPI interface MISO connected to GPIOA PIN6.\n");
        adxl362_interface_debug_print("adxl362: SPI interface MOSI connected to GPIOA PIN7.\n");
        adxl362_interface_debug_print("adxl362: SPI interface CS connected to GPIOA PIN4.\n");
        adxl362_interface_debug_print("adxl362: INT connected to GPIOB PIN0.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register adxl362 function */
    shell_init();
    shell_register("adxl362", adxl362);
    uart_print("adxl362: welcome to libdriver adxl362.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("adxl362: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("adxl362: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("adxl362: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("adxl362: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("adxl362: param is invalid.\n");
            }
            else
            {
                uart_print("adxl362: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
