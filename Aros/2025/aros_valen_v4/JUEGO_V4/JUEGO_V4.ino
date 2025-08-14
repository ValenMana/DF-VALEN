#include <Adafruit_NeoPixel.h>
#include <NewPing.h>

const int DATA_PIN = 8;  //sda 8
const int CLK_PIN = 9;

int estado = 0;
int puntaje = 0;

int z = 1;

int medicion;

int tiempoAnterior;
int randomSelect;
int randomSelectAnterior = 0;

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


unsigned int measures[SONAR_NUM];
unsigned int flags[SONAR_NUM];

const bool enabled[12] = {
  1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
};
int randomNum;

int state = 1;

void setup() {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(9600);

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].begin();
  }

  for (int i = 0; i < STICK_NUM; i++) {
    pixels[i].clear();
    pixels[i].show();
  }

  stby_neo(0, 0, 150);


  Serial.println("Inicio mega");
}

void loop() {
  //  randomSeed(analogRead(A5));

  switch (estado) {
    case 0:
      if (digitalRead(CLK_PIN) == HIGH) {
        estado = 1;
        Serial.println("Arranque");
      }
      break;

    case 1:
      //game();
      if (digitalRead(CLK_PIN) == LOW) {
        delay(10);
        Serial.println("Dato enviado: " + String(puntaje));
        estado = 2;
      }
      break;
    case 2:
      delay(50);
      sendByte(puntaje);
      delay(100);
      Serial.println("Fin");
      estado = 0;

      break;
  }
}


void game() {
  switch (state) {
    case 1:
      randomSeed(analogRead(A5));
      randomNum = random(0, 6);
      state = 4;
      break;

    case 4:
      if ((enabled[randomSelect * 2] == 0 && enabled[(randomSelect * 2) + 1] == 0) || randomSelect == randomSelectAnterior) {
        state = 1;
      } else {
        aroSelect(randomSelect);
        state = 2;
      }
      break;

    case 2:
      delay(1);

      medicion = sonar[randomSelect * 2].ping_cm();

      if (enabled[(randomSelect * 2)] == 0) {
        state = 3;
      }
      if (medicion < 35 && medicion != 0) {

        if (enabled[(randomSelect * 2) + 1] == 0) {
          for (int i = 0; i < 4; i++) {
            aroSelect(randomSelect);
            ////delay(110);
            clear_neo();
            //delay(30);
          }
          randomSelectAnterior = randomSelect;
          puntaje += 1;
          state = 1;
        } else {
          state = 3;
        }
      }
      break;

    case 3:
      delay(1);

      medicion = sonar[(randomSelect * 2) + 1].ping_cm();

      if (medicion < 30 && medicion != 0) {
        clear_neo();
        randomSelectAnterior = randomSelect;
        state = 1;
        puntaje += 1;
      }

      break;
  }
}

void sendByte(byte data) {
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, HIGH);
  while (puntaje != 0) {
    digitalWrite(DATA_PIN, HIGH);
    delay(10);
    digitalWrite(DATA_PIN, LOW);
    delay(10);
    puntaje -= 1;
  }
  digitalWrite(CLK_PIN, LOW);
  pinMode(CLK_PIN, INPUT_PULLUP);
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
