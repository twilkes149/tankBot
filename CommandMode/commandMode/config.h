#ifndef __CONFIG__
#define __CONFIG__
#include <Arduino.h>

// **************************** GLOBAL ***********************************
// NOTE: all commands are capital case
#define COMMAND_OK 0          // Executable and writing commands will return this
#define COMMAND_ERROR 0xff    // Commands that were set to an invalid parameter or command doesn't exit will return this
#define COMMAND_SET -1

struct Config_t {
  // ********************** Module Configuration *************************
  int baudRate;    // Current baud rate to operate on
  int guardTime;   // Time uart must be silent before command char is entered
  int commandChar; // The character we have to enter in order to get into command mode
  int expireTime;
} config;

// Each AT command is two characters long followed by an optional parameter
// ... i.e to enter a command: AT<char1><char1>[optional whitespace][optional parameter]\n||\r||\n\r||\r\n
struct commandTableEntry {
  char c1;                  // 1st character of command
  char c2;                  // 2nd character of command
  int def;                 // Default on boot this command has
  int min;                 // Min value this param can take
  int max;                 // Max value this param can take
  int (*handler)(void*);   // Function pointer to a function that will get/set/execute the command
  void* variable;            // Pointer to the config variable
};

#define mIS_CMD_EXE(c1,c2,def,mn,mx,func,var) (var==NULL)

// **************************** Function prototypes *************************
int getBaud(void* dummy);
int getCommandChar(void* dummy);
int getSetGuardTime(void* param);
int getSetExpireTime(void* param);

// ********************************************** COMMAND TABLE ****************************************
// Note: When adding an entry to the command table, COMMAND_TABLE_SIZE must also be updated
#define COMMAND_TABLE_SIZE 4
static commandTableEntry commandTable[] = {
  #include "commandTable.h" // Command table is in this file for readability
};
// *****************************************************************************************************


// Sets all of the non-executable configs to their default values
void config_init() {
  for (byte i = 0; i < COMMAND_TABLE_SIZE; i++) {
    commandTableEntry entry = commandTable[i];
    // If the command is not executable, we need to set the default value
    if (!mIS_CMD_EXE(entry.c1, entry.c2, entry.def, entry.min, entry.max, entry.func, entry.variable)) {
      *((int *)entry.variable) = entry.def;
    }
  }
}

// ************************* HELPER FUNCTIONS ********************
// returns the index of the command if it is in the table
// ... -1 otherwise
int getCommand(char c1, char c2) {
  for (byte i = 0; i < COMMAND_TABLE_SIZE; i++) {
    if(commandTable[i].c1 == c1 && commandTable[i].c2 == c2) {
      return i;
    }
  }
  return -1;
}

int config_runCommand(char c1, char c2, void* param) {
  int index = getCommand(c1,c2);
  
  // If the command exists
  if (index >= 0 && index < COMMAND_TABLE_SIZE) {
    commandTableEntry command = commandTable[index];    // get the command from the table
    return (*command.handler)(param);               // call the command's function
  }

  return COMMAND_ERROR;
}

// ************************* CONFIG FUNCTIONS ********************

// Function to return the baud rate
int getBaud(void* dummy) {
  // dummy is not used
  return config.baudRate;
}

int getCommandChar(void* dummy) {
  // dummy is not used
  return config.commandChar;
}

// gets or sets an INT command
int getSetConfig(char c1, char c2, void* param) {
  int index = getCommand(c1,c2);
  
  if (index < 0) {
    return COMMAND_ERROR;
  }

  commandTableEntry* entry = &commandTable[index];

  // We are reading the data
  if (param == NULL) {
    return *((int*) entry->variable);
  } else {
    int value = *((int *)param);
    if (value < entry->min || value > entry->max) {
      return COMMAND_ERROR;
    } else {
      *((int*) entry->variable) = value;
      return COMMAND_SET;
    }
  }
}

int getSetGuardTime(void* param) {
  return getSetConfig('G','T', param);
}

int getSetExpireTime(void* param) {
  return getSetConfig('E','T', param);
}

#endif
