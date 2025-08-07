//
//TOFA:
//PONER EL saltoEnCmMAX para que sea ese el que se envie

//WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

//JUEGO
#define IDJUEGO 3

//RFI
//#define DEBUG
#include <SPI.h>
#include <MFRC522.h>
const byte ssPin = 13;    //era 4
const byte resetPin = 5;  //era 2
MFRC522 mfrc522;
String currentID;

//16x02
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//FastLed
#include <FastLED.h>
#define NUM_LEDS 7
#define DATA_PIN 12
CRGB leds[NUM_LEDS];

FASTLED_USING_NAMESPACE
#define BRIGHTNESS 50
#define FRAMES_PER_SECOND 120

#define NUM_LEDS1 8
#define DATA_PIN1 04
CRGB leds1[NUM_LEDS1];


#define TAIL_LENGTH 8  // Number of LEDs in the tail
#define TAIL_DELAY 50  // Delay between tail LEDs (adjust as needed)

uint8_t gHue = 0;
#define NUM_LEDS2 67
#define DATA_PIN2 02
CRGB leds2[NUM_LEDS2];
#define LED_TYPE2 WS2811_400
#define COLOR_ORDER2 BRG

#define TAIL_LENGTH 5  // Number of LEDs in the tail
#define TAIL_DELAY 50

//HCsr04
const int trigPin1 = 25;
const int trigPin2 = 16;
const int trigPin3 = 17;
const int trigPin4 = 27;
const int echoPin1 = 26;
#define SOUND_SPEED 0.034
long hcsr1;
int distanceCm;
int distanceCm1;
int distanceCm2;
int distanceCm3;
int distanceCm4;
int distanceCmOk;
int saltoEnCm;
int saltoLedFloat;
int saltoEnCmPrev = 0;
int saltoFinalEnCm;

int i = 0;

int iteration = 0;
//cuentas Millis
unsigned long previousHcsr_A = 0;
unsigned long previousGameStart = 0;
const long intervalResetGame = 20000;
const long intervaloTiempoReseteo = 5000;  //SE ACABO EL TIEMPO
unsigned long tiempoPrevioParaReseteo = 0;

//logica loop
int saltoLed;
const int alturaDelSensor = 300;  //en cm
const int alturaMinima = 55;
int saltoFinal;
String jugadorN;

bool saltoOk = false;

bool startGame = false;
bool currentGameStartFlag = false;
bool flagTimeOut = false;
bool saltoGrabado = false;
bool tiempoParaReseteo = false;
bool flagTiempoParaReseteo = false;

//Manejo de wifi
String mainURL = "http://34.193.34.135/DesafioFutbol/DBHandler/GrabarHito.aspx?";
const char* ssid = "Personal-WiFi-095-2.4Ghz";
const char* password = "ZXvN6hxVEk";

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println(F("Serial communication started"));
#endif
  delay(250);
  //Conexion a red
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(250);
  //  while (WiFi.status() != WL_CONNECTED)
  //  {
  //    Serial.print(".");
  //    delay(100);
  //  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(".");
    Serial.println("CONECTADO");
  }

  //RFID
  SPI.begin();
  mfrc522.PCD_Init(ssPin, resetPin);
  mfrc522.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max);
  delay(100);

  //1602 init
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("DESAFIO");
  lcd.setCursor(4, 1);
  lcd.print("FUTBOL!");

  FastLED.setBrightness(BRIGHTNESS);
  //FastLed
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);     // GRB ordering is assumed
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds1, NUM_LEDS1);  // GRB ordering is assumed


  FastLED.addLeds<LED_TYPE2, DATA_PIN2, COLOR_ORDER2>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);

  //FastLED.addLeds<WS2811_400, DATA_PIN2,BRG>(leds2, NUM_LEDS2);  // GRB ordering is assumed
  leds1[0] = CRGB::Orange;
  leds1[1] = CRGB::Orange;
  leds1[2] = CRGB::Orange;
  leds1[3] = CRGB::Orange;
  leds1[4] = CRGB::Orange;
  leds1[5] = CRGB::Orange;
  leds1[6] = CRGB::Orange;
  leds1[7] = CRGB::Orange;
  FastLED.show();

  for (int i = 0; i < NUM_LEDS2; i++) {
    leds2[i] = CRGB::Black;
  }
  FastLED.show();



  //Hcsr04-1
  pinMode(trigPin1, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT);   // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT);  // Sets the trigPin as an Output
  pinMode(trigPin3, OUTPUT);  // Sets the trigPin as an Output
  pinMode(trigPin4, OUTPUT);  // Sets the trigPin as an Output
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS2 + TAIL_LENGTH; i++) {
    fill_solid(leds2, NUM_LEDS2, CRGB::Black);

    for (int j = 0; j < TAIL_LENGTH; j++) {
      int pos = i - j;
      if (pos >= 0 && pos < NUM_LEDS2) {
        leds2[pos] = CHSV(160, 255, 255);  // Azul
        leds2[pos].fadeToBlackBy(j * (255 / TAIL_LENGTH));
      }
    }

    FastLED.show();
    delay(TAIL_DELAY);
  }
}

void loop() {

  //fadeall();
  /*
  testCometEffect(leds, NUM_LEDS, CHSV(0, 255, 255));
  testCometEffect(leds1, NUM_LEDS1, CHSV(96, 255, 255));
  testCometEffect(leds2, NUM_LEDS2, CHSV(160, 255, 255));

*/

  if (startGame == false) {


    Serial.print("x");
    // First slide the led in one direction
    //    for (int i = 0; i < NUM_LEDS2; i++) {
    //      // Set the i'th led to red
    //      leds2[i] = CHSV(hue = hue + 3, 255, 120);
    //      // Show the leds
    //      FastLED.show();
    //      // now that we've shown the leds, reset the i'th led to black
    //      // leds[i] = CRGB::Black;
    //      fadeall();
    //      // Wait a little bit before we loop around and do it again
    //      delay(3);
    //    }

    //    // Now go in the other direction.
    //    for (int i = (NUM_LEDS2) - 1; i >= 0; i--) {
    //      // Set the i'th led to red
    //      leds2[i] = CHSV(hue=hue+3, 255, 120);
    //      // Show the leds
    //      FastLED.show();
    //      // now that we've shown the leds, reset the i'th led to black
    //      // leds[i] = CRGB::Black;
    //      //fadeall();
    //      // Wait a little bit before we loop around and do it again
    //      delay(3);
    //    }
    //
    //    FastLED.clear();
    //
    //    FastLED.show();
  }

  if (saltoOk == true) {




    //Serial.println(saltoLed);
    // leds2[saltoLed] = CHSV( gHue, 255, 192);
    leds2[saltoLed] += CHSV(gHue, 255, 192);

    FastLED.show();


    //  fadeToBlackBy( leds2, saltoLed, 50);
    //  int pos = beatsin16( 8, 0, saltoLed - 0 );
    //leds2[pos] += CHSV( gHue, 255, 192);
    saltoOk = false;
  }

  EVERY_N_MILLISECONDS(20) {
    gHue = gHue + 5;  // slowly cycle the "base color" through the rainbow
  }



  //lecturaDeSalto();
  //Serial.println(303-distanceCm);

  if (startGame == false) {
    lecturaDeRfid();
  }

  if (startGame == true) {
    lecturaDeSalto();
    currentGameStarted();
  }
  if (tiempoParaReseteo == true) {
    displayDesafioFutbol();
  }
}


void lecturaDeRfid() {
  boolean changedValue = false;

#ifdef DEBUG
  if (changedValue) {
    Serial.print(F("Detected tag: "));
    Serial.println(currentID);
    Serial.println(F("---"));
  }
#endif

  mfrc522.PCD_Init();
  String readRFID = "";
  currentID = readRFID;
  delay(30);

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    readRFID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    jugadorN = readRFID;
  }

  if (readRFID != currentID) {
    changedValue = true;
    currentID = readRFID;

    leds[0] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SALTA BIEN ALTO!");

    startGame = true;
    tiempoParaReseteo = false;
    tiempoPrevioParaReseteo = 0;
    currentGameStartFlag = false;
    flagTimeOut = false;
    saltoGrabado = false;
    tiempoParaReseteo = false;
    flagTiempoParaReseteo = false;
    FastLED.clear();
    FastLED.show();
    leds1[0] = CRGB::Orange;
    leds1[1] = CRGB::Orange;
    leds1[2] = CRGB::Orange;
    leds1[3] = CRGB::Orange;
    leds1[4] = CRGB::Orange;
    leds1[5] = CRGB::Orange;
    leds1[6] = CRGB::Orange;
    leds1[7] = CRGB::Orange;
    FastLED.show();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  static uint8_t hue = 0;
  EVERY_N_MILLISECONDS(20) {
    gHue = gHue + 5;  // slowly cycle the "base color" through the rainbow
  }

  if (i < NUM_LEDS2) {
    i++;
    leds2[i] = CHSV(hue = hue + 3, 255, 120);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();

    if (i >= NUM_LEDS2) {
      i = 0;
    }
  }
  delay(1);
}

void sendingData(int _valor) {
  Serial.println("manda datos por wifi");
  Serial.println(WiFi.status());

  //GRABO DATOS
  //Esperar conexion
  if ((WiFi.status() == WL_CONNECTED)) {
    Serial.println("CONECTADO");
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(5000);

    if (http.begin(client, mainURL + "cd_jugador=" + jugadorN + "&cd_juego=" + IDJUEGO + "&vl_resultado=" + _valor)) {
      Serial.println("SENDING");
      //delay(100);
      int httpCode = http.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET... codigo: %d", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... error: %s", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.printf("[HTTP} No se ha podido conectar");
    }
  }
  delay(1000);
}

void lecturaDeSalto() {

  delay(30);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  digitalWrite(trigPin3, HIGH);
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  hcsr1 = pulseIn(echoPin1, HIGH, 15000);

  // Calculate the distance
  distanceCm = hcsr1 * SOUND_SPEED / 2;

  if (iteration == 1) {
    distanceCm2 = distanceCm;
    if ((((distanceCm2) - (distanceCm1)) < 10) && (((distanceCm2) - (distanceCm1)) > -10)) {
      distanceCmOk = distanceCm2;
    }
    iteration = 0;
  }

  if (iteration == 0) {
    distanceCm1 = distanceCm;
    //    Serial.print("iteration 0: ");
    //    Serial.println(iteration);
    iteration = 1;
  }

  // if (true) {

  saltoEnCm = alturaDelSensor - distanceCmOk;

  if (saltoEnCm < 80) {
    saltoOk = false;
  }

  if (saltoEnCm < 270 && saltoEnCm > 80) {
    //leds[0] = CRGB::Green;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("ALTURA MAX:");
    lcd.setCursor(4, 1);
    lcd.print(saltoEnCm);
    lcd.setCursor(9, 1);
    lcd.print("mts");
    //leds[0] = CRGB::Black;
    saltoGrabado = true;
    saltoOk = true;
    saltoLed = map(saltoEnCm, 320, 80, 60, 16);

    if (saltoEnCm > saltoEnCmPrev) {
      saltoEnCmPrev = saltoEnCm;
    }
  }
  //}
}

void currentGameStarted() {
  if (flagTimeOut == false) {
    previousGameStart = millis();
    flagTimeOut = true;
  }
  unsigned long currentGameStart = millis();

  currentGameStartFlag = true;

  if (currentGameStartFlag == true && saltoGrabado == false) {
    if (currentGameStart - previousGameStart >= intervalResetGame) {
      // save the last time you blinked the LED
      previousGameStart = currentGameStart;
      currentGameStartFlag = false;
      startGame = false;
      flagTimeOut = false;
      tiempoParaReseteo = true;
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("SE ACABO");
      lcd.setCursor(4, 1);
      lcd.print("EL TIEMPO");
    }
  }

  if (currentGameStartFlag == true && saltoGrabado == true) {
    if (currentGameStart - previousGameStart >= intervalResetGame) {
      // save the last time you blinked the LED
      previousGameStart = currentGameStart;
      currentGameStartFlag = false;

      lcd.clear();
      flagTimeOut = false;
      leds[0] = CRGB::Blue;
      FastLED.show();
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("SALTO GRABADO");
      lcd.setCursor(4, 1);
      lcd.print(saltoEnCmPrev);
      lcd.setCursor(9, 1);
      lcd.print("mts");
      leds[0] = CRGB::Black;
      FastLED.show();
      saltoGrabado = false;
      delay(100);
      tiempoParaReseteo = true;
      startGame = false;
      //saltoFinal = saltoEnCmPrev;

      sendingData(MapeaDato(saltoEnCmPrev));
    }
  }
}

int MapeaDato(int _dato) {
  _dato = map(saltoFinal, 10, 27, 10, 100);

  //FALTA MAPEAR DATO
  return (int)_dato;
}

void displayDesafioFutbol() {

  FastLED.clear();
  FastLED.show();

  saltoGrabado = false;
  saltoOk = false;
  saltoLed = 0;
  saltoEnCm = 0;
  saltoEnCmPrev = 0;

  if (flagTiempoParaReseteo == false) {
    tiempoPrevioParaReseteo = millis();
    flagTiempoParaReseteo = true;
  }
  unsigned long tiempoActualParaReseteo = millis();
  if (tiempoActualParaReseteo - tiempoPrevioParaReseteo > intervaloTiempoReseteo) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DESAFIO FUTBOL !");
    tiempoParaReseteo = false;
    flagTiempoParaReseteo = false;
  }
}

String dump_byte_array(byte* buffer, byte bufferSize) {
  String read_rfid = "";

  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }

  return read_rfid;
}

void testCometEffect(CRGB* ledArray, int numLeds, CHSV color) {
  for (int i = 0; i < numLeds + TAIL_LENGTH; i++) {
    // Apagar todos
    fill_solid(ledArray, numLeds, CRGB::Black);

    // Dibujar cola
    for (int j = 0; j < TAIL_LENGTH; j++) {
      int pos = i - j;
      if (pos >= 0 && pos < numLeds) {
        ledArray[pos] = color;
        ledArray[pos].fadeToBlackBy(j * (255 / TAIL_LENGTH));  // Cola más tenue
      }
    }

    FastLED.show();
    delay(TAIL_DELAY);
  }
}
