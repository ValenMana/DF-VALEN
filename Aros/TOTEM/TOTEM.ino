#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>


const int BIT_DELAY = 100;  // ms entre bits

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



  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pixels.begin();
  setGreen();

  Serial.println("Inicio Totem");

  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT_PULLUP);
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
      if (millis() - tiempo > 10000) {  // 10 segundos de juego
        digitalWrite(CLK_PIN, LOW);     // Guarda puntaje recibido
        Serial.println("Espera puntos");
        receiveByte();
        Serial.print("Puntaje recibido del esclavo: ");
        Serial.println(cuentaPuntos);

        estado = 2;
      }
      break;

    case 2:
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

void receiveByte() {
  pinMode(CLK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT_PULLUP);

  Serial.println(digitalRead(CLK_PIN));
  while (digitalRead(CLK_PIN) == HIGH) {
    Serial.println(digitalRead(CLK_PIN));
    if (digitalRead(DATA_PIN) == HIGH) {
      delay(1);
      if (digitalRead(DATA_PIN) == LOW) {
        cuentaPuntos += 1;
      }
    }
  }
  Serial.println("Recibido: " + String(cuentaPuntos));
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);
}
