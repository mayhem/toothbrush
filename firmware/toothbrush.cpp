#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdarg.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "ws2812.h"

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

#define LED_PIN 3

void set_color(uint8_t red, uint8_t blue, uint8_t green) 
{
    uint8_t buffer[3];

    buffer[0] = red;
    buffer[1] = blue;
    buffer[2] = green;
    ws2812_sendarray((uint8_t *)buffer, 3);
}

void startup_animation(void)
{
    uint8_t i;

    for(i = 0; i < 5; i++)
    {
        set_color(255, 150, 0);
        _delay_ms(50);
        set_color(255, 0, 255);
        _delay_ms(50);
    }
    set_color(0, 0, 32);
}

int main(void)
{ 
    set_output(DDRD, LED_PIN);
    startup_animation();

    for(;;)
        ;

    return 0;
}
