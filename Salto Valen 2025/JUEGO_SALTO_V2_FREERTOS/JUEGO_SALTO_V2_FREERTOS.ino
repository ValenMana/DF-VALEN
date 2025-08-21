#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

// Pines del sensor ultrasónico
const int trigPin = 26;
const int echoPin = 17;

int estado = 0;
int cuentaPuntos = 0;

uint8_t r = 100;
uint8_t g = 60;
uint8_t b = 30;

int maxHeight = 0;

// Variables multitarea
volatile long altura = 0;  // compartida entre cores
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// NeoPixel
#define PIN_TOTEM 12
#define NUMPIXELS_TOTEM 7
#define PIN 16
#define NUM_LEDS 63

Adafruit_NeoPixel pixels(NUMPIXELS_TOTEM, PIN_TOTEM, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tiraLed(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RFID
#define SS_PIN 13
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte nuidPICC[4];
String cardID = "";
long tiempo;

void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  pixels.begin();
  tiraLed.begin();
  tiraLed.show();

  lcd.backlight();
  lcd.clear();
  setGreen();
  Serial.println("Inicio Totem");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  // Crear tarea para leer el sensor en core 0
  xTaskCreatePinnedToCore(
    sensorTask,     // Función
    "Sensor Task",  // Nombre
    10000,          // Stack
    NULL,           // Param
    1,              // Prioridad
    NULL,           // Manejador
    0               // Core 0
  );
}

void loop() {
  long alturaTemp;

  // Leer altura actual de forma segura
  portENTER_CRITICAL(&mux);
  alturaTemp = altura;
  portEXIT_CRITICAL(&mux);

  switch (estado) {
    case 0:
      standby();
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(5, 1);
      lcd.print("SALTO !");
      if (rfid.PICC_IsNewCardPresent()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     SALTA");
        lcd.setCursor(0, 1);
        lcd.print("   BIEN ALTO !");
        setYellow();
        Serial.println("Start");
        tiraLed.clear();
        tiraLed.show();
        tiempo = millis();
        estado = 1;
        delay(500);
      }
      break;

    case 1:
      if (maxHeight < alturaTemp) {
        maxHeight = alturaTemp;
        showHeight(map(alturaTemp, 0, 300, 0, NUM_LEDS) - 5);
      }

      if (millis() - tiempo > 25000) {
        estado = 3;
        Serial.println("End");
        setRed();

        for (int i = 0; i < NUM_LEDS; i++) {
          tiraLed.setPixelColor(i, pixels.Color(255, 0, 0));
          tiraLed.show();
        }
      }
      break;

    case 3:
      Serial.println("estado 3: " + String(cuentaPuntos));
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Altura Maxima");
      lcd.setCursor(7, 1);
      lcd.print(maxHeight);
      lcd.print(" cm");
      delay(6000);
      lcd.clear();
      maxHeight = 0;
      estado = 0;
      setGreen();
      break;
  }
}

// ---------- FUNCIONES AUXILIARES ----------

long calcularAltura() {
  long duration;
  long distance = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  delay(100);

  distance = 300 - distance;

  if (distance > 20) {
    Serial.println(distance);
  }
  return distance;
}

void sensorTask(void* parameter) {
  for (;;) {
    long medida = calcularAltura();

    portENTER_CRITICAL(&mux);
    altura = medida;
    portEXIT_CRITICAL(&mux);

    delay(50);  // Tiempo entre mediciones
  }
}

void showHeight(int cantidadMax) {
  for (int i = 0; i < cantidadMax; i++) {
    tiraLed.setPixelColor(i, tiraLed.Color(148, 211, 0));
    tiraLed.show();
    delay(15);
  }
}

void setGreen() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 20, 0));
    pixels.show();
  }
}

void setRed() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {
    pixels.setPixelColor(i, pixels.Color(20, 0, 0));
    pixels.show();
  }
}

void setYellow() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {
    pixels.setPixelColor(i, pixels.Color(20, 20, 0));
    pixels.show();
  }
}

void standby() {
  static uint8_t desplazamiento = 0;
  static int8_t direccion = 1;
  static unsigned long ultimoTiempo = 0;
  const uint8_t espera = 1;

  unsigned long ahora = millis();
  if (ahora - ultimoTiempo >= espera) {
    ultimoTiempo = ahora;

    for (int i = 0; i < NUM_LEDS; i++) {
      // Cálculo de tono HSV similar a CHSV
      uint16_t hue = (i * 256 / NUM_LEDS + desplazamiento) % 256;
      // NeoPixel usa hue de 0–65535, por eso *256
      tiraLed.setPixelColor(i, tiraLed.ColorHSV(hue * 256, 255, 255));
    }

    tiraLed.show();

    desplazamiento += direccion;
    if (desplazamiento == 255 || desplazamiento == 0) {
      direccion = -direccion;
    }
  }
}
