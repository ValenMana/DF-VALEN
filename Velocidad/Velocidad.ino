//WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

//JUEGO
#define IDJUEGO 1

//RFID 522
#define DEBUG
#include <SPI.h>
#include <MFRC522.h>
const byte ssPin = 13;
const byte resetPin = 5;
const String correctIDs[] = {"92571f37"};//Aca va el id de la tarjeta
MFRC522 mfrc522;
String currentID;

//16x02
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//FastLed
#include <FastLED.h>
#define NUM_LEDS 7
#define DATA_PIN 12
CRGB leds[NUM_LEDS];

//HCsr04
const int trigPin1 = 16;//verde amarillo
const int echoPin1 = 17;//marron naranja
const int trigPin2 = 25;//verde
const int echoPin2 = 26;//blanco
#define SOUND_SPEED 0.034
long hcsr1;
float distanceCm1;
long hcsr2;
float distanceCm2;
const float distanciaEntreSensores = 7; //en mts
int distanciaMinima = 30; // CORRECTA ES 30!!
int distanciaMaxima = 85;

//cuentas Millis
unsigned long previousHcsr_A = 0;
unsigned long previousHcsr_B = 0;
unsigned long previousGameStart = 0;
const long interval = 5000;
//const long intervalResetGame = 10000;
long tiempoFinal = 0;
const long intervalResetGame = 10000;
const long intervaloTiempoReseteo = 2000; //tiempo de
unsigned long tiempoPrevioParaReseteo = 0;


//logica loop
String jugadorN;
bool readHcsr2 = false;
float velocidadFinal;


bool startGame = false;
bool currentGameStartFlag = false;
bool flagTimeOut = false;
bool velocidadGrabada = false;
bool tiempoParaReseteo = false;
bool flagTiempoParaReseteo = false;
bool flagHcsr2 = false;

//Manejo de wifi
String mainURL = "http://34.193.34.135/DesafioFutbol/DBHandler/GrabarHito.aspx?";
const char* ssid     = "Personal-WiFi-095-2.4Ghz";
const char* password = "ZXvN6hxVEk";

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println(F("Serial communication started"));
#endif
  delay (250);

  //Conexion a red
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(250);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
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

  //FastLed
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  leds[0] = CRGB::White;
  leds[1] = CRGB::White;
  leds[2] = CRGB::White;
  leds[3] = CRGB::White;
  leds[4] = CRGB::White;
  leds[5] = CRGB::White;
  leds[6] = CRGB::White;
  leds[7] = CRGB::White;
  FastLED.show();
  delay(500);
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Black;
  leds[6] = CRGB::Black;
  leds[7] = CRGB::Black;

  FastLED.show();
  delay(500);

  //Hcsr04-1
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  //Hcsr04-2
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

}

void loop() {

  if (startGame == false) {
    lecturaDeRfid();
  }

  if (startGame == true) {
    lecturaDeVelocidad();
    currentGameStarted();
  }

  displayDesafioFutbol();
}

void lecturaDeRfid() {
  boolean changedValue = false;
  //
  //#ifdef DEBUG
  //  if (changedValue) {
  //    Serial.print(F("Detected tag: "));
  //    Serial.println(currentID);
  //    Serial.println(F("---"));
  //
  //  }
  //  Serial.print("distanceCm1 ");
  //  Serial.println(distanceCm1);
  //  Serial.print("distanceCm1 ");
  //  Serial.println(distanceCm2);
  //#endif

  mfrc522.PCD_Init();
  String readRFID = "";
    currentID = readRFID;
    delay(30);


  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    readRFID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    jugadorN  = readRFID;
  }

  if (readRFID != currentID) {
    changedValue = true;
    currentID = readRFID;

    leds[0] = CRGB::Red;
    leds[1] = CRGB::Red;
    leds[2] = CRGB::Red;
    leds[3] = CRGB::Red;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
    leds[6] = CRGB::Red;
    leds[7] = CRGB::Red;

    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    FastLED.show();
    delay(500);

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("A CORRER !!");
    //maxDistanceCm = 25;
    startGame = true;
    tiempoParaReseteo = false;
    tiempoPrevioParaReseteo = 0;
    currentGameStartFlag = true; //era false
    flagTimeOut = false;
    velocidadGrabada = false;
    tiempoParaReseteo = false;
    flagTiempoParaReseteo = false;
    previousHcsr_A = millis();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(100);
}

void sendingData (int _valor) {
  Serial.println("manda datos por wifi");
  Serial.println(WiFi.status());

  //GRABO DATOS
  //Esperar conexion
  if ((WiFi.status() == WL_CONNECTED)) {
    Serial.println("CONECTADO");
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(5000);

    if (http.begin(client, mainURL + "  cd_jugador=" + jugadorN + "&cd_juego=" + IDJUEGO + "&vl_resultado=" + _valor))
    {
      Serial.println("SENDING");
      delay(100);
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
    }
    else
    {
      Serial.printf("[HTTP} No se ha podido conectar");
    }
  }
  delay(1000);
}



void lecturaDeVelocidad() {
  //delay(10);

  unsigned long currentHcsr_A = millis();
  unsigned long currentHcsr_B = millis();

  if (readHcsr2 == false) {
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    hcsr1 = pulseIn(echoPin1, HIGH, 6400); // VALOR OK 6400

    // Calculate the distance
    distanceCm1 = hcsr1 * SOUND_SPEED / 2;

    if (distanceCm1 > distanciaMinima && distanceCm1 < distanciaMaxima && velocidadGrabada == false) {
      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[3] = CRGB::Green;
      leds[4] = CRGB::Green;
      leds[5] = CRGB::Green;
      leds[6] = CRGB::Green;
      leds[7] = CRGB::Green;
      FastLED.show();
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("CORRIENDO");
      readHcsr2 = true;

    }
    if (currentHcsr_A - previousHcsr_A >= interval && readHcsr2 == false) {
      // save the last time you blinked the LED
      previousHcsr_A = currentHcsr_A;
      vuelveAIntentarlo();
      
      //previousGameStart = currentGameStart;
    }
  }
  if (readHcsr2 == true) {
    delay(100); // VER SI SE PUEDE SACAR ESTE DELAY
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    hcsr2 = pulseIn(echoPin2, HIGH, 5000); // VALOR OK 5000 !!
    delay(10);
    // Calculate the distance
    distanceCm2 = hcsr2 * SOUND_SPEED / 2;

    Serial.println("Ultra1: " + String(distanceCm1) + " " + "Ultra2: " + String(distanceCm2));
    if (distanceCm2 > distanciaMinima && distanceCm2 < distanciaMaxima  ) {
      previousHcsr_B = currentHcsr_B;
      
      

      tiempoFinal = (previousHcsr_B - previousHcsr_A);

      if (tiempoFinal < interval) {
        velocidadFinal = (distanciaEntreSensores / tiempoFinal) * 3600;
        lcd.clear();
        //          lcd.setCursor(0, 1);
        //          lcd.print(tiempoFinal);
        lcd.setCursor(3, 0);
        lcd.print("VELOCIDAD:");
        lcd.setCursor(3, 1);
        lcd.print((distanciaEntreSensores / tiempoFinal) * 3600);
        lcd.setCursor(9, 1);
        lcd.print("Km/h");
        delay(300);
        leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        leds[2] = CRGB::Red;
        leds[3] = CRGB::Red;
        leds[4] = CRGB::Red;
        leds[5] = CRGB::Red;
        leds[6] = CRGB::Red;
        leds[7] = CRGB::Red;

        FastLED.show();
        delay(10000);
        leds[0] = CRGB::Black;
        leds[1] = CRGB::Black;
        leds[2] = CRGB::Black;
        leds[3] = CRGB::Black;
        leds[4] = CRGB::Black;
        leds[5] = CRGB::Black;
        leds[6] = CRGB::Black;
        leds[7] = CRGB::Black;
        FastLED.show();
        //previousGameStart = currentGameStart;
        //flagTimeOut = false;
        velocidadGrabada = true;
        startGame = false; // VER
        readHcsr2 = false;
        previousHcsr_A = currentHcsr_A;
      }
    }
    if (currentHcsr_B - previousHcsr_B >= interval) {
        // save the last time you blinked the LED
        previousHcsr_B = currentHcsr_B;
        readHcsr2 = false;
      }
  }
  //  if ((currentHcsr_A - previousHcsr_A > interval) && readHcsr2 == true) {
  //
  //    leds[0] = CRGB::Red;
  //    leds[1] = CRGB::Red;
  //    leds[2] = CRGB::Red;
  //    leds[3] = CRGB::Red;
  //    leds[4] = CRGB::Red;
  //    leds[5] = CRGB::Red;
  //    leds[6] = CRGB::Red;
  //    leds[7] = CRGB::Red;
  //
  //    readHcsr2 = false;
  //    lcd.clear();
  //    lcd.setCursor(0, 0);
  //    lcd.print("VUELVE A");
  //    lcd.setCursor(0, 1);
  //    lcd.print("INTENTARLO");
  //    delay(1000);
  //    FastLED.show();
  //    leds[0] = CRGB::Black;
  //    leds[1] = CRGB::Black;
  //    leds[2] = CRGB::Black;
  //    leds[3] = CRGB::Black;
  //    leds[4] = CRGB::Black;
  //    leds[5] = CRGB::Black;
  //    leds[6] = CRGB::Black;
  //    leds[7] = CRGB::Black;
  //    FastLED.show();
  //  }
}


void currentGameStarted() {
  if (flagTimeOut == false) {
    previousGameStart = millis();
    flagTimeOut = true;
  }
  unsigned long currentGameStart = millis();

  currentGameStartFlag = true;

  if (currentGameStartFlag == true && velocidadGrabada == false) {
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

  if (currentGameStartFlag == true && velocidadGrabada == true) {
    if (currentGameStart - previousGameStart >= intervalResetGame) {
      // save the last time you blinked the LED
      previousGameStart = currentGameStart;

      delay(399);
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      leds[4] = CRGB::Black;
      leds[5] = CRGB::Black;
      leds[6] = CRGB::Black;
      leds[7] = CRGB::Black;

      lcd.setCursor(3, 0);
      lcd.print("VELOCIDAD:");
      lcd.setCursor(3, 1);
      lcd.print((distanciaEntreSensores / tiempoFinal) * 3600);
      lcd.setCursor(9, 1);
      lcd.print("Km/h");
      //delay(300);

      leds[0] = CRGB::Green;
      leds[1] = CRGB::Green;
      leds[2] = CRGB::Green;
      leds[3] = CRGB::Green;
      leds[4] = CRGB::Green;
      leds[5] = CRGB::Green;
      leds[6] = CRGB::Green;
      leds[7] = CRGB::Green;
      FastLED.show();

      delay(4000);
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      leds[4] = CRGB::Black;
      leds[5] = CRGB::Black;
      leds[6] = CRGB::Black;
      leds[7] = CRGB::Black;

      FastLED.show();
      velocidadGrabada = false;
      delay(100);
      tiempoParaReseteo = true;
      startGame = false;
      flagTimeOut = false;
      currentGameStartFlag = false;
      //saltoFinal = alturaDelSensor - maxDistanceCm;

      sendingData(MapeaDato(velocidadFinal));
      //displayDesafioFutbol();
    }
  }
}


int MapeaDato(float _dato)
{
  _dato = map(velocidadFinal, 5, 15, 10, 100);

  //FALTA MAPEAR DATO
  return (int)_dato;
}

void displayDesafioFutbol() {
  if (tiempoParaReseteo == true) {
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
      leds[0] = CRGB::Blue;
      leds[1] = CRGB::Blue;
      leds[2] = CRGB::Blue;
      leds[3] = CRGB::Blue;
      leds[4] = CRGB::Blue;
      leds[5] = CRGB::Blue;
      leds[6] = CRGB::Blue;
      leds[7] = CRGB::Blue;
      FastLED.show();
      delay(500);
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      leds[4] = CRGB::Black;
      leds[5] = CRGB::Black;
      leds[6] = CRGB::Black;
      leds[7] = CRGB::Black;
    }
  }
}

String dump_byte_array(byte *buffer, byte bufferSize) {
  String read_rfid = "";

  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }

  return read_rfid;
}


void vuelveAIntentarlo() {

  leds[0] = CRGB::Red;
  leds[1] = CRGB::Red;
  leds[2] = CRGB::Red;
  leds[3] = CRGB::Red;
  leds[4] = CRGB::Red;
  leds[5] = CRGB::Red;
  leds[6] = CRGB::Red;
  leds[7] = CRGB::Red;


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("VUELVE A");
  lcd.setCursor(0, 1);
  lcd.print("INTENTARLO");
  delay(1000);
  FastLED.show();
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Black;
  leds[6] = CRGB::Black;
  leds[7] = CRGB::Black;
  FastLED.show();

  readHcsr2 = false;
  startGame = false;

}
