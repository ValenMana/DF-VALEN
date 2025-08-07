#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define TCA_ADDR1 0x70
#define TCA_ADDR2 0x74
#define NUM_SENSORES 7

Adafruit_VL53L0X sensor;
bool sensoresDetectados[NUM_SENSORES];

// Selecciona un canal del TCA9548A
void tcaSelect(uint8_t canal, uint8_t ADDR) {
  if (canal > 7) return;
  Wire.beginTransmission(ADDR);
  Wire.write(1 << canal);
  Wire.endTransmission();
}

// Inicializa todos los sensores disponibles y guarda su estado
void inicializarSensores() {
  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    tcaSelect(i, TCA_ADDR1);
    delay(10);
    if (sensor.begin(0x29, true)) {
      sensoresDetectados[i] = true;
      Serial.print("Canal ");
      Serial.print(i);
      Serial.println(": Sensor detectado.");
    } else {
      sensoresDetectados[i] = false;
      Serial.print("Canal ");
      Serial.print(i);
      Serial.println(": No detectado.");
    }
  }
  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    tcaSelect(i, TCA_ADDR2);
    delay(10);
    if (sensor.begin(0x29, true)) {
      sensoresDetectados[i] = true;
      Serial.print("Canal ");
      Serial.print(i);
      Serial.println(": Sensor detectado.");
    } else {
      sensoresDetectados[i] = false;
      Serial.print("Canal ");
      Serial.print(i);
      Serial.println(": No detectado.");
    }
  }
  Serial.println("Inicialización completa.");
}

// Devuelve la distancia o -1 si no hay medición o sensor
int leerDistancia(uint8_t canal) {
  if (canal >= NUM_SENSORES || !sensoresDetectados[canal]) return -1;

  if (canal < 7) {
    tcaSelect(canal, TCA_ADDR1);
  } else {
    tcaSelect(canal, TCA_ADDR2);
  }
  delay(2);

  VL53L0X_RangingMeasurementData_t medida;
  sensor.rangingTest(&medida, false);

  // Verifica que la medición sea válida
  if (medida.RangeStatus == 0 && medida.RangeMilliMeter > 195 || medida.RangeMilliMeter < 150) {
    return 1;
  } else {
    return -1;  // Medición inválida
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  inicializarSensores();
}

void loop() {
  int distancia = leerDistancia(0);

  Serial.print("Canal ");
  //Serial.print(i);
  if (distancia == -1) {
    Serial.println(" - No detectado o error");
  } else {
    Serial.print(" - Distancia: ");
    Serial.print(distancia);
    Serial.println(" mm");
  }
  delay(50);  // Evita colisiones I2C
}
