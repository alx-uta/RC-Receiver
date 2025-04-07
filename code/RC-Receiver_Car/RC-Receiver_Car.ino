/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

// ESP32 specific files
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

// User Custom Config
#include "user_config.hpp"

// Libraries
#include <SPI.h>
#include <RadioLib.h>
#include <ESP32Servo.h>

// Custom Files
#include "src/rx/rx.hpp"
#include "src/car/car.hpp"

// Init and other files
#include "src/init.hpp"
#include "src/setup.hpp"

/**
* We'll use core:0 to execute specific actions
*/
void func_execute( void * pvParameters ){
    while(true){
        _CAR.ServoMove(_RX.right_joystick_left_right);
        _CAR.EscMove(_RX.left_joystick_up_down);
        feedTheDog();
    }
}

/**
 * We'll use core:1 to receive the data
 */
void func_txReceiveData( void * pvParameters ){
    while(true){
        /**
         * Receive the data
         */
        receiveData();

        /**
         * Feed the dog
         */
        feedTheDog();
    }
}

void setup() {
    delay(500);

    #if ENABLE_SERIAL_PRINT
        Serial.begin(115200);
    #endif

    /**
     * SX1280 setup
     */
    SX1280_setup();

    /**
     * Servo Library Setup
     */
    SERVO_setup();

    xTaskCreatePinnedToCore(
        func_txReceiveData,
        "task_txReceiveData",
        100 * 1024,
        NULL,
        20,
        &task_txReceiveData,
        taskCore1
    );

    xTaskCreatePinnedToCore(
        func_execute,
        "task_updateTFT",
        100 * 1024,
        NULL,
        19,
        &task_updateTFT,
        taskCore2
    );

}

void loop() {
    // nothing to do
}
