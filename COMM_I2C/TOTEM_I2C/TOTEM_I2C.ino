#include <Wire.h>
int estado = 0;
int anterior;


void setup() {
  Wire1.begin(25, 26, 100000);  // SDA=21, SCL=22
  Serial.begin(115200);
  pinMode(12, INPUT_PULLUP);

  Serial.println("Inicio TOTEM");
  //delay(10000);
}

void loop() {

  switch (estado) {
    case 0:
      if (digitalRead(12) == 0) {
        delay(150);
        Wire1.beginTransmission(8);  // Dirección del esclavo
        Wire1.write(254);
        Wire1.endTransmission();
        Serial.println("Inicio juego");
        anterior = millis();
        estado = 1;
      }
      break;

    case 1:

      if (millis() - anterior > 10000) {
        Wire1.beginTransmission(8);  // Dirección del esclavo (8)
        Wire1.write(255);
        Wire1.endTransmission();

        delay(10);

        Wire1.requestFrom(8, 1);

        if (Wire1.available()) {
          byte puntaje = Wire1.read();
          Serial.print("Puntaje recibido del esclavo: ");
          Serial.println(puntaje);
        }
        estado = 0;
        Serial.println("Fin");
      } else {
        Serial.println("En juego");
      }
      break;
  }
  //Serial.println(digitalRead(12));
}
