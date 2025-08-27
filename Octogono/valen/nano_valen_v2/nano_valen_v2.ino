#include <Wire.h>
#include <Adafruit_MPU6050.h>





#define NUM_LEDS 46

#define DELAY_TIME 400  // Tiempo entre cada LED (en ms)


// Definir los pines a los que están conectadas las tiras de LED




#define TCA9548A_ADDR 0x70      // Dirección I2C del TCA9548A
#define MOVEMENT_THRESHOLD 0.3  // Sensibilidad del movimiento (ajustable)

Adafruit_MPU6050 mpu;

// Variables para almacenar la aceleración anterior en cada canal
float lastAx[8] = { 0 }, lastAy[8] = { 0 }, lastAz[8] = { 0 };

// Función para seleccionar el canal del TCA9548A
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Función para detectar movimiento basado en cambios de aceleración
int readMPU6050(uint8_t channel) {
  tcaSelect(channel);  // Seleccionar el canal correspondiente
  if (mpu.begin()) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Calcular la diferencia de aceleración respecto a la lectura anterior
    float deltaAx = abs(a.acceleration.x - lastAx[channel]);
    float deltaAy = abs(a.acceleration.y - lastAy[channel]);
    float deltaAz = abs(a.acceleration.z - lastAz[channel]);

    // Guardar las lecturas actuales para la próxima comparación
    lastAx[channel] = a.acceleration.x;
    lastAy[channel] = a.acceleration.y;
    lastAz[channel] = a.acceleration.z;
    /*
    Serial.print("Canal ");
    Serial.print(channel);
    Serial.print(" - ΔX: ");
    Serial.print(deltaAx, 4);
    Serial.print(" ΔY: ");
    Serial.print(deltaAy, 4);
    Serial.print(" ΔZ: ");
    Serial.println(deltaAz, 4);*/
    delay(100);

    // Si el cambio en cualquier eje supera el umbral, hay movimiento
    if (deltaAx > MOVEMENT_THRESHOLD || deltaAy > MOVEMENT_THRESHOLD || deltaAz > MOVEMENT_THRESHOLD) {
      return 1;  // Movimiento detectado
    } else {
      return 0;  // No hay movimiento
    }
  }

  Serial.print("No se detectó MPU6050 en canal ");
  Serial.println(channel);
  return 0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Escaneando canales del TCA9548A...");

  
}

void loop() {

  Serial.println(getMove(3));
}

int getMove(int channel) {
  int movimiento = readMPU6050(channel);
  return movimiento;
}
