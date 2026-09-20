#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

struct SensorData {
    float temperature;
    float humidity;
    int lightLevel;
    bool motionDetected;
};

// The compiler is complaining because it can't find this line:
enum class DisplayMode { TEMPERATURE, HUMIDITY, LIGHT, MOTION };

extern QueueHandle_t sensorQueue;
extern SemaphoreHandle_t serialMutex;
extern DisplayMode currentDisplayMode; // And this line!