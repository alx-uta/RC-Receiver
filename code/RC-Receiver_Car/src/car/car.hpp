/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#ifndef CAR_HPP
#define CAR_HPP

#include "../config/config.hpp"
#include "../rx/rx.hpp"
#include <ESP32Servo.h>

extern int DEFAULT_SERVO_POSITION;

class Car {
public:
    Car(Config& config, Rx& rx, Servo& ESC, Servo& SERVO);
    void ServoMove(int acceleration);
    void EscMove(int direction);
    void stop();

    int DC_POSITION = 127;
    int DIRECTION = 0;
    int SERVO_POSITION = 90;

 private:
    Config& _config;
    Rx& _rx;
    Servo& _esc;
    Servo& _servo;

    // Used to define if it was already stopped
    bool STOPPED = true;

    // Default Values
    int DEFAULT_VALUE = 127;
    int DEFAULT_SERVO_VALUE = DEFAULT_SERVO_POSITION;
    int DEFAULT_ESC_VALUE = 127;
};

#endif  // CAR_HPP
