/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#include "rover.hpp"
#include <Adafruit_NeoPixel.h>

// Cutoff period;
extern int CUTOFF_TIME;
// Define motor control pins for Side A
extern const int MOTOR_LEFT_1_PIN_1;
extern const int MOTOR_LEFT_1_PIN_2;

extern const int MOTOR_LEFT_2_PIN_1;
extern const int MOTOR_LEFT_2_PIN_2;

// Define motor control pins for Side B
extern const int MOTOR_RIGHT_1_PIN_1;
extern const int MOTOR_RIGHT_1_PIN_2;
extern const int MOTOR_RIGHT_2_PIN_1;
extern const int MOTOR_RIGHT_2_PIN_2;

int offset = 10;

// Knight Rider effect
int knightRiderIndex = 7; // Start at the first middle LED
int knightRiderDirection = -1; // 1 for forward, -1 for backward
unsigned long lastKnightRiderUpdate = 0; // Time of the last update
const int knightRiderInterval = 250; // Interval between updates in milliseconds

// Emergency lights effect
unsigned long lastEmergencyLightsUpdate = 0; // Time of the last update
const int emergencyLightsInterval = 150; // Interval between updates in milliseconds
int emergencyLightsPhase = 0; // Tracks the current phase of the effect


Rover::Rover(
    Config& config, Rx& rx, Adafruit_NeoPixel& lights
)
    :   _config(config),
        _rx(rx),
        _lights(lights)
        {}

void Rover::Move(int speed, int direction) {
    int current_time = millis();

    if ((current_time - _rx.last_received_time) > CUTOFF_TIME) {
        Rover::Stop();
        return;
    }

    // Map speed to a range of 0-255 for PWM control
    int mappedSpeed = constrain(map(abs(speed - 127), 0, 127, 0, 255), 0, 255);
    int mappedDirection = constrain(map(abs(direction - 127), 0, 127, 0, 255), 0, 255);

    if(speed >= 127 - offset && speed <= 127 + offset) {
        if (direction < 127) {
            // Move Right by only using the left side motors forward
            Rover::StopRightSide();
            analogWrite(MOTOR_LEFT_1_PIN_1, mappedDirection);
            analogWrite(MOTOR_LEFT_1_PIN_2, 0);
        
            analogWrite(MOTOR_LEFT_2_PIN_1, mappedDirection);
            analogWrite(MOTOR_LEFT_2_PIN_2, 0);
        } else {
            // Move Left by only using the right side motors forward
            Rover::StopLeftSide();
            analogWrite(MOTOR_RIGHT_1_PIN_1, mappedDirection);
            analogWrite(MOTOR_RIGHT_1_PIN_2, 0);
        
            analogWrite(MOTOR_RIGHT_2_PIN_1, mappedDirection);
            analogWrite(MOTOR_RIGHT_2_PIN_2, 0);
        }
        Serial.println();
    } else {
        Serial.print("Direction/Speed While moving");
        Serial.print("[");
        Serial.print(speed);
        Serial.print("]");
        Serial.println();

        if (speed > 127) {
            MoveDirectionForward(direction, mappedSpeed);
        } else {
            MoveDirectionBackward(direction, mappedSpeed);
        }
    }
}

void Rover::Setup() {
    pinMode(MOTOR_LEFT_1_PIN_1, OUTPUT);
    pinMode(MOTOR_LEFT_1_PIN_2, OUTPUT);

    pinMode(MOTOR_LEFT_2_PIN_1, OUTPUT);
    pinMode(MOTOR_LEFT_2_PIN_2, OUTPUT);
    
    pinMode(MOTOR_RIGHT_1_PIN_1, OUTPUT);
    pinMode(MOTOR_RIGHT_1_PIN_2, OUTPUT);
    
    pinMode(MOTOR_RIGHT_2_PIN_1, OUTPUT);
    pinMode(MOTOR_RIGHT_2_PIN_2, OUTPUT);
}

void Rover::Stop() {
    Rover::StopLeftSide();
    Rover::StopRightSide();
    Serial.println("STOPPING");
}

void Rover::StopLeftSide() {
    analogWrite(MOTOR_LEFT_1_PIN_1, 0);
    analogWrite(MOTOR_LEFT_1_PIN_2, 0);

    analogWrite(MOTOR_LEFT_2_PIN_1, 0);
    analogWrite(MOTOR_LEFT_2_PIN_2, 0);
}

void Rover::StopRightSide() {
    analogWrite(MOTOR_RIGHT_1_PIN_1, 0);
    analogWrite(MOTOR_RIGHT_1_PIN_2, 0);

    analogWrite(MOTOR_RIGHT_2_PIN_1, 0);
    analogWrite(MOTOR_RIGHT_2_PIN_2, 0);
}

void Rover::MoveDirectionForward(int direction, int speed) {
    int adjustment = constrain(map(abs(direction - 127), 0, 127, 0, 255), 0, 255);

    int leftSpeed = speed;
    int rightSpeed = speed;

    if (direction > 127) {
        // Turning Right: reduce left motor speed
        leftSpeed = (speed < adjustment) ? 0 : speed - adjustment;

        // Increase the right motor speed to account for the drag
        rightSpeed = constrain(speed + adjustment, 0, 255);

    } else if (direction < 127) {
        // Turning Left: reduce right motor speed
        rightSpeed = (speed < adjustment) ? 0 : speed - adjustment;
        
        // Increase the left motor speed to account for the drag
        leftSpeed = constrain(speed + adjustment, 0, 255);
    }

    Serial.print("MoveDirection");
    Serial.print("[");
    Serial.print(leftSpeed);
    Serial.print("]");
    Serial.print("[");
    Serial.print(rightSpeed);
    Serial.print("]");
    Serial.println();
    // Set motor speeds
    analogWrite(MOTOR_LEFT_1_PIN_1, leftSpeed);
    analogWrite(MOTOR_LEFT_1_PIN_2, 0);

    analogWrite(MOTOR_LEFT_2_PIN_1, leftSpeed);
    analogWrite(MOTOR_LEFT_2_PIN_2, 0);

    analogWrite(MOTOR_RIGHT_1_PIN_1, rightSpeed);
    analogWrite(MOTOR_RIGHT_1_PIN_2, 0);

    analogWrite(MOTOR_RIGHT_2_PIN_1, rightSpeed);
    analogWrite(MOTOR_RIGHT_2_PIN_2, 0);
}

void Rover::MoveDirectionBackward(int direction, int speed) {
    int adjustment = constrain(map(abs(direction - 127), 0, 127, 0, 255), 0, 255);

    int leftSpeed = speed;
    int rightSpeed = speed;

    if (direction > 127) {
        // Turning Right: reduce left motor speed
        leftSpeed = (speed < adjustment) ? 0 : speed - adjustment;
    } else if (direction < 127) {
        // Turning Left: reduce right motor speed
        rightSpeed = (speed < adjustment) ? 0 : speed - adjustment;
    }

    // Set motor speeds
    analogWrite(MOTOR_LEFT_1_PIN_1, 0);
    analogWrite(MOTOR_LEFT_1_PIN_2, leftSpeed);

    analogWrite(MOTOR_LEFT_2_PIN_1, 0);
    analogWrite(MOTOR_LEFT_2_PIN_2, leftSpeed);

    analogWrite(MOTOR_RIGHT_1_PIN_1, 0);
    analogWrite(MOTOR_RIGHT_1_PIN_2, rightSpeed);

    analogWrite(MOTOR_RIGHT_2_PIN_1, 0);
    analogWrite(MOTOR_RIGHT_2_PIN_2, rightSpeed);
}

void Rover::SwitchLightsOnOff() {

    // Map brightness from 0-255
    int pot_1 = constrain(map(_rx.potentiometer_1, 0, 255, 0, 255), 0, 255);
    int pot_2 = constrain(map(_rx.potentiometer_2, 0, 255, 0, 255), 0, 255);
    int pot_3 = constrain(map(_rx.potentiometer_3, 0, 255, 0, 255), 0, 255);
    int pot_4 = constrain(map(_rx.potentiometer_4, 0, 255, 0, 255), 0, 255);

    if (_rx.switch_1 and !_rx.switch_3) { // Only switch lights on if switch_1 is pressed and switch_3 is not active
        // Define colors and brightness for each section
        uint32_t rearRightColor = _lights.Color(255, 0, 0); // Red
        uint32_t rearLeftColor = _lights.Color(255, 0, 0); // Red

        uint32_t frontRightColor = _lights.Color(255, 255, 255); // White
        uint32_t frontLeftColor = _lights.Color(255, 255, 255); // White

        uint32_t frontMiddleColor = _lights.Color(255, 255, 255); // White

        // Set colors and brightness for each section
        for (int i = 0; i < _lights.numPixels(); i++) {
            if (i >= 0 && i <= 2) {
                _lights.setPixelColor(i, scaleColorBrightness(rearRightColor, pot_1));
            } else if (i >= 3 && i <= 4) {
                _lights.setPixelColor(i, scaleColorBrightness(frontRightColor, pot_2));
            } else if (i >= 5 && i <= 10) {
                if(!(_rx.switch_2)) {
                    _lights.setPixelColor(i, scaleColorBrightness(frontMiddleColor, pot_3));
                }
            } else if (i >= 11 && i <= 12) {
                _lights.setPixelColor(i, scaleColorBrightness(frontLeftColor, pot_2));
            } else if (i >= 13 && i <= 16) {
                _lights.setPixelColor(i, scaleColorBrightness(rearLeftColor, pot_1));
            }
        }
        _lights.show();
    } else if (!_rx.switch_3) { // Skip turning off LEDs if Emergency Lights effect is active
        // Turn off all LEDs
        for (int i = 0; i < _lights.numPixels(); i++) {
            if (_rx.switch_2 && i >= 5 && i <= 10) {
                continue; // Skip turning off middle lights if Knight Rider effect is active
            }
            _lights.setPixelColor(i, _lights.Color(0, 0, 0)); // Off
        }
        _lights.show();
    }

    if(_rx.switch_2) {
        Rover::KnightRiderEffect(pot_3);
    }

    if(_rx.switch_3) {
        Rover::EmergencyLightsEffect(pot_4);
    }
}

// Helper function to scale color brightness
uint32_t Rover::scaleColorBrightness(uint32_t color, int brightness) {
    uint8_t r = (uint8_t)((color >> 16) & 0xFF);
    uint8_t g = (uint8_t)((color >> 8) & 0xFF);
    uint8_t b = (uint8_t)(color & 0xFF);

    r = (r * brightness) / 255;
    g = (g * brightness) / 255;
    b = (b * brightness) / 255;

    return _lights.Color(r, g, b);
}

void Rover::KnightRiderEffect(int brightness) {
    unsigned long currentTime = millis();

    // Check if it's time to update the effect
    if (currentTime - lastKnightRiderUpdate >= knightRiderInterval) {
        lastKnightRiderUpdate = currentTime;

        // Turn off all middle LEDs
        for (int i = 5; i <= 10; i++) {
            _lights.setPixelColor(i, _lights.Color(0, 0, 0)); // Off
        }

        // Scale the brightness for the red color
        uint8_t scaledRed = (255 * brightness) / 255; // Scale brightness (0-255)
        uint32_t redColor = _lights.Color(scaledRed, 0, 0); // Red with scaled brightness

        // Set the current LED to the scaled red color
        _lights.setPixelColor(knightRiderIndex, redColor);

        // Update the index for the next LED
        knightRiderIndex += knightRiderDirection;

        // Reverse direction if we reach the ends
        if (knightRiderIndex >= 10) {
            knightRiderDirection = -1; // Reverse to backward
        } else if (knightRiderIndex <= 5) {
            knightRiderDirection = 1; // Reverse to forward
        }

        // Show the updated LEDs
        _lights.show();
    }
}

void Rover::EmergencyLightsEffect(int brightness) {
    unsigned long currentTime = millis();

    // Check if it's time to update the effect
    if (currentTime - lastEmergencyLightsUpdate >= emergencyLightsInterval) {
        lastEmergencyLightsUpdate = currentTime;

        // Scale the brightness for the red and blue colors
        uint8_t scaledRed = (255 * brightness) / 255; // Scale brightness (0-255)
        uint8_t scaledBlue = (255 * brightness) / 255; // Scale brightness (0-255)
        uint32_t redColor = _lights.Color(scaledRed, 0, 0); // Red with scaled brightness
        uint32_t blueColor = _lights.Color(0, 0, scaledBlue); // Blue with scaled brightness
        uint32_t offColor = _lights.Color(0, 0, 0); // Off

        // Update LEDs based on the current phase
        for (int i = 0; i < _lights.numPixels(); i++) {
            if (emergencyLightsPhase == 0) {
                // Phase 0: Front LEDs red, back LEDs blue
                if (i >= 0 && i <= 7) { // Front LEDs
                    _lights.setPixelColor(i, redColor);
                } else if (i >= 8 && i <= 15) { // Back LEDs
                    _lights.setPixelColor(i, blueColor);
                }
            } else if (emergencyLightsPhase == 1) {
                // Phase 1: Alternate red and blue
                if (i % 2 == 0) {
                    _lights.setPixelColor(i, redColor);
                } else {
                    _lights.setPixelColor(i, blueColor);
                }
            } else if (emergencyLightsPhase == 2) {
                // Phase 2: Turn off some LEDs for a flashing effect
                if (i >= 0 && i <= 7) { // Front LEDs
                    _lights.setPixelColor(i, (i % 2 == 0) ? redColor : offColor);
                } else if (i >= 8 && i <= 15) { // Back LEDs
                    _lights.setPixelColor(i, (i % 2 == 0) ? blueColor : offColor);
                }
            }
        }

        // Show the updated LEDs
        _lights.show();

        // Move to the next phase
        emergencyLightsPhase = (emergencyLightsPhase + 1) % 3; // Cycle through phases 0, 1, and 2
    }
}


void Rover::PendingConnectionEffect(int brightness) {
    static unsigned long lastPendingUpdate = 0; // Time of the last update
    static int breathingStep = 0; // Tracks the current step in the breathing effect
    const int breathingInterval = 20; // Interval between updates in milliseconds
    const int breathingSteps = 100; // Total steps for one breathing cycle (0 to max and back)

    unsigned long currentTime = millis();

    // Check if it's time to update the effect
    if (currentTime - lastPendingUpdate >= breathingInterval) {
        lastPendingUpdate = currentTime;

        // Calculate the brightness for the current step
        float phase = (float)breathingStep / breathingSteps * 3.14159; // Map step to 0-π (half sine wave)
        int scaledBrightness = (int)(brightness * (0.5 * (1 - cos(phase)))); // Smooth breathing effect

        // Scale the brightness for the red color
        uint8_t scaledRed = (255 * scaledBrightness) / 255; // Scale brightness (0-255)
        uint32_t redColor = _lights.Color(scaledRed, 0, 0); // Red with scaled brightness

        // Light up all LEDs with the breathing effect
        for (int i = 0; i < _lights.numPixels(); i++) {
            _lights.setPixelColor(i, redColor);
        }

        // Show the updated LEDs
        _lights.show();

        // Update the breathing step
        breathingStep = (breathingStep + 1) % (2 * breathingSteps); // Cycle through 0 to max and back
    }
}
