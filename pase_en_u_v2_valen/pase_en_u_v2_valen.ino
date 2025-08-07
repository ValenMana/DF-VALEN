#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define PIN 26  // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 7  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16

#define SS_PIN 5
#define RST_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

int estado = 0;

int readings[3];

#define SENSOR1_PIN 14
#define SENSOR2_PIN 16
#define SENSOR3_PIN 17

long hcsr1;

int pulseInLimitador_1 = 7000;
int pulseInLimitador_2 = 7000;
int pulseInLimitador_3 = 7000;

#define SOUND_SPEED 0.034

int anterior;
int cuentaPuntos = 0;
bool started = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();       // initialize the lcd
  lcd.init();
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("DESAFIO");
  lcd.setCursor(4, 1);
  lcd.print("PASE U!");

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  setGreen();
  anterior = millis();
}

void loop() {

  if(millis() - anterior > 60000 && started == true){
    estado = 6;
  }

  switch (estado) {
    case 0:
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(4, 1);
      lcd.print("PASE U!");
      if (rfid.PICC_IsNewCardPresent()) {
        Serial.println("Tarjeta detectada.");
        estado = 1;
        started = true;
        anterior = millis();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("PASES RAPIDOS !");

        lcd.setCursor(3, 1);
        lcd.print("PASES: ");
        lcd.setCursor(11, 1);
        lcd.print(cuentaPuntos);
        setYellow();
      }
      break;

    case 1:

      readings[0] = readUs(SENSOR1_PIN);
      readings[2] = readUs(SENSOR3_PIN);

      if (readings[0] < 50 && readings[0] != 0) {
        Serial.println("Derecha");
        estado = 2;
      }
      if (readings[2] < 50 && readings[2] != 0) {
        Serial.println("Izquierda");
        estado = 3;
      }

      //Serial.println(readings[0]);
      delay(100);
      break;

    case 2:
      readings[1] = readUs(SENSOR2_PIN);
      if (readings[1] < 50 && readings[1] != 0) {
        //Serial.println("Derecha");
        estado = 4;
      }
      break;

    case 4:
      readings[2] = readUs(SENSOR3_PIN);
      if (readings[2] < 50 && readings[2] != 0) {
        //Serial.println("Derecha");
        cuentaPuntos += 1;
        Serial.println(cuentaPuntos);
        lcd.setCursor(11, 1);
        lcd.print(cuentaPuntos);

        estado = 1;
      }
      break;

    case 3:
      readings[1] = readUs(SENSOR2_PIN);
      if (readings[1] < 50 && readings[1] != 0) {
        //Serial.println("Derecha");
        estado = 5;
      }
      break;

    case 5:
      readings[0] = readUs(SENSOR1_PIN);
      if (readings[0] < 50 && readings[0] != 0) {
        //Serial.println("Derecha");
        cuentaPuntos += 1;
        estado = 1;
        Serial.println(cuentaPuntos);
        lcd.setCursor(11, 1);
        lcd.print(cuentaPuntos);
      }
      break;

    case 6:

      setRed();

      delay(12000);

      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(4, 1);
      lcd.print("PASE U!");

      setGreen();
      cuentaPuntos = 0;
      started = false;
      estado = 0;
      //ESP.restart();



      break;
  }


  //Serial.println(readUs(trigPin2, echoPin2));
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

int readUs(int US_PIN) {
  pinMode(US_PIN, OUTPUT);
  digitalWrite(US_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(US_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_PIN, LOW);

  pinMode(US_PIN, INPUT);
  hcsr1 = pulseIn(US_PIN, HIGH, pulseInLimitador_1);
  return hcsr1 * SOUND_SPEED / 2.0;
}
