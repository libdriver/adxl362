[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL362

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl362/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL362是一款超低功耗的3轴MEMS加速度计，在100 Hz输出数据速率下消耗小于2μA，在运动触发唤醒模式下消耗270 nA。与使用功率占空比循环来实现低功耗的加速计不同，ADXL362不会通过欠采样来混淆输入信号；ADXL362始终提供12位输出分辨率；当较低分辨率足够时，还提供8位格式化数据用于更有效的单字节传输。测量范围为±2 g、±4 g和±8 g，在±2 g范围内分辨率为1 mg/LSB。对于需要低于ADXL362正常550μg/√Hz噪声水平的应用，可以在电源电流最小增加的情况下选择两种较低噪声模式中的一种（典型值为175μg/√Hz）。除了超低功耗外，ADXL362还具有许多功能，可实现真正的系统级功率降低。它包括一个深度多模输出FIFO、一个内置微功率温度传感器和多种活动检测模式，包括可调阈值睡眠和唤醒操作，在6Hz（近似）测量速率下，可低至270nA。如果需要，当检测到活动时，提供引脚输出以直接控制外部开关。此外，ADXL362还提供了采样时间和外部时钟的外部控制。

LibDriver ADXL362是LibDriver推出的ADXL362全功能驱动，该驱动提供加速度读取、加速度FIFO模式采集、运动非运动状态检测等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example motion](#example-motion)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver ADXL362的源文件。

/interface目录包含了LibDriver ADXL362与平台无关的SPI总线模板。

/test目录包含了LibDriver ADXL362驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver ADXL362编程范例。

/doc目录包含了LibDriver ADXL362离线文档。

/datasheet目录包含了ADXL362数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的SPI总线模板，完成指定平台的SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

### 文档

在线文档: [https://www.libdriver.com/docs/adxl362/index.html](https://www.libdriver.com/docs/adxl362/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。