#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno_SoftSerial (A2, A3);
char c;
String dataIn = "3";
int mensajeEsp = 123;

bool flag = false;
bool showLed = true;
bool comienzaOcto = false;
bool arrancaJuego = false;
bool apagaLeds = true;
bool apagaLeds2 = true;


int reseter = 10;

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define TCAADDR 0x70
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

#include "FastLED.h"

#define NUM_LEDS 45
#define NUM_STRIPS 8

CRGB leds[NUM_LEDS];
CLEDController *controllers[NUM_STRIPS];
uint8_t gBrightness = 70;
uint8_t gHue = 0;
#define FRAMES_PER_SECOND  120

bool nuevoRandom = true;
bool flagOn = true;
int leerSensorNum = 0;
int rangoGolpe = 5;
int prevLeerSensorNum = 123;
int cuentaPases = 0;
//int intervalo = 10000;
//int tiempoMaxJuego = 20000;

unsigned long startedSensorTime = 0;
unsigned long startedGame = 0;

float neutral_0 = -1.4;
float neutral_1 = -1.0;
float neutral_2 = -0.9;
float neutral_3 =  0.2;
float neutral_4 = -1.3;
float neutral_5 = -0.8;
float neutral_6 = -0.7;
float neutral_7 = -2.4;


void setup(void)
{
  FastLED.clear();
  FastLED.show();
  controllers[1] = &FastLED.addLeds<WS2811, 2, BRG>(leds, NUM_LEDS);
  controllers[2] = &FastLED.addLeds<WS2811, 3, BRG>(leds, NUM_LEDS);
  controllers[3] = &FastLED.addLeds<WS2811, 5, BRG>(leds, NUM_LEDS);
  controllers[4] = &FastLED.addLeds<WS2811, 6, BRG>(leds, NUM_LEDS);
  controllers[5] = &FastLED.addLeds<WS2811, 8, BRG>(leds, NUM_LEDS);
  controllers[6] = &FastLED.addLeds<WS2811, 9, BRG>(leds, NUM_LEDS);
  controllers[7] = &FastLED.addLeds<WS2811, 11, BRG>(leds, NUM_LEDS);
  controllers[0] = &FastLED.addLeds<WS2811, 12, BRG>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  digitalWrite(reseter, HIGH);
  delay(200);
  pinMode(10, OUTPUT);
  digitalWrite(reseter, HIGH);
  //digitalWrite(A0, HIGH);
  //Serial.begin(9600);
  ArduinoUno_SoftSerial.begin(300);

  //Serial.println("Accelerometer Test"); Serial.println("");
  randomSeed(analogRead(A0));

  //  fill_solid(leds, NUM_LEDS, CRGB::Green);
  //  controllers[6]->showLeds(gBrightness);
  //  delay(1000);
  //  fill_solid(leds, NUM_LEDS, CRGB::Black);
  //  controllers[6]->showLeds(gBrightness);


  //digitalWrite(reseter, LOW);
  //Wire.write(16);

  // Wire.read();

  // Wire.endTransmission();

  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    // Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    // while (1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);



  if (leerSensorNum == 0) {
    Wire.beginTransmission(0x70);
    Wire.write(1);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("   RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 1) {
    Wire.beginTransmission(0x70);
    Wire.write(2);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 2) {
    Wire.beginTransmission(0x70);
    Wire.write(4);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 3) {
    Wire.beginTransmission(0x70);
    Wire.write(8);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 4) {
    Wire.beginTransmission(0x70);
    Wire.write(16);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 5) {
    Wire.beginTransmission(0x70);
    Wire.write(32);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 6) {
    Wire.beginTransmission(0x70);
    Wire.write(64);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum++;
    }
  }

  if (leerSensorNum == 7) {
    Wire.beginTransmission(0x70);
    Wire.write(128);
    Wire.endTransmission();
    sensors_event_t event;
    accel.getEvent(&event);
    //Serial.println(event.acceleration.z);
    if (event.acceleration.z == 0 )
    {
      Serial.println(leerSensorNum);
      Serial.println("RESETER");
      leerSensorNum = 0;

      digitalWrite(reseter, LOW);
    }
    else {
      leerSensorNum = 0;
    }
  }
  
}

void loop(void)
{
  //FastLED.show();
  // insert a delay to keep the framerate modest

  if (apagaLeds2 == true) {
    gBrightness = 70;
    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) {
      gHue++;  // slowly cycle the "base color" through the rainbow
    }
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
    controllers[0]->showLeds(gBrightness);
    controllers[1]->showLeds(gBrightness);
    controllers[2]->showLeds(gBrightness);
    controllers[3]->showLeds(gBrightness);
    controllers[4]->showLeds(gBrightness);
    controllers[5]->showLeds(gBrightness);
    controllers[6]->showLeds(gBrightness);
    controllers[7]->showLeds(gBrightness);
  }

  // Serial.println("ANDO");
  while (ArduinoUno_SoftSerial.available()) {
    c = ArduinoUno_SoftSerial.read();
    if (c != '\n') {
      if (c == '8') {
        //Serial.println("888");
        arrancaJuego = true;
        comienzaOcto = true;
        apagaLeds = false;
        apagaLeds2 = false;
        gBrightness = 250;

      }
      if (dataIn == '3') {
        //Serial.println("333");
        apagaLeds = true;
        apagaLeds2 = true;
        comienzaOcto = false;
        nuevoRandom = false;
      }
      dataIn += c;
    }
    else {
      break;
    }
  }

  if (c == '\n') {
    Serial.println(dataIn);
    //ArduinoUno_SoftSerial.print ("UNO MANDA\n");
    //mensajeEsp = dataIn.toInt();
    if (dataIn == "8") {
      gBrightness = 250;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      controllers[0]->showLeds(gBrightness);
      controllers[1]->showLeds(gBrightness);
      controllers[2]->showLeds(gBrightness);
      controllers[3]->showLeds(gBrightness);
      controllers[4]->showLeds(gBrightness);
      controllers[5]->showLeds(gBrightness);
      controllers[6]->showLeds(gBrightness);
      controllers[7]->showLeds(gBrightness);
      dataIn = "123";
      //Serial.println("888");
      arrancaJuego = true;
      comienzaOcto = true;
      apagaLeds = false;
    }
    if (dataIn == "3") {
      //Serial.println("333");
      apagaLeds = true;
      apagaLeds2 = true;
      comienzaOcto = false;
      nuevoRandom = false;
    }
    c = 0;
    dataIn = "";
  }
  //  ----

  if (arrancaJuego == true) {
    arrancaJuego = false;
    //startedGame = millis();
    //Serial.println("ArrancaJuego");
    cuentaPases = 0;
    comienzaOcto = true;
    nuevoRandom = true;
  }

  if (apagaLeds == true) {
    apagaLeds = false;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    controllers[0]->showLeds(gBrightness);
    controllers[1]->showLeds(gBrightness);
    controllers[2]->showLeds(gBrightness);
    controllers[3]->showLeds(gBrightness);
    controllers[4]->showLeds(gBrightness);
    controllers[5]->showLeds(gBrightness);
    controllers[6]->showLeds(gBrightness);
    controllers[7]->showLeds(gBrightness);
    comienzaOcto = false;
    nuevoRandom = true;
    cuentaPases = 0;
  }

  if (comienzaOcto == true) {
    //Serial.println("comienzaOcto");
    if (nuevoRandom == true) {
      //Serial.println("NUEVORANDOM");
      leerSensorNum = random(8);
      if (leerSensorNum > 7 || leerSensorNum < 0) {
        leerSensorNum = random(8);
      }
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      controllers[0]->showLeds(gBrightness);
      controllers[1]->showLeds(gBrightness);
      controllers[2]->showLeds(gBrightness);
      controllers[3]->showLeds(gBrightness);
      controllers[4]->showLeds(gBrightness);
      controllers[5]->showLeds(gBrightness);
      controllers[6]->showLeds(gBrightness);
      controllers[7]->showLeds(gBrightness);


      if (prevLeerSensorNum != leerSensorNum) {

        nuevoRandom = false;
        prevLeerSensorNum = leerSensorNum;
      }
    }


    if (leerSensorNum == 0) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[0]->showLeds(gBrightness);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      controllers[7]->showLeds(gBrightness);
      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(1);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);
      if (event.acceleration.z > (neutral_0 + rangoGolpe) || event.acceleration.z < (neutral_0 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[0]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 1) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[1]->showLeds(gBrightness);
      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(2);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);
      if (event.acceleration.z > (neutral_1 + rangoGolpe) || event.acceleration.z < (neutral_1 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[1]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 2) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[2]->showLeds(gBrightness);
      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(4);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);
      if (event.acceleration.z > (neutral_2 + rangoGolpe) || event.acceleration.z < (neutral_2 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[2]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 3) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[3]->showLeds(gBrightness);
      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(8);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);
      if (event.acceleration.z > (neutral_3 + rangoGolpe) || event.acceleration.z < (neutral_3 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[3]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 4) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[4]->showLeds(gBrightness);

      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(16);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);

      if (event.acceleration.z > (neutral_4 + rangoGolpe) || event.acceleration.z < (neutral_4 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[4]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 5) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[5]->showLeds(gBrightness);

      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(32);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);

      if (event.acceleration.z > (neutral_5 + rangoGolpe) || event.acceleration.z < (neutral_5 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[5]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 6) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[6]->showLeds(gBrightness);
      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(64);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);

      if (event.acceleration.z > (neutral_6 + rangoGolpe) || event.acceleration.z < (neutral_6 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[6]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }

    if (leerSensorNum == 7) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      controllers[7]->showLeds(gBrightness);

      Wire.beginTransmission(0x70);
      // OKs: 01 02 04 08 16 32 64 128
      Wire.write(128);
      Wire.endTransmission();
      sensors_event_t event;
      accel.getEvent(&event);
      Serial.println(event.acceleration.z);
      if (event.acceleration.z > (neutral_7 + rangoGolpe) || event.acceleration.z < (neutral_7 - rangoGolpe))
      {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        controllers[7]->showLeds(gBrightness);
        nuevoRandom = true;
        cuentaPases++;
        ArduinoUno_SoftSerial.print (cuentaPases);
        ArduinoUno_SoftSerial.print ("\n");
      }
    }
  }
}
