#This the incomplete code still working on this 

#include <Servo.h>
#include <math.h>

Servo servo1, servo2, servo3, servo4;

float x = 0, y = 0, z = 0;
float l, theta, phi, a1, a2, k, h, alpha;
int potPin = A0;
float val;

void moveAngle(float theta, float a1, float a2, float val) {
  servo1.write(constrain(theta, 0, 180));
  servo2.write(constrain(a1, 0, 180));
  servo3.write(constrain(a2, 0, 180));
  servo4.write(constrain(val, 0, 180));
}

void moveTo(float x, float y, float z, float val) {
  l = sqrt((x * x) + (y * y));
  theta = atan2(y, x) * 180 / M_PI;  // More stable calculation
  
  h = sqrt((l * l) + (z * z));
  phi = atan2(z, l) * 180 / M_PI;
  
  k = sqrt((7 * 7) + ((h / 2) * (h / 2)));
  alpha = acos(fmax(fmin(h / 14.0, 1.0), -1.0)) * 180 / M_PI;

  a1 = phi + alpha;
  a2 = phi - alpha;

  moveAngle(theta, a1, a2, val);
}

void setup() {
  Serial.begin(115200);

  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A4, INPUT);

  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);

  val = map(analogRead(A0), 0, 1023, 0, 180);
  moveTo(x, y, z, val);
}

void loop() {
  val = map(analogRead(A0), 0, 1023, 0, 180);

  Serial.println("Enter X coordinate: ");
  while (!Serial.available()) {}
  x = Serial.parseFloat();
  Serial.println(x);
  delay(500);

  Serial.println("Enter Y coordinate: ");
  while (!Serial.available()) {}
  y = Serial.parseFloat();
  Serial.println(y);
  delay(500);

  Serial.println("Enter Z coordinate: ");
  while (!Serial.available()) {}
  z = Serial.parseFloat();
  Serial.println(z);
  delay(500);

  moveTo(x, y, z, val);
  delay(500);

  Serial.print("Calculated Arm Length: ");
  Serial.println(l);
}
