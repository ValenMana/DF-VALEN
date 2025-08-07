#include <Wire.h>
#include <MPU6050.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define NUM_LEDS 29
#define NUM_LEDS_TOTEM 7

#define LED_PIN 25  //7
#define LED_PIN_TOTEM 26


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

byte bloqueLleno[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

#define SS_PIN 5
#define RST_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

CRGB leds[NUM_LEDS];
CRGB totem[NUM_LEDS_TOTEM];


MPU6050 mpu(0x68);

float umbralImpacto = 1.5;  // Ajustá esto para cambiar sensibilidad (en g)
float baseAccZ = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA = GPIO21, SCL = GPIO22 en ESP32
  FastLED.addLeds<WS2811, LED_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<WS2812, LED_PIN_TOTEM, BRG>(totem, NUM_LEDS_TOTEM).setCorrection(TypicalLEDStrip);

  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init
  lcd.init();

  lcd.createChar(0, bloqueLleno);

  for (int row = 0; row < 2; row++) {
    lcd.setCursor(5, row);  // columna 5, fila 0 y 1
    for (int col = 0; col < 4; col++) {
      lcd.write(byte(0));  // imprimir bloque lleno
    }
  }

  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Error de conexión con el MPU6050. Revisá las conexiones.");
    while (true)
      ;
  }

  // Configurar a ±16g
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);

  // Mostrar rango seteado
  uint8_t rango = mpu.getFullScaleAccelRange();
  Serial.print("Rango configurado: ±");
  Serial.print(2 << rango);  // 2, 4, 8, 16
  Serial.println("g");

  // Estimar aceleración en reposo
  delay(1000);
  baseAccZ = leerAccZg();
  Serial.print("Base Z en reposo (g): ");
  Serial.println(baseAccZ);

  Serial.println("Sensor listo.");

  lcd.backlight();
  lcd.clear();

  setGreen();
  encenderLedsRojo(29);  // Modo espera
}

int data;
int prom = 0;
int estado = 0;

void loop() {
  //espera();

  //fill_rainbow(totem, NUM_LEDS_TOTEM, millis() / 10);
  //FastLED.show();
  //delay(20);

  switch (estado) {
    case 0:
      lcd.setCursor(3, 0);
      lcd.print("DESAFIO");
      lcd.setCursor(5, 1);
      lcd.print("POTENCIA !");
      stby_led();
      if (rfid.PICC_IsNewCardPresent()) {
        //Serial.println("Tarjeta detectada.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     PEGALE");
        lcd.setCursor(0, 1);
        lcd.print("     FUERTE !");
        setYellow();
        estado = 1;
      }
      break;

    case 1:
      data = detect();
      if (data != -1) {

        prom += data;
        Serial.println(data);

        estado = 2;
      }
      break;

    case 2:
      data = detect();
      if (data != -1) {

        prom += data;

        estado = 3;
      }
      break;

    case 3:
      data = detect();
      if (data != -1) {

        prom += data;
        setRed();
        estado = 4;
      }
      break;

    case 4:
      prom /= 3;
      Serial.println(prom);
      encenderLedsRojo(29);  // Modo espera
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Puntos: ");
      lcd.print(prom);
      lcd.print("/16");
      lcd.setCursor(0, 1);
      for (int i = 0; i < prom; i++) {
        lcd.write(byte(255));  // 255 = carácter de bloque lleno (█)
        delay(60);            // Simular animación de carga
      }

      delay(6000);
      lcd.clear();
      prom = 0;
      estado = 0;
      setGreen();



      break;
  }
  //detect();
}

int detect() {
  float pico = detectarImpacto();
  int cantidadLeds = 0;
  int val;

  if (pico > 0) {
    Serial.print("Impacto detectado: ");
    Serial.print(pico);
    Serial.println(" g");

    cantidadLeds = map(pico * 100, 150, 1600, 0, 30);  // 1.5g a 5.0g
    cantidadLeds = constrain(cantidadLeds, 0, 30);

    efectoMartillo(cantidadLeds);
    delay(1500);
    apagarLeds();
    val = pico;

  } else {
    val = -1;
    encenderLedsAzul(29);  // Modo espera
  }

  delay(10);
  return val;
}

float leerAccZg() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  return az / 2048.0;  // Para ±16g => 2048 LSB/g
}

float detectarImpacto() {
  static unsigned long ultimaLectura = 0;

  if (millis() - ultimaLectura < 2000) return -1;

  float maxZ = 0;
  unsigned long t0 = millis();

  while (millis() - t0 < 300) {
    float actualZ = leerAccZg();
    float delta = abs(actualZ - baseAccZ);
    if (delta > maxZ) maxZ = delta;
    delay(5);
  }

  if (maxZ > umbralImpacto) {
    ultimaLectura = millis();
    return maxZ;
  }

  return -1;
}

void efectoMartillo(uint8_t cantidad) {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (uint8_t i = 0; i < cantidad; i++) {
    leds[i] = (i > NUM_LEDS * 0.9) ? CRGB::Red : (i > NUM_LEDS * 0.6) ? CRGB::Yellow
                                                                      : CRGB::Green;
    FastLED.show();
    delay(10);
  }
}

void apagarLeds() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void encenderLedsAzul(uint8_t cantidad) {
  cantidad = constrain(cantidad, 0, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < cantidad; i++)
    leds[i] = CRGB::Blue;
  FastLED.show();
}

void encenderLedsRojo(uint8_t cantidad) {
  cantidad = constrain(cantidad, 0, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < cantidad; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(10);
  }
}

void espera() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    delay(10);
  }


  for (uint8_t i = NUM_LEDS; i > 0; i--) {
    leds[i] = CRGB::Black;
    delay(10);
  }

  FastLED.show();
}

void stby_led() {
  static uint8_t desplazamiento = 0;
  static int8_t direccion = 1;
  static unsigned long ultimoTiempo = 0;
  const uint8_t espera = 10;

  unsigned long ahora = millis();
  if (ahora - ultimoTiempo >= espera) {
    ultimoTiempo = ahora;

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV((i * 256 / NUM_LEDS + desplazamiento) % 256, 255, 255);
    }
    FastLED.show();

    desplazamiento += direccion;
    if (desplazamiento == 255 || desplazamiento == 0) {
      direccion = -direccion;
    }
  }
}

void setGreen() {
  fill_solid(totem, NUM_LEDS_TOTEM, CRGB(0, 0, 255));
  FastLED.show();
}

void setRed() {
  fill_solid(totem, NUM_LEDS_TOTEM, CRGB(255, 0, 0));
  FastLED.show();
}

void setYellow() {
  fill_solid(totem, NUM_LEDS_TOTEM, CRGB(255, 0, 255));
  FastLED.show();
}
