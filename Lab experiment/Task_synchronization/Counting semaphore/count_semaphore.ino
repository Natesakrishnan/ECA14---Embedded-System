
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t countingSemaphore;

void ProducerTask(void *parameter)
{
  int count = 0;

  while (1)
  {
    count++;

    Serial.print("Producer: Generating Signal ");
    Serial.println(count);

    if (xSemaphoreGive(countingSemaphore) == pdTRUE)
    {
      Serial.print("Signal Added. Current Count: ");
      Serial.println(uxSemaphoreGetCount(countingSemaphore));
    }
    else
    {
      Serial.println("Semaphore FULL! Signal Not Added.");
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void ConsumerTask(void *parameter)
{
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(2000));

    if (xSemaphoreTake(countingSemaphore, 0) == pdTRUE)
    {
      Serial.print("Consumer: Signal Received. Remaining Count: ");
      Serial.println(uxSemaphoreGetCount(countingSemaphore));
    }
    else
    {
      Serial.println("Consumer: No Pending Signal");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("COUNTING SEMAPHORE EXPERIMENT");

  countingSemaphore = xSemaphoreCreateCounting(3, 0);

  if (countingSemaphore == NULL)
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
