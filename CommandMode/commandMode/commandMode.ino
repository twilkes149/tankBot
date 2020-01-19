#define COMMAND_DEBUG
#include "command.h"
#include "config.h"
#include"globals.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  config_init();
  command_init();
}

void loop() {
  command_tick();
  timer.update();
}
