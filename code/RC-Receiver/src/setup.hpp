/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

/**
 * SX1280
 */
void SX1280_setup() {
    int state = radio.beginGFSK();
    #if ENABLE_SERIAL_PRINT
        #if ENABLE_RADIO_LIB_DEBUG
            if (state == RADIOLIB_ERR_NONE) {
                Serial.println(F("success!"));
            } else {
                Serial.print(F("failed, code "));
                Serial.println(state);
                while (true);
            }
        #endif
    #endif

    state = radio.setOutputPower(SX1280_OUTPUT_POWER);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setOutputPower:");Serial.println(state);
    #endif

    state = radio.setGainControl(SX1280_GAIN_CONTROL);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setGainControl:");Serial.println(state);
    #endif

    state = radio.setFrequency(SX1280_FREQUENCY);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setFrequency:");Serial.println(state);
    #endif

    state = radio.setFrequencyDeviation(SX1280_FREQUENCY_DEVIATION);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setFrequencyDeviation:");Serial.println(state);
    #endif

    state = radio.setBitRate(SX1280_BIT_RATE);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setBitRate:");Serial.println(state);
    #endif

    state = radio.setDataShaping(SX1280_DATA_SHAPING);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setDataShaping:");Serial.println(state);
    #endif

    state = radio.setCRC(SX1280_CRC_VALUE);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setCRC:");Serial.println(state);
    #endif

    state = radio.setPreambleLength(SX1280_PREAMBLE_LENGTH);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setSyncWord:");Serial.println(state);
    #endif

    state = radio.setHighSensitivityMode(SX1280_SET_HIGH_SENSITIVITY_MODE);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setHighSensitivityMode:");Serial.println(state);
    #endif

    #if ENABLE_SERIAL_PRINT
        #if ENABLE_RADIO_LIB_DEBUG
            if (state != RADIOLIB_ERR_NONE) {
                Serial.print(F("Unable to set configuration, code "));
                Serial.println(state);
                while (true);
            }
        #endif
    #endif

    radio.setDio1Action(setFlag);
    // start listening for FSK packets
    state = radio.startReceive();

    #if ENABLE_SERIAL_PRINT
        #if ENABLE_RADIO_LIB_DEBUG
            Serial.print(F("[SX1280] Starting to listen ... "));
            if (state == RADIOLIB_ERR_NONE) {
                Serial.println(F("success!"));
            } else {
                Serial.print(F("failed, code "));
                Serial.println(state);
                while (true);
            }
        #endif
    #endif
}

/**
 * SX1280 Data Receive
 */
void receiveData() {
    // There's a default of two bytes used by the binding key
    // and two bytes used by the config
    int received_payload_size = 4;
    if(receivedFlag) {
      enableInterrupt = false;
  
      // reset flag
      receivedFlag = false;

      int state = radio.readData(_payload, _payload_size);
      if (state == RADIOLIB_ERR_NONE) {
        _RX.setData(
            _payload
        );
      }

    #if ENABLE_SERIAL_PRINT
        #if ENABLE_RADIO_LIB_DEBUG
            if (state == RADIOLIB_ERR_CRC_MISMATCH) {
                // packet was received, but is malformed
                #if ENABLE_CNC_ERROR
                    Serial.println(F("CRC error!"));
                #endif
            } else if(state != RADIOLIB_ERR_NONE) {
                // some other error occurred
                Serial.print(F("failed, code "));
                Serial.println(state);
            }
        #endif

        #if ENABLE_DEBUG
            #if DEBUG_ALL_CHANNELS
                // Left Joystick
                Serial.print(_RX.left_joystick_up_down);
                Serial.print("\t");
                Serial.print(_RX.left_joystick_left_right);
                Serial.print("\t");
                
                // Right Joystick
                Serial.print(_RX.right_joystick_up_down);
                Serial.print("\t");
                Serial.print(_RX.right_joystick_left_right);
                Serial.print("\t");
                
                // Potentiometers
                int potentiometerValues[] = {
                    _RX.potentiometer_1,
                    _RX.potentiometer_2,
                    _RX.potentiometer_3,
                    _RX.potentiometer_4,
                    _RX.potentiometer_5,
                    _RX.potentiometer_6
                };

                for (int i = 0; i < 6; ++i) {
                    Serial.print(potentiometerValues[i]);
                    if(i<5) {
                        Serial.print(":");
                    }
                }
                Serial.print("\t");

                // Switches
                int switchesValues[] = {
                    _RX.switch_1,
                    _RX.switch_2,
                    _RX.switch_3,
                    _RX.switch_4,
                    _RX.switch_5
                };
                for (int i = 0; i < 5; ++i) {
                    Serial.print(switchesValues[i]);
                    if(i<4) {
                        Serial.print(":");
                    }
                }
                Serial.print("\t");

                // Left Rotary Encoder
                Serial.print(_RX.left_rotary_encoder_position);
                Serial.print("\t");
                Serial.print(_RX.left_rotary_encoder_up);
                Serial.print(":");
                Serial.print(_RX.left_rotary_encoder_down);
                Serial.print(":");
                Serial.print(_RX.left_rotary_encoder_left);
                Serial.print(":");
                Serial.print(_RX.left_rotary_encoder_right);
                Serial.print(":");
                Serial.print(_RX.left_rotary_encoder_middle);
                Serial.print("\t");

                // Right Rotary Encoder
                Serial.print(_RX.right_rotary_encoder_position);
                Serial.print("\t");
                Serial.print(_RX.right_rotary_encoder_up);
                Serial.print(":");
                Serial.print(_RX.right_rotary_encoder_down);
                Serial.print(":");
                Serial.print(_RX.right_rotary_encoder_left);
                Serial.print(":");
                Serial.print(_RX.right_rotary_encoder_right);
                Serial.print(":");
                Serial.print(_RX.right_rotary_encoder_middle);

            #else
                uint8_t current_position = 4;
                bool data_received = false;

                Serial.print("size:");
                if(_RX.received_payload_size<10) {
                    Serial.print(" ");
                }
                Serial.print(_RX.received_payload_size);
                Serial.print(" bytes \t");

                for (int i = 3; i < 16; i++) {
                    uint8_t channel = i - 2;
                    if (_RX.payload_config[i]) {
                        uint8_t required_bytes = _RX.channels[channel-1].required_bytes;
                        uint8_t first_byte = _payload[current_position++];
                        uint8_t second_byte = (required_bytes == 2) ? _payload[current_position++] : 0;
                        data_received = true;

                        received_payload_size = received_payload_size + required_bytes;

                        Serial.print("#");
                        Serial.print(channel);
                        Serial.print(":");
                        Serial.print(first_byte);
                        if(second_byte) {
                            Serial.print(" ");
                            Serial.print(second_byte);
                        }
                        Serial.print("\t");
                    }
                }
            #endif

            Serial.println("");
        #endif
    #endif
      radio.startReceive();
      enableInterrupt = true;
    }
}

/**
 * Feed the dog
 */
void feedTheDog() {
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed = 1;
  TIMERG0.wdt_wprotect = 0;
}