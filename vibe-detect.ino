#define VIBE_PIN 9             // Digital pin connected to SW-420 module DO pin
#define LED_PIN 12             // LED pin to show vibration
const unsigned long debounceDelay = 100;        // Ignore repeated triggers within 100ms
const unsigned long minPulseDuration = 10;      // Minimum pulse duration (ms)
const unsigned long detectWindow = 2000;        // 2 second window to check for 3 hits
const unsigned long resetWindow = 5000;         // 5 second window to reset counter

unsigned long lastTriggerTime = 0;
unsigned long windowStartTime = 0;
unsigned long lastResetTime = 0;
int vibrationCount = 0;

void setup() {
  pinMode(VIBE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int signal = digitalRead(VIBE_PIN);
  Serial.print("Threshold: ");
  Serial.println(debounceDelay);

  if (signal == HIGH) {
    unsigned long pulseStart = millis();

    // Wait for signal to go LOW or timeout
    while (digitalRead(VIBE_PIN) == HIGH && (millis() - pulseStart < 100)) {
      // wait
    }

    unsigned long pulseDuration = millis() - pulseStart;

    if (pulseDuration >= minPulseDuration && millis() - lastTriggerTime >= debounceDelay) {
      if (vibrationCount == 0) {
        windowStartTime = millis(); // Start detection window
      }

      vibrationCount++;
      lastTriggerTime = millis();
      Serial.print("✔ Vibration Detected! Count: ");
      Serial.println(vibrationCount);
    }
  }

  unsigned long now = millis();

  // Check if 2-second detection window passed
  if (vibrationCount >= 3 && (now - windowStartTime <= detectWindow)) {
    flashLED(5);
    vibrationCount = 0;
    lastResetTime = now;
  }

  // Reset counter after 5 seconds regardless of vibration count
  if (now - lastResetTime >= resetWindow) {
    vibrationCount = 0;
    lastResetTime = now;
  }
}

void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}