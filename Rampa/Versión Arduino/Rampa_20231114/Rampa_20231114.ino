#include <NewPing.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>

// ESTADOS

int state;
#define WAITING_SERIAL 0
#define START_GAME 1
#define GAME 2
#define END_GAME 3
#define END_FULL_GAME 4

// NEOPIXELS

#define NP_PIN_1 22
#define NP_PIN_2 24
#define NP_PIN_3 26
#define NP_PIN_4 28
#define NP_PIN_5 30
#define NP_PIN_6 32
#define NP_PIN_7 34
#define NP_PIN_8 36
#define NP_PIN_9 38
#define NP_PIN_10 40
#define NP_PIN_11 42

#define PIXELS_NUM 33
#define STRING_NUM 11 // CAMBIAR

Adafruit_NeoPixel pixels[STRING_NUM] = {
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_7, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_8, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_9, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_10, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_11, NEO_GRB + NEO_KHZ800)
};
// ULTRASONIDOS

#define US_PIN_1 2                  // AGREGAR
#define US_PIN_2 3
#define US_PIN_3 4
#define US_PIN_4 5
#define US_PIN_5 6
#define US_PIN_6 7
#define US_PIN_7 8
#define US_PIN_8 9
#define US_PIN_9 10
#define US_PIN_10 11
#define US_PIN_11 12

#define MAX_DISTANCE 200
#define SONAR_NUM 11               // CAMBIAR
unsigned int measures[SONAR_NUM];

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
  NewPing(US_PIN_11, US_PIN_11, MAX_DISTANCE)
};

// TIMERS

int timer_1;
int timer_2;
int timer_3;
double timer_4;

// JUEGO

int objective;
int current;
int biggest;
int last;
int error;
int victory;
int puntos = 0;

void setup() {
  Serial.begin(115200);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timer_interrupt);

  for (int i = 0; i < STRING_NUM; i++) {
    pixels[i].begin();
  }

  for (int i = 0; i < STRING_NUM; i++) {
    pixels[i].clear();
    pixels[i].show();
  }

  state = WAITING_SERIAL;
}

void loop() {
  game();
}

void game() {
  switch (state) {
    case WAITING_SERIAL:
      current = -1;
      last = -1;
      biggest = -1;
      if (Serial.available() > 0) {
        String input = Serial.readString();
        // Serial.println(input);
        if (input.toInt() == -1)
        {
          Serial.println(-2);
          state = START_GAME;
          puntos = 0;
          timer_4 = 0;

        }
      }
      //state = START_GAME;

      break;

    case START_GAME:
      for (int i = 0; i < STRING_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }
      // ALGUNA SECUENCIA?

      current = -1;
      last = -1;
      biggest = -1;
      choose_random();
      state = GAME;
      timer_2 = 0;
      break;

    case GAME:

      main_game();
      update_sensors();
      update_neopixels();
      //state=END_FULL_GAME;
      //print_sensors();
      break;

    case END_GAME:
      if (victory == 1) {
        for (int n = 0; n < STRING_NUM; n++) {
          for (int i = 0; i < PIXELS_NUM; i++) {
            pixels[n].setPixelColor(i, pixels[n].Color(0, 150, 0));
            pixels[n].show();
          }
        }

      }
      else {
        for (int n = 0; n < STRING_NUM; n++) {
          for (int i = 0; i < PIXELS_NUM; i++) {
            pixels[n].setPixelColor(i, pixels[n].Color(100, 0, 0));
            pixels[n].show();
          }
        }
      }

      if (timer_3 >= 3000) {
        for (int i = 0; i < STRING_NUM; i++) {
          pixels[i].clear();
          pixels[i].show();
        }
        if (victory == 1) {
          puntos ++;
        }
        state = START_GAME;
        timer_2 = 0;
      }

      if (timer_4 > 9000) {
        state = END_FULL_GAME;
      }
      break;

    case END_FULL_GAME:
      for (int i = 0; i < STRING_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }
      Serial.println(puntos);
      state = WAITING_SERIAL;
      break;
  }
}

void choose_random() {
  randomSeed(analogRead(A0));
  objective = random(STRING_NUM);
  for (int i = 0; i < PIXELS_NUM; i++) {
    pixels[objective].setPixelColor(i, pixels[objective].Color(0, 150, 0));
    pixels[objective].show();
  }
  if (objective + 1 != STRING_NUM) {
    for (int i = 0; i < PIXELS_NUM; i++) {
      pixels[objective + 1].setPixelColor(i, pixels[objective + 1].Color(0, 150, 0));
      pixels[objective + 1].show();
    }
  }
}

void main_game() {
  if (current > biggest) {
    biggest = current;
  }
  if (current < biggest) {
    if (biggest == objective) {
      victory = 1;
      state = END_GAME;
      timer_3 = 0;
    }
    else {
      victory = 0;
      state = END_GAME;
      timer_3 = 0;
    }
  }
  if (timer_2 >= 5000) {
    if (biggest == objective) {
      victory = 1;
      state = END_GAME;
      timer_3 = 0;
    }
    else {
      victory = 0;
      state = END_GAME;
      timer_3 = 0;
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

void update_sensors() {
  if (timer_1 >= 15) {
    for (int i = 0; i < SONAR_NUM; i++) {
      delay(5);
      if (sonar[i].ping_cm() <= 38) {
        //Serial.println(sonar[i].ping_cm(), i);
        measures[i] = 1;
        current = i;
      }
      else {
        measures[i] = 0;
      }
    }
    timer_1 = 0;
  }
}

void update_neopixels() {
  if (current != last) {
    for (int i = 0; i < STRING_NUM; i++) {
      pixels[i].clear();
      pixels[i].show();
    }
    if (current != objective && current - 1 != objective && current + 1 != objective) {
      for (int i = 0; i < PIXELS_NUM; i++) {
        pixels[current].setPixelColor(i, pixels[current].Color(0, 0, 150));
        pixels[current].show();
      }
      if (current + 1 != STRING_NUM) {
        for (int i = 0; i < PIXELS_NUM; i++) {
          pixels[current + 1].setPixelColor(i, pixels[current + 1].Color(0, 0, 150));
          pixels[current + 1].show();
        }
      }
      for (int i = 0; i < PIXELS_NUM; i++) {
        pixels[objective].setPixelColor(i, pixels[objective].Color(0, 150, 0));
        pixels[objective].show();
      }
      if (objective + 1 != STRING_NUM) {
        for (int i = 0; i < PIXELS_NUM; i++) {
          pixels[objective + 1].setPixelColor(i, pixels[objective + 1].Color(0, 150, 0));
          pixels[objective + 1].show();
        }
      }
    }
    else if (current == objective ) {
      for (int i = 0; i < PIXELS_NUM; i++) {
        pixels[current].setPixelColor(i, pixels[current].Color(0, 150, 150));
        pixels[current].show();
      }
      if (current + 1 != STRING_NUM) {
        for (int i = 0; i < PIXELS_NUM; i++) {
          pixels[current + 1].setPixelColor(i, pixels[current + 1].Color(0, 150, 150));
          pixels[current + 1].show();
        }
      }
    }
    else if (current - 1 == objective) { //ACA
      for (int i = 0; i < PIXELS_NUM; i++) {
        pixels[current].setPixelColor(i, pixels[current].Color(0, 150, 150));
        pixels[current].show();
      }
      if (current + 1 != STRING_NUM) {
        for (int i = 0; i < PIXELS_NUM; i++) {
          pixels[current + 1].setPixelColor(i, pixels[current + 1].Color(0, 0, 150));
          pixels[current + 1].show();
        }
      }
      for (int i = 0; i < PIXELS_NUM; i++) {
        pixels[objective].setPixelColor(i, pixels[objective].Color(0, 150, 0));
        pixels[objective].show();
      }
    }
    else if (current + 1 == objective) {
      for (int i = 0; i < PIXELS_NUM; i++) {
        pixels[current].setPixelColor(i, pixels[current].Color(0, 0, 150));
        pixels[current].show();
      }
      if (current + 1 != STRING_NUM) {
        for (int i = 0; i < PIXELS_NUM; i++) {
          pixels[current + 1].setPixelColor(i, pixels[current + 1].Color(0, 150, 150));
          pixels[current + 1].show();
        }
      }
      if (objective + 1 != STRING_NUM) {
        for (int i = 0; i < PIXELS_NUM; i++) {
          pixels[objective + 1].setPixelColor(i, pixels[objective + 1].Color(0, 150, 0));
          pixels[objective + 1].show();
        }
      }
    }

    timer_2 = 0;
    last = current;
  }
}


void timer_interrupt() {
  timer_1 ++;
  timer_2 ++;
  timer_3 ++;
  timer_4 ++;
}
