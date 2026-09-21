#include "rtos_objects.h"

#define PIR_PIN 27
#define INACTIVITY_TIMEOUT_MS 5000 

void MotionTask(void *pvParameters) {
    pinMode(PIR_PIN, INPUT);
    uint32_t lastMotionTime = xTaskGetTickCount() * portTICK_PERIOD_MS;
    bool systemActive = false;

    while (1) {
        bool motionDetected = digitalRead(PIR_PIN) == HIGH;
        uint32_t currentTime = xTaskGetTickCount() * portTICK_PERIOD_MS;

        if (motionDetected) {
            lastMotionTime = currentTime;
            
            if (!systemActive) {
                systemActive = true;
                if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                    Serial.println("System State: ACTIVE");
                    xSemaphoreGive(serialMutex);
                }
            }
            xEventGroupSetBits(systemEvents, MOTION_DETECTED_BIT);
        } else {
            xEventGroupClearBits(systemEvents, MOTION_DETECTED_BIT);
        }

        if (systemActive && (currentTime - lastMotionTime >= INACTIVITY_TIMEOUT_MS)) {
            systemActive = false;
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                Serial.println("System State: INACTIVE");
                xSemaphoreGive(serialMutex);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}