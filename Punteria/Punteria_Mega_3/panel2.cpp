#include "panel2.h"
#include <FastLED.h>

extern CRGB leds[];
extern CRGB leds2[];

#define OFFSET_START 218

// Corrige el desplazamiento desde el LED 218
int fixIndex(int i) {
  if (i >= OFFSET_START) return i - 1;
  return i;
}

// Pinta un rango de LEDs
void setRange(int start, int end, CRGB color) {
  for (int i = fixIndex(start); i < fixIndex(end); i++) {
    leds2[i] = color;
  }
}

// Aplica varios rangos
void panelSet(CRGB color, int ranges[][2], int count) {
  for (int i = 0; i < count; i++) {
    setRange(ranges[i][0], ranges[i][1], color);
  }
  FastLED.show();
}

void ledsOff() {

  for (int i = 0; i < 338; i++) leds2[i] = CRGB::Black;
  for (int i = 0; i < 241; i++) leds[i] = CRGB::Black;

  FastLED.show();
}

CRGB VERDE = CRGB::Blue;
CRGB AMARILLO = CRGB(180,20,255);
CRGB ROJO = CRGB::Red;

void panelOnVerde_1(){
  int r[][2]={{0,12},{288,298},{60,72},{298,308}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_2(){
  int r[][2]={{12,24},{218,228},{48,60},{288,298}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_3(){
  int r[][2]={{24,36},{208,218},{36,48},{218,228}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_6(){
  int r[][2]={{60,72},{278,288},{72,84},{308,318}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_7(){
  int r[][2]={{228,238},{84,96},{48,60},{278,288}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_8(){
  int r[][2]={{36,48},{198,208},{96,108},{228,238}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_11(){
  int r[][2]={{72,84},{268,278},{132,144},{318,328}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_12(){
  int r[][2]={{84,96},{238,248},{120,132},{268,278}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_13(){
  int r[][2]={{96,108},{188,198},{108,120},{238,248}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_16(){
  int r[][2]={{132,144},{258,268},{144,156},{328,338}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_17(){
  int r[][2]={{120,132},{248,258},{156,168},{258,268}};
  panelSet(VERDE,r,4);
}

void panelOnVerde_18(){
  int r[][2]={{108,120},{179,188},{168,179},{248,258}};
  panelSet(VERDE,r,4);
}

/* ================= AMARILLO ================= */

void panelOnAmarillo_1(){
  int r[][2]={{0,12},{289,298},{60,72},{298,308}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_2(){
  int r[][2]={{12,24},{218,228},{48,60},{289,298}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_3(){
  int r[][2]={{24,36},{208,218},{36,48},{218,228}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_6(){
  int r[][2]={{60,72},{279,288},{72,84},{308,318}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_7(){
  int r[][2]={{228,238},{84,96},{48,60},{279,288}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_8(){
  int r[][2]={{36,48},{198,208},{96,108},{228,238}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_11(){
  int r[][2]={{72,84},{268,278},{132,144},{318,328}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_12(){
  int r[][2]={{84,96},{238,248},{120,132},{268,278}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_13(){
  int r[][2]={{96,108},{188,198},{108,120},{238,248}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_16(){
  int r[][2]={{132,144},{258,268},{144,156},{328,338}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_17(){
  int r[][2]={{120,132},{248,258},{156,168},{258,268}};
  panelSet(AMARILLO,r,4);
}

void panelOnAmarillo_18(){
  int r[][2]={{108,120},{179,188},{168,179},{248,258}};
  panelSet(AMARILLO,r,4);
}

/* ================= ROJO ================= */

void panelOnRojo_1(){
  int r[][2]={{0,12},{288,298},{60,72},{298,308}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_2(){
  int r[][2]={{12,24},{218,228},{48,60},{288,298}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_3(){
  int r[][2]={{24,36},{208,218},{36,48},{218,228}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_6(){
  int r[][2]={{60,72},{278,288},{72,84},{308,318}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_7(){
  int r[][2]={{228,238},{84,96},{48,60},{278,288}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_8(){
  int r[][2]={{36,48},{198,208},{96,108},{228,238}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_11(){
  int r[][2]={{72,84},{268,278},{132,144},{318,328}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_12(){
  int r[][2]={{84,96},{238,248},{120,132},{268,278}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_13(){
  int r[][2]={{96,108},{188,198},{108,120},{238,248}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_16(){
  int r[][2]={{132,144},{258,268},{144,156},{328,338}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_17(){
  int r[][2]={{120,132},{248,258},{156,168},{258,268}};
  panelSet(ROJO,r,4);
}

void panelOnRojo_18(){
  int r[][2]={{108,120},{179,188},{168,179},{248,258}};
  panelSet(ROJO,r,4);
}