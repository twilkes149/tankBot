#ifndef __GLOBALS__
#define __GLOBALS__

#include "Timer.h"

Timer timer;

int globals_pow(int base, int exponent) {
  int sum = 1;
  for (int i = 0; i < exponent; i++) {
    sum *= base;
  }

  return sum;
}

// Returns true if ch is a hex chacter [0-9a-fA-F]
bool globals_isHex(char ch) {
  if ((ch >= '0' && ch <= '9') || // if the digit is 0-9
      (ch >= 'a' && ch <= 'f') || // if the digit is a-f
      (ch >= 'A' && ch <= 'F')    // if the digit is A-F
  ) {
    return true;
  }
  return false;
}

// Converts a hex character to a number
// precondition: ch must be a valid hex character [0-9a-fA-F].
// ... this can be checked by globals-isHex()
int globals_convertToHex(char ch) {
  if (ch >= '0' && ch <= '9') { // if the digit is 0-9
    return ch - '0';
  } else if (ch >= 'a' && ch <= 'f') {
    return ch - 'a';
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A';
  } else {
    return 0;
  }
}

#endif
