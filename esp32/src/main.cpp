#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

QueueHandle_t sensorQueue;
SemaphoreHandle_t serialMutex;

extern void DisplayTask(void *pvParameters);
extern void SensorTask(void *pvParameters);
extern void InputTask(void *pvParameters);
extern void AlarmTask(void *pvParameters); // Declare the new task

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    sensorQueue = xQueueCreate(1, sizeof(struct SensorData));
    serialMutex = xSemaphoreCreateMutex();

    if (sensorQueue != NULL && serialMutex != NULL) {
        xTaskCreate(DisplayTask, "Display", 4096, NULL, 1, NULL); 
        xTaskCreate(SensorTask,  "Sensors", 4096, NULL, 2, NULL); 
        xTaskCreate(InputTask,   "Input",   2048, NULL, 3, NULL); 
        xTaskCreate(AlarmTask,   "Alarm",   2048, NULL, 4, NULL); // Priority 4
    }
}