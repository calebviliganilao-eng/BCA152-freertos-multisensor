#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

// Step 25: Create a Queue
QueueHandle_t sensorQueue;

extern void SensorTask(void *pvParameters);

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    // Create a queue capable of holding 1 SensorData structure
    sensorQueue = xQueueCreate(1, sizeof(struct SensorData));

    // Only start tasks if the queue was created successfully
    if (sensorQueue != NULL) {
        xTaskCreate(SensorTask, "Sensors", 4096, NULL, 2, NULL);
    }
}