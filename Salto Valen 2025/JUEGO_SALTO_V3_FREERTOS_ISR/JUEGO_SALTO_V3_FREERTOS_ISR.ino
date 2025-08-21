#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

// Pines del sensor ultrasónico
const int trigPin = 26;
const int echoPin = 17;

// Variables multitarea ISR
volatile unsigned long tiempoInicio = 0;
volatile unsigned long duracionPulso = 0;
volatile bool pulsoCompleto = false;

// Variable compartida entre cores
volatile long altura = 0;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

// Otros componentes
int estado = 0;
int cuentaPuntos = 0;
uint8_t r = 100, g = 60, b = 30;
int maxHeight = 0;

#define PIN_TOTEM 12
#define NUMPIXELS_TOTEM 7
#define PIN 16
#define NUM_LEDS 63

Adafruit_NeoPixel pixels(NUMPIXELS_TOTEM, PIN_TOTEM, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tiraLed(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 13
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);

long tiempo;

// --- Función ISR para medir pulso en echoPin ---
void IRAM_ATTR echo_ISR() {
  if (digitalRead(echoPin) == HIGH) {
    tiempoInicio = micros();
  } else {
    duracionPulso = micros() - tiempoInicio;
    pulsoCompleto = true;
  }
}

// --- Tarea que corre en core 0 ---
void sensorTask(void* parameter) {
  for (;;) {
    // Disparar el pulso ultrasónico
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Esperar ISR que complete el pulso
    unsigned long startWait = millis();
    while (!pulsoCompleto) {
      if (millis() - startWait > 50) {  // timeout
        break;
      }
    }

    pulsoCompleto = false;

    long distance;
    if (duracionPulso == 0) {
      distance = 0;
    } else {
      distance = (int)(duracionPulso * 0.034 / 2);
      if (distance < 50) distance = 0;
      else distance = 300 - distance;
    }

    portENTER_CRITICAL(&mux);
    altura = distance;
    portEXIT_CRITICAL(&mux);

    delay(100);
  }
}

void setup() {
  Serial.begin(9600);

  // Setup periféricos
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

  // Configurar ISR
  attachInterrupt(digitalPinToInterrupt(echoPin), echo_ISR, CHANGE);

  // Crear tarea en core 0
  xTaskCreatePinnedToCore(
    sensorTask,
    "Sensor Task",
    10000,
    NULL,
    1,
    NULL,
    0);
}

void loop() {
  long alturaTemp;
  portENTER_CRITICAL(&mux);
  alturaTemp = altura;
  portEXIT_CRITICAL(&mux);

  switch (estado) {
    case 0:
      stby_led();  // animación tipo arco iris
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
      if (alturaTemp < 0) {
        alturaTemp = 0;
      }
      if (alturaTemp > 0) {
        Serial.println(alturaTemp);
      }

      if(alturaTemp < 130 && alturaTemp > 70){
        alturaTemp += 25;
      }
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

// ------- Funciones auxiliares -------
void stby_led() {
  static uint8_t desplazamiento = 0;
  static int8_t direccion = 1;
  static unsigned long ultimoTiempo = 0;
  const uint8_t espera = 10;
  unsigned long ahora = millis();
  if (ahora - ultimoTiempo >= espera) {
    ultimoTiempo = ahora;
    for (int i = 0; i < NUM_LEDS; i++) {
      uint16_t hue = (i * 256 / NUM_LEDS + desplazamiento) % 256;
      tiraLed.setPixelColor(i, tiraLed.ColorHSV(hue * 256, 255, 255));
    }
    tiraLed.show();
    desplazamiento += direccion;
    if (desplazamiento == 255 || desplazamiento == 0) direccion = -direccion;
  }
}

void showHeight(int cantidadMax) {
  for (int i = 0; i < cantidadMax; i++) {
    tiraLed.setPixelColor(i, tiraLed.ColorHSV(50 * 256));
    tiraLed.show();
    delay(25);
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
