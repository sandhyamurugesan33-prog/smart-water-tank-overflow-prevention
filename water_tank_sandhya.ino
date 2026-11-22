#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- PINS ----------
const int TRIG_PIN   = 9;
const int ECHO_PIN   = 10;
const int RELAY_PIN  = 7;
const int BUZZER_PIN = 8;

// ---------- TANK SETTINGS (measure and adjust, in cm) ----------
const float FULL_DIST  = 5.0;   // this close to sensor = tank FULL -> pump OFF
const float EMPTY_DIST = 20.0;  // this far from sensor = tank LOW  -> pump ON

// ---------- RELAY TYPE (swap if pump works in reverse) ----------
const int RELAY_ON  = LOW;
const int RELAY_OFF = HIGH;

// ---------- BUZZER SETTINGS ----------
// Resting level of the buzzer pin when silent.
// If the buzzer buzzes constantly, change this to HIGH.
const int BUZZER_OFF = LOW;
const int BUZZER_FREQ = 2000;   // tone in Hz
const int BUZZER_TIME = 500;    // beep length in milliseconds

LiquidCrystal_I2C lcd(0x27, 16, 2);  // try 0x3F if the LCD stays blank

bool pumpOn = false;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.0343 / 2.0;
}

float averageDistance() {
  float sum = 0;
  int count = 0;
  for (int i = 0; i < 5; i++) {
    float d = readDistanceCm();
    if (d > 0) { sum += d; count++; }
    delay(30);
  }
  if (count == 0) return -1;
  return sum / count;
}

// One single beep
void beepOnce() {
  tone(BUZZER_PIN, BUZZER_FREQ);
  delay(BUZZER_TIME);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, BUZZER_OFF);
}

void setPump(bool on) {
  pumpOn = on;
  digitalWrite(RELAY_PIN, on ? RELAY_ON : RELAY_OFF);
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, BUZZER_OFF);
  setPump(false);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Water Tank");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  beepOnce();                            // startup test beep (delete this line later)
  delay(1000);
  lcd.clear();
}

void loop() {
  float d = averageDistance();

  if (d < 0) {
    setPump(false);                      // safety: pump off if sensor fails
    lcd.setCursor(0, 0);
    lcd.print("Sensor error    ");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring    ");
    delay(500);
    return;
  }

  // Convert distance to level percentage
  float level = (EMPTY_DIST - d) / (EMPTY_DIST - FULL_DIST) * 100.0;
  level = constrain(level, 0, 100);

  // Pump logic with two thresholds, one beep each time it switches
  if (pumpOn && d <= FULL_DIST) {
    setPump(false);                      // tank full -> pump OFF
    beepOnce();
  } else if (!pumpOn && d >= EMPTY_DIST) {
    setPump(true);                       // tank low -> pump ON
    beepOnce();
  }

  // Display
  lcd.setCursor(0, 0);
  lcd.print("Water Level:");
  lcd.print((int)level);
  lcd.print("%  ");
  lcd.setCursor(0, 1);
  lcd.print(pumpOn ? "Pump: ON         " : "Pump: OFF        ");

  Serial.print("Distance: ");
  Serial.print(d);
  Serial.print(" cm  Level: ");
  Serial.print(level);
  Serial.print("%  Pump: ");
  Serial.println(pumpOn ? "ON" : "OFF");

  delay(300);
}