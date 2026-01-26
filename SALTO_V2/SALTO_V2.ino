#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

const int Trigger = 3;  //Pin digital 2 para el Trigger del sensor
const int Echo = 2;

#define PIN 4
#define PIN_TOTEM 7
#define OFFSET 5

#define NUMPIXELS 183
#define NUMPIXELS_TOTEM 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel totem(NUMPIXELS_TOTEM, PIN_TOTEM, NEO_GRB + NEO_KHZ800);


int distance;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  Serial.begin(9600);
  pixels.begin();
  totem.begin();
  totem.clear();
  totem.show();
  pixels.clear();
  pixels.show();
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  lcd.init();       // initialize the lcd
  lcd.init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  pinMode(Trigger, OUTPUT);    //pin como salida
  pinMode(Echo, INPUT);        //pin como entrada
  digitalWrite(Trigger, LOW);  //Inicializamos el pin con 0

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  setGreen();
  lcd.backlight();
  lcd.clear();
}

int maxValue;
int ledsOn;
int state = 0;
long timerGame;
bool detected = false;
void loop() {
  
  switch (state) {
    case 0:
      stby_led();
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(5, 1);
      lcd.print("SALTO !");
      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        // Reiniciar cardID
        setYellow();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     SALTA");
        lcd.setCursor(0, 1);
        lcd.print("   BIEN ALTO !");
        cardID = "";

        for (byte i = 0; i < rfid.uid.size; i++) {
          if (rfid.uid.uidByte[i] < 0x10) {
            cardID += "0";
          }
          cardID += String(rfid.uid.uidByte[i], HEX);
        }
        unsigned long cardIDInt = strtoul(cardID.c_str(), NULL, 16);
        Serial.println(cardIDInt);
        for (int i = NUMPIXELS; i > 0; i--) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
          pixels.show();
          delay(3);
        }
        maxValue = 0;
        timerGame = millis();
        state = 1;
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }
      break;

    case 1:
      game();
      if (maxValue > 20) {
        //Serial.println(maxValue);
        timerGame = millis();
        state = 2;
      }
      if (millis() - timerGame > 30000) {
        state = 2;
      }
      delay(100);
      break;
    case 2:
      game();
      if (millis() - timerGame > 15000) {
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(150, 0, 0));
          pixels.show();
        }
        setRed();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Altura Maxima");
        lcd.setCursor(7, 1);
        lcd.print(maxValue);
        lcd.print(" cm");
        delay(6000);
        lcd.clear();
        setGreen();
        state = 0;
        //Serial.println(maxValue);
      }
      delay(100);
  }
  //Serial.println(maxValue);
}

void showHeight(int cantidadMax) {
  for (int i = 0; i < cantidadMax; i++) {
    pixels.setPixelColor(i, pixels.ColorHSV(50 * 256));
    pixels.show();
    delay(25);
  }
}

void game() {
  long t;  //timepo que demora en llegar el eco

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);  //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH);  //obtenemos el ancho del pulso
  distance = t / 59;        //escalamos el tiempo a una distancia en cm

  Serial.println(distance);

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
}

void stby_led() {
  static uint8_t desplazamiento = 0;
  static int8_t direccion = 1;
  static unsigned long ultimoTiempo = 0;
  const uint8_t espera = 25;
  unsigned long ahora = millis();
  if (ahora - ultimoTiempo >= espera) {
    ultimoTiempo = ahora;
    for (int i = 0; i < NUMPIXELS; i++) {
      uint16_t hue = (i * 256 / NUMPIXELS + desplazamiento) % 256;
      pixels.setPixelColor(i, pixels.ColorHSV(hue * 256, 255, 255));
    }
    pixels.show();
    desplazamiento += direccion * 5;
    if (desplazamiento == 255 || desplazamiento == 0) direccion = -direccion;
  }
}

void setGreen() {
  totem.clear();
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {
    totem.setPixelColor(i, totem.Color(0, 255, 0));
    totem.show();
  }
}

void setRed() {
  totem.clear();
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {
    totem.setPixelColor(i, totem.Color(255, 0, 0));
    totem.show();
  }
}

void setYellow() {
  totem.clear();
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {
    totem.setPixelColor(i, totem.Color(255, 255, 0));
    totem.show();
  }
}
