
#include "panel1.h"
#include "panel2.h"

#include <FastLED.h>
#define NUM_LEDS 241
#define NUM_LEDS2 338
#define DATA_PIN 9
#define DATA_PIN2 8
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];
FASTLED_USING_NAMESPACE
#define BRIGHTNESS 255
#define FRAMES_PER_SECOND 6

String dataIn = "";
int dat;
int randomNumber;


const int paneles[20] = {
  33, 31, 29, 53, 51, 27, 25, 23, 49, 47, 2, 3, 4, 45, 43, 5, 6, 7, 41, 39
};



bool statePane1 = LOW;
bool statePane2 = LOW;
bool statePane3 = LOW;
bool statePane4 = LOW;
bool statePane5 = LOW;
bool statePane6 = LOW;
bool statePane7 = LOW;
bool statePane8 = LOW;
bool statePane9 = LOW;
bool statePane10 = LOW;
bool statePane11 = LOW;
bool statePane12 = LOW;
bool statePane13 = LOW;
bool statePane14 = LOW;
bool statePane15 = LOW;
bool statePane16 = LOW;
bool statePane17 = LOW;
bool statePane18 = LOW;
bool statePane19 = LOW;
bool statePane20 = LOW;

int cuentaPuntos = 1;

unsigned long previousMillis = 0;
const uint8_t interval = 500;  // medio segundo
uint8_t contador = 0;
bool esperando = false;

const void* ledsYellow[20] = {
  panelOnAmarillo_1, panelOnAmarillo_2, panelOnAmarillo_3, panelOnAmarillo_4, panelOnAmarillo_5,
  panelOnAmarillo_6, panelOnAmarillo_7, panelOnAmarillo_8, panelOnAmarillo_9, panelOnAmarillo_10,
  panelOnAmarillo_11, panelOnAmarillo_12, panelOnAmarillo_13, panelOnAmarillo_14, panelOnAmarillo_15,
  panelOnAmarillo_16, panelOnAmarillo_17, panelOnAmarillo_18, panelOnAmarillo_19, panelOnAmarillo_20
};

const void* ledsGreen[20] = {
  panelOnVerde_1, panelOnVerde_2, panelOnVerde_3, panelOnVerde_4, panelOnVerde_5,
  panelOnVerde_6, panelOnVerde_7, panelOnVerde_8, panelOnVerde_9, panelOnVerde_10,
  panelOnVerde_11, panelOnVerde_12, panelOnVerde_13, panelOnVerde_14, panelOnVerde_15,
  panelOnVerde_16, panelOnVerde_17, panelOnVerde_18, panelOnVerde_19, panelOnVerde_20
};

const void* ledsRed[20] = {
  panelOnRojo_1, panelOnRojo_2, panelOnRojo_3, panelOnRojo_4, panelOnRojo_5,
  panelOnRojo_6, panelOnRojo_7, panelOnRojo_8, panelOnRojo_9, panelOnRojo_10,
  panelOnRojo_11, panelOnRojo_12, panelOnRojo_13, panelOnRojo_14, panelOnRojo_15,
  panelOnRojo_16, panelOnRojo_17, panelOnRojo_18, panelOnRojo_19, panelOnRojo_20
};


void (*panel)();


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  randomSeed(analogRead(0));

  for (int i = 0; i < 20; i++) {
    pinMode(paneles[i], INPUT_PULLUP);
  }

  // Uncomment/edit one of the following lines for your leds arrangement.
  // ## Clockless types ##

  FastLED.setBrightness(BRIGHTNESS);
  //FastLed
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is assumed

  FastLED.addLeds<WS2811, DATA_PIN2, GRB>(leds2, NUM_LEDS2);  // GRB ordering is assumed

  FastLED.clear();  // clear all pixel data
  FastLED.show();
  Serial.println("Arranque");
}

#define INIT 0
#define START 1
#define WAIT 2

int estado = 0;
int statePanel;


void loop() {

  if (Serial1.available()) {
    dat = Serial1.readString().toInt();
    //cuentaPuntos -= 1;
    if (dat == 3) {
      for (int i = 0; i < 12; i++) {
        Serial1.println(String(cuentaPuntos));
        Serial.println(cuentaPuntos);
        delay(10);
      }

      estado = INIT;
    }
  }



  switch (estado) {
    case INIT:
      standby();
      //secuencia(ledsGreen[10]);
      //delay(3000);
      //getPanel();
      //Serial.println(getPanel()); //23

      //estado = START;

      if (dat == 8) {
        cuentaPuntos = 0;
        estado = START;
        ledsOff();
        Serial.println("ok");
      }
      break;

    case START:

      randomNumber = random(0, 20);
      panel = ledsGreen[randomNumber];
      ledsOff();
      panel();
      Serial.println(randomNumber);
      estado = WAIT;
      break;

    case WAIT:

      statePanel = getPanel();
      Serial.println(statePanel);
      //Serial.println(randomNumber);
      if (statePanel != -1 && statePanel == randomNumber + 1) {
        secuencia(panel);
        cuentaPuntos++;
        estado = START;
      }
      if (statePanel != -1 && statePanel != randomNumber + 1) {
        panel = ledsRed[statePanel - 1];
        secuencia(panel);
        estado = START;
      }
      break;
  }
}

int getPanel() {
  //if(digitalRead())

  if (digitalRead(paneles[0]) == 0) {
    return 1;
  } else if (digitalRead(paneles[1]) == 0) {
    return 2;
  } else if (digitalRead(paneles[2]) == 0) {
    return 3;
  } else if (digitalRead(paneles[3]) == 0) {
    return 4;
  } else if (digitalRead(paneles[4]) == 0) {
    return 5;
  } else if (digitalRead(paneles[5]) == 0) {
    return 6;
  } else if (digitalRead(paneles[6]) == 0) {
    return 7;
  } else if (digitalRead(paneles[7]) == 0) {
    return 8;
  } else if (digitalRead(paneles[8]) == 0) {
    return 9;
  } else if (digitalRead(paneles[9]) == 0) {
    return 10;
  } else if (digitalRead(paneles[10]) == 0) {
    return 11;
  } else if (digitalRead(paneles[11]) == 0) {
    return 12;
  } else if (digitalRead(paneles[12]) == 0) {
    return 13;
  } else if (digitalRead(paneles[13]) == 0) {
    return 14;
  } else if (digitalRead(paneles[14]) == 0) {
    return 15;
  } else if (digitalRead(paneles[15]) == 0) {
    return 16;
  } else if (digitalRead(paneles[16]) == 0) {
    return 17;
  } else if (digitalRead(paneles[17]) == 0) {
    return 18;
  } else if (digitalRead(paneles[18]) == 0) {
    return 19;
  } else if (digitalRead(paneles[19]) == 0) {
    return 20;
  } else {
    return -1;
  }


  //return z + 1;
}

void standby() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    contador += 1;
    //Serial.println(contador);

    if (contador >= 40) {
      contador = 0;
    }
    if (contador % 2 == 0) {

      panel = ledsYellow[random(0, 10)];
      panel();
      panel = ledsYellow[random(10, 20)];
      panel();
    } else {
      ledsOff();
    }
  }
}

void secuencia(void* fun) {

  int count = 0;
  while (count < 4) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 400) {
      //previousMillis = currentMillis;
      panel = fun;
      panel();
      if (currentMillis - previousMillis >= 400 * 2) {
        previousMillis = currentMillis;
        ledsOff();
        count++;
      }
    }
  }
}
