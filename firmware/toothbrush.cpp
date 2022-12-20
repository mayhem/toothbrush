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

#define TIMER1_INIT      0xFFF8
#define TIMER1_FLAGS     _BV(CS12)|(1<<CS10); // 8Mhz / 1024 / 8 = .001024 per tick
#define BOUNCE_TIME 250

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
    static uint32_t last_button_down_time = 0;

    // Set outputs
    sbi(DDRD, PD3);
sbi(DDRC, PC2);
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
    uint16_t tick_count = 0;
    uint32_t last_ticks = 0;
    for(;;)
    {
        cli();
        uint32_t ticks = g_time;
        sei();

        if (ticks != last_ticks)
            tbi(PORTC, PC2);

        last_ticks = ticks;

#if 0
        cli();
        uint8_t button_time = button_down_time;
        sei();

        if (button_time != 0)
        {
            if (last_button_down_time != 0 && button_time - last_button_down_time < BOUNCE_TIME)
                continue;

            if (state == 0)
            {
                state = 1;
                set_color(32,0,0);
                last_button_down_time = button_time;
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
#endif
    }

    return 0;
}
