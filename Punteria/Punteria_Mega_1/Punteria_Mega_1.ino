char c;
String dataIn = "3";

bool comienzaPunteria = false;  //false
bool arrancaJuego = false;      //false
bool apagaLeds = true;          //true
bool apagaLeds2 = true;         //true


bool avisaEsp32ShowLed = 0;
bool avisaEsp32ShowLed2 = false;
bool esp32AvisaHold = false;
bool flag = false;
bool showLed = true;

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

int pane1 = 33;
int pane2 = 31;
int pane3 = 29;
int pane4 = 53;
int pane5 = 51;
int pane6 = 27;
int pane7 = 25;
int pane8 = 23;
int pane9 = 49;
int pane10 = 47;
int pane11 = 2;
int pane12 = 3;
int pane13 = 4;
int pane14 = 45;
int pane15 = 43;
int pane16 = 5;
int pane17 = 6;
int pane18 = 7;
int pane19 = 41;
int pane20 = 39;

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

int prevRndNum = 123;
bool nuevoRandom = true;
int rndNumb = 0;
int cuentaPuntos = 1;

void setup() {
  Serial.begin(9600);
  Serial1.begin(300);
  randomSeed(analogRead(0));


  pinMode(pane1, INPUT_PULLUP);
  pinMode(pane2, INPUT_PULLUP);
  pinMode(pane3, INPUT_PULLUP);
  pinMode(pane4, INPUT_PULLUP);
  pinMode(pane5, INPUT_PULLUP);
  pinMode(pane6, INPUT_PULLUP);
  pinMode(pane7, INPUT_PULLUP);
  pinMode(pane8, INPUT_PULLUP);
  pinMode(pane9, INPUT_PULLUP);
  pinMode(pane10, INPUT_PULLUP);
  pinMode(pane11, INPUT_PULLUP);
  pinMode(pane12, INPUT_PULLUP);
  pinMode(pane13, INPUT_PULLUP);
  pinMode(pane14, INPUT_PULLUP);
  pinMode(pane15, INPUT_PULLUP);
  pinMode(pane16, INPUT_PULLUP);
  pinMode(pane17, INPUT_PULLUP);
  pinMode(pane18, INPUT_PULLUP);
  pinMode(pane19, INPUT_PULLUP);
  pinMode(pane20, INPUT_PULLUP);

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




void loop() {


  
  if (apagaLeds2 == true) {


    int cicladoPanel = 300;
    int rndNumb2 = random(20) + 1;

    delay(150);

    if (rndNumb2 == 1) {
      panelOnAmarillo_1();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 2) {
      panelOnAmarillo_2();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 3) {
      panelOnAmarillo_3();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 4) {
      panelOnAmarillo_4();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 5) {
      panelOnAmarillo_5();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 6) {
      panelOnAmarillo_6();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 7) {
      panelOnAmarillo_7();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 8) {
      panelOnAmarillo_8();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 9) {
      panelOnAmarillo_9();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 10) {
      panelOnAmarillo_10();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 11) {
      panelOnAmarillo_11();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 12) {
      panelOnAmarillo_12();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 13) {
      panelOnAmarillo_13();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 14) {
      panelOnAmarillo_14();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 15) {
      panelOnAmarillo_15();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 16) {
      panelOnAmarillo_16();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 17) {
      panelOnAmarillo_17();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 18) {
      panelOnAmarillo_18();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 19) {
      panelOnAmarillo_19();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
    if (rndNumb2 == 20) {
      panelOnAmarillo_20();
      delay(cicladoPanel);
      FastLED.clear();  // clear all pixel data
      FastLED.show();
    }
  }

  while (Serial1.available()) {
    c = Serial1.read();
    if (c != '\n') {
      if (c == '8') {
        Serial.println("888");
        arrancaJuego = true;
        comienzaPunteria = true;
        apagaLeds = false;
        apagaLeds2 = false;
      }
      if (dataIn == '3') {
        Serial.println("333");
        apagaLeds = true;
        apagaLeds2 = true;
        comienzaPunteria = false;
        nuevoRandom = false;
      }
      dataIn += c;
    } else {
      break;
    }
  }

  if (c == '\n') {
    Serial.println(dataIn);
    //Serial1.print ("UNO MANDA\n");
    if (dataIn == "8") {
      FastLED.clear();
      FastLED.show();
      dataIn = "123";
      //Serial.println("888");
      arrancaJuego = true;
      comienzaPunteria = true;
      apagaLeds = false;
    }
    if (dataIn == "3") {
      //Serial.println("333");
      apagaLeds = true;
      apagaLeds2 = true;
      comienzaPunteria = false;
      nuevoRandom = false;
    }
    c = 0;
    dataIn = "";
  }


  if (arrancaJuego == true) {
    arrancaJuego = false;
    //Serial.println("ArrancaJuego");
    cuentaPuntos = 1;
    comienzaPunteria = true;
    nuevoRandom = true;
  }

  if (apagaLeds == true) {
    apagaLeds = false;
    FastLED.clear();
    FastLED.show();
    comienzaPunteria = false;
    nuevoRandom = true;
    cuentaPuntos = 1;
  }

  if (comienzaPunteria == true) {  // if (comienzaPunteria == true)
    apagaLeds2 = false;
    if (nuevoRandom == true) {

      nuevoRandom = false;
      rndNumb = random(20) + 1;
      Serial.println(rndNumb);
      FastLED.clear();  // clear all pixel data
      FastLED.show();

      if (rndNumb == 1) {
        panelOnVerde_1();
      }
      if (rndNumb == 2) {
        panelOnVerde_2();
      }
      if (rndNumb == 3) {
        panelOnVerde_3();
      }
      if (rndNumb == 4) {
        panelOnVerde_4();
      }
      if (rndNumb == 5) {
        panelOnVerde_5();
      }
      if (rndNumb == 6) {
        panelOnVerde_6();
      }
      if (rndNumb == 7) {
        panelOnVerde_7();
      }
      if (rndNumb == 8) {
        panelOnVerde_8();
      }
      if (rndNumb == 9) {
        panelOnVerde_9();
      }
      if (rndNumb == 10) {
        panelOnVerde_10();
      }
      if (rndNumb == 11) {
        panelOnVerde_11();
      }
      if (rndNumb == 12) {
        panelOnVerde_12();
      }
      if (rndNumb == 13) {
        panelOnVerde_13();
      }
      if (rndNumb == 14) {
        panelOnVerde_14();
      }
      if (rndNumb == 15) {
        panelOnVerde_15();
      }
      if (rndNumb == 16) {
        panelOnVerde_16();
      }
      if (rndNumb == 17) {
        panelOnVerde_17();
      }
      if (rndNumb == 18) {
        panelOnVerde_18();
      }
      if (rndNumb == 19) {
        panelOnVerde_19();
      }
      if (rndNumb == 20) {
        panelOnVerde_20();
      }


      //    if (rndNumb != prevRndNum ) {
      //      prevRndNum = rndNumb;
      //      nuevoRandom = false;
      //    }
      FastLED.show();
    }

    statePane1 = digitalRead(pane1);
    statePane2 = digitalRead(pane2);
    statePane3 = digitalRead(pane3);
    statePane4 = digitalRead(pane4);
    statePane5 = digitalRead(pane5);
    statePane6 = digitalRead(pane6);
    statePane7 = digitalRead(pane7);
    statePane8 = digitalRead(pane8);
    statePane9 = digitalRead(pane9);
    statePane10 = digitalRead(pane10);
    statePane11 = digitalRead(pane11);
    statePane12 = digitalRead(pane12);
    statePane13 = digitalRead(pane13);
    statePane14 = digitalRead(pane14);
    statePane15 = digitalRead(pane15);
    statePane16 = digitalRead(pane16);
    statePane17 = digitalRead(pane17);
    statePane18 = digitalRead(pane18);
    statePane19 = digitalRead(pane19);
    statePane20 = digitalRead(pane20);




    if (statePane1 == 0) {
      if (rndNumb == 1) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_1();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_1();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_1();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_1();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane2 == 0) {
      if (rndNumb == 2) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_2();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_2();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_2();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_2();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane3 == 0) {
      if (rndNumb == 3) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_3();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_3();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_3();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_3();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane4 == 0) {
      if (rndNumb == 4) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.println(cuentaPuntos);
        nuevoRandom = true;
        panelOnVerde_4();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_4();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_4();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_4();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane5 == 0) {
      if (rndNumb == 5) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_5();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_5();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_5();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_5();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane6 == 0) {
      if (rndNumb == 6) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_6();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_6();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_6();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_6();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane7 == 0) {
      if (rndNumb == 7) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_7();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_7();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_7();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_7();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane8 == 0) {
      if (rndNumb == 8) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_8();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_8();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_8();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_8();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane9 == 0) {
      if (rndNumb == 9) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_9();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_9();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_9();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_9();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane10 == 0) {
      if (rndNumb == 10) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_10();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_10();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_10();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_10();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane11 == 0) {
      if (rndNumb == 11) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_11();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_11();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_11();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_11();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane12 == 0) {
      if (rndNumb == 12) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_12();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_12();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_12();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_12();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane13 == 0) {
      if (rndNumb == 13) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        Serial.print(cuentaPuntos);
        Serial.print("\n");
        nuevoRandom = true;
        panelOnVerde_13();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_13();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_13();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_13();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }


    if (statePane14 == 0) {
      if (rndNumb == 14) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_14();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_14();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_14();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_14();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }


    if (statePane15 == 0) {
      if (rndNumb == 15) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_15();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_15();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_15();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_15();
        delay(1500);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane16 == 0) {
      if (rndNumb == 16) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_16();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_16();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_16();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_16();
        delay(1600);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane17 == 0) {
      if (rndNumb == 17) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_17();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_17();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_17();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_17();
        delay(1700);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane18 == 0) {
      if (rndNumb == 18) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_18();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_18();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_18();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_18();
        delay(1800);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }

    if (statePane19 == 0) {
      if (rndNumb == 19) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_19();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_19();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_19();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_19();
        delay(1900);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    if (statePane20 == 0) {
      if (rndNumb == 20) {
        Serial1.print(cuentaPuntos);
        Serial1.print("\n");
        cuentaPuntos++;
        nuevoRandom = true;
        panelOnVerde_20();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_20();
        delay(300);
        ledsOff();
        delay(300);
        panelOnVerde_20();
        delay(300);
        ledsOff();
        delay(300);
      } else {
        nuevoRandom = true;
        panelOnRojo_20();
        delay(2000);
        ledsOff();
        FastLED.clear();  // clear all pixel data
        FastLED.show();
      }
    }
    apagaLeds2 = false;
  }

  
  /*
  panelOnVerde_1();

  delay(1000);
  ledsOff();

  panelOnVerde_2();
  delay(1000);
  ledsOff();

  panelOnVerde_3();
  delay(1000);
  ledsOff();

  panelOnVerde_6();
  delay(1000);
  ledsOff();

  panelOnVerde_7();
  delay(1000);
  ledsOff();

  panelOnVerde_8();
  delay(1000);
  ledsOff();

  panelOnVerde_11();
  delay(1000);
  ledsOff();

  panelOnVerde_12();
  delay(1000);
  ledsOff();

  panelOnVerde_13();
  delay(1000);
  ledsOff();

  panelOnVerde_16();
  delay(1000);
  ledsOff();

  panelOnVerde_17();
  delay(1000);
  ledsOff();

  panelOnVerde_18();
  delay(1000);
  ledsOff();*/
}