# BCA152 FreeRTOS Multisensor Room Monitoring System

## Project Overview
A robust, multi-threaded real-time room monitoring system built for the ESP32 microcontroller using FreeRTOS, PlatformIO, and the Wokwi simulation environment. The system concurrent handles sensor acquisition, OLED UI navigation, motion detection, and alarm logic while ensuring deterministic execution and thread safety.

---

## Features
* **Real-Time Multi-Threading:** Operates dedicated tasks for sensors, UI display, rotary encoder inputs, and system monitoring using FreeRTOS primitives.
* **Inter-Task Communication:** Utilizes thread-safe queues (`xQueue`) for sensor telemetry and event groups (`xEventGroup`) for system state transitions.
* **Resource Protection:** Implements mutex semaphores (`SemaphoreHandle_t`) to prevent race conditions during serial terminal logging.
* **Fault Handling & Simulation:** Designed with robust task scheduling, configurable priorities, and fault isolation mechanisms.

---

## Learning Objectives
* Master FreeRTOS task creation, priority assignment, and cooperative/preemptive scheduling mechanics.
* Implement thread-safe synchronization patterns using queues, mutexes, and event flags on resource-constrained microcontrollers.
* Apply industry-standard static analysis, unit testing, and version control workflows to embedded software development.

---

## System Architecture
*(Insert System Architecture Diagram here[cite: 11])*
*Description: High-level architectural overview showing hardware integration, task separation, and control flow.*

---

## FreeRTOS Architecture
* **Task Priorities & Allocation:** Preemptive priority scheme separating critical safety tasks from background UI rendering.
* **Scheduling Model:** Tick-based scheduling with configured blocking delays to prevent CPU starvation.

---

## Hardware / Simulated Components
* **MCU:** ESP32 Doit DevKit V1[cite: 5]
* **Sensor Suite:** DHT22 (Temperature & Humidity), LDR (Light Intensity), PIR (Motion Detection)
* **User Interface:** Rotary Encoder (Navigation), OLED Display (UI Page Cycling), Piezo Buzzer / Alarm Indicator

---

## Pin Configuration
| Component | ESP32 Pin | Description |
| :--- | :--- | :--- |
| DHT22 Data | GPIO 4 | Temperature & Humidity Sensor |
| LDR Analog | GPIO 34 | Ambient Light Sensor |
| PIR Motion | GPIO 27 | Motion Detection Input |
| Rotary Encoder (CLK/DT/SW) | GPIO 18, 19, 23 | UI Navigation Controls |
| OLED Display (SDA/SCL) | GPIO 21, 22 | I2C Display Interface |
| Alarm / Buzzer | GPIO 25 | Audio Warning Output |

---

## Task Design
* `SensorTask`: Periodically samples hardware telemetry and pushes updates to the shared queue.
* `DisplayTask`: Manages UI page rendering and user feedback based on active menu selections.
* `InputTask`: Polls rotary encoder and PIR states to drive system state machine events.
* `AlarmTask`: Monitors threshold violations and triggers audio-visual alarms when limits are exceeded.

---

## Inter-Task Communication
*(Insert FreeRTOS Task-Communication Diagram here[cite: 11])*
*Description: Flow chart illustrating data transfer via queues and synchronization via event groups.*

---

## State Machine
*(Insert State-Machine Diagram here[cite: 11])*
*Description: Finite state machine governing active, inactive, and alarm operating modes.*

---

## Repository Structure
```text
├── include/           # Header files and shared definitions
├── lib/               # Custom modular library components
├── src/               # Application source files (tasks, main)
├── test/              # Unit testing modules
├── platformio.ini     # PlatformIO build and static analysis configuration
├── wokwi.toml         # Wokwi simulation configuration
<<<<<<< HEAD
└── README.md          # Public project documentation
=======
└── README.md          # Public project documentation
>>>>>>> fcbc7448ffc8941cccb25785f824e1c156ee3c0f
