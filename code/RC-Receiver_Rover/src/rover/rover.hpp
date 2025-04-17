/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#ifndef ROVER_HPP
#define ROVER_HPP

#include "../config/config.hpp"
#include "../rx/rx.hpp"
#include <Adafruit_NeoPixel.h>

class Rover {
public:
    Rover(Config& config, Rx& rx, Adafruit_NeoPixel& lights);
    void Stop();
    void Move(int speed, int direction);
    void MoveDirectionForward(int direction, int speed);
    void MoveDirectionBackward(int direction, int speed);
    void StopLeftSide();
    void StopRightSide();
    void Setup();
    void SwitchLightsOnOff();
    void KnightRiderEffect(int brightness);
    void EmergencyLightsEffect(int brightness);
    void PendingConnectionEffect(int brightness);
    uint32_t scaleColorBrightness(uint32_t color, int brightness);

 private:
    Config& _config;
    Rx& _rx;
    Adafruit_NeoPixel& _lights;

    // Used to define if it was already stopped
    bool STOPPED = true;
};

#endif  // ROVER_HPP
