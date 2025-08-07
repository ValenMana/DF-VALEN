void setup() {
  Serial.begin(300);   // Monitor serial
  Serial1.begin(300);  // Comunicación con ESP32
  Serial.println("arranque");
}

void loop() {


  Serial1.print("8\n");
  delay(500);
  
}
