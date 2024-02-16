[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver ADXL362

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/adxl362/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der ADXL362 ist ein extrem stromsparender 3-Achsen-MEMS-Beschleunigungsmesser, der weniger als 2 μA bei einer Ausgangsdatenrate von 100 Hz und 270 nA im bewegungsgesteuerten Weckmodus verbraucht. Im Gegensatz zu Beschleunigungsmessern, die Power Duty Cycling verwenden, um einen geringen Stromverbrauch zu erreichen, aliasiert der ADXL362 Eingangssignale nicht durch Undersampling; es tastet die volle Bandbreite des Sensors bei allen Datenraten ab. Der ADXL362 bietet immer eine Ausgangsauflösung von 12 Bit; 8-Bit-formatierte Daten werden auch für effizientere Einzelbyte-Übertragungen bereitgestellt, wenn eine niedrigere Auflösung ausreicht. Messbereiche von ±2 g, ±4 g und ±8 g sind verfügbar, mit einer Auflösung von 1 mg/LSB im Bereich von ±2 g. Für Anwendungen, bei denen ein niedrigerer Rauschpegel als die normalen 550 μg/√Hz des ADXL362 gewünscht wird, kann bei minimaler Erhöhung des Versorgungsstroms einer von zwei niedrigeren Rauschmodi (bis zu 175 μg/√Hz typisch) ausgewählt werden Aufgrund seines extrem niedrigen Stromverbrauchs verfügt der ADXL362 über viele Funktionen, um eine echte Leistungsreduzierung auf Systemebene zu ermöglichen. Es enthält einen tiefen Multimode-Ausgangs-FIFO, einen eingebauten Micropower-Temperatursensor und mehrere Aktivitätserkennungsmodi, einschließlich einstellbarem Schwellen-Schlaf- und Aufwachbetrieb, der bis zu 270 nA bei einer Messrate von 6 Hz (ungefähr) laufen kann. Ein Pin-Ausgang ist vorgesehen, um bei Bedarf einen externen Schalter direkt zu steuern, wenn Aktivität erkannt wird. Darüber hinaus verfügt der ADXL362 über Vorkehrungen für die externe Steuerung der Abtastzeit und/oder einer externen Uhr.

LibDriver ADXL362 ist der voll funktionsfähige Treiber von adxl362, der von LibDriver gestartet wurde. Es bietet Beschleunigungsablesung, Beschleunigungs-FIFO-Moduserfassung, Aktivitäts-/Inaktivitätszustandserkennung und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example motion](#example-motion)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver ADXL362-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver ADXL362 SPI.

/test enthält den Testcode des LibDriver ADXL362-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver ADXL362-Beispielcode.

/doc enthält das LibDriver ADXL362-Offlinedokument.

/Datenblatt enthält ADXL362-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/adxl362/index.html](https://www.libdriver.com/docs/adxl362/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.