#include <NewPing.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#include <AlmostRandom.h>

const int DATA_PIN = 8;  //sda 8
const int CLK_PIN = 9;


byte data = 0;

// ESTADOS

int state;
#define WAITING_SERIAL 0
#define START_GAME 1
#define GAME 2
#define END_GAME 3

// NEOPIXELS

#define NP_PIN_1 23
#define NP_PIN_2 25
#define NP_PIN_3 27
#define NP_PIN_4 29
#define NP_PIN_5 31
#define NP_PIN_6 33

#define PIXELS_NUM 40
#define STICK_NUM 6

AlmostRandom ar;


Adafruit_NeoPixel pixels[STICK_NUM] = {
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_6, NEO_GRB + NEO_KHZ800)
};

// ULTRASONIDOS

#define US_PIN_1 22
#define US_PIN_2 24
#define US_PIN_3 26
#define US_PIN_4 28
#define US_PIN_5 30
#define US_PIN_6 32
#define US_PIN_7 34
#define US_PIN_8 36
#define US_PIN_9 38
#define US_PIN_10 40
#define US_PIN_11 42
#define US_PIN_12 44

#define MAX_DISTANCE 200
#define SONAR_NUM 12

unsigned int measures[SONAR_NUM];
unsigned int flags[SONAR_NUM];


int medicion;

int estado = -1;
int z = 1;

int puntaje = 0;
int tiempoAnterior;
int randomSelect;
int randomSelectAnterior = 0;

unsigned long timer_1 = 0;

const bool enabled[12] = {
  1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
};

NewPing sonar[SONAR_NUM] = {  // AGREGAR
  NewPing(US_PIN_1, US_PIN_1, MAX_DISTANCE),
  NewPing(US_PIN_2, US_PIN_2, MAX_DISTANCE),
  NewPing(US_PIN_3, US_PIN_3, MAX_DISTANCE),
  NewPing(US_PIN_4, US_PIN_4, MAX_DISTANCE),
  NewPing(US_PIN_5, US_PIN_5, MAX_DISTANCE),
  NewPing(US_PIN_6, US_PIN_6, MAX_DISTANCE),
  NewPing(US_PIN_7, US_PIN_7, MAX_DISTANCE),
  NewPing(US_PIN_8, US_PIN_8, MAX_DISTANCE),
  NewPing(US_PIN_9, US_PIN_9, MAX_DISTANCE),
  NewPing(US_PIN_10, US_PIN_10, MAX_DISTANCE),
  NewPing(US_PIN_11, US_PIN_11, MAX_DISTANCE),
  NewPing(US_PIN_12, US_PIN_12, MAX_DISTANCE)
};

void setup() {
  Serial.begin(9600);

  pinMode(CLK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);

  randomSeed(analogRead(A15));


  Timer1.initialize(1000);
  Timer1.attachInterrupt(timer_interrupt);

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].begin();
  }

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].clear();
    pixels[i].show();
  }

  stby_neo(0, 0, 150);
  delay(500);
  Serial.println("Inicio");
}

String msg;

void loop() {
  //Serial.println(timer_1);
  maquina();
  //aroSelect(5);
}

void maquina() {

  byte received = readByte();

  if (received == 254) {
    estado = 0;
  }

  if (received == 255) {
    sendByte(puntaje);
    estado = 5;
  }

  switch (estado) {

    case -1:

      break;


    case 0:
      estado = 1;
      timer_1 = 0;
      puntaje = 0;
      clear_neo();
      break;

    case 1:
      randomSelect = ar.getRandomByte() % 6;
      Serial.println("Random: " + String(randomSelect));
      Serial.println("Sensores: ");
      Serial.println(randomSelect * 2);
      Serial.println((randomSelect * 2) + 1);
      //delay(1000);

      estado = 4;

      break;

    case 4:
      //delay(1);
      if ((enabled[randomSelect * 2] == 0 && enabled[(randomSelect * 2) + 1] == 0) || randomSelect == randomSelectAnterior) {
        Serial.println("Error, regresando");
        /*
        for (int i = 0; i < STICK_NUM; i++) {
          pixels[i].clear();
          pixels[i].show();
        }*/

        estado = 1;
      } else {
        Serial.println("ok");
        aroSelect(randomSelect);
        delay(20);
        estado = 2;
      }
      break;

    case 2:
      delay(1);

      medicion = sonar[randomSelect * 2].ping_cm();
      Serial.println("midiendo");
      Serial.println(randomSelect * 2);
      Serial.println("Medicion:" + String(medicion));


      if (enabled[(randomSelect * 2)] == 0) {
        estado = 3;
      }
      if (medicion < 35 && medicion != 0) {

        if (enabled[(randomSelect * 2) + 1] == 0) {
          for (int i = 0; i < 4; i++) {
            aroSelect(randomSelect);
            delay(110);
            clear_neo();
            delay(30);
          }
          randomSelectAnterior = randomSelect;
          Serial3.println("9");
          estado = 1;
        } else {
          estado = 3;
        }
      }
      break;

    case 3:
      delay(1);

      medicion = sonar[(randomSelect * 2) + 1].ping_cm();
      Serial.println("midiendo 2");
      Serial.println((randomSelect * 2) + 1);

      if (medicion < 30 && medicion != 0) {


        for (int i = 0; i < 4; i++) {
          aroSelect(randomSelect);
          delay(110);
          clear_neo();
          delay(30);
        }
        Serial3.println("9");
        randomSelectAnterior = randomSelect;
        estado = 1;
        puntaje += 1;
      }

      break;

    case 5:


      estado = -1;
      clear_neo();
      Serial.println("Puntaje" + String(puntaje));
      //delay(1000);
      puntaje = 0;
      stby_neo(0, 0, 150);
      break;
  }
}

void stby_neo(int r, int g, int b) {
  for (int i = 0; i < STICK_NUM; i++) {
    for (int j = 0; j < PIXELS_NUM; j++) {
      pixels[i].setPixelColor(j, pixels[i].Color(r, g, b));
      pixels[i].show();
    }
  }
}

void clear_neo() {

  for (int i = 0; i < STICK_NUM; i++) {
    for (int j = 0; j < PIXELS_NUM; j++) {
      pixels[i].setPixelColor(j, pixels[i].Color(0, 0, 0));
      pixels[i].show();
    }
  }
}

void aroSelect(int aro) {

  for (int j = 0; j < PIXELS_NUM; j++) {
    pixels[aro].setPixelColor(j, pixels[aro].Color(0, 150, 0));
    pixels[aro].show();
  }
}

void timer_interrupt() {
  timer_1++;
}

byte readByte() {
  byte value = 0;
  while (digitalRead(CLK_PIN) == HIGH)
    ;  // Espera LOW

  for (int i = 7; i >= 0; i--) {
    while (digitalRead(CLK_PIN) == LOW)
      ;
    value |= (digitalRead(DATA_PIN) << i);
    while (digitalRead(CLK_PIN) == HIGH)
      ;
  }
  return value;
}

void sendByte(byte data) {
  pinMode(DATA_PIN, OUTPUT);  // cambia dirección del pin

  for (int i = 7; i >= 0; i--) {
    while (digitalRead(CLK_PIN) == LOW)
      ;  // Espera subida
    digitalWrite(DATA_PIN, (data >> i) & 1);
    while (digitalRead(CLK_PIN) == HIGH)
      ;  // Espera bajada
  }

  pinMode(DATA_PIN, INPUT);  // vuelve a modo escucha
}
