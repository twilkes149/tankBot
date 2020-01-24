#define COMMAND_DEBUG
#include "command.h"
#include "config.h"
#include"globals.h"
#include "tankbot_hardware.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);

  tankbot_hardwareInit();
  config_init();
  command_init();  
}

void loop() {
  command_tick();
  timer.update();
}
