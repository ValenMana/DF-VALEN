const int buttonPin = 8;  // the number of the pushbutton pin

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  // set initial LED state
}

void loop() {
  Serial.println(debounce(8));
}

bool debounce(int pin) {
  const unsigned long debounceDelay = 50; // Tiempo de rebote en ms

  static int lastButtonState = LOW;
  static int buttonState = LOW;
  static unsigned long lastDebounceTime = 0;

  int reading = digitalRead(pin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  lastButtonState = reading;

  return buttonState;
}
