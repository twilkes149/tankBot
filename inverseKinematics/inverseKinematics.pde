import org.gamecontrolplus.gui.*;
import org.gamecontrolplus.*;
import net.java.games.input.*;

ControlIO control;
ControlDevice stick;
float px, py, wristAngle=3*PI/4;
boolean moveArm = false;


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
}

Arm a = new Arm(150,150, 100);
int x = 550,y=150;

void draw() {
  //make axis the traditional one
  scale(1, -1);
  translate(0, -height);
  //set background color
  background(51);
  
  //draw the arm
  a.drawArm();
  moveArm = stick.getButton("MOVE").pressed();
  py = map(stick.getSlider("Y-AXIS").getValue(),   -1, 1, -10, 10);
  px = map(stick.getSlider("X-AXIS").getValue(), -1, 1, -10, 10);
  wristAngle = map(stick.getSlider("Z-AXIS").getValue(), -1,1, 3*PI/2, PI/12);
  //System.out.println(px + ", " + py + ", " + moveArm);
  
  if (moveArm) {
    
   if (px < -2) {
     x += map(px, -2, -10, 1,3);
   }
   else if (px > 2) {
     x -= map(px, 2, 10, 1, 3);
   }
   
   if (py < -2) {
     y += map(py, -2, -10, 1, 3);
   }
   else if (py > 2) {
     y -= map(py, 2, 10, 1, 3);
   }
    
    float distance = sqrt(sq(x-425) + sq(y-50));  
    
    if (distance < a.getShoulderLen() + a.getElbowLen()) {//mouse is within reach, grab it
      float q1 = atan2(y-50,x-425);
      float q2 = acos((sq(a.getShoulderLen()) - sq(a.getElbowLen())+sq(distance))/(2*a.getShoulderLen()*distance));
      float t2 = acos((sq(a.getShoulderLen()) + sq(a.getElbowLen()) - sq(distance))/(2*a.getShoulderLen()*a.getElbowLen()));
      
      //System.out.println("Distance from base: " + mouseDistance + " mouseAngle: " + degrees(q1));
      //System.out.println("shoulder: " + degrees(q1+q2) + " elbow: " + degrees(t2) + ": " + x + "," + y);
      
      a.setShoulderAngle(-1*(q1+q2));
      a.setElbowAngle(-1*t2);
      a.setWristAngle(wristAngle);
      
      System.out.println("End distance: " + calcEnd(-1*(q1+q2), -1*t2, wristAngle));
    }
  }
  
  if (mouseX > 425) {
    int realMouseY = height - mouseY;
    //calculate distance
    float mouseDistance = sqrt(sq(mouseX-425) + sq(realMouseY-50));  
    
    if (mouseDistance > a.getShoulderLen() + a.getElbowLen()) {//if the mouse is outside the reach of our arm
      
    }
    else if (mousePressed) {//mouse is within reach, grab it
      float q1 = atan2(realMouseY-50,mouseX-425);
      float q2 = acos((sq(a.getShoulderLen()) - sq(a.getElbowLen())+sq(mouseDistance))/(2*a.getShoulderLen()*mouseDistance));
      float t2 = acos((sq(a.getShoulderLen()) + sq(a.getElbowLen()) - sq(mouseDistance))/(2*a.getShoulderLen()*a.getElbowLen()));
      
      //System.out.println("Distance from base: " + mouseDistance + " mouseAngle: " + degrees(q1));
      //System.out.println("shoulder: " + degrees(q1+q2) + " elbow: " + degrees(t2) + " mouseX: " + mouseX + " mouseY: " + realMouseY);
      
      System.out.println("End distance: " + calcEnd(-1*(q1+q2), -1*t2, wristAngle));
      
      a.setShoulderAngle(-1*(q1+q2));
      a.setElbowAngle(-1*t2);
      a.setWristAngle(3*PI/4);
    }
  }
}

 PVector calcEnd(float shoulderAngle, float elbowAngle, float wristAngle) {
  PVector result = new PVector(0,0);
  
  float xElbow = a.getShoulderLen()*cos(shoulderAngle) + 425;
  float yElbow = a.getShoulderLen()*sin(shoulderAngle) + 50;
  float elbowRelativeAngle = 180 - 2*shoulderAngle-elbowAngle;
  
  float xWrist = xElbow + a.getElbowLen()*cos(elbowRelativeAngle);
  float yWrist = yElbow + a.getElbowLen()*sin(elbowRelativeAngle);
  float wristRelativeAngle = PI - 2*elbowAngle-wristAngle;
  
  result.x = xElbow;
  result.y = yElbow;
  
  //result.x = xWrist + a.getWristLen()*cos(wristRelativeAngle);
  //result.y = yWrist + a.getWristLen()*sin(wristRelativeAngle);
  
  return result;
}
