[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL362

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl362/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL362는 초저전력 3축 MEMS 가속도계로, 100Hz 출력 데이터 속도에서 2μA 미만을 소비하고 모션 트리거 웨이크업 모드에서는 270nA를 소비합니다. 낮은 전력 소비를 달성하기 위해 전력 듀티 사이클을 사용하는 가속도계와 달리 ADXL362는 언더샘플링으로 입력 신호를 앨리어스하지 않습니다. 모든 데이터 속도에서 센서의 전체 대역폭을 샘플링합니다. ADXL362는 항상 12비트 출력 분해능을 제공합니다. 낮은 해상도로도 충분할 때 보다 효율적인 단일 바이트 전송을 위해 8비트 형식의 데이터도 제공됩니다. ±2g, ±4g 및 ±8g의 측정 범위를 사용할 수 있으며 ±2g 범위에서 1mg/LSB의 분해능을 제공합니다. ADXL362의 일반 550μg/√Hz보다 낮은 잡음 수준이 필요한 응용 제품의 경우 공급 전류를 최소한으로 증가시키면서 두 가지 더 낮은 잡음 모드(일반적으로 175μg/√Hz까지) 중 하나를 선택할 수 있습니다. 초저전력 소모인 ADXL362에는 진정한 시스템 수준의 전력 감소를 가능하게 하는 많은 기능이 있습니다. 여기에는 심층 다중 모드 출력 FIFO, 내장형 마이크로파워 온도 센서, 6Hz(근사치) 측정 속도에서 최저 270nA까지 실행할 수 있는 조정 가능한 임계 절전 및 웨이크업 작동을 포함한 여러 활동 감지 모드가 포함됩니다. 원하는 경우 활동이 감지되면 외부 스위치를 직접 제어하기 위해 핀 출력이 제공됩니다. 또한 ADXL362에는 샘플링 시간 및/또는 외부 클록의 외부 제어 기능이 있습니다.

LibDriver ADXL362는 LibDriver에서 시작한 adxl362의 전체 기능 드라이버입니다. 가속 판독, 가속 FIFO 모드 획득, 활성/비활성 상태 감지 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example motion](#example-motion)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver ADXL362의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver ADXL362용 플랫폼 독립적인 SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver ADXL362드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver ADXL362프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver ADXL362오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 ADXL362데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인SPI버스 템플릿을 참조하여 지정된 플랫폼에 대한SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

```c
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

### 문서

온라인 문서: [https://www.libdriver.com/docs/adxl362/index.html](https://www.libdriver.com/docs/adxl362/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.