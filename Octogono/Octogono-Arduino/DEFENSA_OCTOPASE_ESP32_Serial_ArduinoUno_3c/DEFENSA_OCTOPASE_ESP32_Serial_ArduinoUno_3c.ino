//
//TOFA:
//PONER EL saltoEnCmMAX para que sea ese el que se envie

//for serialCom with arduino /328p
char c;
String dataIn;
//#define p25avisaEsp32showLed 25
//#define p14avisaEsp32lecturaSalto 14

//WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

//JUEGO
#define IDJUEGO 8

//RFI
//#define DEBUG
#include <SPI.h>
#include <MFRC522.h>
const byte ssPin = 13; //era 4
const byte resetPin = 5;  //era 2
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

FASTLED_USING_NAMESPACE
#define BRIGHTNESS          50
#define FRAMES_PER_SECOND  120

#define NUM_LEDS1 8
#define DATA_PIN1 04  
CRGB leds1[NUM_LEDS1];



//int distanceCmOk;
//int saltoEnCm;
//int saltoLedFloat;
//int saltoEnCmPrev = 0;
//int saltoFinalEnCm;

//cuentas Millis
unsigned long previousGameStart = 0;
const long intervalResetGame = 65000;//era 20000
const long intervaloTiempoReseteo = 5000; //SE ACABO EL TIEMPO era 5000
unsigned long tiempoPrevioParaReseteo = 0;

//logica loop
int cuentaReacciones;
String jugadorN;

bool reaccionesOk = false;

bool startGame = false;
bool currentGameStartFlag = false;
bool flagTimeOut = false;
bool reaccionesGrabadas = false;
bool tiempoParaReseteo = false;
bool flagTiempoParaReseteo = false;

//Manejo de wifi
String mainURL = "http://34.193.34.135/DesafioFutbol/DBHandler/GrabarHito.aspx?";
const char* ssid     = "Personal-WiFi-095-2.4Ghz";
const char* password = "ZXvN6hxVEk";

void setup() {
  //  pinMode(p14avisaEsp32lecturaSalto, OUTPUT);
  //  pinMode(p25avisaEsp32showLed, OUTPUT);

#ifdef DEBUG
  //Serial.begin(115200);
  //Serial.println(F("Serial communication started"));
#endif
  delay(250);

  Serial2.begin(300);  // RX, TX pins

  //Conexion a red
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(250);
  //  while (WiFi.status() != WL_CONNECTED)
  //  {
  //    Serial.print(".");
  //    delay(100);
  //  }

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
  lcd.print("DEFENSA !");

  FastLED.setBrightness(BRIGHTNESS);
  //FastLed
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::Green;
  leds[5] = CRGB::Green;
  leds[6] = CRGB::Green;
  leds[7] = CRGB::Green;
  FastLED.show();
}


void loop() {

  if (startGame == false) {
    //    digitalWrite (p14avisaEsp32lecturaSalto, LOW);
    //    digitalWrite (p25avisaEsp32showLed, HIGH);
    lecturaDeRfid();
  }

  if (startGame == true) {
    //    digitalWrite (p14avisaEsp32lecturaSalto, HIGH);
    //    digitalWrite (p25avisaEsp32showLed, LOW);
    lecturaDeDefensa();
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
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    FastLED.show();
    delay(500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("REACCIONA ");
    lcd.setCursor(0, 1);
    lcd.print("RAPIDO !");
    Serial2.print("8\n");
    delay(30);
    Serial2.print("8\n");
    delay(30);
    Serial2.print("8\n");
    delay(30);
  

    startGame = true;
    tiempoParaReseteo = false;
    tiempoPrevioParaReseteo = 0;
    currentGameStartFlag = false;
    flagTimeOut = false;
    reaccionesGrabadas = false;
    tiempoParaReseteo = false;
    flagTiempoParaReseteo = false;
    FastLED.clear();
    FastLED.show();
    leds[0] = CRGB::Orange;
    leds[1] = CRGB::Orange;
    leds[2] = CRGB::Orange;
    leds[3] = CRGB::Orange;
    leds[4] = CRGB::Orange;
    leds[5] = CRGB::Orange;
    leds[6] = CRGB::Orange;
    FastLED.show();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1);
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

    if (http.begin(client, mainURL + "cd_jugador=" + jugadorN + "&cd_juego=" + IDJUEGO + "&vl_resultado=" + _valor))
    {
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
    }
    else
    {
      Serial.printf("[HTTP} No se ha podido conectar");
    }
  }
  delay(1000);
}

void lecturaDeDefensa() {

  while (Serial2.available() )
  {
    c = Serial2.read();
    if (c != '\n') {
      dataIn += c;
    }
    else {
      break;
    }
  }
  
  if (c == '\n') {
    int intDataIn = dataIn.toInt();
    cuentaReacciones = intDataIn;
    c = 0;
    dataIn = "";
  }

  if (cuentaReacciones > 1) {
    reaccionesGrabadas = true;
    reaccionesOk = true;
  }
}

void currentGameStarted() {
  if (flagTimeOut == false) {
    previousGameStart = millis();
    flagTimeOut = true;
  }
  unsigned long currentGameStart = millis();
  currentGameStartFlag = true;

  if (currentGameStartFlag == true && reaccionesGrabadas == false) {
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

      Serial2.print("3\n");
      delay(30);
      Serial2.print("3\n");
      delay(30);
      Serial2.print("3\n");
      delay(30);
      }
  }

  if (currentGameStartFlag == true && reaccionesGrabadas == true) {
    if (currentGameStart - previousGameStart >= intervalResetGame) {
      // save the last time you blinked the LED
      previousGameStart = currentGameStart;
      currentGameStartFlag = false;

      lcd.clear();
      flagTimeOut = false;
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Red;
      leds[3] = CRGB::Red;
      leds[4] = CRGB::Red;
      leds[5] = CRGB::Red;
      leds[6] = CRGB::Red;
      FastLED.show();
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("PUNTOS");
      lcd.setCursor(4, 1);
      lcd.print(cuentaReacciones);

      Serial2.print("3\n");
      delay(30);
      Serial2.print("3\n");
      delay(30);
      Serial2.print("3\n");
      delay(30);
      

      reaccionesGrabadas = false;
      delay(5000);
      tiempoParaReseteo = true;
      startGame = false;
      //saltoFinal = saltoEnCmPrev;
      sendingData(MapeaDato(cuentaReacciones));
    }
  }
}

int MapeaDato(int _dato)
{
  _dato = map(cuentaReacciones, 1, 100, 10, 100);

  //FALTA MAPEAR DATO
  return (int)_dato;
}

void displayDesafioFutbol() {

  FastLED.clear();
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::Green;
  leds[5] = CRGB::Green;
  leds[6] = CRGB::Green;
  FastLED.show();

  reaccionesGrabadas = false;
  reaccionesOk = false;
  cuentaReacciones = 0;


  if (flagTiempoParaReseteo == false) {
    tiempoPrevioParaReseteo = millis();
    flagTiempoParaReseteo = true;
  }
  unsigned long tiempoActualParaReseteo = millis();
  if (tiempoActualParaReseteo - tiempoPrevioParaReseteo > intervaloTiempoReseteo) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("DESAFIO");
    lcd.setCursor(4, 1);
    lcd.print("DEFENSA !");
    tiempoParaReseteo = false;
    flagTiempoParaReseteo = false;
  }

}

String dump_byte_array(byte * buffer, byte bufferSize) {
  String read_rfid = "";

  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }

  return read_rfid;
}
