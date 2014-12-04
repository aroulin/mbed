#include <stdio.h>
#include "gpio_api.h"
#include "wait_api.h"

static gpio_t led;

int main(void)
{
    gpio_init_out_ex(&led, LED, 0);
    while(1) {
        gpio_write(&led, 1);
        wait(1);
        gpio_write(&led, 0);
        wait(1);
    }
}