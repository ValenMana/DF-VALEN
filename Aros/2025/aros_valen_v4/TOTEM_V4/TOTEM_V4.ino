#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SS_PIN 5
#define RST_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

const int DATA_PIN = 25;  //25
const int CLK_PIN = 26;   //26

const int BTN = 14;

int estado = 0;
unsigned long anterior;

int puntaje = 0;

void setup() {
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
  Serial.begin(115200);

  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));

  lcd.init();  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  Serial.println("Inicio totem");
}

void loop() {

  switch (estado) {
    case 0:
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(5, 1);
      lcd.print("AROS !");

      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        // Reiniciar cardID
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     APUNTA");
        lcd.setCursor(0, 1);
        lcd.print("     BIEN !");
        Serial.println("Inicio juego");
        estado = 1;
        digitalWrite(CLK_PIN, HIGH);
        anterior = millis();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }
      break;

    case 1:
      if (millis() - anterior > 10000) {
        digitalWrite(CLK_PIN, LOW);
        estado = 2;
        Serial.println("Espera puntos");
        receiveByte();
      }
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Puntos ganados");
      lcd.setCursor(7, 1);
      lcd.print(puntaje);
      delay(6000);

      lcd.clear();

      puntaje = 0;
      estado = 0;
      Serial.println("Fin");
      break;
  }
}

void receiveByte() {
  pinMode(CLK_PIN, INPUT_PULLUP);
  while (digitalRead(CLK_PIN) == HIGH) {
    if (digitalRead(DATA_PIN) == HIGH) {
      delay(1);
      if (digitalRead(DATA_PIN) == LOW) {
        puntaje += 1;
      }
    }
  }
  Serial.println("Recibido: " + String(puntaje));
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);
}
