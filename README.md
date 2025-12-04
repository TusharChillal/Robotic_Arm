# Robotic_Arm
Robotic Arm Project

This project is a custom-designed 3-axis robotic arm, engineered primarily from wood and powered by four servo motors. The arm supports both manual and wireless control, making it a versatile test platform for future robotics applications.

Hardware & Control System

MCU: ESP32 (upgraded from Arduino Uno)

Actuators: 4× servo motors

Wireless Control: Xbox controller via BluePad32 library

Power & Body: Wood-based structural frame

Initially, the arm supported record & playback functionality, allowing users to capture and repeat motion sequences. With the transition to the ESP32, the system now features Bluetooth game-controller control for more intuitive operation.
I am actively working on implementing Inverse Kinematics (IK), which will enable more precise and coordinate-based control, rather than direct joint movement.
![WhatsApp Image 2025-03-22 at 18 45 05_b64cd6ca](https://github.com/user-attachments/assets/4ccd9c5f-bcc7-4f4f-b3fe-c331828f6eb3)
