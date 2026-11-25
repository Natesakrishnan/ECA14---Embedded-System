#include <Arduino.h>

// FreeRTOS Mutex
SemaphoreHandle_t resourceMutex;

// Task Handles
TaskHandle_t LowTaskHandle = NULL;
TaskHandle_t MediumTaskHandle = NULL;
TaskHandle_t HighTaskHandle = NULL;

// Task priorities
#define LOW_PRIORITY     1
#define MEDIUM_PRIORITY  2
#define HIGH_PRIORITY    3

// Low Priority Task
void LowPriorityTask(void *parameter)
{
    while (1)
    {
        Serial.println("\nLOW PRIORITY TASK: Started");

        // Acquire shared resource
        if (xSemaphoreTake(resourceMutex, portMAX_DELAY) == pdTRUE)
        {
            Serial.println("LOW TASK: Mutex acquired");
            Serial.println("LOW TASK: Using shared resource");

            // Simulate resource usage
            for (int i = 1; i <= 5; i++)
            {
                Serial.print("LOW TASK: Working... ");
                Serial.println(i);

                vTaskDelay(pdMS_TO_TICKS(500));

                UBaseType_t currentPriority =
                    uxTaskPriorityGet(NULL);

                Serial.print("LOW TASK: Current Priority = ");
                Serial.println(currentPriority);
            }

            // Release shared resource
            xSemaphoreGive(resourceMutex);

            Serial.println("LOW TASK: Mutex released");
            Serial.print("LOW TASK: Restored Priority = ");
            Serial.println(uxTaskPriorityGet(NULL));
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

// Medium Priority Task
void MediumPriorityTask(void *parameter)
{
    while (1)
    {
        Serial.println("MEDIUM PRIORITY TASK: Executing");

        // Simulate CPU processing
        for (volatile int i = 0; i < 1000000; i++)
        {
            // Busy processing
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// High Priority Task
void HighPriorityTask(void *parameter)
{
    while (1)
    {
        // Allow Low Priority Task to acquire mutex first
        vTaskDelay(pdMS_TO_TICKS(1000));

        Serial.println("\nHIGH PRIORITY TASK: Requesting shared resource");

        Serial.print("HIGH TASK: Current Priority = ");
        Serial.println(uxTaskPriorityGet(NULL));

        // Request mutex
        if (xSemaphoreTake(resourceMutex, portMAX_DELAY) == pdTRUE)
        {
            Serial.println("HIGH TASK: Mutex acquired");
            Serial.println("HIGH TASK: Using shared resource");

            vTaskDelay(pdMS_TO_TICKS(500));

            xSemaphoreGive(resourceMutex);

            Serial.println("HIGH TASK: Mutex released");
        }

        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("========================================");
    Serial.println(" PRIORITY INVERSION AND INHERITANCE");
    Serial.println("========================================");

    // Create Mutex
    resourceMutex = xSemaphoreCreateMutex();

    if (resourceMutex == NULL)
    {
        Serial.println("ERROR: Mutex creation failed!");
        while (1)
        {
            delay(1000);
        }
    }

    // Create Low Priority Task
    BaseType_t lowResult = xTaskCreate(
        LowPriorityTask,
        "LowTask",
        2048,
        NULL,
        LOW_PRIORITY,
        &LowTaskHandle
    );

    // Create Medium Priority Task
    BaseType_t mediumResult = xTaskCreate(
        MediumPriorityTask,
        "MediumTask",
        2048,
        NULL,
        MEDIUM_PRIORITY,
        &MediumTaskHandle
    );

    // Create High Priority Task
    BaseType_t highResult = xTaskCreate(
        HighPriorityTask,
        "HighTask",
        2048,
        NULL,
        HIGH_PRIORITY,
        &HighTaskHandle
    );

    if (lowResult != pdPASS ||
        mediumResult != pdPASS ||
        highResult != pdPASS)
    {
        Serial.println("ERROR: Task creation failed!");
        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("Mutex created successfully.");
    Serial.println("Low Task Priority = 1");
    Serial.println("Medium Task Priority = 2");
    Serial.println("High Task Priority = 3");
    Serial.println("========================================");
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
