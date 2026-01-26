#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "totem.hpp"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 29
#define NUM_LEDS_TOTEM 7

#define LED_PIN 25  //7
#define LED_PIN_TOTEM 26

#define SS_PIN 5
#define RST_PIN 13

#define TCA9548A_ADDR 0x70  // Dirección del multiplexor

MPU6050 mpu;



MPU6050 sensor;
Totem totem;
void tcaSelect(uint8_t channel);
void lectura(float* value);

int16_t ax, ay, az;
float acc;

// --- NeoPixel en lugar de FastLED ---
Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void stby_led();
void efectoMartillo(uint8_t cantidad);
void apagarLeds();
void encenderLedsAzul(uint8_t cantidad);
void encenderLedsRojo(uint8_t cantidad);
void espera();
void gameLogic();

long gameStarted;
int estado = 0;
String uid;
bool game = false;
int anterior;
int data;
int prom;

void setup() {
  Serial.begin(115200);  //Iniciando puerto serial
  Wire.begin();         //Iniciando I2C
  for (uint8_t ch : {4, 5, 6, 7}) {
    tcaSelect(ch);
    sensor.initialize();
    sensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);


    if (mpu.testConnection()) {
      Serial.print("Canal ");
      Serial.print(ch);
      Serial.println(": MPU6050 conectado correctamente.");
    } else {
      Serial.print("Canal ");
      Serial.print(ch);
      Serial.println(": No se detectó el MPU6050.");
    }
  }
  totem.begin(SS_PIN, RST_PIN, LED_PIN_TOTEM, NUM_LEDS_TOTEM);



  leds.begin();
  leds.show();  // Apagar todos los LEDs al inicio

  if (sensor.testConnection())
    Serial.println("Sensor iniciado correctamente");
  else
    Serial.println("Error al iniciar el sensor");

  totem.setGreen(255);
}

void loop() {
  gameLogic();
  //float value;
  //lectura(&value);
  //totem.clearLCD();
  //totem.print(0, 0, String((int)value));
}

int detect();

int detect() {
  float value;
  lectura(&value);
  int cantidadLeds;
  int val;

  if (value > 1) {
    Serial.print("Impacto detectado: ");
    Serial.print(value);
    Serial.println(" g");

    cantidadLeds = map(value, 1, 32, 0, 30);  // 1.5g a 5.0g
    cantidadLeds = constrain(cantidadLeds, 0, 30);

    efectoMartillo(cantidadLeds);
    delay(1500);
    apagarLeds();
    val = value;

  } else {
    val = -1;
    encenderLedsAzul(29);  // Modo espera
  }

  return val;
}

void gameLogic() {
  if (millis() - gameStarted > 60000 && game == true) {
    estado = 4;
  }

  switch (estado) {
    case 0:
      totem.print(3, 0, "DESAFIO");
      totem.print(5, 1, "POTENCIA !");

      stby_led();
      if (totem.leerTag(uid)) {
        game = true;
        gameStarted = millis();
        totem.clearLCD();
        totem.print(0, 0, "     PEGALE");
        totem.print(0, 1, "     FUERTE !");
        totem.setYellow(100);
        detect();
        anterior = millis();
        data = -1;
        estado = 1;
      }
      break;

    case 1:
      data = detect();
      if (millis() - anterior > 900) {
        if (data != -1) {
          prom += data;
          Serial.println(data);
          estado = 2;
        }
      }
      break;

    case 2:
      data = detect();
      if (data != -1) {
        prom += data;
        estado = 3;
      }
      break;

    case 3:
      data = detect();
      if (data != -1) {
        prom += data;
        totem.setRed(100);
        estado = 4;
      }
      break;

    case 4:
      prom /= 3;
      Serial.println(prom);
      encenderLedsRojo(29);  // Modo espera
      totem.clearLCD();
      totem.print(1, 0, "Puntos: ");
      prom = map(prom, 0, 32, 0, 10);
      totem.print(9, 0, String(prom));
      totem.print(11, 0, "/10");
      totem.setGreen(100);
      delay(6000);
      game = false;
      totem.clearLCD();
      prom = 0;
      estado = 0;
      data = -1;
      break;
  }
}

void lectura(float* value) {
  float sensors[4];
  float accZ = 0;
  float prom = 0;
  int validCount = 0;

  int canales[4] = {7, 6, 5, 4};

  for (int i = 0; i < 4; i++) {
    tcaSelect(canales[i]);
    sensor.getAcceleration(&ax, &ay, &az);

    // Convertir a G
    accZ = az / 1000.0;

    sensors[i] = accZ;

    prom += abs(accZ);   // usar magnitud, no signo
  }
  

  // Evitar división por cero
  prom /= 4;
  

  *value = prom;

  // Imprimir debug
  Serial.print("S1: "); Serial.print(sensors[0]);
  Serial.print("  S2: "); Serial.print(sensors[1]);
  Serial.print("  S3: "); Serial.print(sensors[2]);
  Serial.print("  S4: "); Serial.print(sensors[3]);
  Serial.print("  PROM: "); Serial.println(prom);

  delay(100);
}

void efectoMartillo(uint8_t cantidad) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0);  // Apagar
  }

  for (uint8_t i = 0; i < cantidad; i++) {
    uint32_t color;
    if (i > NUM_LEDS * 0.9)
      color = leds.Color(255, 0, 0);  // Rojo
    else if (i > NUM_LEDS * 0.6)
      color = leds.Color(255, 255, 0);  // Amarillo
    else
      color = leds.Color(0, 255, 0);  // Verde

    leds.setPixelColor(i, color);
    leds.show();
    delay(10);
  }
}

void apagarLeds() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, 0);
  }
  leds.show();
}

void encenderLedsAzul(uint8_t cantidad) {
  cantidad = constrain(cantidad, 0, NUM_LEDS);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    if (i < cantidad)
      leds.setPixelColor(i, leds.Color(0, 0, 255));
    else
      leds.setPixelColor(i, 0);
  }
  leds.show();
}

void encenderLedsRojo(uint8_t cantidad) {
  cantidad = constrain(cantidad, 0, NUM_LEDS);
  for (uint8_t i = 0; i < cantidad; i++) {
    leds.setPixelColor(i, leds.Color(255, 0, 0));
    leds.show();
    delay(10);
  }
}

void espera() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(255, 0, 0));
    leds.show();
    delay(10);
  }

  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds.setPixelColor(i, 0);
    leds.show();
    delay(10);
  }
}

void stby_led() {
  static uint8_t desplazamiento = 0;
  static int8_t direccion = 1;
  static unsigned long ultimoTiempo = 0;
  const uint8_t espera = 10;

  unsigned long ahora = millis();
  if (ahora - ultimoTiempo >= espera) {
    ultimoTiempo = ahora;

    for (int i = 0; i < NUM_LEDS; i++) {
      uint8_t hue = (i * 256 / NUM_LEDS + desplazamiento) % 256;
      uint32_t color = leds.gamma32(leds.ColorHSV(hue * 256));
      leds.setPixelColor(i, color);
    }
    leds.show();

    desplazamiento += direccion;
    if (desplazamiento == 255 || desplazamiento == 0) {
      direccion = -direccion;
    }
  }
}
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}