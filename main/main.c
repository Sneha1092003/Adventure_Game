#include "lcd.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>

#define BUTTON_LEFT 12   // GPIO pin for left button
#define BUTTON_RIGHT 13  // GPIO pin for right button

void button_init() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,               // Disable interrupt
        .mode = GPIO_MODE_INPUT,                      // Set as input
        .pin_bit_mask = (1ULL << BUTTON_LEFT) | (1ULL << BUTTON_RIGHT), // Specify pins
        .pull_up_en = GPIO_PULLUP_ENABLE,             // Enable pull-up resistors
        .pull_down_en = GPIO_PULLDOWN_DISABLE        // Disable pull-down resistors
    };
    gpio_config(&io_conf);  // Apply configuration
}

void app_main(void)
{
    lcd_config_t config = {
        .rs_pin = GPIO_NUM_21,
        .en_pin = GPIO_NUM_22,
        .data_pins = {GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19},
        .is_4bit_mode = 1
    };
    button_init();  // Initialize the buttons
    lcd_init(&config);
    lcd_clear();
    lcd_display("You woke up...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    lcd_clear();
    lcd_display("Go L or R?");
    vTaskDelay(pdMS_TO_TICKS(2000));

    while (1) {
        // Check if the LEFT button is pressed (LOW when pressed)
        if (gpio_get_level(BUTTON_LEFT) == 0) {
            lcd_clear();
            lcd_display("You chose LEFT");
            vTaskDelay(pdMS_TO_TICKS(2000));
            printf("LEFT Button: Pressed (0)\n");
            vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms delay after printf
            lcd_clear();
            lcd_scroll("You find a hidden treasure inside a cave!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll
    lcd_clear();
    lcd_display("You win!");
            break;

        } else {
            printf("LEFT Button: Not Pressed (1)\n");
            vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms delay after printf
        }

        // Check if the RIGHT button is pressed (LOW when pressed)
        if (gpio_get_level(BUTTON_RIGHT) == 0) {
            lcd_clear();
            lcd_display("You chose RIGHT");
            vTaskDelay(pdMS_TO_TICKS(2000));
            printf("RIGHT Button: Pressed (0)\n");
            vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms delay after printf
            lcd_clear();
    lcd_scroll("Oops!There is a snake!!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll

    lcd_clear();
    lcd_display("You lose!");
    vTaskDelay(pdMS_TO_TICKS(200));
            break;

        } else {
            printf("RIGHT Button: Not Pressed (1)\n");
            vTaskDelay(pdMS_TO_TICKS(500));  // 500 ms delay after printf
        }

        vTaskDelay(pdMS_TO_TICKS(200));  // Small delay to debounce
    }
}