#include <AccelStepper.h>


volatile int counter = 0;             // Variable del contador
const int pulsesPerRevolution = 400;  // Pulsos del encoder por vuelta
int angle;

AccelStepper stepper(1, 5, 4);  // Configurar motor en modo de pulso y dirección


void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);


  stepper.setMaxSpeed(1000);  // Velocidad máxima del motor (ajustar según sea necesario)
  stepper.setAcceleration(5000);
}

void loop() {
  // Mantener el contador dentro del rango de una revolución
  if (counter >= pulsesPerRevolution) {
    counter = 0;  // Reinicia a 0 al completar 360°
  } else if (counter < 0) {
    counter = pulsesPerRevolution - 1;  // Si va en sentido contrario, regresa a 359°
  }

  // Mapear el contador a grados
  angle = map(counter, 0, pulsesPerRevolution, 0, 360);

  //Serial.println(angle);
  //delay(10);
  stepper.setSpeed(1000);
  stepper.runSpeed();

  //moveToAngle(Serial.readString().toInt());
  /*
  moveToAngle(0);
  delay(1000);
  moveToAngle(90);
  delay(1000);
  moveToAngle(180);
  delay(1000);
  moveToAngle(270);
  delay(1000);*/
}

void ai0() {
  if (digitalRead(2) == LOW) {
    counter--;
  } else {
    counter++;
  }
}

void ai1() {
  if (digitalRead(3) == LOW) {
    counter++;
  } else {
    counter--;
  }
}

void moveToAngle(int targetAngle) {
  // Convertir el ángulo en pasos del motor
  int targetSteps = map(targetAngle, 0, 360, 0, -pulsesPerRevolution);

  // Mover el motor a la posición deseada
  stepper.moveTo(targetSteps);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  stepper.setSpeed(0);  // Pone la velocidad en 0
  stepper.runSpeed();

  /* Serial.print("Movido a ");
  Serial.print(targetAngle);
  Serial.println(" grados");*/
}
