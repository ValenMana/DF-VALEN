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
//#define NP_PIN_7 35

#define PIXELS_NUM 1
#define STICK_NUM 6

Adafruit_NeoPixel pixels[STICK_NUM] = {
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(PIXELS_NUM, NP_PIN_6, NEO_GRB + NEO_KHZ800)
  /*,
    Adafruit_NeoPixel (PIXELS_NUM, NP_PIN_7, NEO_GRB + NEO_KHZ800)*/

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
#define SONAR_NUM 6

unsigned int measures[SONAR_NUM];

int medicion;

NewPing sonar[SONAR_NUM] = {
  // AGREGAR
  NewPing(US_PIN_1, US_PIN_1, MAX_DISTANCE),
  NewPing(US_PIN_2, US_PIN_2, MAX_DISTANCE),
  NewPing(US_PIN_3, US_PIN_3, MAX_DISTANCE),
  NewPing(US_PIN_4, US_PIN_4, MAX_DISTANCE),
  NewPing(US_PIN_5, US_PIN_5, MAX_DISTANCE),
  NewPing(US_PIN_6, US_PIN_6, MAX_DISTANCE)
  /*,
    NewPing(US_PIN_7, US_PIN_7, MAX_DISTANCE)*/
};

// TIMERS

int timer_1;
int timer_2;
double timer_3;
uint32_t timer_4;

// JUEGO

int objective;
int count;
int current;
int last;
int error;
int victory;
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

      /*
      for (int i = 0; i < STICK_NUM; i++) {
        if(i>0){
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
        pixels[i-1].clear();
        pixels[i-1].show();
        }else
        {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
        pixels[i+1].clear();
        pixels[i+1].show();
        
        }
        delay(300);
      }

      for (int i = STICK_NUM-1; i > 0; i--) {
        if(i>0){
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
        pixels[i+1].clear();
        pixels[i+1].show();
        }else
        {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
        pixels[i].show();
        pixels[i-1].clear();
        pixels[i-1].show();
        }
      delay(200);
      }*/

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
      break;

    case GAME:
      main_game();
      update_sensors();
      update_neopixels();
      // print_sensors();
      if (timer_3 >= 40000) {
        state = END_GAME;
      }

      break;

    case END_GAME:
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 0));
        pixels[i].show();
      }
      delay(5000);

      Serial.println(count);
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
  if (((SONAR_NUM - 1) * 2 - objective) == -1 && objective >= SONAR_NUM) {
    objective = 0;
    last = -1;
    current = -1;
    //delay(20);
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
      if (i == SONAR_NUM - 1) {
        medicion = 30;
      } else {
        medicion = sonar[i].ping_cm();
      }
      if (medicion <= 60 && medicion > 0) {
        measures[i] = 1;
        //measures[i] = medicion;
        current = i;
        // Serial.println(current);
      } else {
        measures[i] = 0;
        // measures[i] = medicion;
      }
    }
    timer_1 = 0;
  }
}

void update_neopixels() {                // ACA
  for (int i = 0; i < SONAR_NUM; i++) {  // AGREGAR ESTO PARA NO BLOQUEAR
    if (measures[i] == 1) {
      current = i;
    }
    if (current != last) {
      for (int i = 0; i < STICK_NUM; i++) {
        pixels[i].clear();
        pixels[i].show();
      }

      if (objective == current && objective < STICK_NUM) {
        objective = objective + 1;
        count = count + 1;
      } else if (((STICK_NUM - 1) * 2 - objective) == current && objective >= STICK_NUM) {
        objective = objective + 1;
      }

      for (int i = 0; i < STICK_NUM; i++) {
        if (i == objective && objective < STICK_NUM) {
          pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
          pixels[i].show();
        } else if (i == objective + 1 && objective < STICK_NUM) {
          pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
          pixels[i].show();
        } else if (i == ((STICK_NUM - 1) * 2 - objective) && objective >= STICK_NUM) {
          pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
          pixels[i].show();
        } else if (i == ((STICK_NUM - 1) * 2 - objective) + 1 && objective >= STICK_NUM) {
          pixels[i].setPixelColor(0, pixels[i].Color(0, 150, 150));
          pixels[i].show();
        }
      }
      last = current;
    }
  }
}

void timer_interrupt() {
  timer_1++;
  timer_2++;
  timer_3++;
  timer_4++;
}

uint8_t arcoirisHue = 0;  // Variable global para animación
uint8_t contadorHue = 0;

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