#ifndef __COMMAND_TABLE___
#define __COMMAND_TABLE___

#define COMMAND_READ(c1,c2,def,func,var)        {c1, c2, def, 0,0, func, var}
#define COMMAND_WRITE(c1,c2,def,mn,mx,func,var) {c1, c2, def, mn, mx, func, var}
#define COMMAND_EXE(c1,c2,func)                 {c1, c2, 0,0,0, func, NULL}


// Because config values can be ints or ascii values we need a way to distinguish them.
// ... ASCII variables will have the upper bit set
// So the biggest config value we can ever have is 0x7FFF
#define mASCII_COMMAND(cmd) (0x8000 | (cmd))
#define mIS_ASCII_COMMAND(cmd) ((cmd) & 0x8000 == 0x8000)
#define mASCII_TO_INT(cmd) ((cmd) & 0x7FFF)

// Note: When adding a command, you must update COMMAND_TABLE_SIZE in config.h

COMMAND_READ('B','D', 9600, getBaud, &config.baudRate),                                   // Baud rate
COMMAND_READ('C','C', mASCII_COMMAND('+'), getCommandChar, &config.commandChar),                          // Command char
COMMAND_WRITE('G','T', 1000, 100, 3000, getSetGuardTime, &config.guardTime),              // Guard time
COMMAND_WRITE('E', 'T', 10000, 1000, 10001, getSetExpireTime, &config.expireTime),        // Command mode expire time

#endif
