#include <stdio.h>
#include "gpio_api.h"
#include "wait_api.h"
#include "us_ticker_api.h"
#include "gps_api.h"
#include "c027_api.h"

static gpio_t led;
static ticker_event_t ticker;
static uint32_t tick_ms = 0;

void ISR_ticker_handler(uint32_t id)
{
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

    c027_init();
    gps_powerOn();
    
    gps_pkt_t gps_pkt;

    int i = 0;
    for(i = 0; i < 5; i++) {
        gps_receiveNMEAPacket(&gps_pkt);
        switch(gps_pkt.type) {
            case GPS_PKT_TYPE_RMC:
                printf("GRMC: %s\n", gps_pkt.data);
            break;
        }
    }
    
    while(1);
}