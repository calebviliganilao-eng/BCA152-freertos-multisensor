#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

// --- Global State Enums ---
enum DisplayMode {
    TEMP_MODE,
    HUMIDITY_MODE,
    LIGHT_MODE,
    MOTION_MODE
};

// --- Data Structures ---
struct SensorData {
    float temperature;
    float humidity;
    int lightLevel;
};

// --- IPC Handles (Extern declarations for global scope) ---
extern QueueHandle_t sensorQueue;
extern QueueHandle_t alarmQueue;
extern SemaphoreHandle_t serialMutex;
extern EventGroupHandle_t systemEvents;

// --- Event Group Bits ---
#define MOTION_DETECTED_BIT (1 << 0)

// --- Global Variables ---
extern DisplayMode currentDisplayMode;

// --- Task Prototypes ---
void SensorTask(void *pvParameters);
void InputTask(void *pvParameters);
void MotionTask(void *pvParameters);
void DisplayTask(void *pvParameters);
void AlarmTask(void *pvParameters);

#endif