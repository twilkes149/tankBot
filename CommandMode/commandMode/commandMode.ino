#define COMMAND_DEBUG
#include "command.h"
#include "config.h"
#include"globals.h"
#include "tankbot_hardware.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);

  config_init(); // This needs to happen first to init all  of the config vars
  command_init();    

  tankbot_hardwareInit();
}

void loop() {
  command_tick();
  timer.update();
  drivePWMTick();
}
