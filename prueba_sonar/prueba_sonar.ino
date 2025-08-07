#include <NewPing.h>

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

#define ULTRASONIC_PIN US_PIN_2

#define MAX_DISTANCE 200  // Distancia máxima en cm que el sensor puede medir

// Crear un objeto NewPing con un solo pin
NewPing sonar(ULTRASONIC_PIN, ULTRASONIC_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Obtener la distancia en cm
  unsigned int distance = sonar.ping_cm();

  // Imprimir la distancia
  Serial.print("Distancia: ");
  if (distance == 0) {
    Serial.println("Fuera de rango");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(500);  // Esperar medio segundo antes de la siguiente medición
}
