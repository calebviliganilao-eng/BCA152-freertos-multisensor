#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

void DisplayTask(void *pvParameters) {
    (void)pvParameters; // Static analysis fix
    struct SensorData data;
    for (;;) {
        if (xQueueReceive(sensorQueue, &data, portMAX_DELAY) == pdPASS) {
            EventBits_t currentEvents = xEventGroupGetBits(systemEvents);
            
            xSemaphoreTake(serialMutex, portMAX_DELAY);
            printf("\n--- ROOM MONITOR ---\n");
            switch (currentDisplayMode) {
                case MODE_TEMPERATURE: printf("Temperature: %.1f C\n", data.temperature); break;
                case MODE_HUMIDITY:    printf("Humidity: %.1f %%\n", data.humidity); break;
                case MODE_LIGHT:       printf("Light Level: %d\n", data.lightLevel); break;
                case MODE_MOTION:      printf("Motion: %s\n", data.motionDetected ? "DETECTED" : "Clear"); break;
            }
            
            printf("\n--- SYSTEM STATUS ---\n");
            if (currentEvents & EVENT_ACTIVE) printf("Status: SYSTEM ACTIVE\n");
            if (currentEvents & EVENT_MOTION) printf("Alert: MOTION DETECTED\n");
            if (currentEvents & EVENT_ALARM)  printf("Alert: ALARM TRIGGERED (HIGH TEMP)\n");
            printf("--------------------\n");
            xSemaphoreGive(serialMutex);
        }
    }
}