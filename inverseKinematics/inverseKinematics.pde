import org.gamecontrolplus.gui.*;
import org.gamecontrolplus.*;
import net.java.games.input.*;
import processing.serial.*;

Serial arduinoPort;

ControlIO control;
ControlDevice stick;
float px, py;
boolean moveArm = false, openClaw = false, closeClaw = false;
final int BUFFER = 5;
int time = 0;

void setup() {
  size(1200,600);
  
  //Set up joystick
  // Initialise the ControlIO
  control = ControlIO.getInstance(this);
  // Find a device that matches the configuration file
  stick = control.getMatchedDevice("armJoystick");
  
  if (stick == null) {
    println("No suitable device configured");
    System.exit(-1); // End the program NOW!
  }
  
  //open arduino port
  printArray(Serial.list());
  arduinoPort = new Serial(this, Serial.list()[4], 9600);
  delay(3000);
  System.out.println("ready");
}

Arm a = new Arm(150,150, 100);
int x = 550,y=150;
float shoulderAngle=0, elbowAngle=0, wristAngle=0;

void draw() {
  //make axis the traditional one
  scale(1, -1);
  translate(0, -height);
  //set background color
  background(51);
  
  //draw the arm
  a.drawArm();
  moveArm = stick.getButton("MOVE").pressed();
  openClaw = stick.getButton("OPEN").pressed();
  closeClaw = stick.getButton("CLOSE").pressed();
  py = map(stick.getSlider("Y-AXIS").getValue(),   -1, 1, -10, 10);
  px = map(stick.getSlider("X-AXIS").getValue(), -1, 1, -10, 10);
  float desiredWristOrientation = map(stick.getSlider("Z-AXIS").getValue(), -1,1, -PI/2, PI/2);  
  
  if (moveArm) {
    
   if (px < -BUFFER) {
     x += map(px, -BUFFER, -10, 1,3);
   }
   else if (px > BUFFER) {
     x -= map(px, BUFFER, 10, 1, 3);
   }
   
   if (py < -BUFFER) {
     y += map(py, -BUFFER, -10, 1, 3);
   }
   else if (py > BUFFER) {
     y -= map(py, BUFFER, 10, 1, 3);
   }
    
   float distance = sqrt(sq(x-425) + sq(y-50));  
   
   if (distance < a.getShoulderLen() + a.getElbowLen()) {
     float q1 = atan2(y-50,x-425);
     float q2 = acos((sq(a.getShoulderLen()) - sq(a.getElbowLen())+sq(distance))/(2*a.getShoulderLen()*distance));
     float t2 = acos((sq(a.getShoulderLen()) + sq(a.getElbowLen()) - sq(distance))/(2*a.getShoulderLen()*a.getElbowLen()));
     
     shoulderAngle = -1*(q1+q2);
     elbowAngle = -1*t2; 
     wristAngle = (PI + desiredWristOrientation) - (PI - shoulderAngle - elbowAngle);
     
     a.setShoulderAngle(shoulderAngle);
     a.setElbowAngle(elbowAngle);
     a.setWristAngle(wristAngle);
   }
  }
  
  //System.out.println("Shoulder: " + -1*degrees(shoulderAngle) + ", elbow: " + -1*degrees(elbowAngle) + ", wrist: " + (360-(-1*degrees(wristAngle))));
   
   if (arduinoPort.available() > 0) {
     System.out.print("Arduino:" );
     while (arduinoPort.available() > 0) { 
       int x = arduinoPort.read();
       print(x);
       System.out.print(" ");
     }
     System.out.println();
   } 
  
  if (millis() > time+5) {
    //check if angles are positive
    float aShoulder = -1*degrees(shoulderAngle), aElbow = -1*degrees(elbowAngle), aWrist = 360-(-1*degrees(wristAngle));
    
    if (aShoulder < 0 || aShoulder > 255 ||
      aElbow < 0 || aElbow > 255 ||
      aWrist < 0 || aWrist > 255) {
        
      return;
    }
    
    byte a = byte(aShoulder);
    arduinoPort.write(a);
    
    a = byte(aElbow);
    arduinoPort.write(a);
    
    a = byte(aWrist);
    arduinoPort.write(a);
    
    if (openClaw) {
      arduinoPort.write(1);
    }
    else if (closeClaw) {
      arduinoPort.write(2);
    }
    else {
      arduinoPort.write(0);
    }
    
    time = millis();
  }
}
