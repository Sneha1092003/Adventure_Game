#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "driver/gpio.h" // For gpio_num_t
#include <stdio.h>

typedef struct {
    gpio_num_t rs_pin;
    gpio_num_t en_pin;
    gpio_num_t data_pins[4]; // For 4-bit mode
    int is_4bit_mode;        // 1 if using 4-bit mode
} lcd_config_t;

void lcd_init(lcd_config_t* config);
void lcd_clear(void);
void lcd_display(const char* text);
void lcd_scroll(const char* text, int speed_ms);
void lcd_write_command(uint8_t cmd);
void lcd_write_data(uint8_t data);

#endif // LCD_H
