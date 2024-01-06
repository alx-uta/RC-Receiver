/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#include "rx.hpp"
extern bool BINDING_KEY[16];

Rx::Rx(
    Config& config
)
    :   _config(config)
        {}

/**
 * Read and set the right values
 */
void Rx::setData(
    uint8_t* _payload
) {

    // Get the binding key from the first two bytes
    uint16_t binding_key_combined_byte = this->combineBytes(
        _payload[0],
        _payload[1]
    );

    // Decode the uint16_t back into config and channels
    bool decoded_binding_key[16];
    this->decodeByteToStatuses(binding_key_combined_byte, decoded_binding_key, 16);

    for (int i = 0; i < 16; i++) {
        if(BINDING_KEY[i] != decoded_binding_key[i]) {
            return;
        }
    }

    // Get the config from the first two bytes
    uint16_t config_combined_byte = this->combineBytes(
        _payload[2],
        _payload[3]
    );

    // Decode the uint16_t back into config and channels
    bool decoded_config[16];
    this->decodeByteToStatuses(config_combined_byte, decoded_config, 16);

    for (int i = 3; i < 16; i++) {
        this->payload_config[i] = decoded_config[i];
    }

    if(this->payload_config[0] == 0) {
        this->setTXpayload(_payload);
    }
}

void Rx::setTXpayload(uint8_t* _payload) {
    this->received_payload_size = 4;

    for (int i = 3; i < 16; i++) {
        uint8_t channel = i - 2;
        if (this->payload_config[i]) {
            uint8_t required_bytes  = this->channels[channel-1].required_bytes;
            int first_byte      = _payload[this->received_payload_size++];
            int second_byte     = (required_bytes == 2) ? _payload[this->received_payload_size++] : 0;

            this->setChannel(channel, first_byte, second_byte);
        } else {
            int default_value = this->channels[channel-1].default_value;
            this->setChannel(channel, default_value, 0);
        }
    }
}

void Rx::setChannel(uint8_t channel, int first_byte, int second_byte) {
    switch (channel) {
        case 1:
            setCh1(first_byte);
            break;
        case 2:
            setCh2(first_byte);
            break;
        case 3:
            setCh3(first_byte);
            break;
        case 4:
            setCh4(first_byte);
            break;
        case 5:
            setCh5(first_byte, second_byte);
            break;
        case 6:
            setCh6(first_byte, second_byte);
            break;
        case 7:
            setCh7(first_byte, second_byte);
            break;
        case 8:
            setCh8(first_byte);
            break;
        case 9:
            setCh9(first_byte);
            break;
        case 10:
            setCh10(first_byte);
            break;
        case 11:
            setCh11(first_byte);
            break;
        case 12:
            setCh12(first_byte);
            break;
        case 13:
            setCh13(first_byte);
            break;
        default:
            // pass
            break;
    }
}

void Rx::setCh1(int byte_1) {
    this->left_joystick_left_right = byte_1;
}
void Rx::setCh2(int byte_1) {
    this->left_joystick_up_down = byte_1;
}
void Rx::setCh3(int byte_1) {
    this->right_joystick_left_right = byte_1;
}
void Rx::setCh4(int byte_1) {
    this->right_joystick_up_down = byte_1;
}
void Rx::setCh5(int byte_1, int byte_2) {
    uint16_t combined_byte = this->combineBytes(
        byte_1, byte_2
    );

    // Decode the uint16_t back into switch statuses
    bool decoded_switch_statuses[15];
    this->decodeByteToStatuses(combined_byte, decoded_switch_statuses, 15);

    // Set the values for the current switches
    this->switch_1 = decoded_switch_statuses[0];
    this->switch_2 = decoded_switch_statuses[1];
    this->switch_3 = decoded_switch_statuses[2];
    this->switch_4 = decoded_switch_statuses[3];
    this->switch_5 = decoded_switch_statuses[4];

    // Set the values for the push buttons
    // Left Rotary Encoder
    this->left_rotary_encoder_up = decoded_switch_statuses[5];
    this->left_rotary_encoder_down = decoded_switch_statuses[6];
    this->left_rotary_encoder_left = decoded_switch_statuses[7];
    this->left_rotary_encoder_right = decoded_switch_statuses[8];
    this->left_rotary_encoder_middle = decoded_switch_statuses[9];

    // Right Rotary Encoder
    this->right_rotary_encoder_up = decoded_switch_statuses[10];
    this->right_rotary_encoder_down = decoded_switch_statuses[11];
    this->right_rotary_encoder_left = decoded_switch_statuses[12];
    this->right_rotary_encoder_right = decoded_switch_statuses[13];
    this->right_rotary_encoder_middle = decoded_switch_statuses[14];
}
void Rx::setCh6(int byte_1, int byte_2) {
    this->left_rotary_encoder_position = this->getAnoRotaryEncoderPosition(
        byte_1,
        byte_2
    );
}
void Rx::setCh7(int byte_1, int byte_2) {
    this->right_rotary_encoder_position = this->getAnoRotaryEncoderPosition(
        byte_1,
        byte_2
    );
}
void Rx::setCh8(int byte_1) {
    this->potentiometer_1 = byte_1;
}
void Rx::setCh9(int byte_1) {
    this->potentiometer_2 = byte_1;
}
void Rx::setCh10(int byte_1) {
    this->potentiometer_3 = byte_1;
}
void Rx::setCh11(int byte_1) {
    this->potentiometer_4 = byte_1;
}
void Rx::setCh12(int byte_1) {
    this->potentiometer_5 = byte_1;
}
void Rx::setCh13(int byte_1) {
    this->potentiometer_6 = byte_1;
}
