#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno_SoftwareSerial (10, 11);
char c;
String dataIn;
bool esp32Avisa = 0;
bool esp32Avisa2 = false;


bool avisaEsp32ShowLed = 0;

bool avisaEsp32ShowLed2 = false;

bool esp32AvisaHold = false;
bool flag = false;
bool showLed = true;

const int trigPin = 5;
const int echoPin = 6;
long duration;
int distance;


#include <FastLED.h>
#define LED_TYPE2    WS2811_400
#define COLOR_ORDER2 BRG
#define NUM_LEDS2 67
#define DATA_PIN2 9
CRGB leds2[NUM_LEDS2];

#define NUM_LEDS1 8
#define DATA_PIN1 12
CRGB leds1[NUM_LEDS1];

uint8_t gHue = 0;

//HCsr04
const int trigPin1 = 5;
const int trigPin2 = 2;
const int trigPin3 = 3;
const int trigPin4 = 4;
const int echoPin1 = 6;
#define SOUND_SPEED 0.034
long hcsr1;
int distanceCm;
int distanceCm1;
int distanceCm2;
int distanceCm3;
int distanceCm4;
int distanceCmOk;
int saltoEnCm;
int saltoLedFloat;
int saltoEnCmPrev = 0;
int saltoFinalEnCm;

int i = 0;

int iteration = 0;
//cuentas Millis
unsigned long previousHcsr_A = 0;
unsigned long previousGameStart = 0;
const long intervalResetGame = 20000;
const long intervaloTiempoReseteo = 5000; //SE ACABO EL TIEMPO
unsigned long tiempoPrevioParaReseteo = 0;

int saltoLed;
const int alturaDelSensor = 300; //en cm
const int alturaMinima = 55;
int saltoFinal;

bool saltoOk = false;

bool startGame = false;
bool currentGameStartFlag = false;
bool flagTimeOut = false;
bool saltoGrabado = false;
bool tiempoParaReseteo = false;
bool flagTiempoParaReseteo = false;

void setup() {

  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(trigPin4, OUTPUT); // Sets the trigPin as an Output

  //pinMode (12, INPUT);
  pinMode(8, INPUT);

  pinMode(A3, INPUT);
  Serial.begin(19200);
  ArduinoUno_SoftwareSerial.begin(9600);
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds1, NUM_LEDS1);  // GRB ordering is assumed
  FastLED.addLeds<LED_TYPE2, DATA_PIN2, COLOR_ORDER2>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(254);

}

void fadeall() {
  for (int i = 0; i < NUM_LEDS2; i++) {
    leds2[i].nscale8(216);
    //delay(1);
  }
}

void loop() {


  esp32Avisa = digitalRead(8);

  avisaEsp32ShowLed = digitalRead(A3);

  if (esp32Avisa == HIGH) {
    esp32Avisa2 = true;
    avisaEsp32ShowLed2 = false;
    avisaEsp32ShowLed = false;

  }

  if (avisaEsp32ShowLed == HIGH) {
    avisaEsp32ShowLed2 = true;
    esp32Avisa2 = false;
    esp32Avisa = false;
  }


  static uint8_t hue = 0;
  EVERY_N_MILLISECONDS( 20 ) {
    gHue = gHue + 5; // slowly cycle the "base color" through the rainbow
  }

  if (avisaEsp32ShowLed2 == true) {

    leds1[0] = CRGB::Black;
    leds1[1] = CRGB::Black;
    leds1[2] = CRGB::Black;
    leds1[3] = CRGB::Black;
    leds1[4] = CRGB::Black;
    leds1[5] = CRGB::Black;
    leds1[6] = CRGB::Black;
    leds1[7] = CRGB::Black;

    flag = false;
    if (i < NUM_LEDS2) {
      i++;
      leds2[i] = CHSV(hue = hue + 3, 255, 253);
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();

      if (i >= NUM_LEDS2) {
        i = 0;
      }
    }
    delay(6);



  }
  // testear esto :   bool val = (digitalRead(inputPin) == HIGH);







  if (esp32Avisa2 == true) { //true era esp32AvisaHold == true

    if (flag == false) {
      flag = true;

      FastLED.clear();
      FastLED.show();

      leds1[0] = CRGB::Orange;
      leds1[1] = CRGB::Orange;
      leds1[2] = CRGB::Orange;
      leds1[3] = CRGB::Orange;
      leds1[4] = CRGB::Orange;
      leds1[5] = CRGB::Orange;
      leds1[6] = CRGB::Orange;
      leds1[7] = CRGB::Orange;
      FastLED.show();
    }

    lecturaDeSalto();

    ArduinoUno_SoftwareSerial.print (distanceCmOk);
    ArduinoUno_SoftwareSerial.print ("\n");

  }
  // put your main code here, to run repeatedly:

}



void lecturaDeSalto() {

  delay(30);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  digitalWrite(trigPin3, HIGH);
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  digitalWrite(trigPin4, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  hcsr1 = pulseIn(echoPin1, HIGH, 15000);

  // Calculate the distance
  distanceCm = hcsr1 * SOUND_SPEED / 2;

  if (iteration == 1) {
    distanceCm2 = distanceCm;
    if ((((distanceCm2 ) - (distanceCm1 )) < 10) && (((distanceCm2 ) - (distanceCm1 )) > -10))
    {
      distanceCmOk = distanceCm2;
    }
    iteration = 0;
  }
  if (iteration == 0) {
    distanceCm1 = distanceCm;
    //    Serial.print("iteration 0: ");
    //    Serial.println(iteration);
    iteration = 1;
  }


  saltoEnCm = alturaDelSensor - distanceCmOk;

  if (saltoEnCm < 80) {
    saltoOk = false;
  }

  if (saltoEnCm < 270 && saltoEnCm > 80) {


    saltoGrabado = true;
    saltoOk = true;
    saltoLed = map(saltoEnCm, 320, 80, 60, 16);

    if (saltoEnCm > saltoEnCmPrev) {

      saltoEnCmPrev = saltoEnCm;


    }


  }
  if (saltoOk == true) {

    leds2[saltoLed] += CHSV( gHue, 255, 192);

    FastLED.show();


    //  fadeToBlackBy( leds2, saltoLed, 50);
    //  int pos = beatsin16( 8, 0, saltoLed - 0 );
    //leds2[pos] += CHSV( gHue, 255, 192);
    saltoOk = false;
  }

}

// YA ESTA DISTANCEOK MANDANDOSE A ESP
