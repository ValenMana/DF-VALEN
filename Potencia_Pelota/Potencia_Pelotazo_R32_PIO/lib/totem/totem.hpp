#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS_TOTEM_MAX 50  // límite máximo de LEDs

class Totem {
public:
    Totem();
    ~Totem();

    void begin(uint8_t ssPin, uint8_t rstPin, uint8_t neoPin, uint8_t numLeds);
    bool leerTag(String &uid);
    void print(uint8_t row, bool column, const String &text);
    void clearLCD(void);
    void setGreen(uint8_t brightness);
    void setYellow(uint8_t brightness);
    void setRed(uint8_t brightness);


private:
    MFRC522* _mfrc522;
    LiquidCrystal_I2C* _lcd;
    Adafruit_NeoPixel* _strip;

    uint8_t _ssPin;
    uint8_t _rstPin;
    uint8_t _neoPin;
    uint8_t _numLeds;
};
