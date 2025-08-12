int estado = 0;
int cuentaPuntos = 0;
String dat;

String dataIn;
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>


#define PIN 14  // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 7  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SS_PIN 5
#define RST_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

long tiempo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(21, 22);

  Serial.println("Inicializando");
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();       // initialize the lcd
  lcd.init();
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  Wire1.begin(25, 26, 100000);  // SDA=21, SCL=22

  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
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
        //Serial.println("Tarjeta detectada.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     APUNTA");
        lcd.setCursor(0, 1);
        lcd.print("     BIEN !");
        setYellow();

        Wire1.beginTransmission(8);  // Dirección del esclavo
        Wire1.write(254);
        Wire1.endTransmission();
        Serial.println("Inicio juego");
        estado = 1;

        Serial.println("Start");


        tiempo = millis();
        //Serial.println(tiempo);
        estado = 1;

        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }
      break;

    case 1:


      if (millis() - tiempo > 60000) {
        Wire1.beginTransmission(8);  // Dirección del esclavo (8)
        Wire1.write(255);
        Wire1.endTransmission();

        delay(10);

        Wire1.requestFrom(8, 1);

        if (Wire1.available()) {
          cuentaPuntos = Wire1.read();
          Serial.print("Puntaje recibido del esclavo: ");
          Serial.println(cuentaPuntos);
        }
        estado = 3;
        Serial.println("Fin");
        Serial.println("End");
      }

      break;

    case 3:
      //cuentaPuntos = recibirPuntaje();

      Serial.println("estado 3: " + String(cuentaPuntos));
      //cuentaPuntos = score();
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
    pixels.setPixelColor(i, pixels.Color(0, 20, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void setRed() {
  pixels.clear();                        // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(20, 0, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void setYellow() {
  pixels.clear();                        // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(20, 20, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}