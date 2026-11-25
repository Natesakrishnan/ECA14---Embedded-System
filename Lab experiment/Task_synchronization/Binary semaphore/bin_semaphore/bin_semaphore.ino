
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t binarySemaphore;

void ProducerTask(void *parameter)
{
  int count = 0;

  while (1)
  {
    count++;

    Serial.print("Producer: Event Generated ");
    Serial.println(count);

    if (xSemaphoreGive(binarySemaphore) == pdTRUE)
    {
      Serial.println("Producer: Signal Sent");
    }
    else
    {
      Serial.println("Producer: Semaphore Full");
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void ConsumerTask(void *parameter)
{
  while (1)
  {
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("Consumer: Signal Received");
      Serial.println("Consumer: Processing Event");
      Serial.println("-------------------------");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("BINARY SEMAPHORE EXPERIMENT");

  binarySemaphore = xSemaphoreCreateBinary();

  if (binarySemaphore == NULL)
  {
    Serial.println("Semaphore Creation Failed!");
    return;
  }

  xTaskCreate(ProducerTask, "Producer", 2048, NULL, 2, NULL);
  xTaskCreate(ConsumerTask, "Consumer", 2048, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
