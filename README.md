# ESP32 Real-Time Multisensor Room Monitor (FreeRTOS)

A concurrent embedded system designed to monitor room conditions in real-time, built on the ESP32 using **FreeRTOS** scheduling. Simulated in **Wokwi**, this project tracks temperature, humidity, ambient light, and physical motion simultaneously. It features a hardware-interrupt-driven user interface via a rotary encoder and an automated priority-based alarm system. To support standard Adafruit sensor drivers within the simulation, this implementation utilizes the Arduino framework operating on top of native FreeRTOS APIs.

Developed for **BCA152 Microcontrollers**, Mindanao State University – Iligan Institute of Technology.

---

## Key Features

*   **Environmental Telemetry:** Real-time temperature and humidity tracking via DHT22.
*   **Ambient Light Detection:** Analog readings from a photoresistor (LDR) mapped to a percentage.
*   **Intelligent Power States:** A PIR motion sensor drives an ACTIVE/INACTIVE state machine that disables the UI during room vacancy to simulate power savings.
*   **Interactive UI:** A 128x64 SSD1306 OLED screen that cycles through data pages.
*   **Hardware Navigation:** KY-040 rotary encoder integration for physical menu control.
*   **Automated Alert Logic:** A buzzer immediately activates if the temperature exceeds the defined 18°C–30°C safe operating threshold.
*   **True Multitasking:** Architecture consisting of 5 specific FreeRTOS tasks running at strict priorities, completely eliminating sequential blocking loops.
*   **Robust IPC:** Task synchronization achieved through dual Queues, a Mutex, and an Event Group.

---

## Hardware / Simulated Components

| Component | Purpose | ESP32 Pin(s) |
| :--- | :--- | :--- |
| **DHT22** | Temperature & humidity sensing | `GPIO 4` |
| **Photoresistor (LDR)** | Ambient light level sensing | `GPIO 34` (ADC) |
| **PIR motion sensor** | Motion detection | `GPIO 27` |
| **Rotary encoder (KY-040)** | Display page navigation | CLK: `GPIO 18`, DT: `GPIO 19`, SW: `GPIO 23` |
| **SSD1306 OLED (I2C)** | Status display | SDA: `GPIO 21`, SCL: `GPIO 22` |
| **Buzzer** | Temperature alarm | `GPIO 25` |

---

## System Architecture

*(Insert your architecture-diagram.jpg here)*

The system separates hardware-polling, rendering, and logic into distinct layers. Environmental sensors feed data into the FreeRTOS processing layer, which subsequently drives the output layer (OLED and Buzzer) based on strict task priorities.

## FreeRTOS Task Communication

*(Insert your task-communication-diagram.jpg here)*

**Synchronization Primitives Used:**
*   `sensorQueue` — Safely transports the complete `SensorData` struct from the polling `SensorTask` to the rendering `DisplayTask`.
*   `alarmQueue` — A dedicated boolean queue ensuring the `AlarmTask` receives immediate threshold breach notifications from the `SensorTask`.
*   `g_systemEvents` (Event Group) — Utilizes specific bit flags (e.g., `MOTION_DETECTED_BIT`) written by the `MotionTask` to notify the rest of the system of state changes.
*   `serialMutex` — A semaphore that locks the shared UART (Serial monitor) and I2C buses, preventing high-priority tasks from scrambling text output from low-priority tasks.

---

## System State Machine

*(Insert your state-machine-diagram.jpg here)*

The ESP32 boots into the **ACTIVE** state. The `MotionTask` continuously evaluates the PIR sensor. If the room remains vacant (no motion detected) for a continuous 15-second window, the system enters the **INACTIVE** state, clearing the OLED screen. Any subsequent PIR trigger immediately resets the timer and restores the **ACTIVE** state.

---

## Visual Verification

### Wokwi Circuit Diagram
*(Insert your wokwi-circuit.png here)*

### Finished System (Running Simulation)
*(Insert your finished-system.png here)*

---

## FreeRTOS Task Summary

| Task | Priority | Period / Trigger | Responsibility |
| :--- | :--- | :--- | :--- |
| `InputTask` | 3 (highest) | Short periodic polling | Read rotary encoder state and update global display mode. |
| `MotionTask` | 3 (highest) | 100 ms polling | Monitor PIR sensor and manage ACTIVE/INACTIVE event flags. |
| `SensorTask` | 2 (medium) | 2 s (`vTaskDelayUntil`) | Sample DHT22/LDR, send queue data, and evaluate alarm limits. |
| `AlarmTask` | 2 (medium) | Data-driven (Queue) | Wait for `alarmQueue` state and drive buzzer output. |
| `DisplayTask` | 1 (lowest) | Data-driven (Queue) | Own the OLED I2C bus and render the selected UI page. |

---

## Project Structure

```text
bca152-freertos-multisensor/
├── include/
│   └── rtos_objects.h
├── src/
│   ├── main.cpp
│   ├── alarm.cpp
│   ├── display.cpp
│   ├── input.cpp
│   ├── motion.cpp
│   ├── sensors.cpp
│   └── system_state.cpp
├── test/
│   └── test_main.cpp
├── docs/
│   ├── laboratory-report.pdf
│   └── images/
├── diagram.json
├── wokwi.toml
└── platformio.ini
