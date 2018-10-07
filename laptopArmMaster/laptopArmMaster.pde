import processing.serial.*;
Serial port;
void setup() {
  printArray(Serial.list());
  port = new Serial(this, Serial.list()[4], 9600);
  delay(5000);
  System.out.println("ready");
}

void draw() {
  
}

void keyPressed() {
  if (key == CODED) {
    System.out.println(keyCode);
    switch (keyCode) {
      case UP:
        port.write(2);
      break;
      
      case DOWN:
        port.write(1);
      break;
      
      case LEFT:
        port.write(4);
      break;
      
      case RIGHT:
        port.write(3);
      break;
      
      case SHIFT:
        port.write(5);
      break;
      
      case CONTROL:
        port.write(6);
      break;
    }
  }
  else {
    System.out.println(key);
  }
}

void keyReleased() {
  port.write(0);
}
