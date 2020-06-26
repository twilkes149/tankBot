import serial
import time
import sys

ENTER_COMMAND_MODE = b'+++'
AT_CMD_PREFIX = b'AT'
DRIVE_CMDS = {        # Maps constants with their AT command values
  "RW_SPEED": b"RS",
  "RW_DIR":   b"RD",
  "LW_SPEED": b"LS",
  "LW_DIR":   b"LD"
}
ARM_CMDS = {
  "TURRET":   b"TS",
  "SHOULDER": b"SS",
  "ELBOW":    b"ES",
  "WRIST":    b"WS",
  "CLAW":     b"CS"
}
AUTO_STOP = 10      # Stop driving after 10 seconds if we don't get a new ros message
ET_TIME = 10        # Arduino command mode expires after 10 seconds of UART silence
NO_RESPONSE_MAX = 5 # If we get no response from the arduino after 5 tries, renter cmd mode

MS_TIME = .001

FWD  = 0
STOP = 1
REV  = 2
PREV = 3

class Wheel:
  def __init__(self, name, defSpeed, dir_):
    self.name = name
    self.speed = defSpeed
    self.dir = dir_
    self.changed = False

class Drive:
  def __init__(self):
    # In order to go forward, we need to have the direction
    # ... of each motor opposite of each other, since they
    # ... are mounted on opposite sides of the tank
    self.rw = Wheel("rw", 0, FWD)
    self.lw = Wheel("lw", 0, REV)
    self.wheels = [self.rw, self.lw]

class Joint:
  def __init__(self, name, default, min, max):
    self.name     = name
    self.angle    = default
    self.state    = STOP
    self.minAngle = min
    self.maxAngle = max
    self.changed  = False

class Arm:
  def __init__(self):
    self.turret   = Joint("Turret",   80,  0, 180)
    self.shoulder = Joint("Shoulder", 0,   0, 130)
    self.elbow    = Joint("Elbow",    132, 0, 180)
    self.wrist    = Joint("Wrist",    158, 0, 180)
    self.claw     = Joint("Claw",     30, 10,  80)
    self.joints   = [self.turret, self.shoulder, self.elbow, self.wrist, self.claw]

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

    self.auto_stop_timer = 0
    self.in_command_mode = False
    self.command_mode_time = 0
    self.no_response_count = 0
    self.arm = Arm()
    self.drive = Drive()

  def open_arduino_port(self):
    self.arduino.open()
    # Sleep to give the arduino time to restart due to
    # ... opening the serial port
    time.sleep(4)
    print("Opened arduino serial port")

  def tank_tick(self):
    # Drive tick
    # Check if its been longer than the auto stop time since
    # we've got a ros message
    if time.time() - self.auto_stop_timer > AUTO_STOP:
      self.drive.rw.speed = 0
      self.drive.lw.speed = 0

    # Arm tick
    for joint in self.arm.joints:
      if joint.state == FWD:
        joint.angle += 2
        joint.changed = True
        if joint.angle > joint.maxAngle:
          joint.angle = joint.maxAngle
      elif joint.state == REV:
        joint.angle -= 2
        joint.changed = True
        if joint.angle < joint.minAngle:
          joint.angle = joint.minAngle

  def arm_commands_callback(self, arm_msg):
    if arm_msg["turret"] != PREV:
      self.arm.turret.state = arm_msg["turret"]
    if arm_msg["shoulder"] != PREV:
      self.arm.shoulder.state = arm_msg["shoulder"]
    if arm_msg["elbow"] != PREV:
      self.arm.elbow.state = arm_msg["elbow"]
    if arm_msg["wrist"] != PREV:
      self.arm.wrist.state = arm_msg["wrist"]
    if arm_msg["claw"] != PREV:
      self.arm.claw.state = arm_msg["claw"]


  def drive_commands_callback(self, drive_msg):
    SPEED = 90

    # Right Wheel
    if drive_msg["rw"] != PREV:
      if drive_msg["rw"] == STOP:
        self.drive.rw.speed = 0
      else:
        self.drive.rw.dir = drive_msg["rw"]
        self.drive.rw.speed = SPEED
      self.drive.rw.changed = True

    # Left Wheel
    if drive_msg["lw"] != PREV:
      if drive_msg["lw"] == STOP:
        self.drive.lw.speed = 0
      else:
        self.drive.lw.dir = drive_msg["lw"]
        self.drive.lw.speed = SPEED
      self.drive.lw.changed = True

    # We got a message update, so update the timer
    self.auto_stop_timer = time.time()

  # Function to enter command mode on the arduino
  def enterCommandMode(self):
    if not self.in_command_mode or (time.time() - self.command_mode_time) >= ET_TIME:
      print("Entering command mode")
      time.sleep(1.2)
      self.arduino.write(ENTER_COMMAND_MODE)
    self.in_command_mode = True
    self.command_mode_time = time.time()

  def write_to_arduino(self):
    written = 0
    if self.drive.rw.changed:
      self.enterCommandMode()
      self.drive.rw.changed = False
      self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["RW_DIR"]   + b" " + str(self.drive.rw.dir).encode('utf-8')   + b"\n")
      self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["RW_SPEED"] + b" " + str(self.drive.rw.speed).encode('utf-8') + b"\n")
      written += 6 # Response should consist of at least 6 bytes (2x "OK\r"")

    if self.drive.lw.changed:
      self.enterCommandMode()
      self.drive.lw.changed = False
      self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["LW_DIR"]   + b" " + str(self.drive.lw.dir).encode('utf-8')   + b"\n")
      self.arduino.write(AT_CMD_PREFIX+DRIVE_CMDS["LW_SPEED"] + b" " + str(self.drive.lw.speed).encode('utf-8') + b"\n")
      written += 6 # Response should consist of at least 6 bytes (2x "OK\r"")

    for index, joint in enumerate(self.arm.joints):
      if joint.changed == True:
        joint.changed = False
        self.enterCommandMode()
        self.arduino.write(AT_CMD_PREFIX+ARM_CMDS[list(ARM_CMDS)[index]] + b" " + str(joint.angle).encode('utf-8') + b"\n")
        self.command_mode_time = time.time() # Keep track of how long its been since we last wrote
        written += 3 # Response should consist of at least 3 bytes ("OK\r"")
                     # ... to the arduino

    # If the arduino hasn't responded after 5 times, re-enter cmd mode
    count = 0
    MAX_DELAY_MS = 50
    while self.arduino.in_waiting < written and count < MAX_DELAY_MS:
      time.sleep(MS_TIME) # Sleep for 1ms
      count += 1

    if count >= MAX_DELAY_MS:
      self.no_response_count += 1
      if self.no_response_count > NO_RESPONSE_MAX:
        self.in_command_mode = False
        self.no_response_count = 0
    else:
      self.no_response_count = 0
      if self.arduino.in_waiting:
        self.arduino.read(self.arduino.in_waiting)


  # Destructor to close the arduino port
  def __del__(self):
    if self.arduino.is_open:
      self.arduino.close()
      print("Closing arduino port")
