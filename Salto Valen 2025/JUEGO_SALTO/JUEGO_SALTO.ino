int estado = 0;
int cuentaPuntos = 0;
String dat;

const int trigPin = 26;  //Pin digital 2 para el Trigger del sensor
const int echoPin = 17;

uint8_t r = 100;
uint8_t g = 60;
uint8_t b = 30;

int maxHeight = 0;

int altura = 0;

String dataIn;
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>


#define PIN_TOTEM 12       // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS_TOTEM 7  // Popular NeoPixel ring size

#define PIN 16
#define NUM_LEDS 63


Adafruit_NeoPixel pixels(NUMPIXELS_TOTEM, PIN_TOTEM, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tiraLed(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SS_PIN 13
#define RST_PIN 5

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

  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();       // initialize the lcd
  lcd.init();
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)

  tiraLed.begin();
  tiraLed.show();

  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  setGreen();
  Serial.println("Inicio Totem");

  pinMode(trigPin, OUTPUT);    //pin como salida
  pinMode(echoPin, INPUT);     //pin como entrada
  digitalWrite(trigPin, LOW);  //Inicializamos el pin con 0
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
        setYellow();
        Serial.println("Start");
        tiraLed.clear();
        tiraLed.show();
        tiempo = millis();
        //Serial.println(tiempo);
        estado = 1;
        delay(500);
      }
      break;

    case 1:

      altura = getUs();
      Serial.println(maxHeight);

      if (maxHeight < altura) {
        maxHeight = altura;
        showHeight(map(altura, 0, 300, 0, NUM_LEDS) - 5);
      }

      if (millis() - tiempo > 25000) {
        estado = 3;
        Serial.println("End");
        setRed();

        for (int i = 0; i < NUM_LEDS; i++) {
          tiraLed.setPixelColor(i, pixels.Color(255, 0, 0));
          tiraLed.show();
          //delay(5);
        }
      }

      break;

    case 3:

      Serial.println("estado 3: " + String(cuentaPuntos));
      //cuentaPuntos = score();
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Altura Maxima");
      lcd.setCursor(7, 1);
      lcd.print(maxHeight);
      lcd.print(" cm");
      delay(6000);
      lcd.clear();
      maxHeight = 0;
      estado = 0;
      setGreen();
      break;
  }
  // put your main code here, to run repeatedly:
  //Serial.print("8\n");
  //Serial.println(Serial.read());
}

void setGreen() {
  pixels.clear();                              // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(0, 20, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void setRed() {
  pixels.clear();                              // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(20, 0, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void setYellow() {
  pixels.clear();                              // Set all pixel colors to 'off'
  for (int i = 0; i < NUMPIXELS_TOTEM; i++) {  // For each pixel...
    pixels.setPixelColor(i, pixels.Color(20, 20, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.
  }
}

void standby() {




  for (int i = 1; i < NUM_LEDS; i++) {

    for (int z = 0; z < 10; z++) {
      tiraLed.setPixelColor(i + z, pixels.Color(r, g, b));
    }
    //tiraLed.setPixelColor(i - 1, 0);
    tiraLed.show();

    delay(12);
  }

  for (int i = NUM_LEDS; i > 1; i--) {

    for (int z = 10; z > 0; z--) {
      tiraLed.setPixelColor(i - z, pixels.Color(r, g, b));
    }
    tiraLed.setPixelColor(i + 1, 0);
    tiraLed.show();

    delay(12);
  }
  r += random(30, 255);
  //g += random(30, 255);
  b += random(30, 255);
}


long getUs() {

  // defines variables
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH, 15000);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  if (distance < 50) {
    distance = 0;
  }else{
    distance = 300 - distance;
  }

  return distance;

  delay(60);
  //delay(500);
}


void showHeight(int cantidadMax) {
  for (int i = 0; i < cantidadMax; i++) {
    tiraLed.setPixelColor(i, tiraLed.ColorHSV(50 * 256));
    tiraLed.show();
    delay(25);
  }
}