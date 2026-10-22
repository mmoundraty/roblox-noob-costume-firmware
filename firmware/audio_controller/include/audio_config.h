#pragma once

#include <Arduino.h>

namespace AudioConfig {

constexpr uint8_t DFPLAYER_RX_PIN = 4;
constexpr uint8_t DFPLAYER_TX_PIN = 5;
constexpr uint32_t DFPLAYER_BAUD = 115200;

constexpr int16_t BREATHING_TRACK = 1;
constexpr int16_t STARTUP_VOICE_TRACK = 2;
constexpr int16_t OOF_SOUND = 3;
constexpr int16_t MONSTER_MASH_SONG = 4;
constexpr uint8_t VOLUME = 20;

constexpr uint8_t SWC = 2; // because theres 2 files

// Senders must use the same Wi-Fi channel. Payload: 1 byte, low 4 bits = glove FSR mask.
constexpr uint8_t ESPNOW_CHANNEL = 1;

// Track to play for each glove mask (bits: FSR4 FSR3 FSR2 FSR1). 0 = silent.
// ponytail: placeholder tracks equal the mask; set the real sound per combo.


}  // namespace AudioConfig