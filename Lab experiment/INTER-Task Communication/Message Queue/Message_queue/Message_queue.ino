#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t messageQueue;

void ProducerTask(void *parameter)
{
  uint32_t data = 100;

  while (1)
  {
    if (xQueueSend(messageQueue, &data, 0) == pdPASS)
    {
      Serial.print("Producer: Sent Data = ");
      Serial.println(data);
    }
    else
    {
      Serial.println("Producer: Queue FULL!");
    }

    data += 100;

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void ConsumerTask(void *parameter)
{
  uint32_t receivedData;

  while (1)
  {
    if (xQueueReceive(messageQueue, &receivedData,
                      portMAX_DELAY) == pdPASS)
    {
      Serial.print("Consumer: Received Data = ");
      Serial.println(receivedData);

      Serial.println("Consumer: Processing Data");
      Serial.println("-------------------------");

      vTaskDelay(pdMS_TO_TICKS(1500));
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("MESSAGE QUEUE EXPERIMENT");

  messageQueue = xQueueCreate(5, sizeof(uint32_t));

  if (messageQueue == NULL)
  {
    Serial.println("Queue Creation Failed!");
    return;
  }

  xTaskCreate(ProducerTask, "Producer", 2048, NULL, 2, NULL);
  xTaskCreate(ConsumerTask, "Consumer", 2048, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
