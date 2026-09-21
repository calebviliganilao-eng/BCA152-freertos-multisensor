#  ESP32 Real-Time Room Monitoring System

![C++](https://img.shields.io/badge/Language-C++-00599C?logo=c%2B%2B)
![Framework](https://img.shields.io/badge/Framework-ESP--IDF%20%2F%20FreeRTOS-E7352C?logo=espressif)
![Environment](https://img.shields.io/badge/Environment-PlatformIO-F6822B?logo=platformio)
![Status](https://img.shields.io/badge/Build-Passing-brightgreen)

> **Note:** Insert a GIF or image of your physical hardware setup or Wokwi simulation running here to immediately grab the reader's attention.

##  Project Overview
This project is a multi-threaded, real-time room monitoring system built for the ESP32. Designed to demonstrate industry-standard embedded software principles, the firmware concurrently handles environmental data acquisition, OLED UI navigation, unauthorized motion detection, and immediate hardware alarms. 

It is engineered with a strict focus on **deterministic execution** and **thread safety**, utilizing FreeRTOS primitives to prevent CPU starvation, race conditions, and UI blocking.

##  Key Engineering Features
* **Preemptive RTOS Scheduling:** Workloads are distributed across 5 distinct FreeRTOS tasks with explicitly assigned priorities, ensuring critical safety alerts preempt background UI rendering.
* **Thread-Safe IPC:** Data and state transitions are handled securely using `xQueue` (sensor telemetry) and `xEventGroup` (global state flags).
* **Resource Protection:** Shared resources, such as the I2C display and serial terminal, are protected via `SemaphoreHandle_t` (Mutexes) to prevent race conditions.
* **Deterministic Timing:** Sensor polling utilizes `vTaskDelayUntil()` to eliminate timing drift commonly caused by standard blocking delays.
* **Test-Driven Design:** Built with PlatformIO, incorporating modular testing environments for both native unit testing and physical ESP32 target builds.

##  System Architecture & Task Hierarchy
The software architecture follows a strictly decoupled approach. Hardware inputs trigger state changes, which are processed via IPC, eventually driving hardware outputs.

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

    %% Mutex Locks 
    SensTask -. Locks .-> MUT
    InpTask -. Locks .-> MUT
    MonTask -. Locks .-> MUT
    DispTask -. Locks .-> MUT
    AlrmTask -. Locks .-> MUT
