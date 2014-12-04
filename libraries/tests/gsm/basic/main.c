#include <stdio.h>
#include "gpio_api.h"
#include "wait_api.h"
#include "us_ticker_api.h"

static gpio_t led;
static ticker_event_t ticker;
static uint32_t tick_ms = 0;

void ISR_ticker_handler(uint32_t id) {
    us_ticker_insert_event(&ticker, us_ticker_read() + 1000, (uint32_t) &ticker);
    // Toggling led every second
    tick_ms = (tick_ms + 1) % 1000;
    if(tick_ms == 0) {
        gpio_write(&led, !gpio_read(&led));
    }
}

int main(void)
{
    gpio_init_out_ex(&led, LED, 0);

    us_ticker_init();
    us_ticker_set_handler(&ISR_ticker_handler);
    us_ticker_insert_event(&ticker, us_ticker_read() + 1000, (uint32_t) &ticker);

    while(1);
}