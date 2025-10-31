#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>

int estado = 0;
int cuentaPuntos = 0;
String dat;
int r, g, b;

#include <Ultrasonic.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 4

#define OFFSET 5

#define NUMPIXELS 183

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Ultrasonic ultrasonic(3, 2);


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

int maxValue;
int ledsOn;
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();       // initialize the lcd
  lcd.init();
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)


  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  Serial.println("Inicio Totem");

  //pin como entrada
}

void loop() {

  switch (estado) {
    case 0:
      standby();
      //showHeight(30);
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(5, 1);
      lcd.print("SALTO !");
      if (rfid.PICC_IsNewCardPresent()) {
        //Serial.println("Tarjeta detectada.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     SALTA");
        lcd.setCursor(0, 1);
        lcd.print("   BIEN ALTO !");
        Serial.println("Start");
        pixels.clear();
        pixels.show();
        tiempo = millis();
        //Serial.println(tiempo);
        estado = 1;
        delay(500);
      }
      break;

    case 1:

      game();

      if (millis() - tiempo > 25000) {
        estado = 3;
        Serial.println("End");

        
      }

      break;

    case 3:

      Serial.println("estado 3: " + String(cuentaPuntos));
      //cuentaPuntos = score();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Altura Maxima");
        lcd.setCursor(7, 1);
        lcd.print(maxValue);
        lcd.print(" cm");
        delay(6000);
        lcd.clear();
      maxValue = 0;
      estado = 0;
      break;
  }
  // put your main code here, to run repeatedly:
  //Serial.print("8\n");
  //Serial.println(Serial.read());
}



void game() {

  // Pass INC as a parameter to get the distance in inches
  //pixels.clear();
  distance = ultrasonic.read();


  distance = 300 - distance;
  if (distance < 20) {
    distance = 0;
  } else {
    //distance += 60;
  }
  if (distance > maxValue) {
    maxValue = distance;
    ledsOn = map(distance, 0, 300, 0, NUMPIXELS);
    ledsOn -= OFFSET;
    for (int i = 0; i < ledsOn; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      pixels.show();
      delayMicroseconds(10);
    }
    //pixels.clear();
  }




  Serial.println(maxValue);
  //showHeight(maxValue);
  delay(100);
}

void standby() {




  for (int i = 1; i < NUMPIXELS; i++) {

    for (int z = 0; z < 10; z++) {
      pixels.setPixelColor(i + z, pixels.Color(r, g, b));
    }
    //pixels.setPixelColor(i - 1, 0);
    pixels.show();

    delay(12);
  }

  for (int i = NUMPIXELS; i > 1; i--) {

    for (int z = 10; z > 0; z--) {
      pixels.setPixelColor(i - z, pixels.Color(r, g, b));
    }
    pixels.setPixelColor(i + 1, 0);
    pixels.show();

    delay(12);
  }
  r += random(30, 255);
  //g += random(30, 255);
  b += random(30, 255);
}


void showHeight(int cantidadMax) {
  for (int i = 0; i < cantidadMax; i++) {
    pixels.setPixelColor(i, pixels.ColorHSV(50 * 256));
    pixels.show();
    delay(25);
  }
}