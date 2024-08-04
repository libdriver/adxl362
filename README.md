[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL362

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl362/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The ADXL362 is an ultralow power, 3-axis MEMS accelerometer that consumes less than 2 μA at a 100 Hz output data rate and 270 nA when in motion triggered wake-up mode. Unlike accelerometers that use power duty cycling to achieve low power consumption, the ADXL362 does not alias input signals by undersampling; it samples the full bandwidth of the sensor at all data rates.The ADXL362 always provides 12-bit output resolution; 8-bit formatted data is also provided for more efficient single-byte transfers when a lower resolution is sufficient. Measurement ranges of ±2 g, ±4 g, and ±8 g are available, with a resolution of 1 mg/LSB on the ±2 g range. For applications where a noise level lower than the normal 550 μg/√Hz of the ADXL362 is desired, either of two lower noise modes (down to 175 μg/√Hz typical) can be selected at minimal increase in supply current.In addition to its ultralow power consumption, the ADXL362 has many features to enable true system level power reduction. It includes a deep multimode output FIFO, a built-in micropower temperature sensor, and several activity detection modes including adjustable threshold sleep and wake-up operation that can run as low as 270 nA at a 6 Hz (approximate) measurement rate. A pin output is provided to directly control an external switch when activity is detected, if desired. In addition, the ADXL362 has provisions for external control of sampling time and/or an external clock.

LibDriver ADXL362 is the full function driver of adxl362 launched by LibDriver. It provides acceleration reading, acceleration FIFO mode acquisition, activity /inactivity state detection and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example motion](#example-motion)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver ADXL362 source files.

/interface includes LibDriver ADXL362 SPI platform independent template.

/test includes LibDriver ADXL362 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver ADXL362 sample code.

/doc includes LibDriver ADXL362 offline document.

/datasheet includes ADXL362 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface SPI platform independent template and finish your platform SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_adxl362_basic.h"

uint8_t res;
uint32_t i;
float g[3];

/* basic init */
res = adxl362_basic_init();
if (res != 0)
{
    return 1;
}

...
    
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
    
    ...
}

...
    
/* basic deinit */
(void)adxl362_basic_deinit();

return 0;
```

#### example fifo

```C
#include "driver_adxl362_fifo.h"

uint8_t res;
uint8_t g_flag;
uint32_t timeout;
uint8_t (*g_gpio_irq)(void) = NULL;

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

...
    
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
    
    ...
}

...
    
/* gpio deinit */
(void)gpio_interrupt_deinit();

/* fifo deinit */
(void)adxl362_fifo_deinit();
g_gpio_irq = NULL;

return 0;
```

#### example motion

```C
#include "driver_adxl362_motion.h"

uint8_t res;
uint8_t g_flag;
uint32_t timeout;
uint8_t (*g_gpio_irq)(void) = NULL;

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

...
    
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
    
    ...
}

...
    
/* gpio deinit */
(void)gpio_interrupt_deinit();

/* motion deinit */
(void)adxl362_motion_deinit();
g_gpio_irq = NULL;

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/adxl362/index.html](https://www.libdriver.com/docs/adxl362/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.