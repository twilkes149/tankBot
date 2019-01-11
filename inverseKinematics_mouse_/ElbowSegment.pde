class ElbowSegment {
  PVector a;
  float angle;
  float len;
  PVector b;
 
  ElbowSegment (float x, float y, float len_, float angle_) {
    a = new PVector(x,y);
    angle = angle_;
    len = len_;
    calculateEnd();
  }
  
  void setAngle(float angle_) {
    angle = angle_;
    calculateEnd();
  }
  
  float getAngle() {
    return angle;
  }
  
  void calculateEnd() {
    float dx = len*cos(angle);
    float dy = len*sin(angle);
    
    b = new PVector(a.x+dx,a.y+dy);
  }
  
  void follow(float x, float y) {
    float dx = a.x - x;
    float dy = a.y - y;
    
    angle = PI+atan2(dy,dx);
    calculateEnd();
  }
  
  void setStart(float x, float y) {
    a.set(x,y);
  }
  
  PVector getEnd() {
    return b;
  }
  
  void show() {
    stroke(255);
    strokeWeight(4);
    line(a.x,a.y,b.x,b.y);
  }
}
