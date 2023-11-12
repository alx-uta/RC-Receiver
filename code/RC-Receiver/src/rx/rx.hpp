/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#ifndef RX_HPP
#define RX_HPP

#include "../config/config.hpp"

class Rx {
public:
    Rx(Config& config);

    void setData(uint8_t* _payload);
    void setTXpayload(uint8_t* _payload);

    void setChannel(uint8_t channel, int first_byte, int second_byte);

    // Set each channel
    void setCh1(int byte_1);
    void setCh2(int byte_1);
    void setCh3(int byte_1);
    void setCh4(int byte_1);
    void setCh5(int byte_1, int byte_2);
    void setCh6(int byte_1, int byte_2);
    void setCh7(int byte_1, int byte_2);
    void setCh8(int byte_1);
    void setCh9(int byte_1);
    void setCh10(int byte_1);
    void setCh11(int byte_1);
    void setCh12(int byte_1);
    void setCh13(int byte_1);

    // Utils
    int16_t getAnoRotaryEncoderPosition(uint8_t byte_1, uint8_t byte_2);
    void decodeByteToStatuses(uint16_t encodedByte, bool statuses[], int num);
    uint16_t combineBytes(uint8_t byte1, uint8_t byte2);

    float valRound(float val);

    // Current Values
    // Left Joystick
    int left_joystick_up_down;
    int left_joystick_left_right;
    
    // Right Joystick
    int right_joystick_up_down;
    int right_joystick_left_right;

    // Potentiometers
    int potentiometer_1;
    int potentiometer_2;
    int potentiometer_3;
    int potentiometer_4;
    int potentiometer_5;
    int potentiometer_6;

    // Left Rotary Encoder
    // Position
    uint8_t left_rotary_encoder_position;

    // Push Buttons
    uint8_t left_rotary_encoder_up;
    uint8_t left_rotary_encoder_down;
    uint8_t left_rotary_encoder_left;
    uint8_t left_rotary_encoder_right;
    uint8_t left_rotary_encoder_middle;

    // Right Rotary Encoder
    // Position
    uint8_t right_rotary_encoder_position;

    // Push Buttons
    uint8_t right_rotary_encoder_up;
    uint8_t right_rotary_encoder_down;
    uint8_t right_rotary_encoder_left;
    uint8_t right_rotary_encoder_right;
    uint8_t right_rotary_encoder_middle;

    // Switches
    uint8_t switch_1;
    uint8_t switch_2;
    uint8_t switch_3;
    uint8_t switch_4;
    uint8_t switch_5;

    /**
     * TX Payload Config
     * TX0 : Remote TX
     * TX1 : Sensors 1
     * TX2 : Sensors 2
     * 
     * CH1  : j1y - Left Joystick Up/Down
     * CH2  : j1x - Left Joystick Left/Right
     * CH3  : j2y - Right Joystick Up/Down
     * CH4  : j2x - Right Joystick Left/Right
     * CH5  : switches_state_1 + switches_state_2
     * CH6  : re1_byte_1 + re1_byte_2
     * CH7  : re2_byte_1 + re2_byte_2
     * CH8  : p1
     * CH9  : p2
     * CH10 : p3
     * CH11 : p4
     * CH12 : p5
     * CH13 : p6
     */
    bool payload_config[16] = {
        0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    typedef struct Channel {
        uint8_t required_bytes;
        uint8_t default_value;
    };

    /**
     * CH1  : Left Joystick Up/Down
     * CH2  : Left Joystick Left/Right
     * CH3  : Right Joystick Up/Down
     * CH4  : Right Joystick Left/Right
     * CH5  : Switches/Push buttons (switches_state_1 + switches_state_2)
     * CH6  : Left Rotary Encoder (re1_byte_1 + re1_byte_2)
     * CH7  : Right Rotary Encoder (re2_byte_1 + re2_byte_2)
     * CH8  : Potentiometer 1
     * CH9  : Potentiometer 2
     * CH10 : Potentiometer 3
     * CH11 : Potentiometer 4
     * CH12 : Potentiometer 5
     * CH13 : Potentiometer 7
     */
    Channel channels[13] = {
        {1, 127},
        {1, 127},
        {1, 127},
        {1, 127},
        {2, 0},
        {2, 0},
        {2, 0},
        {1, 0},
        {1, 0},
        {1, 0},
        {1, 0},
        {1, 0},
        {1, 0}
    };

    uint8_t received_payload_size;

 private:
    Config& _config;

    // Rotary Encoders
    int rotary_min = -32768;
    int rotary_max = 32768;
};

#endif  // RX_HPP
