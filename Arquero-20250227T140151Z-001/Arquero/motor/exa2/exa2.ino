#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(1, 7, 6);

void setup() {
  Serial.begin(115200);
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(3000);
}

void loop() {
  //stepper.moveTo(stepper.currentPosition() + Serial.readString().toInt());
  //stepper.runToPosition();

  if (Serial.available()) {
    //Serial.println(Serial.readString().toInt());

    stepper.moveTo(stepper.currentPosition() + Serial.readString().toInt());
    stepper.runToPosition();
  }

  /*
  stepper.moveTo(0);
  stepper.runToPosition();
  delay(1000);
  stepper.moveTo(90);
  stepper.runToPosition();
  delay(1000);

  stepper.moveTo(180);
  stepper.runToPosition();
  delay(1000);*/
}
