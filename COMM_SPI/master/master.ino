const int DATA_PIN = 25;  //25
const int CLK_PIN = 26;   //26

const int BTN = 14;

int estado = 0;
unsigned long anterior;

int puntaje = 0;

void setup() {
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Inicio totem");
}

void loop() {

  switch (estado) {
    case 0:
      if (digitalRead(BTN) == 0) {
        Serial.println("Inicio juego");
        estado = 1;
        digitalWrite(CLK_PIN, HIGH);
        anterior = millis();
      }
      break;

    case 1:
      if (millis() - anterior > 10000) {
        digitalWrite(CLK_PIN, LOW);
        estado = 2;
        Serial.println("Espera puntos");
        receiveByte();
      }
      break;

    case 2:
      

        puntaje = 0;
        estado = 0;
        Serial.println("Fin");
        break;
      }
  }

  void receiveByte() {
    pinMode(CLK_PIN, INPUT_PULLUP);
    while (digitalRead(CLK_PIN) == HIGH) {
      if (digitalRead(DATA_PIN) == HIGH) {
        delay(1);
        if (digitalRead(DATA_PIN) == LOW) {
          puntaje += 1;
        }
      }
    }
    Serial.println("Recibido: " + String(puntaje));
    pinMode(CLK_PIN, OUTPUT);
    digitalWrite(CLK_PIN, LOW);
  }
