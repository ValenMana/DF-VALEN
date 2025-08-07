#include <FastLED.h>

#define NUM_LEDS 241
#define DATA_PIN 9

CRGB leds[NUM_LEDS];

#define INITIAL_BRIGHTNESS 100
#define FRAMES_PER_SECOND 6

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(INITIAL_BRIGHTNESS);
}

void loop() {
  gradualFill(leds, NUM_LEDS, CRGB::Red);
  delay(1000);
  gradualFill(leds, NUM_LEDS, CRGB::Green);
  delay(1000);
  gradualFill(leds, NUM_LEDS, CRGB::Blue);
  delay(1000);
}

void gradualFill(CRGB* ledArray, int numLeds, CRGB color) {
  for (int i = 0; i < numLeds; i++) {
    ledArray[i] = color;
    FastLED.show();
    delay(1000 / FRAMES_PER_SECOND / 4);
  }
}
