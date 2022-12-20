#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdarg.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <time.h>
#include "ws2812.h"

// Bit manipulation macros
#define sbi(a, b) ((a) |= 1 << (b))       //sets bit B in variable A
#define cbi(a, b) ((a) &= ~(1 << (b)))    //clears bit B in variable A
#define tbi(a, b) ((a) ^= 1 << (b))       //toggles bit B in variable A

#define TIMER1_INIT      0xFFEF
#define TIMER1_FLAGS     _BV(CS12)|(1<<CS10); // 8Mhz / 1024 / 8 = .001024 per tick

volatile uint32_t button_down_time = 0;
volatile uint32_t g_time = 0;

ISR (TIMER1_OVF_vect)
{
    g_time++;
    TCNT1 = TIMER1_INIT;
}

ISR(INT0_vect)
{
    if (PIND & (1<<PIND2))
        if (button_down_time == 0)
            button_down_time = g_time;
}

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

void delay(int16_t d)
{
    for(int i = 0; i < d; i++)
    {
        _delay_ms(1);
    }
}

void enable_motor(uint8_t state)
{
    if (state)
        sbi(PORTB, PB1);
    else
        cbi(PORTB, PB1);
}


int main(void)
{ 
    // Set outputs
    sbi(DDRD, PD3);
    sbi(DDRB, PB1);

    // turn off motor
    cbi(PORTB, PB1);

    // Turn on pull up
    sbi(PORTD, PD2);

    startup_animation();

    // enable INT0
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);

    // enable timer for clock
    TCCR1B |= TIMER1_FLAGS;
    TCNT1 = TIMER1_INIT;
    TIMSK1 |= (1<<TOIE1);

    sei();

    uint8_t state = 0;
    for(;;)
    {
        cli();
        uint8_t button_time = button_down_time;
        sei();

        if (button_time != 0)
        {
            if (state == 0)
            {
                state = 1;
                set_color(128,0,0);
            }
            else
            {
                state = 0;
                set_color(0,0,0);
            }
            cli();
            button_down_time = 0;
            sei();
        } 
    }

    return 0;
}
