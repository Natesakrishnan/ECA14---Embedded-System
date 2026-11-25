
#include <Arduino.h>

#define BLOCK_SIZE 32
#define NUM_BLOCKS 4

// Fixed-size memory pool
uint8_t pool[NUM_BLOCKS][BLOCK_SIZE];

// Block allocation status
bool used[NUM_BLOCKS] = {false, false, false, false};

// Memory Pool Allocation Function
int allocateBlock()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (!used[i])
        {
            used[i] = true;
            return i;
        }
    }

    return -1;
}

// Memory Pool Release Function
void releaseBlock(int block)
{
    if (block >= 0 && block < NUM_BLOCKS)
    {
        used[block] = false;
    }
}

// FreeRTOS Memory Management Task
void MemoryTask(void *parameter)
{
    Serial.println("\n--- HEAP MEMORY ---");

    // Measure heap before allocation
    size_t before = ESP.getFreeHeap();

    Serial.print("Heap before allocation: ");
    Serial.println(before);

    // Allocate memory for 10 integers
    int *data = (int *)pvPortMalloc(10 * sizeof(int));

    if (data != NULL)
    {
        // Initialize allocated memory
        for (int i = 0; i < 10; i++)
        {
            data[i] = i + 1;
        }

        size_t after = ESP.getFreeHeap();

        Serial.print("Heap after allocation: ");
        Serial.println(after);

        Serial.print("Heap reduction: ");
        Serial.println(before - after);

        // Release allocated memory
        vPortFree(data);

        Serial.print("Heap after release: ");
        Serial.println(ESP.getFreeHeap());
    }
    else
    {
        Serial.println("Dynamic memory allocation FAILED!");
    }

    // Stack High-Water Mark
    Serial.println("\n--- STACK HIGH-WATER MARK ---");

    UBaseType_t highWaterMark =
        uxTaskGetStackHighWaterMark(NULL);

    Serial.print("Minimum remaining stack: ");
    Serial.print(highWaterMark);
    Serial.println(" words");

    Serial.print("Approximate remaining stack: ");
    Serial.print(highWaterMark * sizeof(StackType_t));
    Serial.println(" bytes");

    // Memory Pool Test
    Serial.println("\n--- MEMORY POOL ---");

    int blocks[NUM_BLOCKS];

    Serial.print("Block size: ");
    Serial.print(BLOCK_SIZE);
    Serial.println(" bytes");

    Serial.print("Number of blocks: ");
    Serial.println(NUM_BLOCKS);

    Serial.print("Total pool capacity: ");
    Serial.print(NUM_BLOCKS * BLOCK_SIZE);
    Serial.println(" bytes");

    // Allocate all four blocks
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        blocks[i] = allocateBlock();

        if (blocks[i] != -1)
        {
            Serial.print("Block ");
            Serial.print(blocks[i] + 1);
            Serial.println(" allocated successfully");
        }
        else
        {
            Serial.println("Allocation failed!");
        }
    }

    // Attempt to allocate fifth block
    int extraBlock = allocateBlock();

    if (extraBlock == -1)
    {
        Serial.println("Block 5: FAILED - Pool FULL");
    }
    else
    {
        Serial.println("Unexpected additional allocation");
    }

    // Release all allocated blocks
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        releaseBlock(blocks[i]);
    }

    Serial.println("All blocks released successfully");

    // Verify memory reuse
    int testBlock = allocateBlock();

    if (testBlock != -1)
    {
        Serial.print("Block ");
        Serial.print(testBlock + 1);
        Serial.println(" successfully reallocated");

        releaseBlock(testBlock);

        Serial.println("Reallocated block released");
    }
    else
    {
        Serial.println("Memory pool reallocation FAILED");
    }

    Serial.println("\n--- MEMORY TEST COMPLETE ---");

    // Delete current task
    vTaskDelete(NULL);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("================================");
    Serial.println(" RTOS MEMORY MANAGEMENT TEST");
    Serial.println("================================");

    // Create FreeRTOS task
    BaseType_t result = xTaskCreate(
        MemoryTask,
        "MemoryTask",
        4096,
        NULL,
        1,
        NULL
    );

    if (result != pdPASS)
    {
        Serial.println("ERROR: Task creation failed!");
    }
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}
