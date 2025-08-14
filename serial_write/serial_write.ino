#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 9); // RX, TX
#define COMM_PIN 8


void setup() {
  mySerial.begin(4800);
  Serial.begin(9600);
}

void loop() {
  mySerial.write(0x10);

  Serial.println("Bytes enviados...");
  delay(1000);
}
