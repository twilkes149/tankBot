#ifndef _COMMAND_
#define _COMMAND_
#include <Arduino.h>
#include "config.h"
#include "globals.h"

#define MAX_PARAM_CHARS 5 // The max number of characters the user can enter to set a config 

enum command_state {
  COMMAND_WAIT,           // Initial state
  COMMAND_MODE_CHAR,      // The user has entered 3 commandChars and is ready to enter 'AT'
  COMMAND_ENTRY,          // State in which we are ready to accept the 2 letter command name
  COMMAND_PARAM,          // We have the 2 letter command, and are ready to accept a parameter
  COMMAND_EXECUTE        // We have everything to execute the command
};
static command_state state;

// ################ GLOBALS ######################
static byte commandChars;
static long silenceTime;    // Time the uart line has been silent
static char lastChar;       // The last char read on the uart line
static int commandTimerID;  // The id of the timer for exiting command mode (used when restarting the timer)
static int index;           // used to keep track of which command char we are expecting next
static const char commandChar[] = {'A', 'T'};
static char commandName[2]; // the name of the command we are running/setting/reading
static int commandParam;    // the param the user typed in
static char paramString[MAX_PARAM_CHARS] = {0};


// ############### FUNCTIONS #####################

void command_init() {
  commandChars = 0;
  silenceTime = millis();
  state = COMMAND_WAIT;
  index = 0;
  commandParam = -1;
}

void exit_command_mode(void* dummy) {
  command_init();
}

void command_print_variable(int var) {
  // Because config values can be ints or ascii values we need a way to distinguish them
  // ... ASCII variables will have the upper bit set
  // So the biggest config value we can ever have is 0x7FFF
  
  if (mIS_ASCII_COMMAND(var)) {
    Serial.write(mASCII_TO_INT(var));
    Serial.print('\r');
  } else {
    Serial.println(var);
  }
}

void command_tick() {
#ifdef COMMAND_DEBUG
  if (state != COMMAND_WAIT) {
    digitalWrite(13,HIGH);
  } else {
    digitalWrite(13, LOW);
  }
#endif
  
  char c = 0;
  if (Serial.available()) {
    c = Serial.read();

    // if we are in command mode, refresh the timeout timer
    if (state != COMMAND_WAIT) {
      timer.stop(commandTimerID);      
      commandTimerID = timer.after(config.expireTime, exit_command_mode, NULL);
    }
  }

  // State transitions
  switch (state) {
    case COMMAND_WAIT:
      if (commandChars >= 3) {
        commandChars = 0;
        state = COMMAND_MODE_CHAR;
        Serial.write("OK\r");

        // Set a timer for exiting command mode after the expire time
        // ... save the timer ID for later when updating the timer
        commandTimerID = timer.after(config.expireTime, exit_command_mode, NULL);
      }
    break;

    case COMMAND_MODE_CHAR: // User is ready to enter 'AT'
      if (c != 0) {
        if (toupper(c) == commandChar[index]) {
          index++;
        } else {
          index = 0;
          command_init(); // user entered wrong sequence, exit command mode
        }

        // user has entered 'AT'
        if (index == 2) {
          state = COMMAND_ENTRY;
          index = 0;
        }
      }
    break;

    case COMMAND_ENTRY: // user is entering the 2 character command
      if (c != 0) {
        if (index < 2) {                        // user is entering 2 letter command name
          commandName[index] = toupper(c);
          index++;
        } else if (c == '\n' || c == '\r') {
          state = COMMAND_EXECUTE;              // we have the 2 letter command name and user pressed enter
          index = 0;
        } else if (c == ' ' || c == '\t') {
          state = COMMAND_PARAM;                // user pressed space or tab to enter a parameter
          index = 0;                            // user will start entering characters at the end of the paramString array
        }
      }
    break;

    case COMMAND_PARAM:
      if (c != 0) {
        if (c == '\r' || c == '\n') {
          // the user has entered a command parameter (the ascii characters of which are in paramString).
          // We need to convert those characters to an int (store it in commandParam) and then change 
          // ... state to run the command

          // loop through the string array and convert each digit to a number
          bool set = false;
          commandParam = 0;
          int maxPow = strnlen(paramString, MAX_PARAM_CHARS)-1;          
          
          for (int i = 0; i < MAX_PARAM_CHARS; i++) {
            if (globals_isHex(paramString[i])) {
              set = true;
              int digit = globals_convertToHex(paramString[i]);              
              int power = globals_pow(10, (maxPow-i));              
              commandParam += digit*power;
            }
          } // end for          
          
          if (!set) {
            // if user didn't enter in a parameter, reset commandParam to signify this
            commandParam = -1;
          }          
          index = 0;
          state = COMMAND_EXECUTE;
        }
      }
    break;
  }
  
  // State actions
  int result; // result of running the command
  switch (state) {
    case COMMAND_WAIT:
      if (commandChars == 0) {
        // If we haven't started receiving config chars then we need to make sure
        // ... that the uart line has been silent for at least the guard time
        if (mASCII_COMMAND(c) == config.commandChar && 
            (millis() > silenceTime + config.guardTime)
        ) {
          commandChars++;
        } else if (mASCII_COMMAND(c) != config.commandChar && c != 0) { // If c == 0 we didn't read anything in from the UART on this tick
          // We need to receive 3 commandChars in a row with nothing inbetween
          // ... so reset the counter if we get something else
          commandChars = 0;          
        }
      } else if (mASCII_COMMAND(c) == config.commandChar) {
        commandChars++;
      } else if (mASCII_COMMAND(c) != config.commandChar && c != 0) { 
        // We need to receive 3 commandChars in a row with nothing inbetween
        // ... so reset the counter if we get something else
        commandChars = 0;
      }
    break;

    case COMMAND_PARAM: // NOTE: this state action might change state if user error
      if (c != 0 && c != ' ' && c != '\t') { // ignore whitespace
        if (index < MAX_PARAM_CHARS) {
          paramString[index] = c;
          index++;
        } else {
          index = 0;
          Serial.print("ERROR\r");
          state = COMMAND_MODE_CHAR;
        }
      }
    break;

    case COMMAND_EXECUTE: // NOTE: This action will change state
      if (commandParam == -1) {
        result = config_runCommand(commandName[0], commandName[1], NULL);
      } else {
        result = config_runCommand(commandName[0], commandName[1], &commandParam);
      }

      switch (result) {
          case COMMAND_SET: // we just set the command
            Serial.print("OK\r");
          break;

          case COMMAND_ERROR: // There was an error reading/setting the command
            Serial.print("ERROR\r");
          break;

          default: // Reading the command
            command_print_variable(result);
          break;
        }

        index = 0;
        // We are done executing the command. Go back to the state where we expect the 'AT'
        state = COMMAND_MODE_CHAR;
    break;
  }

  // If we read something from the uart this tick
  if (c != 0) {
    silenceTime = millis();
  }
}
#endif
