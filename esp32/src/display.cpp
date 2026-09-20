#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

void DisplayTask(void *pvParameters) {
    struct SensorData receivedData;

    for (;;) {
        // Wait in the Blocked state until fresh data arrives in the queue
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            
            // Protect the terminal output with a mutex to prevent text overlapping
            xSemaphoreTake(serialMutex, portMAX_DELAY);
            
            // Exact initial OLED output format required by the lab manual[cite: 2]
            printf("\n--------------------\n");
            printf("ROOM MONITOR\n");
            printf("Temperature\n");
            printf("%.1f C\n", receivedData.temperature);
            printf("--------------------\n");
            
            xSemaphoreGive(serialMutex);
        }
    }
}