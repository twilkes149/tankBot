void setup() {
  size(1200,600);
}

Arm a = new Arm(300,300);

void draw() {
  //make axis the traditional one
  scale(1, -1);
  translate(0, -height);
  //set background color
  background(51);
  
  //draw the arm
  a.drawArm();
  
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
      System.out.println("shoulder: " + degrees(q1+q2) + " elbow: " + degrees(t2) + " mouseX: " + mouseX + " mouseY: " + realMouseY);
      
      a.setShoulderAngle(-1*(q1+q2));
      a.setElbowAngle(-1*t2);
    }
  }
}
