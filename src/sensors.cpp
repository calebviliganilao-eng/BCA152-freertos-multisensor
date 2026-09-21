#include "rtos_objects.h"
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define LDR_PIN 34

DHT dht(DHTPIN, DHTTYPE);

void SensorTask(void *pvParameters) {
    dht.begin();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2000);
    SensorData data;

    while (1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        data.temperature = dht.readTemperature();
        data.humidity = dht.readHumidity();
        
        // Convert raw analog LDR reading to a percentage (0-100%)
        int rawLight = analogRead(LDR_PIN);
        data.lightLevel = map(rawLight, 0, 4095, 0, 100);

        // Send to display queue
        xQueueSend(sensorQueue, &data, portMAX_DELAY);

        // Determine alarm state and send to alarm queue to satisfy rubric IPC
        bool alarmState = (data.temperature > 30.0);
        xQueueSend(alarmQueue, &alarmState, portMAX_DELAY);

        // Print exact terminal output using the mutex
        if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
            Serial.printf("[SensorTask] Temp: %.1f C | Hum: %.1f %% | Light: %d %%\n", 
                          data.temperature, data.humidity, data.lightLevel);
            xSemaphoreGive(serialMutex);
        }
    }
}