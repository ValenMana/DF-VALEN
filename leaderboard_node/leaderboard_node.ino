// Learn about the ESP32 WiFi simulation in
// https://docs.wokwi.com/guides/esp32-wifi

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0

String dataIn = "";

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const char* ssid = "Personal-WiFi-095-2.4Ghz";
const char* password = "ZXvN6hxVEk";


/*
const char* ssid = "el ses";
const char* password = "12345678";
*/
char si[3] = "SI";
char no[3] = "NO";

struct states {
  int wifi;
  int serial;
};

states state;

int estado = 0;
unsigned long cardIDInt;

void setup() {


  state.wifi = 0;
  state.serial = 0;

  Serial.begin(115200);
  lcd.init();  // initialize the lcd
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  lcd.setCursor(0, 0);
  lcd.print("Wifi: " + String(state.wifi));
  lcd.setCursor(0, 1);
  lcd.print("Compu: " + String(state.serial));



  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  state.wifi = 1;
  state.serial = 0;
}

void loop() {

  switch (estado) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Wifi: " + String(state.wifi));
      lcd.setCursor(0, 1);
      lcd.print("Compu: " + String(state.serial));

      if (Serial.available()) {
        state.serial = 1;
        dataIn = Serial.readString();
        if (dataIn.toInt() == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Ingrese el");
          lcd.setCursor(0, 1);
          lcd.print("nombre");
          estado = 1;
        }
        /*
        if (dataIn.toInt() == -1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Nuevo turno, espere...");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ONLINE");
          //reset();

        } else {
          state.serial = 0;
        }*/
        break;

        case 1:
          if (Serial.available() > 0) {
            dataIn = Serial.readString();
            if (dataIn.toInt() == 1) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Acerque la");
              lcd.setCursor(0, 1);
              lcd.print("tarjeta ");
              lcd.blink();
              estado = 2;
            }
          }
          break;

        case 2:
          if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
            // Reiniciar cardID
            cardID = "";

            // Leer los bytes del UID y construir la cadena hexadecimal
            for (byte i = 0; i < rfid.uid.size; i++) {
              if (rfid.uid.uidByte[i] < 0x10) {
                cardID += "0";  // Agregar un cero si el byte es menor a 0x10
              }
              cardID += String(rfid.uid.uidByte[i], HEX);
            }

            // Convertir el ID en un valor decimal
            cardIDInt = strtoul(cardID.c_str(), NULL, 16);
            // Serial.print("Card ID (Decimal): ");

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Codigo leido:");
            lcd.setCursor(0, 1);
            lcd.print(cardIDInt);
            Serial.println(cardIDInt);

            // Serial.write(cardID);

            // Detener la comunicación con la tarjeta
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
            delay(2000);
          }
      }
      break;
  }
}

void reset() {
  ESP.restart();
}
