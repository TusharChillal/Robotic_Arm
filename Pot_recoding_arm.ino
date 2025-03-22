#include <Servo.h>

// Servo objects
Servo servo1, servo2, servo3, servo4;

// Potentiometer input pins
const int potPins[4] = {A0, A1, A2, A4};

// Button pins
const int buttonRecord = 12;
const int buttonPlay = 13;

// Servo control pins
const int servoPins[4] = {3, 10, 5, 11};

// Number of recorded positions
const int maxPositions = 8;

// Arrays to store saved positions
int servoPositions[4][maxPositions] = {0};

// Track recorded positions
int savedPositions = 0;
bool playbackMode = false;

void setup() {
    Serial.begin(9600);

    // Attach servos
    for (int i = 0; i < 4; i++) {
        servo1.attach(servoPins[0]);
        servo2.attach(servoPins[1]);
        servo3.attach(servoPins[2]);
        servo4.attach(servoPins[3]);
        servo1.write(90);
        servo2.write(90);
        servo3.write(90);
        servo4.write(90);
    }

    // Set button pins as input
    pinMode(buttonRecord, INPUT);
    pinMode(buttonPlay, INPUT);
}

void loop() {
    int potValues[4];

    // Read potentiometer values and move servos accordingly
    for (int i = 0; i < 4; i++) {
        potValues[i] = analogRead(potPins[i]);
        potValues[i] = map(potValues[i], 0, 1023, 10, 180);
    }

    servo1.write(potValues[0]);
    servo2.write(potValues[1]);
    servo3.write(potValues[2]);
    servo4.write(potValues[3]);

    // Check if the record button is pressed
    if (digitalRead(buttonRecord) == HIGH) {
        delay(250);  // Debounce delay
        if (savedPositions < maxPositions) {
            for (int i = 0; i < 4; i++) {
                servoPositions[i][savedPositions] = potValues[i];
            }
            Serial.print("Position #");
            Serial.print(savedPositions + 1);
            Serial.println(" Saved");
            savedPositions++;
        } else {
            Serial.println("Memory Full! Cannot save more positions.");
        }
    }

    // Check if the play button is pressed
    if (digitalRead(buttonPlay) == HIGH) {
        delay(250);  // Debounce delay
        playbackMode = true;
    }

    // Playback recorded positions
    if (playbackMode) {
        for (int i = 0; i < savedPositions; i++) {
            servo1.write(servoPositions[0][i]);
            servo2.write(servoPositions[1][i]);
            servo3.write(servoPositions[2][i]);
            servo4.write(servoPositions[3][i]);
            Serial.print("Playing Position #");
            Serial.println(i + 1);
            delay(2000);
        }
        playbackMode = false;  // Stop playback after one cycle
    }

    delay(100);  // Reduce loop execution speed
}
