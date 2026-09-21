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
graph TD
    subgraph Hardware Inputs
        DHT22[DHT22 Sensor]
        LDR[LDR Sensor]
        ENC[Rotary Encoder]
        PIR[PIR Sensor]
    end

    subgraph FreeRTOS Tasks
        SensTask("SensorTask [P2]")
        InpTask("InputTask [P3]")
        MonTask("MonitorTask [P1]")
        DispTask("DisplayTask [P1]")
        AlrmTask("AlarmTask [P4]")
    end

    subgraph IPC Mechanisms
        SQ[(sensorQueue)]
        EG{systemEvents}
        MUT[serialMutex]
    end

    subgraph Hardware Outputs
        OLED[OLED Screen]
        BUZ[Buzzer]
    end

    %% Data Acquisition
    DHT22 --> SensTask
    LDR --> SensTask
    ENC --> InpTask
    PIR --> MonTask

    %% Writing to IPC
    SensTask -- Writes --> SQ
    InpTask -- Sets --> EG
    MonTask -- Flags --> EG

    %% Reading from IPC
    SQ -- Reads --> DispTask
    SQ -- Peeks --> AlrmTask
    EG -- State --> DispTask
    EG -- Triggers --> AlrmTask

    %% Hardware Control
    DispTask --> OLED
    AlrmTask --> BUZ

    %% Mutex Locks (Routed neatly at the bottom)
    SensTask -. Locks .-> MUT
    InpTask -. Locks .-> MUT
    MonTask -. Locks .-> MUT
    DispTask -. Locks .-> MUT
    AlrmTask -. Locks .-> MUT
