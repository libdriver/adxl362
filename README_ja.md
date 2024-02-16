[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL362

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl362/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

ADXL362 は超低消費電力の 3 軸 MEMS 加速度センサーで、消費電力は 100 Hz の出力データ レートで 2 μA 未満、モーション トリガー ウェイクアップ モードでは 270 nA です。 パワー デューティ サイクリングを使用して低消費電力を実現する加速度計とは異なり、ADXL362 はアンダーサンプリングによって入力信号をエイリアシングしません。 すべてのデータ レートでセンサーの全帯域幅をサンプリングします。ADXL362 は常に 12 ビットの出力分解能を提供します。 より低い解像度で十分な場合、より効率的なシングルバイト転送のために、8 ビット形式のデータも提供されます。 ±2 g、±4 g、および ±8 g の測定範囲が利用可能で、±2 g 範囲で 1 mg/LSB の分解能があります。 ADXL362 の通常の 550 μg/√Hz よりも低いノイズ レベルが必要なアプリケーションでは、供給電流の増加を最小限に抑えて、2 つの低ノイズ モード (標準で 175 μg/√Hz まで) のいずれかを選択できます。 超低消費電力の ADXL362 には、真のシステム レベルの電力削減を可能にする多くの機能があります。 これには、ディープ マルチモード出力 FIFO、内蔵のマイクロパワー温度センサー、および 6 Hz (概算) 測定レートで 270 nA という低さで実行できる調整可能なしきい値のスリープおよびウェイクアップ動作を含むいくつかのアクティビティ検出モードが含まれています。 必要に応じて、アクティビティが検出されたときに外部スイッチを直接制御するためのピン出力が提供されます。 さらに、ADXL362 には、サンプリング時間および/または外部クロックの外部制御機能があります。

LibDriver ADXL362 は、LibDriver によって起動される adxl362 のフル機能ドライバーです。 アクセラレーションの読み取り、アクセラレーション FIFO モードの取得、アクティブ/非アクティブ状態の検出などの機能を提供します。 LibDriver は MISRA 準拠です。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example motion](#example-motion)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver ADXL362のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver ADXL362用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver ADXL362ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver ADXL362プログラミング例が含まれています。

/ docディレクトリには、LibDriver ADXL362オフラインドキュメントが含まれています。

/ datasheetディレクトリには、ADXL362データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/adxl362/index.html](https://www.libdriver.com/docs/adxl362/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。