#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rtos_objects.h"
#include <stdio.h>

#define ENC_CLK GPIO_NUM_25
#define ENC_DT  GPIO_NUM_26

// This is the specific variable the linker could not find
DisplayMode currentDisplayMode = DisplayMode::TEMPERATURE;

void InputTask(void *pvParameters) {
    gpio_set_direction(ENC_CLK, GPIO_MODE_INPUT);
    gpio_set_direction(ENC_DT, GPIO_MODE_INPUT);

    int lastClk = gpio_get_level(ENC_CLK);

    for (;;) {
        int currentClk = gpio_get_level(ENC_CLK);

        if (currentClk != lastClk && currentClk == 1) {
            if (gpio_get_level(ENC_DT) != currentClk) {
                // Clockwise Navigation
                switch(currentDisplayMode) {
                    case DisplayMode::TEMPERATURE: currentDisplayMode = DisplayMode::HUMIDITY; break;
                    case DisplayMode::HUMIDITY:    currentDisplayMode = DisplayMode::LIGHT; break;
                    case DisplayMode::LIGHT:       currentDisplayMode = DisplayMode::MOTION; break;
                    case DisplayMode::MOTION:      currentDisplayMode = DisplayMode::TEMPERATURE; break;
                }
            } else {
                // Counter-Clockwise Navigation
                switch(currentDisplayMode) {
                    case DisplayMode::TEMPERATURE: currentDisplayMode = DisplayMode::MOTION; break;
                    case DisplayMode::HUMIDITY:    currentDisplayMode = DisplayMode::TEMPERATURE; break;
                    case DisplayMode::LIGHT:       currentDisplayMode = DisplayMode::HUMIDITY; break;
                    case DisplayMode::MOTION:      currentDisplayMode = DisplayMode::LIGHT; break;
                }
            }
        }
        
        lastClk = currentClk; 
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}