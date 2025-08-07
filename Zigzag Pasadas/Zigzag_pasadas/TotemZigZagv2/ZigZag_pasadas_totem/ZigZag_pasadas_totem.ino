//WIFI
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

//JUEGO RAMPA 1
#define IDJUEGO 96

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

#define ESP32

//#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
//const int HX711_dout = 25; //mcu > HX711 dout pin
//const int HX711_sck = 26; //mcu > HX711 sck pin

//HX711 constructor:
//HX711_ADC LoadCell(HX711_dout, HX711_sck);

//const int calVal_eepromAdress = 0;
//unsigned long t = 0;


//float potenciaMinima = 3;
//float potenciaMaxima = 20;

//float potenciaMedida = 0;
//float potenciaMedidaMax = 0;


//cuentas Millis
unsigned long tiempoDeInicio = 0;

unsigned long tiempoActual = 0;
unsigned long tiempoHcsr_B = 0;
const long tiempoMaxDeJuego = 15000;
long tiempoFinal = 0;


//logica loop
String jugadorN;


//bool flagMedidaOk = false;
//bool flagMidiendo = false;

//bool flagCurrentHcsr_A = false;
//bool flagCurrentHcsr_B = false;
//bool startGame = false;

//Manejo de wifi
String mainURL = "http://34.193.34.135/DesafioFutbol/DBHandler/GrabarHito.aspx?";
const char* ssid     = "Personal-WiFi-095-2.4Ghz";
const char* password = "ZXvN6hxVEk";

int state;
#define WAITING_TAG 0
#define START_GAME 1
#define GAME 2
#define END_GAME 3

void setup() {
  Serial.begin(115200);
  state = WAITING_TAG;




  /* //Conexion a red
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
  */

  //RFID
  SPI.begin();
  mfrc522.PCD_Init(ssPin, resetPin);
  mfrc522.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max);
  delay(100);


  //1602 init
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("DESAFIO");
  lcd.setCursor(5, 1);
  lcd.print("ZIG-ZAG");


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

  //LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  //float calibrationValue; // calibration value (see example file "Calibration.ino")
  //calibrationValue = 18432.0; // uncomment this if you want to set the calibration value in the sketch
  //#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
  //#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  //unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  //boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  //LoadCell.start(stabilizingtime, _tare);

  /*if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
    }*/
  /*else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
    }*/

}

void loop() {
  //Serial.print("Load_cell output val: ");
  //Serial.println(potenciaMedida);

  switch (state) {
    case WAITING_TAG:
      if (changeRfid()) {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("CORRE ENTRE");
        lcd.setCursor(3, 1);
        lcd.print("LOS POSTES");
        state = START_GAME;
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
        leds[0] = CRGB::Yellow;
        leds[1] = CRGB::Yellow;
        leds[2] = CRGB::Yellow;
        leds[3] = CRGB::Yellow;
        leds[4] = CRGB::Yellow;
        leds[5] = CRGB::Yellow;
        leds[6] = CRGB::Yellow;
        leds[7] = CRGB::Yellow;
        FastLED.show();
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      delay(10);
      break;

    case START_GAME:
      Serial.println(-1);
      state = GAME;
      break;

    case GAME:
      if (Serial.available() > 0) {
        //char[] valor_leido=Serial.readString();
        String input = Serial.readString();
        Serial.println(input);
        if (input.toInt() == -2) {
          state = END_GAME;
        }
      }
      break;

    case END_GAME:
      if (Serial.available() > 0) {
        //char valor_leido=Serial.readString();
        //float tiempo= (float)Serial.readString();
        //sendingData(tiempo);
        //  Serial.println("enviando tiempo "+tiempo);

        state = WAITING_TAG;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("FIN DEL JUEGO");
        lcd.setCursor(2, 1);
        lcd.print("Puntaje:  " + String(Serial.readString().toInt()));
        leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        leds[2] = CRGB::Red;
        leds[3] = CRGB::Red;
        leds[4] = CRGB::Red;
        leds[5] = CRGB::Red;
        leds[6] = CRGB::Red;
        leds[7] = CRGB::Red;
        FastLED.show();
        delay(5000);
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("DESAFIO");
        lcd.setCursor(5, 1);
        lcd.print("ZIG-ZAG");

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
      break;
  }
}

boolean changeRfid() {
  boolean changedValue = false;

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
    return true;
  }
  else {
    return false;
  }
}

void sendingData (int _valor) {
  //Serial.println("manda datos por wifi");
  //Serial.println(WiFi.status());

  //GRABO DATOS
  //Esperar conexion
  if ((WiFi.status() == WL_CONNECTED)) {
    //Serial.println("CONECTADO");
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(5000);

    if (http.begin(client, mainURL + "cd_jugador=" + jugadorN + "&cd_juego=" + IDJUEGO + "&vl_resultado=" + _valor))
    {
      Serial.println("SENDING" + mainURL);
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



void desafioFutbol () {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("DESAFIO");
  lcd.setCursor(4, 1);
  lcd.print("ZIGZAG!");
  /* leds[0] = CRGB::Blue;
    leds[1] = CRGB::Blue;
    leds[2] = CRGB::Blue;
    leds[3] = CRGB::Blue;
    leds[4] = CRGB::Blue;
    leds[5] = CRGB::Blue;
    leds[6] = CRGB::Blue;
    leds[7] = CRGB::Blue;
    FastLED.show();*/

  //  LoadCell.start(stabilizingtime, _tare);
  //  LoadCell.tareNoDelay();
  //potenciaMedida = 0;
  //potenciaMedidaMax = 0;

}
/*
  int MapeaDato(float _dato)
  {
  _dato = map(potenciaMedidaMax, 3, 25, 10, 99);
  if (_dato > 100) {
    _dato = 99;
  }

  //FALTA MAPEAR DATO
  return (int)_dato;
  }*/


String dump_byte_array(byte * buffer, byte bufferSize) {
  String read_rfid = "";

  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }

  return read_rfid;
}

/*
  void vuelveAIntentarlo() {
  /*leds[0] = CRGB::Red;
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
  delay(2000);
  /*FastLED.show();
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Black;
  leds[6] = CRGB::Black;
  leds[7] = CRGB::Black;
  FastLED.show();

  startGame = false;
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("DESAFIO");
  lcd.setCursor(4, 1);
  lcd.print("POTENCIA!");
  /*leds[0] = CRGB::Blue;
  leds[1] = CRGB::Blue;
  leds[2] = CRGB::Blue;
  leds[3] = CRGB::Blue;
  leds[4] = CRGB::Blue;
  leds[5] = CRGB::Blue;
  leds[6] = CRGB::Blue;
  leds[7] = CRGB::Blue;
  FastLED.show();
  //  LoadCell.start(stabilizingtime, _tare);
  // LoadCell.tareNoDelay();
  potenciaMedida = 0;
  potenciaMedidaMax = 0;

  }*/
