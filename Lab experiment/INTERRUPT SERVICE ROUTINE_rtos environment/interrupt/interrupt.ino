
#include <Arduino.h>

#define BUTTON_PIN 0

TaskHandle_t InterruptTaskHandle = NULL;

volatile int interruptCount = 0;
volatile unsigned long lastInterruptTime = 0;

// Interrupt Service Routine
void IRAM_ATTR ButtonISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Button debounce (1000 ms)
    unsigned long currentTime = millis();

    if (currentTime - lastInterruptTime >= 200)
    {
        lastInterruptTime = currentTime;

        interruptCount++;

        // Notify high priority task
        vTaskNotifyGiveFromISR(
            InterruptTaskHandle,
            &xHigherPriorityTaskWoken
        );

        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
}

// High Priority Task
void InterruptTask(void *parameter)
{
    while (1)
    {
        // Wait for interrupt notification
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        int count;

        // Safely read interrupt counter
        noInterrupts();
        count = interruptCount;
        interrupts();

        Serial.print("HIGH PRIORITY TASK -> Interrupt Count: ");
        Serial.println(count);

        Serial.println("Deferred processing executed.");
        Serial.println("------------------------------");
    }
}

// Low Priority Background Task
void BackgroundTask(void *parameter)
{
    while (1)
    {
        Serial.println(
            "LOW PRIORITY TASK -> Background processing"
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.println("=================================");
    Serial.println(" ISR PRIORITY AND DEFERRED PROCESSING");
    Serial.println("=================================");

    // Create High Priority Task
    BaseType_t task1 = xTaskCreate(
        InterruptTask,
        "InterruptTask",
        2048,
        NULL,
        3,
        &InterruptTaskHandle
    );

    // Create Low Priority Task
    BaseType_t task2 = xTaskCreate(
        BackgroundTask,
        "BackgroundTask",
        2048,
        NULL,
        1,
        NULL
    );

    // Check task creation
    if (task1 != pdPASS || task2 != pdPASS)
    {
        Serial.println("ERROR: Task creation failed!");
        while (1)
        {
            delay(1000);
        }
    }

    // Configure GPIO interrupt
    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        ButtonISR,
        FALLING
    );

    Serial.println("Interrupt configured on GPIO 0.");
    Serial.println("Interrupt Task Priority = 3");
    Serial.println("Background Task Priority = 1");
    Serial.println("Press the BOOT button.");
    Serial.println("=================================");
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
