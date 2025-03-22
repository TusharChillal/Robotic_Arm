#include <Servo.h>

// Define servo objects
Servo servo1, servo2, servo3, servo4;

// Define potentiometer pins
const int pot1 = A0;
const int pot2 = A1;
const int pot3 = A2;
const int pot4 = A3;

// Define servo pins
const int servoPin1 = 13;
const int servoPin2 = 14;
const int servoPin3 = 15;
const int servoPin4 = 16;

void setup() {
    // Attach servos to their respective pins
    servo1.attach(servoPin1);
    servo2.attach(servoPin2);
    servo3.attach(servoPin3);
    servo4.attach(servoPin4);
}

void loop() {
    // Read potentiometer values (0-1023 for ESP32 ADC)
    int val1 = analogRead(pot1);
    int val2 = analogRead(pot2);
    int val3 = analogRead(pot3);
    int val4 = analogRead(pot4);

    // Map values from 0-1023 to servo range (0-180 degrees)
    val1 = map(val1, 0, 1023, 0, 180);
    val2 = map(val2, 0, 1023, 0, 180);
    val3 = map(val3, 0, 1023, 0, 180);
    val4 = map(val4, 0, 1023, 0, 180);

    // Move servos
    servo1.write(val1);
    servo2.write(val2);
    servo3.write(val3);
    servo4.write(val4);

    // Small delay for stability
    delay(15);
}
