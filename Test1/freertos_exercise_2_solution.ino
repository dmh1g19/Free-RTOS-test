#include <FreeRTOS.h>
#include <task.h>

#define SERIAL_PORT Serial

#define BLINK_ON_TIME  2000
#define BLINK_OFF_TIME 2000

#define STACK_SIZE 200

StaticTask_t xTaskBuffer_A;
StaticTask_t xTaskBuffer_B;

StackType_t xStack_A[STACK_SIZE];
StackType_t xStack_B[STACK_SIZE];

TaskHandle_t xHandle_A;
TaskHandle_t xHandle_B;

void setup() 
{
  SERIAL_PORT.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  xHandle_A = xTaskCreateStatic(led_ON, "LED ON Task", STACK_SIZE, NULL, 1, xStack_A, &xTaskBuffer_A);
  xHandle_B = xTaskCreateStatic(led_OFF, "LED OFF Task", STACK_SIZE, NULL, 1, xStack_B, &xTaskBuffer_B);
}

// Turn on the LED
// Wait for ON duration
// Notify the other task to turn off the LED
// Wait for the notification to turn back on
void led_ON(void *pvParameters)
{
  while (1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    SERIAL_PORT.println("LED ON!");
    vTaskDelay(pdMS_TO_TICKS(BLINK_ON_TIME));
    xTaskNotifyGive(xHandle_B);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
}

// Wait for the notification to turn off the LED
// Turn off the LED
// Wait for OFF duration
// Notify the other task to turn on the LED
void led_OFF(void *pvParameters)
{
  while (1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    SERIAL_PORT.println("LED OFF!");
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(BLINK_OFF_TIME));
    xTaskNotifyGive(xHandle_A);
  }
}

void loop() { 
}
