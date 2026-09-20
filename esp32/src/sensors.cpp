#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

void SensorTask(void *pvParameters) {
    (void)pvParameters; // Static analysis fix
    struct SensorData data;
    for (;;) {
        data.temperature = 25.4; 
        data.humidity = 50.0;    
        data.lightLevel = 2048;  
        data.motionDetected = true; 
        
        xQueueOverwrite(sensorQueue, &data);
        
        if (data.motionDetected) xEventGroupSetBits(systemEvents, EVENT_MOTION);
        else xEventGroupClearBits(systemEvents, EVENT_MOTION);

        xSemaphoreTake(serialMutex, portMAX_DELAY);
        printf("[SensorTask] Data queued.\n");
        xSemaphoreGive(serialMutex);
        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}