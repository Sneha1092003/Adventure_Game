#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <string.h>

#define LCD_DELAY 5  // Adjust the delay as needed

// Define LCD control pins 
#define LCD_RS     GPIO_NUM_21
#define LCD_EN     GPIO_NUM_22
#define LCD_D4     GPIO_NUM_16
#define LCD_D5     GPIO_NUM_17
#define LCD_D6     GPIO_NUM_18
#define LCD_D7     GPIO_NUM_19

// Short delay function (using FreeRTOS vTaskDelay)
void lcd_delay_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));  // Convert milliseconds to FreeRTOS ticks
}

// Send a 4-bit nibble to LCD
void lcd_send_nibble(uint8_t nibble) {
    gpio_set_level(LCD_D4, (nibble >> 0) & 0x01);
    gpio_set_level(LCD_D5, (nibble >> 1) & 0x01);
    gpio_set_level(LCD_D6, (nibble >> 2) & 0x01);
    gpio_set_level(LCD_D7, (nibble >> 3) & 0x01);

    // Pulse ENABLE pin to send data
    gpio_set_level(LCD_EN, 1);  // Set ENABLE PIN high
    lcd_delay_ms(1);  
    gpio_set_level(LCD_EN, 0);  // Set ENABLE  low
    lcd_delay_ms(1);  
}

// Send a full byte (2 nibbles) to LCD
void lcd_send_byte(uint8_t byte, bool is_data) {
    gpio_set_level(LCD_RS, is_data);  // RS = 0 for command, RS = 1 for data

    lcd_send_nibble(byte >> 4);  // Send the high nibble
    lcd_send_nibble(byte & 0x0F);  // Send the low nibble

    lcd_delay_ms(10);  // Wait for the LCD to process
}

// Initialize LCD 
void lcd_init() {
    // Setting LCD PINs Mode
    gpio_set_direction(LCD_RS, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_EN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D4, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D5, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D6, GPIO_MODE_OUTPUT);
    gpio_set_direction(LCD_D7, GPIO_MODE_OUTPUT);

    lcd_delay_ms(20);  

    // LCD initialization sequence
    lcd_send_nibble(0x03);  // Function set
    lcd_delay_ms(10);
    lcd_send_nibble(0x03);  // Function set
    lcd_delay_ms(10);
    lcd_send_nibble(0x03);  // Function set
    lcd_delay_ms(10);
    lcd_send_nibble(0x02);  // Set 4-bit mode

    // Set 4-bit mode, 2-line display, 5x8 dots
    lcd_send_byte(0x28, false);
    // Turn on display, cursor off, blink off
    lcd_send_byte(0x0C, false);
    // Clear display
    lcd_send_byte(0x01, false);
    lcd_delay_ms(5);
    // Set entry mode (increment cursor, no shift)
    lcd_send_byte(0x06, false);
}

// Write a single character to LCD
void lcd_write_char(char c) {
    lcd_send_byte(c, true);
}

// Write a string to the LCD
void lcd_write_string(const char* str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}
// Write a command to LCD
void lcd_write_command(uint8_t cmd)
{
    lcd_send_byte(cmd, false);
}

void lcd_clear() {
    // Send clear command to LCD
    lcd_write_command(0x01);
    vTaskDelay(pdMS_TO_TICKS(LCD_DELAY));
}

void lcd_display(const char* text) {
    while (*text) {
        lcd_write_char(*text++);
    }
}

void lcd_scroll(const char* text, int speed_ms) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        lcd_clear();
        lcd_display(&text[i]);
        vTaskDelay(pdMS_TO_TICKS(speed_ms));
    }
}
