#include "panel2.h"
#include <FastLED.h>

extern CRGB leds[];  // Declaración externa del arreglo definido en el .ino
extern CRGB leds2[];  // Declaración externa del arreglo definido en el .ino


void ledsOff() {
  for (int i = 0; i < 338; i++) {
    leds2[i] = CRGB::Black;
  }
  for (int i = 0; i < 241; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void panelOnVerde_1() {

  for (int i = 0; i < 12; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 288; i < 298; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 60; i < 72; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 298; i < 308; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_2() {

  for (int i = 12; i < 24; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 218; i < 228; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 48; i < 60; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 288; i < 298; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_3() {
  for (int i = 24; i < 36; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 208; i < 218; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 36; i < 48; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 218; i < 228; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}

void panelOnVerde_6() {

  for (int i = 60; i < 72; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 278; i < 288; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 72; i < 84; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 308; i < 318; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_7() {
  for (int i = 228; i < 238; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 84; i < 96; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 48; i < 60; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 278; i < 288; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_8() {
  for (int i = 36; i < 48; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 198; i < 208; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 96; i < 108; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 228; i < 238; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}

void panelOnVerde_11() {
  for (int i = 72; i < 84; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 268; i < 278; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 132; i < 144; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 318; i < 328; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_12() {
  for (int i = 84; i < 96; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 238; i < 248; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 120; i < 132; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 268; i < 278; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_13() {
   for (int i = 96; i < 108; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 188; i < 198; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 108; i < 120; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 238; i < 248; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}

void panelOnVerde_16() {
    for (int i = 132; i < 144; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 258; i < 268; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 144; i < 156; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 328; i < 338; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_17() {
  for (int i = 120; i < 132; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 248; i < 258; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 156; i < 168; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 258; i < 268; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}
void panelOnVerde_18() {
  for (int i = 108; i < 120; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 179; i < 188; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 168; i < 179; i++) {
    leds2[i] = CRGB::Blue;
  }

  for (int i = 248; i < 258; i++) {
    leds2[i] = CRGB::Blue;
  }
  FastLED.show();
}

void panelOnAmarillo_1() {

  for (int i = 0; i < 12; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 288; i < 298; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 60; i < 72; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 298; i < 308; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_2() {

  for (int i = 12; i < 24; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 218; i < 228; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 48; i < 60; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 288; i < 298; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_3() {
  for (int i = 24; i < 36; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 208; i < 218; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 36; i < 48; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 218; i < 228; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}

void panelOnAmarillo_6() {

  for (int i = 60; i < 72; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 278; i < 288; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 72; i < 84; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 308; i < 318; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_7() {
  for (int i = 228; i < 238; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 84; i < 96; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 48; i < 60; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 278; i < 288; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_8() {
  for (int i = 36; i < 48; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 198; i < 208; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 96; i < 108; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 228; i < 238; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}

void panelOnAmarillo_11() {
  for (int i = 72; i < 84; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 268; i < 278; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 132; i < 144; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 318; i < 328; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_12() {
  for (int i = 84; i < 96; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 238; i < 248; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 120; i < 132; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 268; i < 278; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_13() {
   for (int i = 96; i < 108; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 188; i < 198; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 108; i < 120; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 238; i < 248; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}

void panelOnAmarillo_16() {
    for (int i = 132; i < 144; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 258; i < 268; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 144; i < 156; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 328; i < 338; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_17() {
  for (int i = 120; i < 132; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 248; i < 258; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 156; i < 168; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 258; i < 268; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}
void panelOnAmarillo_18() {
  for (int i = 108; i < 120; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 179; i < 188; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 168; i < 179; i++) {
    leds2[i] = CRGB::Yellow;
  }

  for (int i = 248; i < 258; i++) {
    leds2[i] = CRGB::Yellow;
  }
  FastLED.show();
}

void panelOnRojo_1() {

  for (int i = 0; i < 12; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 288; i < 298; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 60; i < 72; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 298; i < 308; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_2() {

  for (int i = 12; i < 24; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 218; i < 228; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 48; i < 60; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 288; i < 298; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_3() {
  for (int i = 24; i < 36; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 208; i < 218; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 36; i < 48; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 218; i < 228; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}

void panelOnRojo_6() {

  for (int i = 60; i < 72; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 278; i < 288; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 72; i < 84; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 308; i < 318; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_7() {
  for (int i = 228; i < 238; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 84; i < 96; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 48; i < 60; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 278; i < 288; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_8() {
  for (int i = 36; i < 48; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 198; i < 208; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 96; i < 108; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 228; i < 238; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}

void panelOnRojo_11() {
  for (int i = 72; i < 84; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 268; i < 278; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 132; i < 144; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 318; i < 328; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_12() {
  for (int i = 84; i < 96; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 238; i < 248; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 120; i < 132; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 268; i < 278; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_13() {
   for (int i = 96; i < 108; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 188; i < 198; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 108; i < 120; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 238; i < 248; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}

void panelOnRojo_16() {
    for (int i = 132; i < 144; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 258; i < 268; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 144; i < 156; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 328; i < 338; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_17() {
  for (int i = 120; i < 132; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 248; i < 258; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 156; i < 168; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 258; i < 268; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}
void panelOnRojo_18() {
  for (int i = 108; i < 120; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 179; i < 188; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 168; i < 179; i++) {
    leds2[i] = CRGB::Red;
  }

  for (int i = 248; i < 258; i++) {
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
}