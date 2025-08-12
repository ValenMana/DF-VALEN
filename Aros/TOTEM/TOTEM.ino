#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>


const int BIT_DELAY = 100; // ms entre bits

// Pines de comunicación con el esclavo
const int DATA_PIN = 25;
const int CLK_PIN = 26;

int estado = 0;
int cuentaPuntos = 0;
long tiempo;

#define PIN 14
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C del LCD

#define SS_PIN 5
#define RST_PIN 13
MFRC522 rfid(SS_PIN, RST_PIN);  // Lector RFID

MFRC522::MIFARE_Key key;
byte nuidPICC[4];

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT_PULLUP);

  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pixels.begin();
  setGreen();

  Serial.println("Inicio Totem");
}

void loop() {
  switch (estado) {
    case 0:
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(5, 1);
      lcd.print("AROS !");

      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     APUNTA");
        lcd.setCursor(0, 1);
        lcd.print("     BIEN !");
        setYellow();

        //sendByte(254);  // Iniciar juego
        digitalWrite(CLK_PIN, HIGH);
        Serial.println("Inicio juego");
        tiempo = millis();
        estado = 1;

        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }
      break;

    case 1:
      if (millis() - tiempo > 60000) {  // 10 segundos de juego
        digitalWrite(CLK_PIN, LOW);     // Guarda puntaje recibido
        byte recibido = esperarYRecibirNumero();

        Serial.print("Puntaje recibido del esclavo: ");
        Serial.println(recibido);

        Serial.println(cuentaPuntos);

        estado = 3;
      }
      break;

    case 3:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Puntos ganados");
      lcd.setCursor(7, 1);
      lcd.print(cuentaPuntos);
      delay(6000);

      lcd.clear();
      cuentaPuntos = 0;
      estado = 0;
      setGreen();
      break;
  }
}

void setGreen() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 20, 0));
  }
  pixels.show();
}

void setRed() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(20, 0, 0));
  }
  pixels.show();
}

void setYellow() {
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(20, 20, 0));
  }
  pixels.show();
}

byte esperarYRecibirNumero() {
  // Esperar hasta detectar el bit de inicio (LOW)
  while (digitalRead(DATA_PIN) == HIGH) {
    delay(1);  // Evita bloqueo excesivo de CPU
  }

  // Esperar un poco para centrar el primer bit (opcional)
  delay(BIT_DELAY / 2);

  byte valor = 0;
  for (int i = 0; i < 8; i++) {
    bool bitLeido = digitalRead(DATA_PIN);
    bitWrite(valor, i, bitLeido);
    delay(BIT_DELAY);
  }

  return valor;
}
