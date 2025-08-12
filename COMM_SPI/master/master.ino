const int DATA_PIN = 25;
const int CLK_PIN  = 26;

void setup() {
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  byte toSend = 42;
  sendByte(toSend);
  delayMicroseconds(500); // espera mínima
  byte response = receiveByte();
  
  Serial.print("Enviado: ");
  Serial.print(toSend);
  Serial.print(" | Recibido: ");
  Serial.println(response);
  
  delay(1000);
}

void sendByte(byte data) {
  pinMode(DATA_PIN, OUTPUT);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(DATA_PIN, (data >> i) & 1);
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(100);
  }
}

byte receiveByte() {
  byte value = 0;
  pinMode(DATA_PIN, INPUT); // cambia dirección del pin
  for (int i = 7; i >= 0; i--) {
    digitalWrite(CLK_PIN, HIGH);
    delayMicroseconds(100);
    value |= (digitalRead(DATA_PIN) << i);
    digitalWrite(CLK_PIN, LOW);
    delayMicroseconds(100);
  }
  return value;
}
