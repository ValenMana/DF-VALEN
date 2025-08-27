#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <FastLED.h>
#include <AlmostRandom.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2, A3);  // RX en A2, TX en A3
AlmostRandom ar;

#define NUM_LEDS 46
#define NUM_STRIPS 1
CRGB leds[NUM_LEDS];
CLEDController *controllers[8];

#define DELAY_TIME 400
char c;
int dat;

unsigned long previousMillis = 0;
int currentLED = 0;
bool encendiendo = true;
uint8_t randomSelect;
uint8_t randomSelectAnterior = 10;

int currentStrip = 0;
int estado = 0;
int score = 0;
bool movement = false;

#define TCA9548A_ADDR 0x70

float MOVEMENT_THRESHOLD[8] = { 4.0, 4.0, 4.0, 4.0, 4.5, 4.0, 4.0, 4.0 };
float lastAx[8] = { 0 }, lastAy[8] = { 0 }, lastAz[8] = { 0 };

Adafruit_MPU6050 mpu;
bool mpu_initialized[8] = { false };

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mySerial.begin(300);
  estado = 0;
  Setup_neo();
  delay(3000);
}

void loop() {
  //Serial.println(readMPU6050(3));
  
  if(mySerial.available() && estado != 0){
    dat = mySerial.readString().toInt();
    if(dat == 3){
      estado = 0;
    }
  }

  switch (estado) {
    case 0:
      neo_stby();
      while (mySerial.available()) {
        c = mySerial.read();
        if (c != '\n' && c == '8') {
          score = 0;
          estado = 1;
          for (int i = 0; i < 7; i++) stripOff(i);
        }
      }
      break;

    case 1:
      randomSelect = ar.getRandomByte() % 8;
      if (randomSelect != randomSelectAnterior && randomSelect != 3) {
        setStripColorGreen(randomSelect);
        estado = 2;
      }
      delay(10);

      break;

    case 2:
      if (readMPU6050(randomSelect)) {
        if (readMPU6050(randomSelect)) {
          movement = true;
          score++;
          mySerial.println(score);
          estado = 3;
        }
      }
      break;

    case 3:
      movement = false;
      stripOff(randomSelect);
      randomSelectAnterior = randomSelect;
      delay(50);
      estado = 1;
      break;
  }
}

void neo_stby() {
  static unsigned long previousMillis = 0;
  static int currentStrip = 0;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= DELAY_TIME) {
    previousMillis = currentMillis;
    int prevStrip = (currentStrip - 1 + 8) % 8;
    stripOff(prevStrip);
    setStripColorBlue(currentStrip);
    currentStrip = (currentStrip + 1) % 8;
  }
}

void setStripColorGreen(int stripIndex) {
  fill_solid(leds, NUM_LEDS, CRGB::Pink);
  controllers[stripIndex]->showLeds(30);
}

void setStripColorBlue(int stripIndex) {
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  controllers[stripIndex]->showLeds(30);
}

void stripOff(int stripIndex) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  controllers[stripIndex]->showLeds(30);
}

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

bool readMPU6050(uint8_t channel) {
  tcaSelect(channel);

  if (!mpu_initialized[channel]) {
    if (!mpu.begin()) {
      Serial.print("No se detectó MPU6050 en canal ");
      Serial.println(channel);
      return false;
    }
    mpu_initialized[channel] = true;
    Serial.print("MPU6050 inicializado en canal ");
    Serial.println(channel);
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float deltaAx = abs(a.acceleration.x - lastAx[channel]);
  float deltaAy = abs(a.acceleration.y - lastAy[channel]);
  float deltaAz = abs(a.acceleration.z - lastAz[channel]);

  lastAx[channel] = a.acceleration.x;
  lastAy[channel] = a.acceleration.y;
  lastAz[channel] = a.acceleration.z;

  return (deltaAx > MOVEMENT_THRESHOLD[channel] || deltaAy > MOVEMENT_THRESHOLD[channel] || deltaAz > MOVEMENT_THRESHOLD[channel]);
}

void Setup_neo() {
  FastLED.clear();
  FastLED.show();
  controllers[0] = &FastLED.addLeds<WS2812, 2, BRG>(leds, NUM_LEDS);
  controllers[1] = &FastLED.addLeds<WS2812, 3, BRG>(leds, NUM_LEDS);
  controllers[2] = &FastLED.addLeds<WS2812, 5, BRG>(leds, NUM_LEDS);
  controllers[3] = &FastLED.addLeds<WS2812, 6, BRG>(leds, NUM_LEDS);
  controllers[4] = &FastLED.addLeds<WS2812, 8, BRG>(leds, NUM_LEDS);
  controllers[5] = &FastLED.addLeds<WS2812, 9, BRG>(leds, NUM_LEDS);
  controllers[6] = &FastLED.addLeds<WS2812, 11, BRG>(leds, NUM_LEDS);
  controllers[7] = &FastLED.addLeds<WS2812, 12, BRG>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}
