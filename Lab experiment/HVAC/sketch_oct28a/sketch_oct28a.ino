#define TEMP A0
#define FAN  8

volatile byte pwm = 0;
volatile byte count = 0;

ISR(TIMER1_COMPA_vect)
{
  count++;

  if (count >= 100)
    count = 0;

  digitalWrite(FAN, count < pwm);
}

void setup()
{
  Serial.begin(9600);
  pinMode(FAN, OUTPUT);

  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11);

  OCR1A = 199;

  TIMSK1 = (1 << OCIE1A);

  sei();
}

void loop()
{
  int adc = analogRead(TEMP);

  float temp = adc * 500.0 / 1023.0;

  if (temp < 25)
    pwm = 0;
  else if (temp < 30)
    pwm = 30;
  else if (temp < 35)
    pwm = 60;
  else
    pwm = 90;

  Serial.print("Temperature: ");
  Serial.print(temp, 1);

  Serial.print(" C  PWM: ");
  Serial.print(pwm);

  Serial.println("%");

  delay(1000);
}
