#include <FastLED.h>

#define NUM_LEDS 241
#define NUM_LEDS2 400

#define DATA_PIN 9
#define DATA_PIN2 8

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];

#define INITIAL_BRIGHTNESS 100  // Para no sobrecargar la fuente
#define FRAMES_PER_SECOND 6

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(INITIAL_BRIGHTNESS);
}

void loop() {
  gradualFill(leds, NUM_LEDS, CRGB::Red);
  gradualFill(leds2, NUM_LEDS2, CRGB::Red);
  delay(1000);

  gradualFill(leds, NUM_LEDS, CRGB::Green);
  gradualFill(leds2, NUM_LEDS2, CRGB::Green);
  delay(1000);

  gradualFill(leds, NUM_LEDS, CRGB::Blue);
  gradualFill(leds2, NUM_LEDS2, CRGB::Blue);
  delay(1000);
}

// Enciende los LEDs de forma progresiva
void gradualFill(CRGB* ledArray, int numLeds, CRGB color) {
  for (int i = 0; i < numLeds; i++) {
    ledArray[i] = color;
    FastLED.show();
    delay(1000 / FRAMES_PER_SECOND / 4);  // Ajusta esto si querés más lento o más rápido
  }
}
