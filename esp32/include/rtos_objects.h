#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h" // Required for mutex functions

struct SensorData {
    float temperature;
    float humidity;
    int lightLevel;
    bool motionDetected;
};

extern QueueHandle_t sensorQueue;
extern SemaphoreHandle_t serialMutex; // Shares the mutex across all your .cpp files