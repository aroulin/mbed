#include <stdio.h>
#include "gpio_api.h"
#include "wait_api.h"
#include "us_ticker_api.h"
#include "serial_api.h"
#include "C027_api.h"

typedef int bool;

static gpio_t led;
static ticker_event_t ticker;
static uint32_t tick_ms = 0;

static serial_t serial_gsm;

void ISR_ticker_handler(uint32_t id)
{
    us_ticker_insert_event(&ticker, us_ticker_read() + 1000, (uint32_t) &ticker);
    // Toggling led every second
    tick_ms = (tick_ms + 1) % 1000;
    if(tick_ms == 0) {
        gpio_write(&led, !gpio_read(&led));
    }
}

void ISR_serial_gsm(uint32_t id, SerialIrq event)
{
    if (event == RxIrq) {
        char c = serial_getc(&serial_gsm);
        if(c == '\r') {
            printf("GSM: <CR>\n");
        } else if(c == '\n') {
            printf("GSM: <LF>\n");
        } else {
            printf("GSM: %c\n", c);
        }
    }
}

void serial_puts(serial_t *obj, const char *str)
{
    while (*str != 0) {
        serial_putc(obj, *str);
        str++;
    }
}

int main(void)
{
    gpio_init_out_ex(&led, LED, 0);

    us_ticker_init();
    us_ticker_set_handler(&ISR_ticker_handler);
    us_ticker_insert_event(&ticker, us_ticker_read() + 1000, (uint32_t) &ticker);

    c027_init();
    c027_mdm_powerOn(0);
    serial_init(&serial_gsm, MDMTXD, MDMRXD);
    serial_irq_handler(&serial_gsm, &ISR_serial_gsm, (uint32_t) &serial_gsm);
    serial_irq_set(&serial_gsm, RxIrq, 1);

    // Enter command mode
    wait(1);
    serial_puts(&serial_gsm, "+++");
    wait(1);
    // Disable DTE flow control
    serial_puts(&serial_gsm, "AT&K0\r");

    while(1);
}
