int driverPUL = 7;    // PUL- pin
int driverDIR = 6;    // DIR- pin

int pd = 300;       // Pulse Delay period 2000 - 300 (Velocidad)
int poiints_per_rev = 800; // (Config del driver)
boolean setdir = HIGH; // Set Direction

int points;
int angles;
int obj;
int cur;

void setup() {
  Serial.begin(115200);
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
}

void loop() {
  /*
  if (Serial.available() ) {
    angles = String(Serial.readString()).toInt();
  }*/
  move_to(0);
  delay(1000);
  move_to(90);
  delay(1000);
  move_to(180);
  delay(1000);
}

void move_to(int ang) {
  if (ang >= cur) {
    obj = map(ang, 0, 360, 0, poiints_per_rev);
    if (points != obj) {
      move_motor_f();
    }
    else {
      cur = ang;
    }
  }
  else {
    obj = map(ang, 0, 360, 0, poiints_per_rev);
    if (points != obj) {
      move_motor_b();
    }
    else {
      cur = ang;
    }
  }
}

void move_motor_f() {
  setdir = HIGH;
  digitalWrite(driverDIR, setdir);
  digitalWrite(driverPUL, HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPUL, LOW);
  delayMicroseconds(pd);
  points++;
  if (points > poiints_per_rev) {
    points = 0;
  }
}

void move_motor_b() {
  setdir = LOW;
  digitalWrite(driverDIR, setdir);
  digitalWrite(driverPUL, HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPUL, LOW);
  delayMicroseconds(pd);
  points--;
  if (points < 0) {
    points = poiints_per_rev;
  }
}
