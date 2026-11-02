
#define GAS_PIN A0
#define PIR_PIN 2
#define TRIG_PIN 8
#define ECHO_PIN 9
#define BUZZER 10
#define LED 11

int gasThreshold = 500;
int occupancyDistance = 100; // cm

void setup()
{
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
}

void loop()
{
  int gas = analogRead(GAS_PIN);
  int motion = digitalRead(PIR_PIN);

  // Ultrasonic distance measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = -1;

  if (duration > 0)
  {
    distance = duration * 0.0343 / 2;
  }

  // Determine occupancy
  bool occupied = (motion == HIGH) ||
                 (distance > 0 &&
                  distance <= occupancyDistance);

  bool gasAlert = (gas > gasThreshold);

  // Gas alert outputs
  digitalWrite(BUZZER, gasAlert ? HIGH : LOW);

  // LED indicates occupancy or gas alert
  digitalWrite(LED, (occupied || gasAlert) ? HIGH : LOW);

  // Serial Monitor / Proteus Virtual Terminal
  Serial.print("Gas Level: ");
  Serial.println(gas);

  Serial.print("Motion: ");
  Serial.println(motion ? "DETECTED" : "NOT DETECTED");

  Serial.print("Distance: ");

  if (distance < 0)
    Serial.println("No Echo");
  else
  {
    Serial.print(distance, 1);
    Serial.println(" cm");
  }

  if (gasAlert)
    Serial.println("GAS ALERT!");
  else if (occupied)
    Serial.println("OCCUPANCY DETECTED");
  else
    Serial.println("SYSTEM SAFE - NO OCCUPANCY");

  Serial.println("--------------------");

  delay(1000);
}
