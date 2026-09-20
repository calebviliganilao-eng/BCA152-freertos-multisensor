#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

void MonitorTask(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(serialMutex, portMAX_DELAY);
        printf("\n[SYSTEM MONITOR] Free RTOS Heap: %d bytes\n", xPortGetFreeHeapSize());
        xSemaphoreGive(serialMutex);
        vTaskDelay(pdMS_TO_TICKS(10000)); 
    }
}