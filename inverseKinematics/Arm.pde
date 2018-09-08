class Arm {
  private ShoulderSegment shoulder;
  private ElbowSegment elbow;
  
  private float shoulderA, elbowA;
  private int l1, l2;
  
  Arm (int l1_, int l2_) {
    l1 = l1_;
    l2 = l2_;
    
    shoulder = new ShoulderSegment(425,50,l1, PI/4);
    elbow = new ElbowSegment(shoulder.getEnd().x, shoulder.getEnd().y, l2, PI/4);
  }
  
  void drawArm() {    
    rect(400,0,50,50);//draw base
    
    shoulderA = shoulder.getAngle();
    elbow.setStart(shoulder.getEnd().x,shoulder.getEnd().y);
    
    shoulder.show();
    elbow.show();
  }
  
  int getShoulderLen() {
    return l1;
  }
  
  int getElbowLen() {
    return l2;
  }
  
  void setShoulderAngle(float a) {
    shoulder.setAngle(-1*a);
    shoulderA = a;
  }
  
  void setElbowAngle(float B) {
    elbowA = PI - (B + shoulderA);
    elbow.setAngle(elbowA);
  }
}
