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
#define MOTOR_PIN 1

void set_color(uint8_t red, uint8_t green, uint8_t blue) 
{
    uint8_t buffer[3];

    buffer[0] = green;
    buffer[1] = red;
    buffer[2] = blue;
    ws2812_sendarray((uint8_t *)buffer, 3);
}

void startup_animation(void)
{
    uint8_t i;

    for(i = 0; i < 5; i++)
    {
        set_color(64, 26, 0);
        _delay_ms(100);
        set_color(64, 0, 64);
        _delay_ms(100);
    }
    set_color(0, 32, 0);
}

int main(void)
{ 
    set_output(DDRB, MOTOR_PIN);
    set_output(DDRD, LED_PIN);
    output_low(DDRB, MOTOR_PIN);
    startup_animation();

    for(;;)
    {
        set_color(32, 0, 0);
        output_high(DDRB, MOTOR_PIN);
        _delay_ms(250);
        _delay_ms(250);
        _delay_ms(250);
        _delay_ms(250);

        set_color(0, 0, 0);
        output_low(DDRB, MOTOR_PIN);
        _delay_ms(250);
        _delay_ms(250);
        _delay_ms(250);
        _delay_ms(250);
    }

    return 0;
}
