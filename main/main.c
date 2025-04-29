#include "lcd.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 1024

void app_main(void)
{
    lcd_config_t config = {
        .rs_pin = GPIO_NUM_21,
        .en_pin = GPIO_NUM_22,
        .data_pins = {GPIO_NUM_16, GPIO_NUM_17, GPIO_NUM_18, GPIO_NUM_19},
        .is_4bit_mode = 1
    };

    lcd_init(&config);
    lcd_clear();

    // UART config
    const uart_port_t uart_num = UART_NUM_0;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    char data[BUF_SIZE];
    lcd_display("You woke up...");
    vTaskDelay(pdMS_TO_TICKS(2000));
    lcd_clear();
    lcd_display("Go L or R?");
    printf("\nType L or R: ");

    while (1) {
        int len = uart_read_bytes(uart_num, (uint8_t*)data, 1, pdMS_TO_TICKS(5000));
        if (len > 0) {
            if (data[0] == 'L' || data[0] == 'l') {
                lcd_clear();
                lcd_display("You chose LEFT!");
                vTaskDelay(pdMS_TO_TICKS(2000)); // wait for scroll

                lcd_clear();
    lcd_scroll("You find a hidden treasure inside a cave!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll

    lcd_clear();
    lcd_display("You win!");
                break;
            } else if (data[0] == 'R' || data[0] == 'r') {
                lcd_clear();
                lcd_display("You chose RIGHT!");
                vTaskDelay(pdMS_TO_TICKS(2000)); // wait for scroll

                lcd_clear();
    lcd_scroll("Oops!There is a snake!!", 300); // scroll text slowly
    vTaskDelay(pdMS_TO_TICKS(6000)); // wait for scroll

    lcd_clear();
    lcd_display("You lose!");
                break;
            } else {
                lcd_clear();
                lcd_display("Invalid input.");
                vTaskDelay(pdMS_TO_TICKS(2000));
                lcd_clear();
                lcd_display("Try again: L/R");
            }
        }
    }
}
