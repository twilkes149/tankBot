import processing.serial.*;
Serial arduinoPort;

boolean isInCmdMode = false;
long cmdModeTime = 0;
final long ET_TIME = 10000; // We are in command mode for 10 seconds 
final int GT_TIME = 1000; // Time to wait before entering +++

final int SHOULDER_MIN = 0;
int SHOULDER = 50;
final int SHOULDER_MAX = 100;
final int ELBOW_MIN = 0;
int ELBOW = 50;
final int ELBOW_MAX = 100;
final int WRIST_MIN = 0;
int WRIST = 110;
final int WRIST_MAX = 180;
final int CLAW_MIN = 10;
int CLAW = 30;
final int CLAW_MAX = 80;

int LEFT_DIR = 0;
int RIGHT_DIR = 1;
int LEFT_SPEED = 0;
int RIGHT_SPEED = 0;

void setup() {
  size(1200,600);

  // Print a list of available com ports
  // Open arduino port
  printArray(Serial.list());
  arduinoPort = new Serial(this, Serial.list()[4], 9600);
  delay(3000); // Wait for the arduino to reboot
  System.out.println("ready");
}

void draw() {
  fill(255);
  
  if (millis() > cmdModeTime + ET_TIME) {
    isInCmdMode = false;
  }
  
  // We are about to give the tank a command, make sure we are in command mode
  if (!isInCmdMode) {
    enterCmdMode();
  }
  
  // Send the parameters to the tank
  arduinoPort.write("atsa " + SHOULDER + "\n");
  arduinoPort.write("atea " + ELBOW + "\n");
  arduinoPort.write("atwa " + WRIST + "\n");
  arduinoPort.write("atca " + CLAW + "\n");
  
  // If we send a parameter, then the command mode timer is updated
  cmdModeTime = millis();
  delay(1);
}

void enterCmdMode() {
  System.out.println("Entering cmd mode");
  delay(GT_TIME);
  arduinoPort.write("+++");
  
  cmdModeTime = millis();
  isInCmdMode = true;
}

void stop_motors() {
  arduinoPort.write("atls 0\n");
  arduinoPort.write("atrs 0\n");
}

void keyReleased() {
  // if we just released an arrow key we need to tell the tank to stop
  switch (keyCode) {
    case 37:
    case 38:
    case 39:
    case 40:
      stop_motors();
    break;
  }
}

void keyPressed() {
  
  switch (key) {
    case 'x':
    case 'X':
      System.out.println("Shoulder up");
      
      SHOULDER += 1;
      if (SHOULDER > SHOULDER_MAX) {
        SHOULDER = SHOULDER_MAX;
      }
    break;
    
    case 'z':
    case 'Z':
      System.out.println("Shoulder down");
      
      SHOULDER -= 1;
      if (SHOULDER < SHOULDER_MIN) {
        SHOULDER = SHOULDER_MIN;
      }
    break;
    
    case 's':
    case 'S':
      System.out.println("Elbow up");
      
      ELBOW += 1;
      if (ELBOW > ELBOW_MAX) {
        ELBOW = ELBOW_MAX;
      }
    break;
    
    case 'a':
    case 'A':
      System.out.println("Elbow down");
      
      ELBOW -= 1;
      if (ELBOW < ELBOW_MIN) {
        ELBOW = ELBOW_MIN;
      }
    break;
    
    case 'w':
    case 'W':
      System.out.println("Wrist up");
      
      WRIST += 1;
      if (WRIST > WRIST_MAX) {
        WRIST = WRIST_MAX;
      }
    break;
    
    case 'q':
    case 'Q':
      System.out.println("Wrist down");
      
      WRIST -= 1;
      if (WRIST < WRIST_MIN) {
        WRIST = WRIST_MIN;
      }
    break;
    
    case 'c':
    case 'C':
      System.out.println("Claw open");
      
      CLAW += 1;
      if (CLAW > CLAW_MAX) {
        CLAW = CLAW_MAX;
      }
    break;
    
    case 'v':
    case 'V':
      System.out.println("Claw closed");
      
      CLAW -= 1;
      if (CLAW < CLAW_MIN) {
        CLAW = CLAW_MIN;
      }
    break;
    
    default:
      // We use the arrow keys for driving the wheels
      // ... and those values are retrieved from keyCode
      switch (keyCode) {
        
        case 38:
          System.out.println("Drive forward");
          
          arduinoPort.write("atld 0\n");
          arduinoPort.write("atrd 1\n");
          arduinoPort.write("atls 150\n");
          arduinoPort.write("atrs 150\n");
        break;
        
        case 40:
          System.out.println("Drive backward");
          
          arduinoPort.write("atld 1\n");
          arduinoPort.write("atrd 0\n");
          arduinoPort.write("atls 150\n");
          arduinoPort.write("atrs 150\n");
        break;
        
        case 37:
          System.out.println("Turn left");
          
          arduinoPort.write("atld 0\n");
          arduinoPort.write("atrd 0\n");
          arduinoPort.write("atls 150\n");
          arduinoPort.write("atrs 150\n");
        break;
        
        case 39:
          System.out.println("Turn right");
          
          arduinoPort.write("atld 1\n");
          arduinoPort.write("atrd 1\n");
          arduinoPort.write("atls 150\n");
          arduinoPort.write("atrs 150\n");
        break;
        
        
        default:
          System.out.println("Stop");
          stop_motors();
        break;
      }
    break;
  }
}
