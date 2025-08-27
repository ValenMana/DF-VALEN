#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <FastLED.h>
#include <AlmostRandom.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2, A3);  // RX en A2, TX en A3

AlmostRandom ar;

#define NUM_LEDS 46  // 8 //46
#define NUM_STRIPS 1

CRGB leds[NUM_LEDS];
CLEDController *controllers[NUM_STRIPS];

#define DELAY_TIME 400

char c;

unsigned long previousMillis = 0;
int currentLED = 0;
bool encendiendo = true;
uint8_t randomSelect;

int currentStrip = 0;  // Índice de la tira actual
int estado = 0;
int score = 0;
bool movement = false;





// Dirección del TCA9548A
byte tcaI2cAdd = 0x70;

// Crear una instancia del sensor MPU6050
Adafruit_MPU6050 mpu;

// Umbral para detectar movimiento basado en el cambio de aceleración
//const float MOVEMENT_THRESHOLD = 2.0;  // Cambia este valor según la sensibilidad deseada

// Variables para almacenar las lecturas anteriores de aceleración

#define TCA9548A_ADDR 0x70  // Dirección I2C del TCA9548A
//#define MOVEMENT_THRESHOLD 1.0  // Sensibilidad del movimiento (ajustable)
float MOVEMENT_THRESHOLD[8] = { 1.0, 1.0, 1.0, 0.8, 1.0, 1.0, 1.0, 1.0 };

float lastAx[8] = { 0 }, lastAy[8] = { 0 }, lastAz[8] = { 0 };


void setup() {
  Serial.begin(9600);
  Serial.println("aaa");
  Wire.begin();
  Serial.println(F("Escaneo TCA9548A y lectura del MPU6050"));
  Serial.println();

  mySerial.begin(300);  // Para la comunicación con el ESP32
  estado = 0;

  Setup_neo();
  Serial.println(estado);
  delay(3000);
  //randomSeed(analogRead(A0) + micros());
}



void loop() {
  // Llamar a la función con el canal 2 del TCA9548A
  //detectMovementInChannel(1);  //2 //3 //4 //5 //6 //7
  //neo_stby();
  //Serial.println(estado);
  //Serial.println(readMPU6050(3));
  /*
  if (mySerial.available()) {
    c = mySerial.read();
    if (c != '\n') {
      if (c == '3') {
        estado = 0;
        for (int i = 0; i < 7; i++) {
          stripOff(i);
        }
      }
    }
  }
*/
  Serial.println(estado);

  switch (estado) {
    case 0:
      neo_stby();
      while (mySerial.available()) {
        c = mySerial.read();
        if (c != '\n') {
          if (c == '8') {
            estado = 1;
            for (int i = 0; i < 7; i++) {
              stripOff(i);
            }
          }
        }
      }
      break;

    case 1:
      randomSelect = ar.getRandomByte() % 8;
      //Serial.println(randomSelect);
      //Serial.println(randomSelect);
      setStripColorGreen(randomSelect);
      estado = 2;
      //Serial.println(estado);
      //Serial.println(randomSelect);
      //Serial.println(analogRead(A0));
      break;

    case 2:

      if (readMPU6050(randomSelect) == true) {
        if (readMPU6050(randomSelect) == true) {
          movement = true;
          score++;
          mySerial.println(score);
          estado = 3;
        } else {
          movement = false;
        }
      }
      //Serial.println(randomSelect);
      break;

    case 3:
      movement = false;
      stripOff(randomSelect);
      delay(50);
      estado = 1;
  }

  //delay(500);  // Espera medio segundo antes de leer nuevamente
}

void neo_stby() {
  static unsigned long previousMillis = 0;
  static int currentStrip = 0;  // Índice de la tira actual

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= DELAY_TIME) {
    previousMillis = currentMillis;

    // Apagar la tira anterior
    int prevStrip = (currentStrip - 1 + 8) % 8;
    stripOff(prevStrip);

    // Encender la tira actual con rojo pasión
    setStripColorBlue(currentStrip);

    // Pasar a la siguiente tira
    currentStrip = (currentStrip + 1) % 8;
  }
}


void setStripColorGreen(int stripIndex) {
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  controllers[stripIndex]->showLeds(30);
}

void setStripColorBlue(int stripIndex) {
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  controllers[stripIndex]->showLeds(30);
}

void stripOff(int stripIndex) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  controllers[stripIndex]->showLeds(30);
}

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Función que detecta movimiento en un canal específico del TCA9548A
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
    //delay(100);

    // Si el cambio en cualquier eje supera el umbral, hay movimiento
    if (deltaAx > MOVEMENT_THRESHOLD[channel] || deltaAy > MOVEMENT_THRESHOLD[channel] || deltaAz > MOVEMENT_THRESHOLD[channel]) {
      return 1;  // Movimiento detectado
    } else {
      return 0;  // No hay movimiento
    }
  }

  Serial.print("No se detectó MPU6050 en canal ");
  Serial.println(channel);
  return 0;
}

void Setup_neo() {
  FastLED.clear();
  FastLED.show();
  controllers[0] = &FastLED.addLeds<WS2812, 2, BRG>(leds, NUM_LEDS);
  controllers[1] = &FastLED.addLeds<WS2812, 3, BRG>(leds, NUM_LEDS);
  controllers[2] = &FastLED.addLeds<WS2812, 5, BRG>(leds, NUM_LEDS);
  controllers[3] = &FastLED.addLeds<WS2812, 6, BRG>(leds, NUM_LEDS);
  controllers[4] = &FastLED.addLeds<WS2812, 8, BRG>(leds, NUM_LEDS);
  controllers[5] = &FastLED.addLeds<WS2812, 9, BRG>(leds, NUM_LEDS);
  controllers[6] = &FastLED.addLeds<WS2812, 11, BRG>(leds, NUM_LEDS);
  controllers[7] = &FastLED.addLeds<WS2812, 12, BRG>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}