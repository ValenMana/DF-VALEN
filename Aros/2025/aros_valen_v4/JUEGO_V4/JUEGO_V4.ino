#include <Adafruit_NeoPixel.h>
#include <NewPing.h>
#include <SoftwareSerial.h>


#define DATA_PIN 9
#define COMM_PIN 10


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
int estado = 0;


void setup() {
  Serial.begin(9600);

  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, LOW);

  pinMode(COMM_PIN, INPUT);

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
  //aroSelect(0);
  
  maquinaPrincipal();
 // Serial.println(estado);
/*
  if(Serial.readString().toInt() == 2){
    Serial.println("Enviando...");
    digitalWrite(DATA_PIN, HIGH);
    delay(500);
    digitalWrite(DATA_PIN, LOW);
  }*/
  
 //Serial.println(state);

  ///game();
}

void maquinaPrincipal(){
  switch (estado) {
    case 0:
      Serial.println(debounce(COMM_PIN));

      if(debounce(COMM_PIN) == 1){
        clear_neo();
        estado = 1;
        state = 1;
        Serial.println("Arranque");
      }   
      break;

    case 1:
      game();
      if (debounce(COMM_PIN) == LOW) {
        //delay(10);
        //Serial.println("Dato enviado: " + String(puntaje));
        
        tiempoAnterior = millis();
        
        estado = 2;
      }
      
      //estado = 0;
      
      break;
    case 2:
     

      //LOGICA DE ENVIO DE PUNTOS IRIA ACA
     
      Serial.println("Fin");
      stby_neo(0,0,150);
      estado = 0;
      //digitalWrite(DATA_PIN, LOW);

      break;
  }
   
}


void game() {
  switch (state) {
    case 1:
      randomSeed(analogRead(A5));
      randomSelect = random(0, 6);
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
            clear_neo();
            //aroSelect(randomSelect);
          
          randomSelectAnterior = randomSelect;
          puntaje += 1;
          digitalWrite(DATA_PIN, HIGH);
          delay(500);   
          digitalWrite(DATA_PIN, LOW);
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
        digitalWrite(DATA_PIN, HIGH);
        delay(500);
        digitalWrite(DATA_PIN, LOW);
      }

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


bool debounce(int pin) {
  const unsigned long debounceDelay = 50; // Tiempo de rebote en ms

  static int lastButtonState = LOW;
  static int buttonState = LOW;
  static unsigned long lastDebounceTime = 0;

  int reading = digitalRead(pin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  lastButtonState = reading;

  return buttonState;
}

void sendBit(bool bitValue) {
  digitalWrite(DATA_PIN, HIGH);
  delay(bitValue ? 20 : 10); // 20 ms = 1, 10 ms = 0
  digitalWrite(DATA_PIN, LOW);
  delay(10); // pausa entre bits
}

void sendByte(byte value) {
  for (int i = 0; i < 8; i++) {
    sendBit(value & (1 << i));
  }
}
