import serial
import time
arduino = serial.Serial()

ENTER_COMMAND_MODE = r'+++'

class PI_Server:

  def __init__(self):
    self.commandMode = False
    arduino.baudrate = 9600
    arduino.port = 'COM8'
    arduino.open()
    # Sleep to give the arduino time to restart due to
    # ... opening the serial port
    time.sleep(4)
    print("Opened arduino serial port")

  # Function to enter command mode on the arduino
  def enterCommandMode(self):
    if not self.commandMode:
      arduino.write(b'+++')
    self.commandMode = True

  def __del__(self):
    if arduino.is_open:
      arduino.close()
      print("Closing arduino port")

def main():
  print("Entering command mode")
  server = PI_Server()

  server.enterCommandMode()

if __name__ == "__main__":
  main()