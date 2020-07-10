final int TEXT_X = 400;
final int TEXT_Y = 200;

void setup() {
  size(1200,600);
}

Arm a = new Arm(80,80, 130);
int x = 444,y=150;
float shoulderAngle=0, elbowAngle=0, wristAngle=0;

void draw() {   
  //make axis the traditional one
  //scale(1, -1);
  //translate(0, -height);
  //set background color
  background(51);
  
  //draw the arm
  a.drawArm();
      
  // Draw text coordinates
  textSize(32);  
  fill(255);
  text("Shoulder: " + degrees(shoulderAngle) + 
      "\nElbow: " + degrees(elbowAngle) +
      "\nX: " + x + 
      "\nY: " + y
      , TEXT_X, TEXT_Y);

  // Movement calculations
  if (mousePressed) {
    if (sqrt(sq(mouseX-425) + sq((600-mouseY)-50)) < a.getShoulderLen() + a.getElbowLen()) {
      y = 600-mouseY;
      x = mouseX;
    }    
  }
  
  float desiredWristOrientation = PI / 4;   
  float distance = sqrt(sq(x-425) + sq(y-50));  
   
  if (distance < a.getShoulderLen() + a.getElbowLen()) {
    float q1 = atan2(-y+50,x-425);
    float q2 = -1*acos((sq(a.getShoulderLen()) - sq(a.getElbowLen())+sq(distance))/(2*a.getShoulderLen()*distance));
    float t2 = acos((sq(a.getShoulderLen()) + sq(a.getElbowLen()) - sq(distance))/(2*a.getShoulderLen()*a.getElbowLen()));
     
    shoulderAngle = -1*(q1+q2);
    elbowAngle = t2; 
    wristAngle = (PI + desiredWristOrientation) - (PI - shoulderAngle - elbowAngle);
    
    a.setShoulderAngle(shoulderAngle);
    a.setElbowAngle(elbowAngle);
    a.setWristAngle(wristAngle);
  }
  
  //System.out.println("Shoulder: " + degrees(shoulderAngle) + ", elbow: " + degrees(elbowAngle) + ", wrist: " + (360-(-1*degrees(wristAngle))));
}

void keyPressed() {
  System.out.println(keyCode);
  
  switch (keyCode) {
    // 37 <- (left arrow)
    // 38 ^  (up arrow)
    // 39 -> (right arrow)
    // 40 v  (down arrow)
    case 37:
      x--;
    break;
    
    case 38:
      y++;
    break;
    
    case 39:
      x++;
    break;
    
    case 40:
      y--;
    break;
  }
}
