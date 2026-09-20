extern "C" void app_main(void) {
    // PART II: Print recognizable startup message
    printf("BCA152 FreeRTOS Multisensor\n");
    printf("System starting...\n");

    sensorQueue = xQueueCreate(1, sizeof(struct SensorData));
    serialMutex = xSemaphoreCreateMutex();
    systemEvents = xEventGroupCreate();

    if (sensorQueue != NULL && serialMutex != NULL && systemEvents != NULL) {
        xTaskCreate(DisplayTask, "Display", 4096, NULL, 1, NULL); 
        xTaskCreate(SensorTask,  "Sensors", 4096, NULL, 2, NULL);  
        xTaskCreate(AlarmTask,   "Alarm",   2048, NULL, 2, NULL);     
        xTaskCreate(MotionTask,  "Motion",  2048, NULL, 3, NULL);   
        xTaskCreate(InputTask,   "Input",   2048, NULL, 3, NULL);     
        xTaskCreate(StateTask,   "State",   2048, NULL, 4, NULL);     
    }
}