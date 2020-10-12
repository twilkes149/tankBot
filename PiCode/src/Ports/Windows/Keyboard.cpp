#include "Keyboard.h"
#include <iostream>

namespace client {
  const int Keyboard::NUM_KEYS;

  Keyboard::Keyboard() 
    : PRESSED_STATE(0x8000)
  {
  }

  bool Keyboard::isPressed(const key_t key) {
    return GetKeyState(key) & PRESSED_STATE;
  }

  int Keyboard::keyToIndexHash(const key_t key) {
    return key - 'A';
  }
}