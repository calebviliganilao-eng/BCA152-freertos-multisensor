#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "rtos_objects.h"
#include <stdio.h>

void SensorTask(void *pvParameters) {
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {};
    config.atten = ADC_ATTEN_DB_12;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config);

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t samplingFrequency = pdMS_TO_TICKS(2000);

    for (;;) {
        int raw_ldr;
        adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &raw_ldr);
        
        // Populate the data structure[cite: 2]
        struct SensorData currentData;
        currentData.lightLevel = (raw_ldr * 100) / 4095;
        currentData.temperature = 25.40;
        currentData.humidity = 61.20;
        currentData.motionDetected = false; 

        // Send data to the queue[cite: 2]
        xQueueOverwrite(sensorQueue, &currentData);
        printf("[SensorTask] Data queued.\n");

        vTaskDelayUntil(&lastWakeTime, samplingFrequency);
    }
}