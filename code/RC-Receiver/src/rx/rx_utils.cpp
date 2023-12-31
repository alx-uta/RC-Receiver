/**
 * RC Receiver – ESP32 / SX1280
 * https://github.com/alx-uta/RC-Receiver
 * 
 * Alex Uta
 * microknot.dev
 */

#include "rx.hpp"

int16_t Rx::getAnoRotaryEncoderPosition(
    uint8_t byte_1,
    uint8_t byte_2
) {
    // Reconstruct the int16_t value from the stored bytes
    int16_t reconstructed_value = (int16_t)((byte_1 << 8) | byte_2);

    if (reconstructed_value >= this->rotary_min && reconstructed_value <= this->rotary_max) {
        return reconstructed_value;
    }

    return 0;
}

void Rx::decodeByteToStatuses(uint16_t encodedByte, bool statuses[], int num) {
  for (int i = 0; i < num; i++) {
    statuses[i] = (encodedByte >> i) & 0x01;
  }
}

uint16_t Rx::combineBytes(uint8_t byte1, uint8_t byte2) {
  return ((uint16_t)byte2 << 8) | byte1;
}

float Rx::valRound(float val) {
  return round(val * 100.0) / 100.0;
}
