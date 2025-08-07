#include <Wire.h>
#include <MPU6050.h>

// Crear objeto MPU6050
MPU6050 mpu(0x68); // Dirección I2C por defecto del MPU6050

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA = GPIO21, SCL = GPIO22 en ESP32

  Serial.println("Iniciando MPU6050...");

  mpu.initialize();

  // Verificar conexión
  if (mpu.testConnection()) {
    Serial.println("MPU6050 conectado correctamente.");
  } else {
    Serial.println("Error al conectar con MPU6050. Verifica las conexiones.");
    while (1);
  }
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  // Obtener datos crudos
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Mostrar en Serial Monitor
  Serial.print("Acelerómetro => ");
  Serial.print("X: "); Serial.print(ax);
  Serial.print(" | Y: "); Serial.print(ay);
  Serial.print(" | Z: "); Serial.print(az);

  Serial.print(" || Giroscopio => ");
  Serial.print("X: "); Serial.print(gx);
  Serial.print(" | Y: "); Serial.print(gy);
  Serial.print(" | Z: "); Serial.println(gz);

  delay(500);
}
