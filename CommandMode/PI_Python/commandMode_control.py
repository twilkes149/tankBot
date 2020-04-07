import serial             # Used to communicate with the arduino
import time               # Used to wait for the arduino to reboot after opening serial port
import sys                # Used to tell what OS we are running on
import http.server
import socketserver
arduino = serial.Serial()

SERVER_PORT = 8080
ENTER_COMMAND_MODE = r'+++'

HTML_DIRECTORY = "html"


class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=HTML_DIRECTORY, **kwargs)

class PI_Server:

  def __init__(self):
    self.commandMode = False
    arduino.baudrate = 9600
    # Check if we are running on windows or linux
    if sys.platform == 'win32':
      arduino.port = 'COM8'
    else:
      arduino.port = '/dev/ttyUSB0'
    self.startServer()

  def open_arduino_port(self):
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

  def startServer(self):
    with socketserver.TCPServer(("", SERVER_PORT), Handler) as httpd:
      print("Listening on port: " + str(SERVER_PORT))
      httpd.serve_forever()

  # Destructor to close the arduino port
  def __del__(self):
    if arduino.is_open:
      arduino.close()
      print("Closing arduino port")

def main():
  print("Starting PI_PY Tankbot server")
  server = PI_Server()

if __name__ == "__main__":
  main()