
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"

StreamBufferHandle_t streamBuffer;

void ProducerTask(void *parameter)
{
  const char message[] = "DATA\n";

  while (1)
  {
    size_t sent = xStreamBufferSend(
      streamBuffer,
      message,
      sizeof(message) - 1,
      pdMS_TO_TICKS(1000)
    );

    if (sent == sizeof(message) - 1)
    {
      Serial.println("Producer: Byte Stream Sent - DATA");
    }
    else
    {
      Serial.println("Producer: Stream Send Incomplete");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void ConsumerTask(void *parameter)
{
  char receivedData[17];

  while (1)
  {
    size_t received = xStreamBufferReceive(
      streamBuffer,
      receivedData,
      16,
      pdMS_TO_TICKS(2000)
    );

    if (received > 0)
    {
      receivedData[received] = '\0';

      Serial.print("Consumer: Received Bytes = ");
      Serial.println(received);

      Serial.print("Consumer: Received Data = ");
      Serial.print(receivedData);

      Serial.println("Consumer: Stream Processed");
      Serial.println("-------------------------");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("STREAM BUFFER EXPERIMENT");

  streamBuffer = xStreamBufferCreate(100, 1);

  if (streamBuffer == NULL)
  {
    Serial.println("Stream Buffer Creation Failed!");
    return;
  }

  xTaskCreate(ProducerTask, "Producer", 2048, NULL, 2, NULL);
  xTaskCreate(ConsumerTask, "Consumer", 2048, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
