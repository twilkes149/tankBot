#ifndef __TANKBOT__
#define __TANKBOT__
#include "tankbot_hardware.h"

extern int config_getSetConfig(char c1, char c2, void* param);

int getSetTurretAngle(void* param) {
  if (param == NULL) {
    return config.turretAngle;
  } else {
    int result = config_getSetConfig('T','A', param);
    
    turret_motor.write(config.turretAngle);
    return result;
  }
}

#endif
