#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

MPU6050 sensor;

int16_t ax, ay, az;
float acc;

void setup() {
  Serial.begin(57600);  //Iniciando puerto serial
  Wire.begin();         //Iniciando I2C
  sensor.initialize();  //Iniciando el sensor

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  lectura(&acc);
  Serial.println(acc);
}

void lectura(float* value) {
  sensor.getAcceleration(&ax, &ay, &az);
  *value = az * (9.81 / 16384.0);
  if (*value < 0) {
    *value = 0;
  }
  delay(100);
}