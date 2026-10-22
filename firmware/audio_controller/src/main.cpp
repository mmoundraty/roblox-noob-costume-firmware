#include <Arduino.h>
#include <DFRobot_DF1201S.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#include "audio_config.h"

HardwareSerial dfSerial(1);
DFRobot_DF1201S dfPlayer;

// Set from the Wi-Fi task, consumed in loop(); 0 = nothing pending.
volatile int16_t pendingTrack = 0;

void onReceive(const uint8_t* mac, const uint8_t* data, int len) {
  if (len != 1) return;
  const uint8_t mask = data[0] & 0x0F;
  // Skip the Star Wars Club files so mask 1 lands on the first Roblox sound.
  if (mask) pendingTrack = mask + AudioConfig::SWC;
}

void setup() {
  Serial.begin(115200);
  dfSerial.begin(AudioConfig::DFPLAYER_BAUD, SERIAL_8N1,
                 AudioConfig::DFPLAYER_RX_PIN,
                 AudioConfig::DFPLAYER_TX_PIN);

  while (!dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer not found. Retrying...");
    delay(1000);
  }

  dfPlayer.switchFunction(dfPlayer.MUSIC);
  dfPlayer.setVol(AudioConfig::VOLUME);

  // Play the startup voice once if track 2 exists.
  if (dfPlayer.getTotalFile() >= AudioConfig::STARTUP_VOICE_TRACK) {
    dfPlayer.setPlayMode(dfPlayer.SINGLE);
    dfPlayer.playFileNum(AudioConfig::STARTUP_VOICE_TRACK);
    while (dfPlayer.isPlaying()) {
      delay(50);
    }
  }

  // Loop breathing forever.
  dfPlayer.setPlayMode(dfPlayer.SINGLECYCLE);
  dfPlayer.playFileNum(AudioConfig::MONSTER_MASH_SONG);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(AudioConfig::ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed.");
    return;
  }
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  const int16_t track = pendingTrack;
  if (!track) return;
  pendingTrack = 0;

  Serial.printf("ESP-NOW: play track %d\n", track);
  dfPlayer.setPlayMode(dfPlayer.SINGLE);
  dfPlayer.playFileNum(track);
}
