#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"

void InputTask(void *pvParameters) {
    (void)pvParameters; // Static analysis fix
    for (;;) {
        // Hardware polling logic for rotary encoder
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}