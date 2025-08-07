#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2  // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 7  // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

const int trigPin1 = 4;  //verde amarillo
const int echoPin1 = 5;  //marron naranja
const int trigPin2 = 6;  //verde
const int echoPin2 = 7;  //blanco
int estado = 0;

struct tiempos {
  int start;
  int finish;
  int startReturn;
  int finishReturn;
};

tiempos tiempo;

int reading;

int maxSpeed;

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
  lcd.print("FUTBOL!");

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));

  tiempo.start = 0;
  tiempo.finish = 0;
  tiempo.startReturn = 0;
  tiempo.finishReturn = 0;

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  setGreen();
}

void loop() {

  switch (estado) {
    case 0:
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(4, 1);
      lcd.print("FUTBOL!");
      if (rfid.PICC_IsNewCardPresent()) {
        Serial.println("Tarjeta detectada.");
        estado = 1;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("A CORRER !!");
        setYellow();
      }
      break;

    case 1:
      reading = readUs(trigPin1, echoPin1);
      Serial.println(reading);
      if (reading < 100 && reading != 0) {
        tiempo.start = millis();
        Serial.println("First read");
        estado = 2;
      } else
        break;

    case 2:
      reading = readUs(trigPin2, echoPin2);

      if (reading < 100 && reading != 0) {
        tiempo.finish = millis();
        Serial.println("Second read");


        delay(10);
        estado = 3;
      }
      break;

    case 3:
      reading = readUs(trigPin2, echoPin2);
      if (reading < 100 && reading != 0) {
        tiempo.startReturn = millis();
        Serial.println("Third read");

        estado = 4;
      }
      break;
    case 4:
      reading = readUs(trigPin1, echoPin1);

      if (reading < 100 && reading != 0) {
        tiempo.finishReturn = millis();
        Serial.println("Fourth read");
        estado = 5;
      }
      break;

    case 5:
      /*
      Serial.println("El tiempo de ida fue: " + String(tiempo.finish - tiempo.start));
      Serial.println("El tiempo de vuelta fue: " + String(tiempo.finishReturn - tiempo.startReturn));*/
      setRed();

      double finalTime1 = (tiempo.finish - tiempo.start) / 3600000.0;
      double finalTime2 = (tiempo.finishReturn - tiempo.startReturn) / 3600000.0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tu velocidad fue:");
      //lcd.print("1: " + String(0.007 / finalTime1) + " km/h");
      if (0.007 / finalTime1 > 0.007 / finalTime2) {
        maxSpeed = 0.007 / finalTime1;
      } else {
        maxSpeed = 0.007 / finalTime2;
      }

      lcd.setCursor(5, 1);
      lcd.print(String(maxSpeed) + " km/h");
      Serial.println("La velocidad de ida fue: " + String(0.007 / finalTime1));
      Serial.println("La velocidad de ida fue: " + String(0.007 / finalTime2));
      delay(12000);

      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(4, 1);
      lcd.print("FUTBOL!");

      tiempo.start = 0;
      tiempo.finish = 0;
      tiempo.startReturn = 0;
      tiempo.finishReturn = 0;

      setGreen();
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

int readUs(int trig, int echo) {
  digitalWrite(trig, LOW);  //Inicializamos el pin con 0
  long t;                   //tiempopo que demora en llegar el eco
  long d;                   //distancia en centiempotros

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);  //Enviamos un pulso de 10us
  digitalWrite(trig, LOW);

  t = pulseIn(echo, HIGH);  //obtenemos el ancho del pulso
  d = t / 59;               //escalamos el tiempo a una distancia en cm
  delay(50);
  return d;
}
