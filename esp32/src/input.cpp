#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"

void InputTask(void *pvParameters) {
    for (;;) {
        // Hardware polling logic for rotary encoder
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}