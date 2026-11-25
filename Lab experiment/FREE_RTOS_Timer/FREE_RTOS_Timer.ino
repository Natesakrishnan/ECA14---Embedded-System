#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

// Event Group bit definitions
#define TIMER1_EVENT (1 << 0)
#define TIMER2_EVENT (1 << 1)

#define ALL_EVENTS (TIMER1_EVENT | TIMER2_EVENT)

// Event Group handle
EventGroupHandle_t eventGroup;

// Software Timer handles
TimerHandle_t timer1;
TimerHandle_t timer2;

// Timer 1 callback - 1000 ms
void Timer1Callback(TimerHandle_t xTimer)
{
  xEventGroupSetBits(eventGroup, TIMER1_EVENT);
}

// Timer 2 callback - 2000 ms
void Timer2Callback(TimerHandle_t xTimer)
{
  xEventGroupSetBits(eventGroup, TIMER2_EVENT);
}

// Event Monitoring Task
void EventMonitorTask(void *parameter)
{
  EventBits_t bits;

  while (1)
  {
    Serial.println("Waiting for both timer events...");

    // Wait until both event bits are set
    bits = xEventGroupWaitBits(
      eventGroup,
      ALL_EVENTS,
      pdTRUE,
      pdTRUE,
      portMAX_DELAY
    );

    // Check whether both events are received
    if ((bits & ALL_EVENTS) == ALL_EVENTS)
    {
      Serial.println("Timer 1 Event Received - BIT 0");
      Serial.println("Timer 2 Event Received - BIT 1");

      Serial.println("Both Events Received!");
      Serial.println("Synchronization Completed");
      Serial.println("------------------------------");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("RTOS TIMER AND EVENT GROUP EXPERIMENT");
  Serial.println("-------------------------------------");

  // Create Event Group
  eventGroup = xEventGroupCreate();

  if (eventGroup == NULL)
  {
    Serial.println("Event Group Creation Failed!");
    return;
  }

  // Create Timer 1 - 1000 ms periodic timer
  timer1 = xTimerCreate(
    "Timer1",
    pdMS_TO_TICKS(1000),
    pdTRUE,
    NULL,
    Timer1Callback
  );

  // Create Timer 2 - 2000 ms periodic timer
  timer2 = xTimerCreate(
    "Timer2",
    pdMS_TO_TICKS(2000),
    pdTRUE,
    NULL,
    Timer2Callback
  );

  if (timer1 == NULL || timer2 == NULL)
  {
    Serial.println("Timer Creation Failed!");
    return;
  }

  // Create Event Monitoring Task
  if (xTaskCreate(
        EventMonitorTask,
        "EventMonitor",
        4096,
        NULL,
        2,
        NULL
      ) != pdPASS)
  {
    Serial.println("Task Creation Failed!");
    return;
  }

  // Start both timers
  if (xTimerStart(timer1, 0) != pdPASS ||
      xTimerStart(timer2, 0) != pdPASS)
  {
    Serial.println("Timer Start Failed!");
    return;
  }

  Serial.println("Both Timers Started Successfully");
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
