#include <Wire.h>
#include <MPU6050.h>
#include <FastLED.h>

#define NUM_LEDS 29
#define LED_PIN 7

CRGB leds[NUM_LEDS];

MPU6050 mpu;

float umbralImpacto = 1.5;  // Ajustá esto para cambiar sensibilidad (en g)
float baseAccZ = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  FastLED.addLeds<WS2812, LED_PIN, BRG>(leds, NUM_LEDS);

  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Error de conexión con el MPU6050. Revisá las conexiones.");
    while (true)
      ;
  }

  // Configurar a ±16g
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);

  // Mostrar rango seteado
  uint8_t rango = mpu.getFullScaleAccelRange();
  Serial.print("Rango configurado: ±");
  Serial.print(2 << rango);  // 2, 4, 8, 16
  Serial.println("g");

  // Estimar aceleración en reposo
  delay(1000);
  baseAccZ = leerAccZg();
  Serial.print("Base Z en reposo (g): ");
  Serial.println(baseAccZ);

  Serial.println("Sensor listo.");

  Serial3.begin(300);

  encenderLedsRojo(29);  // Modo espera
}

int data;
int max;
int estado = 1;

void loop() {
  //espera();

  switch (estado) {
    case 0:
      if (Serial3.available()) {
        data = Serial3.readString().toInt();
        Serial.println(data);
        //efectoMartillo(NUM_LEDS);
        //apagarLeds();
        encenderLedsAzul(29);
        if (data == 3) {
          estado = 1;
        }
      }
      break;

    case 1:
      data = detect();
      if (data != -1) {
        if (data > max) {
          max = data;
          Serial.println(data);
        }
        estado = 2;
      }
      break;

    case 2:
      data = detect();
      if (data != -1) {
        if (data > max) {
          max = data;
        }
        estado = 3;
      }
      break;

    case 3:
      data = detect();
      if (data != -1) {
        if (data > max) {
          max = data;
        }
        estado = 4;
      }
      break;

    case 4:
      Serial3.println("8");
      delay(3000);
      Serial3.println(String(max));
      Serial.println(max);
      encenderLedsRojo(29);  // Modo espera

      max = 0;
      estado = 1;


      break;
  }
  //detect();
}

int detect() {
  float pico = detectarImpacto();
  int cantidadLeds = 0;
  int val;

  if (pico > 0) {
    Serial.print("Impacto detectado: ");
    Serial.print(pico);
    Serial.println(" g");

    cantidadLeds = map(pico * 100, 150, 1600, 0, 30);  // 1.5g a 5.0g
    cantidadLeds = constrain(cantidadLeds, 0, 30);

    efectoMartillo(cantidadLeds);
    delay(1500);
    apagarLeds();
    val = pico;

  } else {
    val = -1;
    encenderLedsAzul(29);  // Modo espera
  }

  delay(10);
  return val;
}

float leerAccZg() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  return az / 2048.0;  // Para ±16g => 2048 LSB/g
}

float detectarImpacto() {
  static unsigned long ultimaLectura = 0;

  if (millis() - ultimaLectura < 2000) return -1;

  float maxZ = 0;
  unsigned long t0 = millis();

  while (millis() - t0 < 300) {
    float actualZ = leerAccZg();
    float delta = abs(actualZ - baseAccZ);
    if (delta > maxZ) maxZ = delta;
    delay(5);
  }

  if (maxZ > umbralImpacto) {
    ultimaLectura = millis();
    return maxZ;
  }

  return -1;
}

void efectoMartillo(uint8_t cantidad) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t i = 0; i < cantidad; i++) {
    leds[i] = (i > NUM_LEDS * 0.9) ? CRGB::Red : (i > NUM_LEDS * 0.6) ? CRGB::Yellow
                                                                      : CRGB::Green;
    FastLED.show();
    delay(10);
  }
}

void apagarLeds() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void encenderLedsAzul(uint8_t cantidad) {
  cantidad = constrain(cantidad, 0, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < cantidad; i++) 
  leds[i] = CRGB::Blue;
  FastLED.show();
}

void encenderLedsRojo(uint8_t cantidad) {
  cantidad = constrain(cantidad, 0, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < cantidad; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(10);
  }
}

void espera() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    delay(10);
  }


  for (uint8_t i = NUM_LEDS; i > 0; i--) {
    leds[i] = CRGB::Black;
    delay(10);
  }

  FastLED.show();
}
