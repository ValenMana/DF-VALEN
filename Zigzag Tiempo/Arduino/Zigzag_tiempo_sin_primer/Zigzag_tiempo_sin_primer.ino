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
#define NP_PIN_7 35

#define PIXELS_NUM 1
#define STICK_NUM 7

Adafruit_NeoPixel pixels[STICK_NUM] = {
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_7, NEO_GRB + NEO_KHZ800)

};

// ULTRASONIDOS

#define US_PIN_1 22
#define US_PIN_2 24
#define US_PIN_3 26
#define US_PIN_4 28
#define US_PIN_5 30
#define US_PIN_6 32
// #define US_PIN_7 34

#define MAX_DISTANCE 200
#define SONAR_NUM 5

unsigned int measures[SONAR_NUM];

NewPing sonar[SONAR_NUM] = {
  // AGREGAR
  NewPing(US_PIN_1, US_PIN_1, MAX_DISTANCE),
  NewPing(US_PIN_2, US_PIN_2, MAX_DISTANCE),
  NewPing(US_PIN_3, US_PIN_3, MAX_DISTANCE),
  NewPing(US_PIN_4, US_PIN_4, MAX_DISTANCE),
  NewPing(US_PIN_5, US_PIN_5, MAX_DISTANCE),
  //NewPing(US_PIN_6, US_PIN_6, MAX_DISTANCE)
  // NewPing(US_PIN_7, US_PIN_7, MAX_DISTANCE)
};

// TIMERS

int timer_1;
int timer_2;
double timer_3;

// JUEGO

int objective;
int count;
int current;
int last;
int error;
int victory;
int seg;
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

  state = WAITING_SERIAL;
}

void loop() {
  game();
  //stby_neo();
}

void game() {
  switch (state) {
    case WAITING_SERIAL:
      objective = 0;
      count = 0;
      current = -1;
      last = -2;

      stby_neo(); 

      if (Serial.available() > 0) {
        if (Serial.readString().toInt() == -1) {
          state = START_GAME;
          Serial.println(-2);
        }
      }
      break;

    case START_GAME:
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }
      // ALGUNA SECUENCIA?
      state = GAME;
      timer_1 = 0;
      timer_2 = 0;
      timer_3 = 0;
      seg = 0;
      break;

    case GAME:
      main_game();
      update_sensors();
      update_neopixels();
      //print_sensors();
      if (timer_3 >= 90000) {
        state = END_GAME;
      }

      break;

    case END_GAME:
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 0));
        pixels[i].show();
      }
      delay(5000);

      //Serial.println(count);
      double tiempo_jugado = seg;
      delay(30);
      Serial.println(tiempo_jugado);  //TERMINÓ JUEGO
      state = WAITING_SERIAL;
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }
      break;
  }
}

void main_game() {
  if (((SONAR_NUM - 1) * 2 - objective) == -1 && objective >= SONAR_NUM) {
    state = END_GAME;
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

void update_sensors() {  // OK

  if (timer_1 >= 5) {
    //Serial.println(objective);
    for (int i = 0; i < SONAR_NUM; i++) {
      delay(1);
      measures[i] = sonar[i].ping_cm();

      if (measures[i] <= 55 && measures[i] > 0) {
        //measures[i] = 1;
        current = i;
        // Serial.println(current);
      } else {
        //measures[i] = 0;
      }

      if ((objective == 0 || objective == 5) || (objective == 10)) {
        if (objective > SONAR_NUM - 1) {
          current = ((SONAR_NUM - 1) * 2) - objective;
        } else {
          current = objective;
        }
      }
    }
    timer_1 = 0;
  }
}

void update_neopixels() {  // ACA
  if (current != last) {
    for (int i = 0; i < STICK_NUM; i++) {
      pixels[i].clear();
      pixels[i].show();
    }

    if (objective == current && objective < SONAR_NUM) {
      objective = objective + 1;
      count = count + 1;
    } else if (((SONAR_NUM - 1) * 2 - objective) == current && objective >= SONAR_NUM) {
      objective = objective + 1;
    }

    for (int i = 0; i < STICK_NUM; i++) {
      if (i == objective && objective < SONAR_NUM) {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
      } else if (i == objective + 1 && objective < SONAR_NUM) {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
      } else if (i == ((SONAR_NUM - 1) * 2 - objective) && objective >= SONAR_NUM) {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
      } else if (i == ((SONAR_NUM - 1) * 2 - objective) + 1 && objective >= SONAR_NUM) {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
      }
    }
    last = current;
  }
}


void timer_interrupt() {
  timer_1++;
  timer_2++;
  if (timer_2 == 1000) {
    seg++;
    timer_2 = 0;
  }
  timer_3++;
}

void stby_neo() {
  static uint8_t hue = 0;
  static uint8_t fade = 0;
  static bool fadingIn = true;
  static unsigned long lastUpdate = 0;

  const unsigned long intervalo = 4;  // Velocidad de cambio (más alto = más lento)

  if (millis() - lastUpdate >= intervalo) {
    lastUpdate = millis();

    for (int i = 0; i < 6; i++) {
      uint8_t pixelHue = hue + i * 30;
      uint32_t color = pixels[i].ColorHSV(pixelHue * 256, 255, fade);
      pixels[i].setPixelColor(0, color);
      pixels[i].show();
    }

    if (fadingIn) {
      if (fade < 255) {
        fade++;
      } else {
        fadingIn = false;
      }
    } else {
      if (fade > 0) {
        fade--;
      } else {
        fadingIn = true;
        hue++;  // Cambiar de color cuando se apaga del todo
      }
    }
  }
}
