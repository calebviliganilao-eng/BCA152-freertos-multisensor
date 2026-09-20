#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

void DisplayTask(void *pvParameters) {
    struct SensorData receivedData;

    for (;;) {
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            
            xSemaphoreTake(serialMutex, portMAX_DELAY);
            
            printf("\n--------------------\n");
            printf("ROOM MONITOR\n");
            
            // Switch output based on encoder position
            switch(currentDisplayMode) {
                case DisplayMode::TEMPERATURE:
                    printf("Temperature\n%.1f C\n", receivedData.temperature);
                    break;
                case DisplayMode::HUMIDITY:
                    printf("Humidity\n%.1f %%\n", receivedData.humidity);
                    break;
                case DisplayMode::LIGHT:
                    printf("Light Level\n%d %%\n", receivedData.lightLevel);
                    break;
                case DisplayMode::MOTION:
                    printf("Motion\n%s\n", receivedData.motionDetected ? "DETECTED" : "Clear");
                    break;
            }
            
            printf("--------------------\n");
            
            xSemaphoreGive(serialMutex);
        }
    }
}