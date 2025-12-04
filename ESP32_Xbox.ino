#include <ESP32Servo.h>
#include <Bluepad32.h>

// Define servos
Servo servo1, servo2, servo3, servo4;

// Define servo pins
#define SERVO1_PIN 13  // Controlled by RT (Right Trigger)
#define SERVO2_PIN 12  // Controlled by Left Stick X
#define SERVO3_PIN 14  // Controlled by Right Stick X
#define SERVO4_PIN 27  // Controlled by LT (Left Trigger)

// Define joystick dead zone
#define DEADZONE 30  

// Define smooth movement step size
#define SMOOTH_STEP 2  

// Store base positions
int baseAngle1 = 90, baseAngle2 = 90, baseAngle3 = 90, baseAngle4 = 90;
int targetAngle1 = 90, targetAngle2 = 90, targetAngle3 = 90, targetAngle4 = 90;
int currentAngle1 = 90, currentAngle2 = 90, currentAngle3 = 90, currentAngle4 = 90;

// Flags to track LT/RT press state
bool RT_wasPressed = false, LT_wasPressed = false;

// Pointer to connected gamepad
GamepadPtr myGamepad = nullptr;

// Callback function when a gamepad is connected
void onConnectedGamepad(GamepadPtr gp) {
    Serial.println("Gamepad Connected!");
    myGamepad = gp;
}

// Callback function when a gamepad is disconnected
void onDisconnectedGamepad(GamepadPtr gp) {
    Serial.println("Gamepad Disconnected!");
    myGamepad = nullptr;
}

void setup() {
    Serial.begin(115200);
    Serial.println("🔵 Initializing Bluepad32...");

    // Attach servos to ESP32 pins
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo3.attach(SERVO3_PIN);
    servo4.attach(SERVO4_PIN);

    // Set servos to default position (90 degrees)
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);

    // Initialize Bluepad32
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    Serial.println("🎮 Waiting for Bluetooth Gamepad...");
}

void loop() {
    BP32.update();  // Update Bluepad32 state

    if (myGamepad) {
        // Read button states
        uint16_t buttons = myGamepad->buttons();
        bool RB_pressed = buttons & BUTTON_SHOULDER_R;  // Right bumper (RB)

        // Read joystick values
        int joyX = myGamepad->axisX();  // Left Stick X
        int joyRX = myGamepad->axisRX(); // Right Stick X

        // Read Trigger values
        int RT = myGamepad->throttle();  // RT (Right Trigger)
        int LT = myGamepad->brake();     // LT (Left Trigger)

        // Apply dead zone to joysticks
        if (abs(joyX) < DEADZONE) joyX = 0;
        if (abs(joyRX) < DEADZONE) joyRX = 0;

        // Map values to target servo angles
        targetAngle1 = baseAngle1 + map(RT, 0, 1023, -45, 45);  // RT controls Servo1
        targetAngle4 = baseAngle4 + map(LT, 0, 1023, -45, 45);  // LT controls Servo4
        targetAngle2 = baseAngle2 + map(joyX, -512, 512, -45, 45); // Left Stick X → Servo2
        targetAngle3 = baseAngle3 + map(joyRX, -512, 512, -45, 45); // Right Stick X → Servo3

        // Constrain angles to stay within bounds
        targetAngle1 = constrain(targetAngle1, 0, 180);
        targetAngle2 = constrain(targetAngle2, 0, 180);
        targetAngle3 = constrain(targetAngle3, 0, 180);
        targetAngle4 = constrain(targetAngle4, 0, 180);

        // If RB is pressed, set new base positions
        if (RB_pressed) {
            baseAngle1 = targetAngle1;
            baseAngle2 = targetAngle2;
            baseAngle3 = targetAngle3;
            baseAngle4 = targetAngle4;
            Serial.println("New base position set!");
        }

        // Detect if RT is released and set new base position
        if (RT > 10) {
            RT_wasPressed = true;
        } else if (RT_wasPressed) {
            baseAngle1 = currentAngle1;  // Save current RT-based position
            RT_wasPressed = false;
            Serial.println("RT released - New base position for Servo1");
        }

        // Detect if LT is released and set new base position
        if (LT > 10) {
            LT_wasPressed = true;
        } else if (LT_wasPressed) {
            baseAngle4 = currentAngle4;  // Save current LT-based position
            LT_wasPressed = false;
            Serial.println("LT released - New base position for Servo4");
        }

        // Gradually move current angles to target angles for smooth movement
        currentAngle1 += (targetAngle1 - currentAngle1) / SMOOTH_STEP;
        currentAngle2 += (targetAngle2 - currentAngle2) / SMOOTH_STEP;
        currentAngle3 += (targetAngle3 - currentAngle3) / SMOOTH_STEP;
        currentAngle4 += (targetAngle4 - currentAngle4) / SMOOTH_STEP;

        // Write smooth angles to servos
        servo1.write(currentAngle1);
        servo2.write(currentAngle2);
        servo3.write(currentAngle3);
        servo4.write(currentAngle4);

        // If Button A is pressed, reset all servos to 90 degrees
        if (buttons & BUTTON_A) {
            Serial.println("Resetting Servos to 90°");
            baseAngle1 = baseAngle2 = baseAngle3 = baseAngle4 = 90;
            targetAngle1 = targetAngle2 = targetAngle3 = targetAngle4 = 90;
            currentAngle1 = currentAngle2 = currentAngle3 = currentAngle4 = 90;
            servo1.write(90);
            servo2.write(90);
            servo3.write(90);
            servo4.write(90);
        }

        // Debug output
        Serial.printf("Servo Angles: %d, %d, %d, %d | RT: %d | LT: %d | RT Pressed: %d | LT Pressed: %d\n", 
                      currentAngle1, currentAngle2, currentAngle3, currentAngle4, RT, LT, RT_wasPressed, LT_wasPressed);
    }

    delay(15); // Small delay for stability
}
