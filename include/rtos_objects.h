#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#define EVENT_ACTIVE (1 << 0) 
#define EVENT_MOTION (1 << 1) 
#define EVENT_ALARM  (1 << 2) 

enum DisplayMode {
    MODE_TEMPERATURE,
    MODE_HUMIDITY,
    MODE_LIGHT,
    MODE_MOTION
};

struct SensorData {
    float temperature;
    float humidity;
    int lightLevel;
    bool motionDetected;
};

extern QueueHandle_t sensorQueue;
extern SemaphoreHandle_t serialMutex;
extern EventGroupHandle_t systemEvents;
extern volatile DisplayMode currentDisplayMode;

#endif