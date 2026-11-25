#include <Arduino.h>

// Task execution counters
int highCount = 0;
int mediumCount = 0;
int lowCount = 0;

// High Priority Task - Priority 3
void HighTask(void *parameter)
{
  while (1)
  {
    highCount++;

    Serial.print("HighTask Running | Count: ");
    Serial.println(highCount);

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// Medium Priority Task - Priority 2
void MediumTask(void *parameter)
{
  while (1)
  {
    mediumCount++;

    Serial.print("MediumTask Running | Count: ");
    Serial.println(mediumCount);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Low Priority Task - Priority 1
void LowTask(void *parameter)
{
  while (1)
  {
    lowCount++;

    Serial.print("LowTask Running | Count: ");
    Serial.println(lowCount);

    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("FreeRTOS Task Priority Experiment");
  Serial.println("---------------------------------");

  // Create High Priority Task
  xTaskCreate(
    HighTask,
    "HighTask",
    2048,
    NULL,
    3,
    NULL
  );

  // Create Medium Priority Task
  xTaskCreate(
    MediumTask,
    "MediumTask",
    2048,
    NULL,
    2,
    NULL
  );

  // Create Low Priority Task
  xTaskCreate(
    LowTask,
    "LowTask",
    2048,
    NULL,
    1,
    NULL
  );
}

void loop()
{
  delay(1000);
}
