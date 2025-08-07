#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 3); // RX, TX

#define NUM_STRIPS 11
#define NUM_LEDS_PER_STRIP 33

// Pines asignados a cada tira (ajustá según tu conexión)
const uint8_t stripPins[NUM_STRIPS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// Creamos un arreglo de objetos NeoPixel
Adafruit_NeoPixel strips[NUM_STRIPS] = {
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 7, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 8, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 9, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 10, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 11, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, 12, NEO_GRB + NEO_KHZ800)
};

#define TCA_ADDR1 0x70
#define TCA_ADDR2 0x74
#define NUM_SENSORES 7

Adafruit_VL53L0X sensor;
bool sensoresDetectados[NUM_SENSORES];

int estado = 0;

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
    tcaSelect(7 - canal, TCA_ADDR1);
  } else {
    tcaSelect(14 - canal, TCA_ADDR2);
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
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  inicializarSensores();

  for (int i = 0; i < NUM_STRIPS; i++) {
    strips[i].begin();
    strips[i].show(); // Inicializa apagados
  }
}

void loop() {
  maquina();


}

void maquina(){
  switch(estado){
    case 0:

  }
}

void test(uint8_t sensor) {
  int distancia = leerDistancia(sensor);

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

void encenderTira(int numeroTira, uint8_t r, uint8_t g, uint8_t b) {
  if (numeroTira < 0 || numeroTira >= NUM_STRIPS) return; 

  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    strips[numeroTira].setPixelColor(i, strips[numeroTira].Color(r, g, b));
  }
  strips[numeroTira].show();
}
