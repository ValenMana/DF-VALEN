const int DATA_PIN = 9;  //sda 8
const int CLK_PIN = 8;

int estado = 0;
int puntaje = 0;

void setup() {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("Inicio mega");
}

void loop() {
  randomSeed(analogRead(A5));

  switch (estado) {
    case 0:
      if (digitalRead(CLK_PIN) == HIGH) {
        estado = 1;
        Serial.println("Arranque");
      }
      break;

    case 1:
      if (digitalRead(CLK_PIN) == LOW) {
        delay(10);
        puntaje = random(0, 255);
        Serial.println("Dato enviado: " + String(puntaje));
        estado = 2;
      }
      break;
    case 2:
      delay(50);
      sendByte(puntaje);
      delay(100);
      Serial.println("Fin");
      estado = 0;

      break;
  }
}

void sendByte(byte data) {
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, HIGH);
  while (puntaje != 0) {
    digitalWrite(DATA_PIN, HIGH);
    delay(10);
    digitalWrite(DATA_PIN, LOW);
    delay(10);
    puntaje -= 1;
  }
  digitalWrite(CLK_PIN, LOW);
  pinMode(CLK_PIN, INPUT_PULLUP);
}
