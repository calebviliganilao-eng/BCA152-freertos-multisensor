#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Step 24: Define Sensor Data
struct SensorData {
    float temperature;
    float humidity;
    int lightLevel;
    bool motionDetected;
};

// Declare the queue globally so multiple tasks can access it
extern QueueHandle_t sensorQueue;