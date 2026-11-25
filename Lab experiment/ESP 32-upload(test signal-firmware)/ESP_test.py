import serial
import time

# Serial configuration
PORT = "COM8"
BAUD_RATE = 115200
REQUIRED_TESTS = 5

pass_count = 0
fail_count = 0

print("ESP32 AUTOMATED FIRMWARE TEST")
print("-----------------------------")

try:
    esp = serial.Serial(PORT, BAUD_RATE, timeout=3)

    time.sleep(2)
    esp.reset_input_buffer()

    print("Connected to ESP32 successfully\n")

    while pass_count < REQUIRED_TESTS:

        data = esp.readline().decode(
            'utf-8', errors='ignore'
        ).strip()

        if not data:
            continue

        print("Received:", data)

        if data == "TEST_SIGNAL":
            pass_count += 1
            print("PASS - Test Cycle", pass_count)

    esp.close()

except serial.SerialException as e:
    print("Serial Error:", e)

print("\n--------- TEST SUMMARY ---------")
print("Total Test Cycles:", REQUIRED_TESTS)
print("Successful Tests:", pass_count)
print("Failed Tests:", fail_count)

if pass_count == REQUIRED_TESTS and fail_count == 0:
    print("FINAL RESULT: PASS")
else:
    print("FINAL RESULT: FAIL")
