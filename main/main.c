#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    lcd_config_t lcd_config = {
        .rs_pin = GPIO_NUM_21,
        .en_pin = GPIO_NUM_22,
        .data_pins = {GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19},
        .is_4bit_mode = 1
    };

    lcd_init(&lcd_config);
    lcd_clear();

    lcd_display("You wake up...");
    vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds

    lcd_clear();
    lcd_display("Go left / right?");
    vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds

    // Let's say automatically we 'go left'
    lcd_clear();
    lcd_display("You chose left.");
    vTaskDelay(pdMS_TO_TICKS(2000)); // 2 seconds

    lcd_clear();
    lcd_scroll("You find a hidden treasure inside a cave!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll

    lcd_clear();
    lcd_display("You win!");
}
