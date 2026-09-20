#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

extern void SensorTask(void *pvParameters);

extern "C" void app_main(void) {
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    // Spawn the SensorTask at Priority 2
    xTaskCreate(SensorTask, "Sensors", 4096, NULL, 2, NULL);
}