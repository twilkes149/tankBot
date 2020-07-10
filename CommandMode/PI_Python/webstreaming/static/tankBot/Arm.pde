class Arm {
  private Segment shoulder;
  private Segment elbow;
  private Segment wrist;
  
  private float shoulderA, elbowA;
  private float wristA = 30;
  private int l1, l2, l3;
  
  //define max angles
  private final float SHOULDER_MAX = 130;
  private final float SHOULDER_MIN = 0;
  
  //define max angles
  private final float ELBOW_MAX = 180;
  private final float ELBOW_MIN = 45;
  
  private final float WRIST_MAX = 90;
  private final float WRIST_MIN = 270;
  
  Arm (int l1_, int l2_, int l3_) {
    l1 = l1_;
    l2 = l2_;
    l3 = l3_;
    
    shoulder = new Segment(425,550,l1, PI/4);
    elbow    = new Segment(shoulder.getEnd().x, shoulder.getEnd().y, l2, PI/4);
    wrist    = new Segment(elbow.getEnd().x, elbow.getEnd().y, l3, PI/6);
  }
  
  void drawArm() {    
    rect(400,550,50,50);//draw base
    
    shoulderA = shoulder.getAngle();
    elbow.setStart(shoulder.getEnd().x,shoulder.getEnd().y);
    wrist.setStart(elbow.getEnd().x, elbow.getEnd().y);
    
    shoulder.show();
    elbow.show();
    wrist.show();
  }
  
  int getShoulderLen() {
    return l1;
  }
  
  int getElbowLen() {
    return l2;
  }
  
  int getWristLen() {
    return l3;
  }
  
  void setShoulderAngle(float a) {
    shoulder.setAngle(-1*a);
    shoulderA = a;
  }
  
  void setElbowAngle(float B) {    
    elbowA = PI - shoulderA - B;
    elbow.setAngle(elbowA);    
  }
  
  void setWristAngle(float a) { 
    wristA = PI - (a + elbowA);
    wrist.setAngle(-1*wristA);    
  }
}
