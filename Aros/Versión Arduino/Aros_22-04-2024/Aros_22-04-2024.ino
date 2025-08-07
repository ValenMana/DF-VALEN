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
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_6, NEO_GRB + NEO_KHZ800)
};

// ULTRASONIDOS

#define US_PIN_1 22
#define US_PIN_2 24
#define US_PIN_3 26
#define US_PIN_4 28
#define US_PIN_5 30
#define US_PIN_6 32
#define US_PIN_7 48
#define US_PIN_8 46
#define US_PIN_9 38
#define US_PIN_10 40
#define US_PIN_11 42
#define US_PIN_12 44

#define MAX_DISTANCE 200
#define SONAR_NUM 12

unsigned int measures[SONAR_NUM];
unsigned int flags[SONAR_NUM];


int medicion;

NewPing sonar[SONAR_NUM] = {        // AGREGAR
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


// TIMERS

int timer_1;
int timer_2;
double timer_3;
double timer_4;

// JUEGO

int objective;
int count;
int current;
int last;
int error;
int victory;


int score = 0;

unsigned int control[STICK_NUM];

void setup() {
  Serial.begin(115200);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timer_interrupt);

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].begin();
  }

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].clear();
    pixels[i].show();
  }

 
 /*for (int i=0;i<PIXELS_NUM;i++){
   pixels[0].setPixelColor(i, pixels[0].Color(0, 150, 0));
   pixels[0].show();
 }*/

  state = WAITING_SERIAL;

}

void loop() {
  game();

}

void game() {
  switch (state) {
    case WAITING_SERIAL:
      count = 0;
      if (Serial.available() > 0) {
        if (Serial.readString().toInt() == -1) {
          state = START_GAME;
          Serial.println(-2);
        }
      }
      for (int i=0;i<STICK_NUM;i++){
        for(int j=0;j<PIXELS_NUM;j++){
        pixels[i].setPixelColor(j, pixels[i].Color(0, 0, 150));
        pixels[i].show();
        }
      }
      break;

    case START_GAME:
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }
     
      state = GAME;
      score = 0;
      timer_1 = 0;
      timer_2 = 0;
      timer_3 = 0;
      timer_4 = 0;
      break;

    case GAME:
      main_game();
      update_sensors();
     // update_neopixels();
      //print_sensors();
      if (timer_4  >= 50000) {
        state = END_GAME;
      }

      break;

    case END_GAME:
      for (int i=0;i<STICK_NUM;i++){
        for(int j=0;j<PIXELS_NUM;j++){
        pixels[i].setPixelColor(j, pixels[i].Color(0, 0, 150));
        pixels[i].show();
        }
      }
      delay(5000);

      Serial.println(score);
      // float tiempo_jugado = timer_2 / 1000;
      delay(30);
      // Serial.println(tiempo_jugado);//TERMINÓ JUEGO
      state = WAITING_SERIAL;
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }
      break;
  }
}

void main_game() {
  for (int i = 0; i < SONAR_NUM; i++) {
    if (i % 2 == 0 && measures[i] == 1) {
      flags[i] = 1;
      timer_3 = 0;
    } else if ((i % 2 != 0 && measures[i] == 1) && (flags[i - 1] == 1 && measures[i - 1] == 0)) {
      score++;
      flags[i] = 0;
      flags[i - 1] = 0;
      //Serial.println("+1 punto");
      showLights(i/2);
    }
  }
  if (timer_3 > 1000) {
    for (int i = 0; i < SONAR_NUM; i++) {
      flags[i] = 0;
    }
  }
}

void print_sensors() {
  for (int i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print(":  ");
    Serial.print(measures[i]);
    Serial.print("   ");
  }
  Serial.println("  ");
}

/*void update_sensors() { // OK
  if (timer_1 >= 50) {
    //Serial.println(objective);
    for (int i = 0; i < SONAR_NUM; i++) {
      delay(2);
      medicion = sonar[i].ping_cm();
      measures[i] = medicion;
    }
    timer_1 = 0;
  }
}*/

void update_sensors() {
  int measure;
  if (timer_1 >= 3) {
    for (int i = 0; i < SONAR_NUM; i++) {
      //delay(3);
      measure=sonar[i].ping_cm();
      //measures[i] = measure;
      if (measure <= 43 && measure >0) {
        //Serial.println(sonar[i].read(), i);
        measures[i] = 1;
      } else {
        measures[i] = 0;
      }
    }
    timer_1 = 0;
  }
}



void timer_interrupt() {
  timer_1 ++;
  timer_2 ++;
  timer_3 ++;
  timer_4 ++;
}

void showLights(int aro){ 
 for (int i=0;i<PIXELS_NUM;i++){
   pixels[aro].setPixelColor(i, pixels[aro].Color(0, 150, 0));
   pixels[aro].show();
 }
  delay(500);
  pixels[aro].clear();
  pixels[aro].show();
}
