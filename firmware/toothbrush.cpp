#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdarg.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "ws2812.h"

void update_leds(void)
{
    ws2812_sendarray((uint8_t *)g_led_buffer, 3 * NUM_LEDS);
}

void clear_led(uint8_t index)
{
    g_color_buffer[index].r = 0;
    g_color_buffer[index].g = 0;
    g_color_buffer[index].b = 0;
    g_led_buffer[index].r = 0;
    g_led_buffer[index].g = 0;
    g_led_buffer[index].b = 0;
}

void get_led(uint8_t index, color_t *col)
{
    if (index >= NUM_LEDS)
        return;

    col->r = (uint8_t)(g_color_buffer[index].r >> COLOR_SHIFT);
    col->g = (uint8_t)(g_color_buffer[index].g >> COLOR_SHIFT);
    col->b = (uint8_t)(g_color_buffer[index].b >> COLOR_SHIFT);
}

void set_led(uint8_t index, color_t *col)
{
    g_color_buffer[index].r = col->r << COLOR_SHIFT;
    g_color_buffer[index].g = col->g << COLOR_SHIFT;
    g_color_buffer[index].b = col->b << COLOR_SHIFT;

    g_led_buffer[index].r = ((g_color_buffer[index].r * g_brightness / 100) >> COLOR_SHIFT) & 0xFF;
    g_led_buffer[index].g = ((g_color_buffer[index].g * g_brightness / 100) >> COLOR_SHIFT) & 0xFF;
    g_led_buffer[index].b = ((g_color_buffer[index].b * g_brightness / 100) >> COLOR_SHIFT) & 0xFF;
}

void set_led_rgb(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    color_t temp;

    temp.r = r;
    temp.g = g;
    temp.b = b;
    set_led(index, &temp);
}

int main(void)
{ 
    for(;;)
        ;

    return 0;
}
