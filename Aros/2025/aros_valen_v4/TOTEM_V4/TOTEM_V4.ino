#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


bool rcpt = false;

#define DATA_PIN 26
#define COMM_PIN 25

byte received;
unsigned long t;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SS_PIN 5
#define RST_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

int estado = 0;
unsigned long anterior;

int puntaje = 0;

void setup() {
  pinMode(COMM_PIN, OUTPUT);
  Serial.begin(115200);



  pinMode(DATA_PIN, INPUT);

  pinMode(COMM_PIN, OUTPUT);


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

  digitalWrite(COMM_PIN, LOW);

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
        digitalWrite(COMM_PIN, HIGH);
        anterior = millis();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }

      break;

    case 1:
      if (millis() - anterior > 60000) {
        estado = 2;
        Serial.println("Espera puntos");
        digitalWrite(COMM_PIN, LOW);
        anterior = millis();
      }
      break;

    case 2:

      //ACA IRIRA LA LOGICA DE RECEPCION DE LOS PUNTOS VIA SERIAL
      estado = 3;

      break;

    case 3:

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
bool debounce(int pin) {
  const unsigned long debounceDelay = 50;  // Tiempo de rebote en ms

  static int lastButtonState = LOW;
  static int buttonState = LOW;
  static unsigned long lastDebounceTime = 0;

  int reading = digitalRead(pin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  lastButtonState = reading;

  return buttonState;
}
