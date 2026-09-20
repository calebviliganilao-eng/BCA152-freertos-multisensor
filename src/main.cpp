#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_objects.h"
#include <stdio.h>

QueueHandle_t sensorQueue;
SemaphoreHandle_t serialMutex;
EventGroupHandle_t systemEvents;
volatile DisplayMode currentDisplayMode = MODE_TEMPERATURE;

extern void DisplayTask(void *pvParameters);
extern void SensorTask(void *pvParameters);
extern void InputTask(void *pvParameters);
extern void AlarmTask(void *pvParameters);
extern void MonitorTask(void *pvParameters);

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    sensorQueue = xQueueCreate(1, sizeof(struct SensorData));
    serialMutex = xSemaphoreCreateMutex();
    systemEvents = xEventGroupCreate();

    if (sensorQueue != NULL && serialMutex != NULL && systemEvents != NULL) {
        xEventGroupSetBits(systemEvents, EVENT_ACTIVE); 

        // Explicit Task Priorities Assigned
        xTaskCreate(AlarmTask,   "Alarm",   2048, NULL, 4, NULL); 
        xTaskCreate(InputTask,   "Input",   2048, NULL, 3, NULL); 
        xTaskCreate(SensorTask,  "Sensors", 4096, NULL, 2, NULL); 
        xTaskCreate(DisplayTask, "Display", 4096, NULL, 1, NULL); 
        xTaskCreate(MonitorTask, "Monitor", 2048, NULL, 1, NULL); 
    }
}