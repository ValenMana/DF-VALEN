#include <NewPing.h>
#include <Adafruit_NeoPixel.h>

#define MAX_DISTANCE 200  // Distancia máxima a medir (en cm)

// Pines de sensores ultrasónicos
#define US_PIN_1 22
#define US_PIN_2 24
#define US_PIN_3 26
#define US_PIN_4 28
#define US_PIN_5 30
#define US_PIN_6 32
#define US_PIN_7 34
#define US_PIN_8 36
#define US_PIN_9 38
#define US_PIN_10 40
#define US_PIN_11 42
#define US_PIN_12 44

// NEOPIXELS

#define NP_PIN_1 23
#define NP_PIN_2 25
#define NP_PIN_3 27
#define NP_PIN_4 29
#define NP_PIN_5 31
#define NP_PIN_6 33

#define PIXELS_NUM 40
#define STICK_NUM 6


Adafruit_NeoPixel pixels[STICK_NUM] = {
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_6, NEO_GRB + NEO_KHZ800)
};

// Crear los objetos de los sensores (modo de pin único)
NewPing sonar[] = {
  NewPing(US_PIN_1, US_PIN_1, MAX_DISTANCE),
  NewPing(US_PIN_2, US_PIN_2, MAX_DISTANCE),
  NewPing(US_PIN_3, US_PIN_3, MAX_DISTANCE),
  NewPing(US_PIN_4, US_PIN_4, MAX_DISTANCE),
  NewPing(US_PIN_5, US_PIN_5, MAX_DISTANCE),
  NewPing(US_PIN_6, US_PIN_6, MAX_DISTANCE),
  NewPing(US_PIN_7, US_PIN_7, MAX_DISTANCE),
  NewPing(US_PIN_8, US_PIN_8, MAX_DISTANCE),
  NewPing(US_PIN_9, US_PIN_9, MAX_DISTANCE),
  NewPing(US_PIN_10, US_PIN_10, MAX_DISTANCE),
  NewPing(US_PIN_11, US_PIN_11, MAX_DISTANCE),
  NewPing(US_PIN_12, US_PIN_12, MAX_DISTANCE)
};

#define NUM_SENSORS 12

void setup() {
  Serial.begin(9600);
  Serial.println("Test de sensores ultrasónicos:");

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].begin();
  }

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].clear();
    pixels[i].show();
  }
}

void loop() {
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    delay(50);  // Pequeño delay entre lecturas para evitar interferencias
    unsigned int distance = sonar[i].ping_cm();
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    if (distance == 0) {
      Serial.println("Sin lectura");
    } else {
      Serial.print(distance);
      Serial.println(" cm");
    }
  }
  Serial.println("--------------------");
  

  /*
  for (int i = 0; i < 6; i++) {
    int estado1 = sonar[i * 2].ping_cm();

    int estado2 = sonar[i * 2 + 1].ping_cm();


    // Si alguno de los dos sensores está activado
    if ((estado1 < 50 || estado2 < 50) && estado1 != 0 && estado2 != 0) {
      aroSelect(i);
      //delay(1000);  // Encender LED
    } else {
      clear_neo();  // Apagar LED
      if (estado1 == 0) {
        Serial.println("Fallo el sensor: " + String(i * 2));
      }
      if (estado2 == 0) {
        Serial.println("Fallo el sensor: " + String(i * 2 + 1));
      }
    }
*/
    delay(2500);  // Espera entre ciclos de lectura
  
}


void aroSelect(int aro) {

  for (int j = 0; j < PIXELS_NUM; j++) {
    pixels[aro].setPixelColor(j, pixels[aro].Color(0, 150, 0));
    pixels[aro].show();
  }
}

void clear_neo() {

  for (int i = 0; i < STICK_NUM; i++) {
    for (int j = 0; j < PIXELS_NUM; j++) {
      pixels[i].setPixelColor(j, pixels[i].Color(0, 0, 0));
      pixels[i].show();
    }
  }
}
