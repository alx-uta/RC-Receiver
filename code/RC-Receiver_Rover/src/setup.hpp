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
    int state = radio.beginFLRC();
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

    state = radio.setFrequency(SX1280_FREQUENCY);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setFrequency:");Serial.println(state);
    #endif

    state = radio.setBitRate(SX1280_BIT_RATE);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setBitRate:");Serial.println(state);
    #endif

    state = radio.setCodingRate(SX1280_CODING_RATE);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setCodingRate:");Serial.println(state);
    #endif

    state = radio.setOutputPower(SX1280_OUTPUT_POWER);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setOutputPower:");Serial.println(state);
    #endif

    state = radio.setGainControl(SX1280_GAIN_CONTROL);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setGainControl:");Serial.println(state);
    #endif

    state = radio.setDataShaping(SX1280_DATA_SHAPING);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setDataShaping:");Serial.println(state);
    #endif

    state = radio.setCRC(2, 0x1D0F, 0x1021);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setCRC:");Serial.println(state);
    #endif

    state = radio.setSyncWord(SX1280_SYNC_WORD, 4);
    #if ENABLE_RADIO_LIB_DEBUG
        Serial.print("setSyncWord:");Serial.println(state);
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
    int received_payload_size = 2;
    if(receivedFlag) {
      enableInterrupt = false;
  
      // reset flag
      receivedFlag = false;

      int state = radio.readData(_payload, _payload_size);
      if (state == RADIOLIB_ERR_NONE) {
        _RX.last_received_time = millis();

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
            // // Debug the new payload
            // for(int i=0; i < _payload_size; i++)
            // {
            //     Serial.print(_payload[i]);
            //     if(i < _payload_size - 1) {
            //         Serial.print(" : ");
            //     }
            // }
            // Serial.print(" (");
            // Serial.print(ESP.getFreeHeap());
            // Serial.println(" bytes free)");
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
