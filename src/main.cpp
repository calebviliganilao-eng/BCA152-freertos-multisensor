#include "rtos_objects.h"

// Define the actual handles here (only once)
QueueHandle_t sensorQueue;
QueueHandle_t alarmQueue;
SemaphoreHandle_t serialMutex;
EventGroupHandle_t systemEvents;
DisplayMode currentDisplayMode = TEMP_MODE;

void setup() {
    Serial.begin(115200);
    
    // Initialize IPC mechanisms
    sensorQueue = xQueueCreate(5, sizeof(SensorData));
    alarmQueue = xQueueCreate(5, sizeof(bool)); // Queue specifically for alarm logic
    serialMutex = xSemaphoreCreateMutex();
    systemEvents = xEventGroupCreate();

    // Spawn FreeRTOS Tasks with rubric-defined priorities
    xTaskCreate(DisplayTask, "DisplayTask", 4096, NULL, 1, NULL);
    xTaskCreate(SensorTask,  "SensorTask",  2048, NULL, 2, NULL);
    xTaskCreate(AlarmTask,   "AlarmTask",   2048, NULL, 2, NULL);
    xTaskCreate(InputTask,   "InputTask",   2048, NULL, 3, NULL);
    xTaskCreate(MotionTask,  "MotionTask",  2048, NULL, 3, NULL);
}

void loop() {
    // FreeRTOS tasks handle everything, so loop can be deleted
    vTaskDelete(NULL);
}