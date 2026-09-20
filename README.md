# BCA152 FreeRTOS Multisensor Room Monitoring System

## Project Overview
A robust, multi-threaded real-time room monitoring system built for the ESP32 microcontroller using FreeRTOS, PlatformIO, and the Wokwi simulation environment. The system concurrently handles sensor acquisition, OLED UI navigation, motion detection, and alarm logic while ensuring deterministic execution and thread safety.

## Features
* **Real-Time Multi-Threading:** Operates dedicated tasks for sensors, UI display, rotary encoder inputs, and system monitoring using FreeRTOS primitives.
* **Inter-Task Communication:** Utilizes thread-safe queues (`xQueue`) for sensor telemetry and event groups (`xEventGroup`) for system state transitions.
* **Resource Protection:** Implements mutex semaphores (`SemaphoreHandle_t`) to prevent race conditions during serial terminal logging and display updates.
* **Fault Handling & Simulation:** Designed with robust task scheduling, configurable priorities, and fault isolation mechanisms.

## System & FreeRTOS Architecture
The system utilizes a preemptive priority scheme separating critical safety tasks from background UI rendering. It relies on tick-based scheduling with configured blocking delays (`vTaskDelayUntil`) to prevent CPU starvation.

```mermaid
graph LR
    %% Hardware Inputs
    DHT22[DHT22 Sensor] --> SensTask
    LDR[LDR Sensor] --> SensTask
    PIR[PIR Sensor] --> MonTask
    ENC[Rotary Encoder] --> InpTask

    %% FreeRTOS Tasks
    SensTask(SensorTask [P2])
    MonTask(MonitorTask [P1])
    InpTask(InputTask [P3])
    DispTask(DisplayTask [P1])
    AlrmTask(AlarmTask [P4])

    %% FreeRTOS IPC
    SQ[(sensorQueue)]
    EG{systemEvents}
    MUT[serialMutex]

    %% Data Flow
    SensTask -- Sends Data --> SQ
    SQ -- Reads Data --> DispTask
    MonTask -- Sets Flags --> EG
    InpTask -- Sets Flags --> EG
    EG -- Triggers --> AlrmTask
    InpTask -. Locks .-> MUT
    DispTask -. Locks .-> MUT
    
    %% Hardware Outputs
    DispTask --> OLED[OLED Screen]
    AlrmTask --> BUZ[Buzzer]
