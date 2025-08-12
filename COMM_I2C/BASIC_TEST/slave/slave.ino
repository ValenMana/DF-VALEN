const int DATA_PIN = 8; //sda 8
const int CLK_PIN  = 9;

void setup() {
  pinMode(CLK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  byte received = readByte();
  Serial.print("Recibido: ");
  Serial.println(received);

  byte toSend = received + 1;
  sendByte(toSend);
  delay(100); // Espera para estabilidad
}

byte readByte() {
  byte value = 0;
  while (digitalRead(CLK_PIN) == HIGH); // Espera LOW
  
  for (int i = 7; i >= 0; i--) {
    while (digitalRead(CLK_PIN) == LOW);
    value |= (digitalRead(DATA_PIN) << i);
    while (digitalRead(CLK_PIN) == HIGH);
  }
  return value;
}

void sendByte(byte data) {
  pinMode(DATA_PIN, OUTPUT); // cambia dirección del pin

  for (int i = 7; i >= 0; i--) {
    while (digitalRead(CLK_PIN) == LOW); // Espera subida
    digitalWrite(DATA_PIN, (data >> i) & 1);
    while (digitalRead(CLK_PIN) == HIGH); // Espera bajada
  }

  pinMode(DATA_PIN, INPUT); // vuelve a modo escucha
}
