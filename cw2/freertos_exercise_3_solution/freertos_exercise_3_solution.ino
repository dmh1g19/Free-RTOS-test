#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <FreeRTOSConfig.h>

// The idea here is to treat the tasks are a binary switch

#define SERIAL_PORT Serial

#define LED_PIN LED_BUILTIN

QueueHandle_t xQueue;

void vPrintString(const char *pcString)
{
  taskENTER_CRITICAL();
  {
    SERIAL_PORT.print(pcString);
    //fflush(stdout);
  }
  taskEXIT_CRITICAL();
}

static void vSenderTask(void *pvParameters)
{
  int32_t lValueToSend = (int32_t)pvParameters;
  BaseType_t xStatus;

  for (;;)
  {
    xStatus = xQueueSendToBack(xQueue, &lValueToSend, pdMS_TO_TICKS(1000));
    if (xStatus != pdPASS)
    {
      vPrintString("Could not send to the queue.\r\n");
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); // Send every 1 second
    lValueToSend = (lValueToSend == 100) ? 200 : 100; // Toggle between 100 and 200
  }
}

static void vReceiverTask(void *pvParameters)
{
  int32_t lReceivedValue;
  BaseType_t xStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(1200);

  for (;;)
  {
    xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);
    if (xStatus == pdPASS)
    {
      // Toggle LED based on the received value
      if (lReceivedValue == 100)
      {
        digitalWrite(LED_PIN, HIGH); // Turn LED ON
        vPrintString("LED ON!\r\n");
      }
      else if (lReceivedValue == 200)
      {
        digitalWrite(LED_PIN, LOW); // Turn LED OFF
        vPrintString("LED OFF!\r\n");
      }
    }
    else
    {
      vPrintString("Could not receive from the queue.\r\n");
    }
  }
}

void setup() {
  SERIAL_PORT.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  xQueue = xQueueCreate(5, sizeof(int32_t));
  if (xQueue != NULL)
  {
    xTaskCreate(vSenderTask, "Sender1", 1000, (void *)100, 1, NULL);
    xTaskCreate(vSenderTask, "Sender2", 1000, (void *)200, 1, NULL);
    xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);
  }
}

void loop() {
}
