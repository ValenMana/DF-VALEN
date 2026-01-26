#include "totem.hpp"

Totem::Totem() 
  : _mfrc522(nullptr), _lcd(nullptr), _strip(nullptr),
    _ssPin(0), _rstPin(0), _neoPin(0), _numLeds(0) {}

Totem::~Totem() {
    if (_mfrc522) delete _mfrc522;
    if (_lcd) delete _lcd;
    if (_strip) delete _strip;
}

void Totem::begin(uint8_t ssPin, uint8_t rstPin, uint8_t neoPin, uint8_t numLeds) {
    _ssPin = ssPin;
    _rstPin = rstPin;
    _neoPin = neoPin;
    _numLeds = numLeds;

    // Inicializar RFID
    SPI.begin();
    _mfrc522 = new MFRC522(_ssPin, _rstPin);
    _mfrc522->PCD_Init();
    Serial.println("RFID inicializado.");

    // Inicializar LCD
    _lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    _lcd->init();
    _lcd->backlight();
    _lcd->clear();
    Serial.println("LCD inicializado.");

    // Inicializar NeoPixel
    _strip = new Adafruit_NeoPixel(_numLeds, _neoPin, NEO_GRB + NEO_KHZ800);
    _strip->begin();
    _strip->show(); // todos apagados
    Serial.println("NeoPixel inicializado.");
}

bool Totem::leerTag(String &uid) {
    if (!_mfrc522) return false;

    if (!_mfrc522->PICC_IsNewCardPresent()) return false;
    if (!_mfrc522->PICC_ReadCardSerial()) return false;

    uid = "";
    for (byte i = 0; i < _mfrc522->uid.size; i++) {
        if (_mfrc522->uid.uidByte[i] < 0x10) uid += "0";
        uid += String(_mfrc522->uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    _mfrc522->PICC_HaltA();
    return true;
}

void Totem::print(uint8_t row, bool column, const String &text) {
    if (!_lcd) return;
    _lcd->setCursor(row, column); // corregido: column, row
    _lcd->print(text);
}

void Totem::clearLCD(void) {
    if (_lcd) _lcd->clear();
}

void Totem::setYellow(uint8_t brightness) {
    if (!_strip) return;
    for (uint8_t i = 0; i < _numLeds; i++) {
        _strip->setPixelColor(i, _strip->Color(brightness, brightness, 0));
    }
    _strip->show();
}

void Totem::setGreen(uint8_t brightness) {
    if (!_strip) return;
    for (uint8_t i = 0; i < _numLeds; i++) {
        _strip->setPixelColor(i, _strip->Color(0, brightness, 0));
    }
    _strip->show();
}

void Totem::setRed(uint8_t brightness) {
    if (!_strip) return;
    for (uint8_t i = 0; i < _numLeds; i++) {
        _strip->setPixelColor(i, _strip->Color(brightness, 0, 0));
    }
    _strip->show();
}
