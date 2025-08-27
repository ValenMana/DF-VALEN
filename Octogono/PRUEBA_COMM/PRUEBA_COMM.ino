#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2, A3);  // RX en A2, TX en A3




void setup() {
  // put your setup code here, to run once:
  mySerial.begin(300);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available()) {
    Serial.println(mySerial.readString().toInt());
  }
}
