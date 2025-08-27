#include <FastLED.h>
#define NUM_LEDS 46 // 8 //46
#define NUM_STRIPS 8

CRGB leds[NUM_LEDS];
CLEDController *controllers[NUM_STRIPS];

void setup() {
  // put your setup code here, to run once:
  Setup_neo();

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 7; i++){
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    controllers[i]->showLeds(10);
  }

}

void Setup_neo()
{
  FastLED.clear();
  FastLED.show();
  controllers[0] = &FastLED.addLeds<WS2812, 2, BRG>(leds, NUM_LEDS);
  controllers[1] = &FastLED.addLeds<WS2812, 3, BRG>(leds, NUM_LEDS);
  controllers[2] = &FastLED.addLeds<WS2812, 5, BRG>(leds, NUM_LEDS);
  controllers[3] = &FastLED.addLeds<WS2812, 6, BRG>(leds, NUM_LEDS);
  controllers[4] = &FastLED.addLeds<WS2812, 8, BRG>(leds, NUM_LEDS);
  controllers[5] = &FastLED.addLeds<WS2812, 9, BRG>(leds, NUM_LEDS);
  controllers[6] = &FastLED.addLeds<WS2812, 11, BRG>(leds, NUM_LEDS);
  controllers[7] = &FastLED.addLeds<WS2812, 12, BRG>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

