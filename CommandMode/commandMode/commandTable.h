#ifndef __COMMAND_TABLE___
#define __COMMAND_TABLE___

#define COMMAND_READ(c1,c2,def,func,var)        {c1, c2, def, 0,0, func, var}
#define COMMAND_WRITE(c1,c2,def,mn,mx,func,var) {c1, c2, def, mn, mx, func, var}
#define COMMAND_EXE(c1,c2,func)                 {c1, c2, 0,0,0, func, NULL}


// Because config values can be ints or ascii values we need a way to distinguish them.
// ... ASCII variables will have the upper bit set
// So the biggest config value we can ever have is 0x7FFF
#define mASCII_COMMAND(cmd) (0x8000 | (cmd))
#define mIS_ASCII_COMMAND(cmd) (((cmd) & 0x8000) == 0x8000)
#define mASCII_TO_INT(cmd) ((cmd) & 0x7FFF)

// Note: When adding a command, you must update COMMAND_TABLE_SIZE in config.h

// ****************************************************** CONFIGURATION COMMANDS *******************************************************************
COMMAND_READ('B','D', 9600, getBaud, &config.baudRate),                                   // Baud rate
COMMAND_READ('C','C', mASCII_COMMAND('+'), getCommandChar, &config.commandChar),          // Command char
COMMAND_WRITE('G','T', 1000, 100, 3000, getSetGuardTime, &config.guardTime),              // Guard time
COMMAND_WRITE('E', 'T', 10000, 1000, 10001, getSetExpireTime, &config.expireTime),        // Command mode expire time
COMMAND_EXE('C', 'N', command_exitCommandMode),

// ************************************************************ TANKBOT COMMANDS *******************************************************************
// Arm joint angles
COMMAND_WRITE('S', 'A', 116, 0, 107, getSetShoulderAngle, &config.shoulderAngle), // TODO, convert defaults
COMMAND_WRITE('E', 'A', 118, 74, 239, getSetElbowAngle, &config.elbowAngle),     // TODO, convert defaults

// Arm servo angles
COMMAND_WRITE('T', 'S', 80, 0, 180, getSetTurretServoAngle, &config.turretServoAngle),
COMMAND_WRITE('S', 'S', 0, 0, 180, getSetShoulderServoAngle, &config.shoulderServoAngle),
COMMAND_WRITE('E', 'S', 132, 0, 180, getSetElbowServoAngle, &config.elbowServoAngle),
COMMAND_WRITE('W', 'S', 158, 0, 180, getSetWristServoAngle, &config.wristServoAngle),
COMMAND_WRITE('C', 'S', 30, 0, 180, getSetClawServoAngle, &config.clawServoAngle),
COMMAND_READ('A', 'X', 0, getArmX, &config.armX),
COMMAND_READ('A', 'Y', 0, getArmY, &config.armY),
// Drive
COMMAND_WRITE('L', 'S', 0, 0, 100, getSetLeftSpeed, &config.lw_speed),
COMMAND_WRITE('R', 'S', 0, 0, 100, getSetRightSpeed, &config.rw_speed),
COMMAND_WRITE('L', 'D', 0, 0, 2, getSetLeftDir, &config.lw_dir),
COMMAND_WRITE('R', 'D', 0, 0, 2, getSetRightDir, &config.rw_dir),

#endif
