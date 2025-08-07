#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 2  // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 7  // Popular NeoPixel ring size

#define SS_PIN 5
#define RST_PIN 0

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int data;
String dataIn = "";

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display



int estado = 0;
int maximo;
unsigned long cardIDInt;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial2.begin(300, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  lcd.init();                              // initialize the lcd
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  lcd.setCursor(4, 0);
  lcd.print("Desafio");
  lcd.setCursor(4, 1);
  lcd.print("Potencia");

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  setPixel(0, 150, 0);
}

void setPixel(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  switch (estado) {
    case 0:
      if (rfid.PICC_IsNewCardPresent()) {
        Serial.println("detect");
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("PEGALE");
        lcd.setCursor(6, 1);
        lcd.print("FUERTE!!!");
        Serial2.println("3");
        setPixel(150, 150, 0);
        estado = 1;
      }
      break;
    case 1:
      if (Serial2.available()) {
        data = Serial2.readString().toInt();
        if (data == 8) {
          estado = 2;
          setPixel(150, 0, 0);
        }
        Serial.println(data);
      }
      break;

    case 2:
      if (Serial2.available()) {
        data = Serial2.readString().toInt();
        maximo = data;
        Serial.println(maximo);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Puntaje: ");
        lcd.print(maximo);
        lcd.print("/16");
        delay(5000);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Desafio");
        lcd.setCursor(4, 1);
        lcd.print("Potencia");
        setPixel(0, 150, 0);
        estado = 0;
      }
      break;
  }
}
