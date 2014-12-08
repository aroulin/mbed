#include <stdio.h>
#include "gpio_api.h"
#include "wait_api.h"
#include "us_ticker_api.h"
#include "serial_api.h"
#include "c027_api.h"

typedef int bool;

static gpio_t led;
static ticker_event_t ticker;
static uint32_t tick_ms = 0;

static serial_t serial_gps;

void ISR_ticker_handler(uint32_t id)
{
    us_ticker_insert_event(&ticker, us_ticker_read() + 1000, (uint32_t) &ticker);
    // Toggling led every second
    tick_ms = (tick_ms + 1) % 1000;
    if(tick_ms == 0) {
        gpio_write(&led, !gpio_read(&led));
    }
}

void ISR_serial_gps(uint32_t id, SerialIrq event)
{
    if (event == RxIrq) {
        char c = serial_getc(&serial_gps);
        if(c == '\r') {
            printf("<CR>");
        } else if(c == '\n') {
            printf("<LF>");
			printf("\n");
        } else {
            printf("%c", c);
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
    c027_gps_powerOn();
    serial_init(&serial_gps, GPSTXD, GPSRXD);
    serial_irq_handler(&serial_gps, &ISR_serial_gps, (uint32_t) &serial_gps);
    serial_irq_set(&serial_gps, RxIrq, 1);

    // Enter command mode
    wait(1);
    serial_puts(&serial_gps, "+++");
    wait(1);
    // Disable DTE flow control
    serial_puts(&serial_gps, "AT&K0\r");

    while(1);
}