int estado = 0;
int cuentaPuntos = 0;

String dataIn;
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 3);  // RX, TX




#define PIN 2  // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 7  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

long tiempo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);


  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();       // initialize the lcd
  lcd.init();
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  setGreen();
}

void loop() {

  switch (estado) {
    case 0:

      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(4, 1);
      lcd.print("PUNTERIA !");
      if (rfid.PICC_IsNewCardPresent()) {
        //Serial.println("Tarjeta detectada.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     APUNTA");
        lcd.setCursor(0, 1);
        lcd.print("     BIEN !");
        setYellow();
        for (int i = 0; i < 5; i++) {
          mySerial.print("8\n");
          delay(50);
        }
        tiempo = millis();
        Serial.println(tiempo);
        estado = 1;
      }
      break;

    case 1:

      if (millis() - tiempo > 60000) {
        estado = 3;
      }
      break;

    case 3:


      Serial.println("finish");
      mySerial.println("3");
      delay(15);
      cuentaPuntos = mySerial.readString().toInt();

      Serial.println(cuentaPuntos);

      //delay(50);

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
  // put your main code here, to run repeatedly:
  //Serial.print("8\n");
  //Serial.println(Serial.read());
}

void setGreen() {
  pixels.clear();                        // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void setRed() {
  pixels.clear();                        // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void setYellow() {
  pixels.clear();                        // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(150, 150, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}