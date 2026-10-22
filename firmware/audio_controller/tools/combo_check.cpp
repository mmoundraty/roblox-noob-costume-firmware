// Host check for combo latching:
//   g++ -std=c++17 -I include tools/combo_check.cpp -o combo_check && ./combo_check
#include <cassert>

#include "combo.h"

int main() {
  uint8_t latched = 0;

  assert(updateCombo(0b000, latched) == 0);  // idle: nothing fires

  assert(updateCombo(0b001, latched) == 0);  // single press fires on release
  assert(updateCombo(0b000, latched) == 0b001);
  assert(updateCombo(0b000, latched) == 0);  // and only once

  assert(updateCombo(0b010, latched) == 0);  // staggered press
  assert(updateCombo(0b011, latched) == 0);
  assert(updateCombo(0b001, latched) == 0);  // staggered release, still held
  assert(updateCombo(0b000, latched) == 0b011);

  assert(updateCombo(0b100, latched) == 0);  // previous combo was reset
  assert(updateCombo(0b000, latched) == 0b100);
  return 0;
}
