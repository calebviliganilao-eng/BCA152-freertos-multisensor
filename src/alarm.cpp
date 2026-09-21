#include "rtos_objects.h"

#define BUZZER_PIN 25

void AlarmTask(void *pvParameters) {
    pinMode(BUZZER_PIN, OUTPUT);
    bool alarmActive = false;

    while (1) {
        // Wait for data from SensorTask via the Queue (Timeout 2 seconds)
        if (xQueueReceive(alarmQueue, &alarmActive, pdMS_TO_TICKS(2000)) == pdPASS) {
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                Serial.println("[AlarmTask] Checking alarm state...");
                xSemaphoreGive(serialMutex);
            }
        }

        if (alarmActive) {
            digitalWrite(BUZZER_PIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(300));
            digitalWrite(BUZZER_PIN, LOW);
            vTaskDelay(pdMS_TO_TICKS(300));
        }
    }
}