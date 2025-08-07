#include <NewPing.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>


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

int estado = 0;
int z = 1;

int puntaje = 0;
int tiempoAnterior;
int randomSelect;

unsigned long timer_1 = 0;

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
  Serial3.begin(9600);

  randomSeed(analogRead(A0));


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
}

String msg;

void loop() {
  //Serial.println(timer_1);

  switch (estado) {
    case 0:

      if (Serial3.available()) {
        msg = Serial3.readStringUntil('\n');
        Serial.println(msg);
        if (msg.toInt() == 8) {
          estado = 1;
          timer_1 = 0;
          clear_neo();
          //Serial.println(-2);
        }
      }
      /*
      while (estado == 0) {
        z += 2;
        Serial.println(z);
        if (sonar[z].ping_cm() < 20 && sonar[z].ping_cm() != 0) {
          Serial.println("Detectado" + String(sonar[z].ping_cm()));
          estado = 1;
          timer_1 = 0;
          clear_neo();
          delay(1);
        } else {
          Serial.println("No detectado");
        }

        if(z > 11){
          z = 1;
        }
        delay(100);
      }
/*
      if (digitalRead(1) == LOW) {

        estado = 1;
        timer_1 = 0;
        clear_neo();
      }*/

      break;

    case 1:
      randomSelect = random(0, 5);

      if (randomSelect == 2 || randomSelect == 0) {
        estado = 1;
      } else {
        aroSelect(randomSelect);
        estado = 2;
      }
      break;

    case 2:

      medicion = sonar[randomSelect * 2].ping_cm();
      delay(50);

      if(Serial3.available()) {
        msg = Serial3.readStringUntil('\n');
        if (msg.toInt() == 3) {
          estado = 0;
          clear_neo();
          Serial.println("Puntaje" + String(puntaje));
          delay(1000);
          Serial3.println(String(puntaje));
          puntaje = 0;
          stby_neo(0, 0, 150);
        }
      }

      if (medicion < 43) {
        estado = 3;
      }
      break;

    case 3:
      medicion = sonar[(randomSelect * 2) + 1].ping_cm();
      delay(50);

      if (medicion < 35) {
        for (int i = 0; i < 4; i++) {
          aroSelect(randomSelect);
          delay(110);
          clear_neo();
          delay(30);
        }
        estado = 1;
        puntaje += 1;
      }
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
