/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#include "car.hpp"

// ESC
extern int ZERO_THROTTLE;
extern int MAX_THROTTLE;
extern int MIN_THROTTLE_REVERSE;
extern int MAX_THROTTLE_REVERSE;

// Servo
extern int MIN_SERVO_POSITION;
extern int MAX_SERVO_POSITION;

// Cutoff period;
extern int CUTOFF_TIME;

Car::Car(
    Config& config, Rx& rx, Servo& ESC, Servo& SERVO
)
    :   _config(config),
        _rx(rx),
        _esc(ESC),
        _servo(SERVO)
        {}


void Car::EscMove(int acceleration) {
    int dcMoveVal = this->DEFAULT_ESC_VALUE;
    int current_time = millis();

    if((
        (current_time - _rx.last_received_time) > CUTOFF_TIME
    ) || (
        acceleration == this->DEFAULT_ESC_VALUE
    )) {
        this->stop();
        return;
    }
    this->STOPPED = false;

    if (acceleration > this->DEFAULT_ESC_VALUE) {
        dcMoveVal = map(acceleration, this->DEFAULT_ESC_VALUE, 255, ZERO_THROTTLE, MAX_THROTTLE);
        this->DIRECTION = 1;
    }

    else if (acceleration < (this->DEFAULT_ESC_VALUE-13)) {
        dcMoveVal = map(acceleration, 0, this->DEFAULT_ESC_VALUE, MIN_THROTTLE_REVERSE, MAX_THROTTLE_REVERSE);

        if(this->DIRECTION==1) {
            Serial.println("STOP!");
            this->stop();
        }

        this->DIRECTION = 0;

    } else {
        dcMoveVal = ZERO_THROTTLE;
        this->DIRECTION = 1;
    }

    if(this->DC_POSITION != dcMoveVal) {
        this->DC_POSITION = dcMoveVal;
        _esc.write(dcMoveVal);
    }

    Serial.print(acceleration);
    Serial.print("->");
    Serial.print(this->DIRECTION);
    Serial.print(":");
    Serial.print(dcMoveVal);
}

void Car::ServoMove(int direction){
    /**
     * Default Value
     */
    int servoPos = this->DEFAULT_SERVO_VALUE;
    int current_time = millis();

    if((current_time - _rx.last_received_time) > CUTOFF_TIME) {
        return;
    }

    /**
     * Right
     */
    if (direction > this->DEFAULT_VALUE) {
        servoPos = map(direction, this->DEFAULT_VALUE, 0, this->DEFAULT_SERVO_VALUE, MIN_SERVO_POSITION);
    }

    /**
     * Left
     */
    if (direction < this->DEFAULT_VALUE) {
        servoPos = map(direction, this->DEFAULT_VALUE, 255, this->DEFAULT_SERVO_VALUE, MAX_SERVO_POSITION);
    }

    _servo.write(servoPos);
    Serial.print("; ");
    Serial.println(servoPos);
}

void Car::stop() {
    if (this->STOPPED) {
        return;
    }
    _esc.write(ZERO_THROTTLE-10);
    delay(45);
    _esc.write(ZERO_THROTTLE);
    delay(35);
    _servo.write(ZERO_THROTTLE);
    this->STOPPED = true;
}