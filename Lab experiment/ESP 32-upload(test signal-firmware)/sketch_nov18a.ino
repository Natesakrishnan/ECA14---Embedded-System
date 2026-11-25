void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("ESP32_READY");
}

void loop() {
  Serial.println("TEST_SIGNAL");
  delay(1000);
}