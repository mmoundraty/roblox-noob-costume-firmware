#pragma once

#include <stdint.h>

// Accumulates every bit pressed during a combo. Returns the finished code once
// all sensors are released, else 0. Staggered presses/releases still count.
inline uint8_t updateCombo(uint8_t pressedMask, uint8_t& latched) {
  latched |= pressedMask;
  if (pressedMask || !latched) return 0;
  const uint8_t code = latched;
  latched = 0;
  return code;
}
