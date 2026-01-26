/*
Version 1.0
Autor Mirko Veckiardo
REACCION codigo ESP32

Juego consiste en tocar la mayor cantidad de botones en un tiempo determinado. Los botones se van encendiendo a mediad que se van aprentado

Ademas si no se presiona un boton por 10seg se pasa al siguente.

*/
/*

i2c para el display
spi para el nfc

Botones pin 36, 35 ,34, 39, 27 ,13 ,16 ,2
neo botones 25, 33, 32, 26,14, 12, 17 ,4

neo pixel pin
el neo pixel q va solo lo desconecte por que necesitba un pin para el rst del RFID

Pines donde funciono o no el neo pixel
//39 35 36 34 1 3 NO funciona el NEO PIXEL // 17 32 33 25 26 14 12 4  OK

*/
#include <Arduino.h>
#include <HardwareSerial.h>

#define VALOR_RANDOM_MAX 8

#define PIN_BOTON_1 36
#define PIN_BOTON_2 35
#define PIN_BOTON_3 34
#define PIN_BOTON_4 39
#define PIN_BOTON_5 27
#define PIN_BOTON_6 13
#define PIN_BOTON_7 16
#define PIN_BOTON_8 2

#define PIN_NEO_BOTON_1 25
#define PIN_NEO_BOTON_2 33
#define PIN_NEO_BOTON_3 32
#define PIN_NEO_BOTON_4 26
#define PIN_NEO_BOTON_5 14
#define PIN_NEO_BOTON_6 12
#define PIN_NEO_BOTON_7 17
#define PIN_NEO_BOTON_8 4

#define ESTADO_ESPERA 10
#define ESTADO_JUGANDO 11
#define ESTADO_FIN 12
#define ESTADO_JUGANDO_NUEVO_VAL 14

int MQ_EST = ESTADO_ESPERA;
int intervalo_pases = 10000;
int intervalo_jugando = 60000;
unsigned long ultimo_milis = 0;
unsigned long ultimo_milis_juego = 0;
unsigned long ultimo_milis_cada_btn = 0;
int intervalo_jugando_por_boton = 10000;

char Val_Nuevo_Random, Val_Viejo_Random;
int cuentaPases = 0;
char Vector_pines_botones[8] = {PIN_BOTON_1, PIN_BOTON_2, PIN_BOTON_3, PIN_BOTON_4, PIN_BOTON_5, PIN_BOTON_6, PIN_BOTON_7, PIN_BOTON_8};
// JUEGO
#define IDJUEGO 10

// RFID
// #define DEBUG
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 15                // Configurable, see typical pin layout above
#define SS_PIN 5                  // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN); // Creamos el objeto para el RC522
String currentID = "";

// 16x02
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// FastLed
#include <FastLED.h>
// #define NUM_LEDS_SOLO 1
// #define PIN_NEO_SOLO 15
#define NUM_LEDS 24
#define NUM_STRIPS 9

CRGB leds_SOLO[1];
CRGB leds[NUM_LEDS];
CLEDController *controllers[NUM_STRIPS];
#define gBrightness 50

FASTLED_USING_NAMESPACE
#define FRAMES_PER_SECOND 120
char flag_neo = 1;

String lecturaDeRfid(void);
String dump_byte_array(byte *buffer, byte bufferSize);

void lecturaDeOctoPases(void);
void Setup_neo();
void neo_Encender_todos();
void neo_Encender_todos_alternado();
void neo_Apagar_todos();
void Lectura_boton(int PIN_BOTON);
void Display_pases();

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_BOTON_1, INPUT);
  pinMode(PIN_BOTON_2, INPUT);
  pinMode(PIN_BOTON_3, INPUT);
  pinMode(PIN_BOTON_4, INPUT);
  pinMode(PIN_BOTON_5, INPUT);
  pinMode(PIN_BOTON_6, INPUT);
  pinMode(PIN_BOTON_7, INPUT);
  pinMode(PIN_BOTON_8, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("DESAFIO!!!!");
  lcd.setCursor(4, 1);
  lcd.print("REACCION !");

  Setup_neo();
  neo_Encender_todos();

  // RFID
  SPI.begin();        // Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522
}

void loop()
{
  int LECTURA_SERIAL;
  int DATO_SERIAL = 8;
  int PASE_ECHOS = 0;
  switch (MQ_EST)
  {
  case ESTADO_ESPERA:
  {
    neo_Encender_todos();
    if (mfrc522.PICC_IsNewCardPresent())
    {
      if (mfrc522.PICC_ReadCardSerial())
      {
        MQ_EST = ESTADO_JUGANDO_NUEVO_VAL;
        delay(50);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PASES A ");
        lcd.setCursor(0, 1);
        lcd.print("LA REDONDA !");
        mfrc522.PICC_HaltA();
        cuentaPases = 0;
        ultimo_milis_juego = millis();
      }
    }
  }
  break;

  case ESTADO_JUGANDO_NUEVO_VAL: // Selecciona un nuevo panel
  {
    Val_Nuevo_Random = random(VALOR_RANDOM_MAX);
    if (Val_Nuevo_Random != Val_Viejo_Random)
    {
      neo_Apagar_todos();
      delay(100);
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      controllers[Val_Nuevo_Random]->showLeds(gBrightness);
      Val_Viejo_Random = Val_Nuevo_Random;
      MQ_EST = ESTADO_JUGANDO;
      Serial.println(Val_Nuevo_Random);
      ultimo_milis_cada_btn = millis();
    }
    else
    {
      delay(100);
    }
  }
  break;

  case ESTADO_JUGANDO:
  {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    controllers[Val_Nuevo_Random]->showLeds(gBrightness);
    Lectura_boton(Vector_pines_botones[Val_Nuevo_Random]);
    if (millis() - ultimo_milis_cada_btn > intervalo_jugando_por_boton)
    {
      ultimo_milis_cada_btn = millis();
      MQ_EST = ESTADO_JUGANDO_NUEVO_VAL;
    }

    if (millis() - ultimo_milis_juego > intervalo_jugando)
    {
      MQ_EST = ESTADO_FIN;
      ultimo_milis = millis();
      Display_pases();
    }
  }
  break;
  case ESTADO_FIN:
  {
    neo_Encender_todos();
    if (millis() - ultimo_milis > intervalo_pases)
    {
      MQ_EST = ESTADO_ESPERA;
      ultimo_milis = millis();
      lcd.init();
      lcd.backlight();
      lcd.setCursor(4, 0);
      lcd.print("DESAFIO!!!!");
      lcd.setCursor(4, 1);
      lcd.print("REACCION !");
    }
  }
  break;
  }
}

void neo_Encender_todos()
{
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  controllers[0]->showLeds(gBrightness);
  controllers[1]->showLeds(gBrightness);
  controllers[2]->showLeds(gBrightness);
  controllers[3]->showLeds(gBrightness);
  controllers[4]->showLeds(gBrightness);
  controllers[5]->showLeds(gBrightness);
  controllers[6]->showLeds(gBrightness);
  controllers[7]->showLeds(gBrightness);
}

void neo_Encender_todos_alternado()
{
  neo_Apagar_todos();
  if (flag_neo == 1)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    controllers[0]->showLeds(gBrightness);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    controllers[4]->showLeds(gBrightness);
  }
  if (flag_neo == 2)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    controllers[1]->showLeds(gBrightness);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    controllers[5]->showLeds(gBrightness);
  }
  if (flag_neo == 3)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    controllers[2]->showLeds(gBrightness);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    controllers[6]->showLeds(gBrightness);
  }
  if (flag_neo == 4)
  {
    flag_neo = 0;
    fill_solid(leds, NUM_LEDS, CRGB::Blue);
    controllers[3]->showLeds(gBrightness);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    controllers[7]->showLeds(gBrightness);
  }
  flag_neo++;
}
void neo_Apagar_todos()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  controllers[0]->showLeds(gBrightness);
  controllers[1]->showLeds(gBrightness);
  controllers[2]->showLeds(gBrightness);
  controllers[3]->showLeds(gBrightness);
  controllers[4]->showLeds(gBrightness);
  controllers[5]->showLeds(gBrightness);
  controllers[6]->showLeds(gBrightness);
  controllers[7]->showLeds(gBrightness);
}

String lecturaDeRfid()
{
  boolean changedValue = false;

  mfrc522.PCD_Init();
  String readRFID = "";
  currentID = readRFID;
  delay(30);

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    readRFID = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    // jugadorN  = readRFID;
    // lcd.setCursor(0, 1);
    // lcd.print(readRFID);
  }

  if (readRFID != currentID)
  {
    changedValue = true;
    currentID = readRFID;
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(1);
}

String dump_byte_array(byte *buffer, byte bufferSize)
{
  String read_rfid = "";

  for (byte i = 0; i < bufferSize; i++)
  {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }

  return read_rfid;
}

void Setup_neo()
{
  FastLED.clear();
  FastLED.show();
  controllers[0] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_1, BRG>(leds, NUM_LEDS);
  controllers[1] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_2, BRG>(leds, NUM_LEDS);
  controllers[2] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_3, BRG>(leds, NUM_LEDS);
  controllers[3] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_4, BRG>(leds, NUM_LEDS);
  controllers[4] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_5, BRG>(leds, NUM_LEDS);
  controllers[5] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_6, BRG>(leds, NUM_LEDS);
  controllers[6] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_7, BRG>(leds, NUM_LEDS);
  controllers[7] = &FastLED.addLeds<WS2812, PIN_NEO_BOTON_8, BRG>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

void Lectura_boton(int PIN_BOTON)
{
  if (digitalRead(PIN_BOTON) == LOW)
  {
    cuentaPases++;
    Display_pases();
    MQ_EST = ESTADO_JUGANDO_NUEVO_VAL;
  }
  delay(50);
}

void Display_pases()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DESAFIO!REACCION");
  lcd.setCursor(0, 1);
  lcd.print("Pases:");
  lcd.setCursor(12, 1);
  lcd.print(cuentaPases);
}