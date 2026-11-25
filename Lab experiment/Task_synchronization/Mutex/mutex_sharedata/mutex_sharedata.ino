
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mutex;

int sharedCounter = 0;

void Task1(void *parameter)
{
  while (1)
  {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
    {
      sharedCounter++;

      Serial.print("Task 1: Shared Counter = ");
      Serial.println(sharedCounter);

      xSemaphoreGive(mutex);
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void Task2(void *parameter)
{
  while (1)
  {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
    {
      sharedCounter++;

      Serial.print("Task 2: Shared Counter = ");
      Serial.println(sharedCounter);

      xSemaphoreGive(mutex);
    }

    vTaskDelay(pdMS_TO_TICKS(700));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("MUTEX SHARED DATA PROTECTION");

  mutex = xSemaphoreCreateMutex();

  if (mutex == NULL)
  {
    Serial.println("Mutex Creation Failed!");
    return;
  }

  xTaskCreate(Task1, "Task1", 2048, NULL, 2, NULL);
  xTaskCreate(Task2, "Task2", 2048, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
