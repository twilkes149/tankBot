ENTER_COMMAND_MODE = b'+++'
AT_CMD_PREFIX = b'AT'
DRIVE_CMDS = {        # Maps constants with their AT command values
  "RW_SPEED": b"RS",
  "RW_DIR":   b"RD",
  "LW_SPEED": b"LS",
  "LW_DIR":   b"LD" 
}
AUTO_STOP = 10      # Stop driving after 10 seconds if we don't get a new ros message
ET_TIME = 10        # Arduino command mode expires after 10 seconds of UART silence
NO_RESPONSE_MAX = 5 # If we get no response from the arduino after 5 tries, renter cmd mode

DRIVE_MSG = {
  "STOP": 0,
  "FWD":  1,
  "REV":  2,
  "NULL": 3
}

class PI_Command:
  def __init__(self):
    self.commandMode = False
    self.arduino = serial.Serial()
    self.arduino.baudrate = 9600

    # Check if we are running on windows or linux
    if sys.platform == 'win32':
      self.arduino.port = 'COM8'
    else:
      self.arduino.port = '/dev/ttyUSB0'

    self.open_arduino_port()
    
    self.rw_speed = 0
    self.lw_speed = 0
    
    # In order to go forward, we need to have the direction
    # ... of each motor opposite of each other, since they
    # ... are mounted on opposite sides of the tank
    self.lw_dir = DRIVE_MSG['FWD']
    self.rw_dir = DRIVE_MSG['REV']

    self.auto_stop_timer = 0
    self.in_command_mode = False
    self.command_mode_time = 0
    self.no_response_count = 0

  def open_arduino_port(self):
    self.arduino.open()
    # Sleep to give the arduino time to restart due to
    # ... opening the serial port
    time.sleep(4)
    print("Opened arduino serial port")

  def remote_commands_callback(self, drive_msg):
    SPEED = 90
    print(drive_msg)

    if drive_msg.rw == DRIVE_MSG['STOP']:
      self.rw_speed = 0
    elif drive_msg.rw == DRIVE_MSG['FWD']:
      self.rw_speed = SPEED
      self.rw_dir = DRIVE_MSG['FWD']
    elif drive_msg.rw == DRIVE_MSG['REV']:
      self.rw_speed = SPEED
      self.rw_dir = DRIVE_MSG['REV']

    if drive_msg.lw == DRIVE_MSG['STOP']:
      self.lw_speed = 0
    elif drive_msg.lw == DRIVE_MSG['FWD']:
      self.lw_speed = SPEED
      self.lw_dir = DRIVE_MSG['FWD']
    elif drive_msg.lw == DRIVE_MSG['REV']:
      self.lw_speed = SPEED
      self.lw_dir = DRIVE_MSG['REV']

    # We got a message update, so update the timer
    self.auto_stop_timer = time.time()

  # Function to enter command mode on the arduino
  def enterCommandMode(self):
    if not self.in_command_mode or (time.time() - self.command_mode_time) >= ET_TIME:
      time.sleep(1.2)
      self.arduino.write(ENTER_COMMAND_MODE)
    self.in_command_mode = True
    self.command_mode_time = time.time()

  def write_to_arduino(self):
    # Check if its been longer than the auto stop time since
    # we've got a ros message
    if time.time() - self.auto_stop_timer > AUTO_STOP:
      self.lw_speed = 0
      self.rw_speed = 0

    self.enterCommandMode()
    self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["LW_DIR"]   + " " + str(self.lw_dir)   + "\n")
    self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["LW_SPEED"] + " " + str(self.lw_speed) + "\n")
    self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["RW_DIR"]   + " " + str(self.rw_dir)   + "\n")
    self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["RW_SPEED"] + " " + str(self.rw_speed) + "\n")
    self.command_mode_time = time.time() # Keep track of how long its been since we last wrote
                                         # ... to the arduino

    # If the arduino hasn't responded after 5 times, re-enter cmd mode
    if not self.arduino.in_waiting:
      self.no_response_count += 1
      if self.no_response_count > NO_RESPONSE_MAX:
        self.in_command_mode = False
        self.no_response_count = 0
    else:
      self.no_response_count = 0
      print(self.arduino.read(self.arduino.in_waiting))


  # Destructor to close the arduino port
  def __del__(self):
    if self.arduino.is_open:
      self.arduino.close()
      print("Closing arduino port")