/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Arduino.h"

class Config {
	public:
		Config();
        /**
         * Generic Pinout
         */
        uint8_t SCK_PIN     = 14;
        uint8_t MISO_PIN    = 12;
        uint8_t MOSI_PIN    = 13;

        /**
         * SX1280 Init
         */
        uint8_t SX1280_NSS  = 2;
        uint8_t SX1280_DI01 = 26;
        uint8_t SX1280_NRST = 5;
        uint8_t SX1280_BUSY = 21;

        float SX1280_FREQUENCY;
	private:
};

#endif  // CONFIG_HPP
