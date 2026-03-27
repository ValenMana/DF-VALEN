#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "totem.hpp"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 29
#define NUM_LEDS_TOTEM 7
#define LED_PIN 25 
#define LED_PIN_TOTEM 26
#define SS_PIN 5
#define RST_PIN 13
#define TCA9548A_ADDR 0x70 

// Configuración de sensibilidad para 16G
#define SCALE_16G 2048.0
#define UMBRAL_DISPARO 2.5 

MPU6050 sensor;
Totem totem;
Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// --- Prototipos de funciones (Indispensable para PlatformIO) ---
void tcaSelect(uint8_t channel);
float leerPicoImpacto();
void efectoMartillo(uint8_t cantidad);
void stby_led();
void apagarLeds();
void gameLogic();

// Variables de control global
int estado = 0;
unsigned long gameStarted;
String uid;
float acumuladoPuntos = 0;
int golpesContados = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); 

  for (uint8_t ch : {4, 5, 6, 7}) {
    tcaSelect(ch);
    sensor.initialize();
    sensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_16); 
    
    if (sensor.testConnection()) {
      Serial.printf("Canal %d: MPU6050 OK\n", ch);
    } else {
      Serial.printf("Canal %d: ERROR\n", ch);
    }
  }

  totem.begin(SS_PIN, RST_PIN, LED_PIN_TOTEM, NUM_LEDS_TOTEM);
  leds.begin();
  leds.show();

  totem.setGreen(255);
  totem.print(3, 0, "DESAFIO");
  totem.print(5, 1, "POTENCIA !");
}

void loop() {
  gameLogic();
}

float leerPicoImpacto() {
  float maxG = 0;
  unsigned long tiempoInicio = millis();
  
  while (millis() - tiempoInicio < 150) {
    for (int i = 4; i <= 7; i++) {
      tcaSelect(i);
      int16_t ax, ay, az;
      sensor.getAcceleration(&ax, &ay, &az);
      
      float gZ = abs((float)az / SCALE_16G);
      float gX = abs((float)ax / SCALE_16G);
      float gY = abs((float)ay / SCALE_16G);
      float actualG = max(gZ, max(gX, gY));
      
      if (actualG > maxG) maxG = actualG;
    }
  }
  
  maxG = maxG - 1.0;
  if (maxG < 0) maxG = 0;
  return maxG;
}

void gameLogic() {
  // Variables locales del switch declaradas fuera para evitar el error "crosses initialization"
  int16_t az_raw;
  float gCheck;
  float impacto;
  int ledsAEncender;
  float promedio;
  int puntosFinales;

  switch (estado) {
    case 0: 
      stby_led();
      if (/*totem.leerTag(uid)*/1) {
        gameStarted = millis();
        totem.clearLCD();
        totem.print(0, 0, "   PEGALE");
        totem.print(0, 1, "   FUERTE !");
        totem.setYellow(100);
        golpesContados = 0;
        acumuladoPuntos = 0;
        estado = 1;
      }
      break;

    case 1: 
      tcaSelect(4); 
      az_raw = sensor.getAccelerationZ();
      gCheck = abs((float)az_raw / SCALE_16G) - 1.0;

      if (gCheck > UMBRAL_DISPARO) {
        impacto = leerPicoImpacto();
        Serial.printf("Impacto detectado: %.2f Gs\n", impacto);
        
        ledsAEncender = map((int)(impacto * 10), 20, 140, 2, NUM_LEDS);
        ledsAEncender = constrain(ledsAEncender, 1, NUM_LEDS);
        Serial.println(ledsAEncender);
        efectoMartillo(ledsAEncender);
        acumuladoPuntos += impacto;
        golpesContados++;
        
        delay(1000); 
        
        
        if (golpesContados >= 3) {
          estado = 2;
        } else {
           totem.clearLCD();
           apagarLeds();
           totem.print(0, 0, "GOLPE " + String(golpesContados));
           totem.print(0, 1, "SIGUIENTE...");
        }
      }
      
      if (millis() - gameStarted > 30000) estado = 2; 
      break;

    case 2: 
      promedio = acumuladoPuntos / 3.0;
      puntosFinales = map((int)(promedio * 10), 0, 150, 0, 10);
      puntosFinales = constrain(puntosFinales, 0, 10);

      totem.clearLCD();
      totem.print(1, 0, "PUNTOS: ");
      totem.print(9, 0, String(puntosFinales) + "/10");
      totem.setGreen(255);
      
      delay(5000);
      apagarLeds();
      estado = 0;
      break;
  }
}

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void efectoMartillo(uint8_t cantidad) {
  apagarLeds();
  for (uint8_t i = 0; i < cantidad; i++) {
    uint32_t color = (i > NUM_LEDS * 0.8) ? leds.Color(255, 0, 0) : 
                     (i > NUM_LEDS * 0.5) ? leds.Color(255, 255, 0) : leds.Color(0, 255, 0);
    leds.setPixelColor(i, color);
    leds.show();
    delay(15); 
  }
}

void stby_led() {
  static uint8_t j = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.ColorHSV(((i * 256 / NUM_LEDS) + j) * 256, 255, 150));
  }
  leds.show();
  j++;
}

void apagarLeds() {
  leds.clear();
  leds.show();
}