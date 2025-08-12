int estado = 0;
int cuentaPuntos = 0;
String dat;

const int DATA_PIN = 25;
const int CLK_PIN = 26;


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

  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  Serial.println("Inicializando");
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();       // initialize the lcd
  lcd.init();
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
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

        sendByte(254);

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
        Wire1.write(255);
        sendByte(255);
        delayMicroseconds(500);  // espera mínima
        byte response = receiveByte();
        Serial.print("Puntaje recibido del esclavo: ");
        Serial.println(response);

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

void sendByte(byte data) {
  pinMode(DATA_PIN, OUTPUT);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(DATA_PIN, (data >> i) & 1);
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(100);
  }
}

byte receiveByte() {
  byte value = 0;
  pinMode(DATA_PIN, INPUT);  // cambia dirección del pin
  for (int i = 7; i >= 0; i--) {
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(100);
    value |= (digitalRead(DATA_PIN) << i);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(100);
  }
  return value;
}