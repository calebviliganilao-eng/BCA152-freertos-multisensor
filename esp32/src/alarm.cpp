#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

void AlarmTask(void *pvParameters) {
    struct SensorData data;
    for (;;) {
        if (xQueuePeek(sensorQueue, &data, portMAX_DELAY) == pdPASS) {
            if (data.temperature > 30.0) {
                xEventGroupSetBits(systemEvents, EVENT_ALARM);
                
                xSemaphoreTake(serialMutex, portMAX_DELAY);
                printf("[ALARM WARNING] High Temperature Detected!\n");
                xSemaphoreGive(serialMutex); 
            } else {
                xEventGroupClearBits(systemEvents, EVENT_ALARM);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}