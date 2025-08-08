#include <Wire.h>

int estado = 0;
int anterior = 0;

byte data = 0;

int puntaje = 0;

void setup() {
  Wire.begin(8);                // Dirección del esclavo
  Wire.onReceive(recibirDato);  // Configura función para recibir datos
  Serial.begin(115200);
  Wire.onRequest(responderAlMaestro);  // Se llama cuando el maestro hace requestFrom()
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Inicio");
}

void loop() {
  switch (estado) {
    case 0:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case 1:
      if (millis() - anterior < 250) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      if (millis() - anterior > 250) {
        digitalWrite(LED_BUILTIN, LOW);
      }
      if (millis() - anterior > 500) {
        anterior = millis();
        puntaje += 1;
        Serial.println(puntaje);
      }
      break;
    case 2:
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Fin");
      estado = 0;
      break;
  }
}

void recibirDato(int cuantos) {
  while (Wire.available()) {

    data = Wire.read();
    Serial.println(data);
    if (data == 254) {
      estado = 1;
      puntaje = 0;
      anterior = millis();
    }
    if (data == 255) {
      estado = 2;
    }
  }
}

void responderAlMaestro() {
  if (data == 255) {
    Serial.println("Puntaje:");
    Serial.println(puntaje);
    Wire.write(puntaje);  // Envía 1 byte (por ejemplo, 99)
  }
  //Serial.println("Respondí al maestro");
}
