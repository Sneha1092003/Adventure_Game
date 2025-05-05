//https://github.com/Sneha1092003/Adventure_Game.git

// main.c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "lcd.h"

#define TOUCH_THRESHOLD 500  // Adjust based on testing
#define TOUCH_LEFT TOUCH_PAD_NUM4  // GPIO13
#define TOUCH_RIGHT TOUCH_PAD_NUM5 // GPIO12

static const char* TAG = "ADVENTURE_GAME";

void touch_init() {
    touch_pad_init();
    touch_pad_config(TOUCH_LEFT, 0);
    touch_pad_config(TOUCH_RIGHT, 0);
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_sw_start();
}

int read_touch_input() {
    uint16_t touch_value_left, touch_value_right;
    touch_pad_read(TOUCH_LEFT, &touch_value_left);
    touch_pad_read(TOUCH_RIGHT, &touch_value_right);
    touch_pad_filter_start(10); // Optional: apply filtering to remove noise
    ESP_LOGI(TAG, "Touch values - Left: %d, Right: %d", touch_value_left, touch_value_right);

    if (touch_value_left < TOUCH_THRESHOLD) return 0;  // Left touched
    if (touch_value_right < TOUCH_THRESHOLD) return 1; // Right touched
    vTaskDelay(pdMS_TO_TICKS(500));
    return -1; // No touch detected
}
void app_main(void)
{
    lcd_config_t config = {
        .rs_pin = GPIO_NUM_21,
        .en_pin = GPIO_NUM_22,
        .data_pins = {GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19},
        .is_4bit_mode = 1
    };
    touch_init();
    lcd_init(&config);
    lcd_clear();
    lcd_display("You woke up!");
    vTaskDelay(pdMS_TO_TICKS(2000));

    lcd_clear();
    lcd_display("Choose L or R");
    vTaskDelay(pdMS_TO_TICKS(2000));

    while (1) {
        int decision = read_touch_input();
        if (decision == 0) {
            lcd_clear();
            lcd_display("You chose LEFT!");
            vTaskDelay(pdMS_TO_TICKS(2000));
            lcd_clear();
            lcd_scroll("You find a hidden treasure inside a cave!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll
    lcd_clear();
    lcd_display("You win!");
            break;
        } else if (decision == 1) {
            lcd_clear();
            lcd_display("You chose RIGHT!");
            vTaskDelay(pdMS_TO_TICKS(2000)); // wait for scroll

                lcd_clear();
    lcd_scroll("Oops!There is a snake!!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll

    lcd_clear();
    lcd_display("You lose!");
    vTaskDelay(pdMS_TO_TICKS(200));
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    lcd_clear();
    lcd_display("Game Over!");
    vTaskDelay(pdMS_TO_TICKS(2000));
    lcd_clear();
    lcd_display("Thanks for playing!");
}

