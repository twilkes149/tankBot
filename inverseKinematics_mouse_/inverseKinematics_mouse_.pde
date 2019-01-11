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
  int realMouseY = height - mouseY;
  if (mouseX > 425) {    
    //calculate distance
    float mouseDistance = sqrt(sq(mouseX-425) + sq(realMouseY-50));  
    
    if (mouseDistance > a.getShoulderLen() + a.getElbowLen()) {//if the mouse is outside the reach of our arm
      
    }
    else if (mousePressed) {//mouse is within reach, grab it
      float q1 = atan2(realMouseY-50,mouseX-425);
      float q2 = acos((sq(a.getShoulderLen()) - sq(a.getElbowLen())+sq(mouseDistance))/(2*a.getShoulderLen()*mouseDistance));
      float t2 = acos((sq(a.getShoulderLen()) + sq(a.getElbowLen()) - sq(mouseDistance))/(2*a.getShoulderLen()*a.getElbowLen()));
      
      a.setShoulderAngle(-1*(q1+q2));
      a.setElbowAngle(-1*t2);
      a.setWristAngle(3*PI/4);
    }
  }
  
  System.out.println("endafector: " + a.getEndX() + "," + a.getEndY() + " mouse: " + mouseX + ",  " + realMouseY);
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
