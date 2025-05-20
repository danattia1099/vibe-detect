#define VIBE_PIN 9            // Digital pin connected to SW-420 module DO pin
#define LED_PIN 12            // LED pin to show vibration
const unsigned long debounceDelay = 100;  // Ignore repeated triggers within 100ms
const unsigned long minPulseDuration = 10; // Must be HIGH for at least 10ms

unsigned long lastTriggerTime = 0;
bool vibeDetected = false;

void setup() {
  pinMode(VIBE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Wait for a stable HIGH signal
  if (digitalRead(VIBE_PIN) == HIGH) {
    unsigned long pulseStart = millis();

    // Stay here while still HIGH, up to 100ms
    while (digitalRead(VIBE_PIN) == HIGH && (millis() - pulseStart < 100)) {
      // do nothing, just wait
    }

    unsigned long pulseDuration = millis() - pulseStart;

    // If pulse was long enough and we're past debounce time
    if (pulseDuration >= minPulseDuration &&
        millis() - lastTriggerTime >= debounceDelay) {

      vibeDetected = true;
      lastTriggerTime = millis();
    }
  }

  if (vibeDetected) {
    Serial.println("✔ Vibration detected!");
    digitalWrite(LED_PIN, HIGH); // Optional LED blink
    delay(50);
    digitalWrite(LED_PIN, LOW);

    vibeDetected = false;
  }
}