#include <Adafruit_NeoPixel.h>

#define NUM_STRIPS 11
#define NUM_LEDS_PER_STRIP 33

// Pines asignados a cada tira (ajustá según tu conexión)
const uint8_t stripPins[NUM_STRIPS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// Creamos un arreglo de objetos NeoPixel
Adafruit_NeoPixel strips[NUM_STRIPS] = {
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 7, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 8, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 9, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 10, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 11, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 12, NEO_GRB + NEO_KHZ800)
};

void setup() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    strips[i].begin();
    strips[i].show(); // Inicializa apagados
  }
}

void loop() {
  // Efecto simple: color rojo subiendo tira por tira
  for (int s = 0; s < NUM_STRIPS; s++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      strips[s].setPixelColor(i, strips[s].Color(255, 0, 0)); // Rojo
      strips[s].show();
      delay(10);  // Delay suave entre píxeles
    }
  }

  // Apagar todos
  for (int s = 0; s < NUM_STRIPS; s++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      strips[s].setPixelColor(i, 0); // Apaga
    }
    strips[s].show();
  }

  delay(1000);
}

void encenderTira(int numeroTira, uint8_t r, uint8_t g, uint8_t b) {
  if (numeroTira < 0 || numeroTira >= NUM_STRIPS) return; 

  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    strips[numeroTira].setPixelColor(i, strips[numeroTira].Color(r, g, b));
  }
  strips[numeroTira].show();
}