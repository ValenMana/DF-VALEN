void setup() {
  Serial.begin(300);             // Monitor serial
  //Serial2.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
}

void loop() {
  //Serial2.println("Hola desde el ESP32");
  Serial.println(456);
  delay(1000);
}
