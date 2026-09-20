#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rtos_objects.h"
#include <stdio.h>

#define BUZZER_PIN GPIO_NUM_32

enum class AlarmState { NORMAL, LOW_TEMPERATURE, HIGH_TEMPERATURE };

AlarmState evaluateTemperature(float temperature) {
    if (temperature < 18.0) return AlarmState::LOW_TEMPERATURE;
    if (temperature > 30.0) return AlarmState::HIGH_TEMPERATURE;
    return AlarmState::NORMAL;
}

void AlarmTask(void *pvParameters) {
    // Configure buzzer hardware
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BUZZER_PIN, 0); 
    
    struct SensorData data;
    
    for (;;) {
        // Peek at the queue without removing the item
        if (xQueuePeek(sensorQueue, &data, portMAX_DELAY) == pdPASS) {
            AlarmState state = evaluateTemperature(data.temperature);
            
            if (state != AlarmState::NORMAL) {
                // Safely log the alarm condition
                xSemaphoreTake(serialMutex, portMAX_DELAY);
                printf("\n[ALARM WARNING] Temp Out of Range: %.1f C!\n", data.temperature);
                xSemaphoreGive(serialMutex);
                
                // Beep the buzzer (500ms ON, 500ms OFF)
                gpio_set_level(BUZZER_PIN, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(500));
            } else {
                // Normal state: ensure buzzer is off and yield CPU for 1 second
                gpio_set_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
    }
}