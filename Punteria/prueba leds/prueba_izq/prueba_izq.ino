#include <FastLED.h>

#define NUM_LEDS2 338 //36
#define DATA_PIN2 8

CRGB leds2[NUM_LEDS2];

#define INITIAL_BRIGHTNESS 100
#define FRAMES_PER_SECOND 1

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(INITIAL_BRIGHTNESS);
}

void loop() {
  gradualFill(leds2, 217, CRGB::Red);
  /*delay(1000);
  gradualFill(leds2, NUM_LEDS2, CRGB::Green);
  delay(1000);
  gradualFill(leds2, NUM_LEDS2, CRGB::Blue);*/
  delay(1000);
  clean(leds2);
  delay(2000);
}

void gradualFill(CRGB* ledArray, int numLeds, CRGB color) {
  for (int i = 0; i < numLeds; i++) {
    ledArray[i] = color;
    FastLED.show();
    delay(10 / FRAMES_PER_SECOND / 4);
  }
}

void clean(CRGB* ledArray) {
  for (int i = 0; i < NUM_LEDS2; i++) {
    ledArray[i] = 0;
    FastLED.show();
    delay(1);
  }
}