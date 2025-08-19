#include <NewPing.h>

#define MAX_DISTANCE 200  // Distancia máxima a medir (en cm)

int sensorTest;
// Pines de sensores ultrasónicos
#define US_PIN_1 22
#define US_PIN_2 24
#define US_PIN_3 26
#define US_PIN_4 28
#define US_PIN_5 30
#define US_PIN_6 38
#define US_PIN_7 34
#define US_PIN_8 36
#define US_PIN_9 32
#define US_PIN_10 40
#define US_PIN_11 42
#define US_PIN_12 44


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
}

void loop() {
  /*
  for (int i = 0; i < NUM_SENSORS; i++) {
    delay(100);  // Pequeño delay entre lecturas para evitar interferencias
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
  delay(1000);  // Espera entre ciclos de lectura*/
  if(Serial.available()){
    sensorTest = Serial.readString().toInt();
  }
  unsigned int distance = sonar[sensorTest].ping_cm();
  Serial.print("Sensor ");
  Serial.print(sensorTest);
  Serial.print(": ");
  if (distance == 0) {
    Serial.println("Sin lectura");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(50);
}