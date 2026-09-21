#include "rtos_objects.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void DisplayTask(void *pvParameters) {
    SensorData receivedData;

    Wire.begin(21, 22);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        vTaskDelete(NULL);
    }
    
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();

    while (1) {
        if (xQueueReceive(sensorQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                
                display.clearDisplay();
                display.setCursor(0,0);
                display.setTextSize(1);
                
                display.println("ROOM MONITOR");
                display.print("Mode: ");
                
                switch (currentDisplayMode) {
                    case TEMP_MODE:
                        display.println("Temperature");
                        display.print("Val:  ");
                        display.print(receivedData.temperature, 1);
                        display.print(" C");
                        break;
                    case HUMIDITY_MODE:
                        display.println("Humidity");
                        display.print("Val:  ");
                        display.print(receivedData.humidity, 1);
                        display.print(" %");
                        break;
                    case LIGHT_MODE:
                        display.println("Light Lvl");
                        display.print("Val:  ");
                        display.print(receivedData.lightLevel);
                        display.print(" %");
                        break;
                    case MOTION_MODE:
                        display.println("Motion");
                        display.print("Val:  ");
                        if ((xEventGroupGetBits(systemEvents) & MOTION_DETECTED_BIT) != 0) {
                            display.print("ACTIVE");
                        } else {
                            display.print("INACTIVE");
                        }
                        break;
                }
                
                display.display(); 
                xSemaphoreGive(serialMutex);
            }
        }
    }
}