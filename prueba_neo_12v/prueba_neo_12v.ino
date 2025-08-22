#include <FastLED.h>

// Configuración básica
#define NUM_LEDS 8
#define DATA_PIN 3
#define LED_TYPE WS2812       // O WS2812, según tus LEDs
#define COLOR_ORDER GRB       // Ajusta si los colores no son correctos
#define BRIGHTNESS 255        // Ajustá entre 0-255 según lo que necesites

// Declaración de arreglos de LEDs
CRGB leds[NUM_LEDS];

void setup() {
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // Rellenar ambas tiras con colores arcoíris animados
  fill_rainbow(leds, NUM_LEDS, (millis() / 10) + 50); // desfase de color

  FastLED.show();
  delay(20);
}
