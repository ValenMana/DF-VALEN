#include <Wire.h>

void setup() {
  Wire.begin(21, 22); // SDA = GPIO21, SCL = GPIO22 para D1 R32 (ESP32)
  Serial.begin(115200);
  while (!Serial); // Esperar conexión serial (opcional)

  Serial.println("\nEscaneando dispositivos I2C...");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en dirección 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  ✓");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Error desconocido en dirección 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No se encontraron dispositivos I2C.\n");
  else
    Serial.println("Escaneo completo.\n");

  delay(3000); // Esperar 3 segundos antes de volver a escanear
}
