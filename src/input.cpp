#include "rtos_objects.h"

#define CLK_PIN 18  
#define DT_PIN 19   
#define SW_PIN 23   

void InputTask(void *pvParameters) {
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);
    
    int lastStateCLK = digitalRead(CLK_PIN); 

    while (1) {
        int currentStateCLK = digitalRead(CLK_PIN);

        if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
            int dtState = digitalRead(DT_PIN);
            if (dtState != currentStateCLK) {
                currentDisplayMode = static_cast<DisplayMode>((currentDisplayMode + 1) % 4);
            } else {
                currentDisplayMode = static_cast<DisplayMode>((currentDisplayMode - 1 + 4) % 4);
            }
        }
        lastStateCLK = currentStateCLK;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}