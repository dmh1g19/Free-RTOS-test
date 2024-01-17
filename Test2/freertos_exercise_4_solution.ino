#include <FreeRTOS.h>
#include <task.h>

#include "hardware/adc.h"
#include "hardware/gpio.h"

#define SERIAL_PORT Serial
#define LED_PIN LED_BUILTIN
#define TEMP_BUFFER_SIZE 5

float tempBuffer[TEMP_BUFFER_SIZE];
int tempBufferIndex = 0; 

/* Task1 flashes the onboard LED, task2 retrieves the avg remperature of the room */
/* Task1 gets higher priority (RMS) as it has a smaller period 5 < 10 */

void task1(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  const TickType_t xFrequency = pdMS_TO_TICKS(5000); // PERIOD

  pinMode(LED_PIN, OUTPUT);

  for (;;) {

    const TickType_t xFlashDuration = pdMS_TO_TICKS(1000); // EXEC TIME
    
    TickType_t xEndTime = xTaskGetTickCount() + xFlashDuration;
    
    SERIAL_PORT.println("Task 1: Flashing LED");

    while (xTaskGetTickCount() < xEndTime) {
      digitalWrite(LED_PIN, HIGH);
      vTaskDelay(pdMS_TO_TICKS(100));
      digitalWrite(LED_PIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

// Task2 could be used to update a numerical display**
// prints the average temperature, reads the temperature of the room
// updates a buffer and performs operations in the 2 seconds

void task2(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(10000); // 10 seconds
  const TickType_t xExecutionTime = pdMS_TO_TICKS(2000); // 2 seconds

  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);

  for (;;) {
    TickType_t xStartTime = xTaskGetTickCount();
    TickType_t xEndTime = xStartTime + xExecutionTime;

    float averageTemp = 0;

    while (xTaskGetTickCount() < xEndTime) {
      uint16_t raw = adc_read();
      const float conversionFactor = 3.3f / (1 << 12);
      float voltage = raw * conversionFactor;
      float temperature = 27 - (voltage - 0.706) / 0.001721;

      tempBuffer[tempBufferIndex] = temperature;
      tempBufferIndex = (tempBufferIndex + 1) % TEMP_BUFFER_SIZE;

      float tempSum = 0;
      for (int i = 0; i < TEMP_BUFFER_SIZE; i++) {
        tempSum += tempBuffer[i];
      }
      
      averageTemp = tempSum / TEMP_BUFFER_SIZE;
    }
    
    SERIAL_PORT.print("Avg Temp: ");
    SERIAL_PORT.print(averageTemp);
    SERIAL_PORT.println(" C");

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void setup() {
  SERIAL_PORT.begin(115200);

  xTaskCreate(task1, "Task 1", 1000, NULL, 2, NULL); // Higher priority
  xTaskCreate(task2, "Task 2", 1000, NULL, 1, NULL); // Lower priority
}

void loop() {
}
