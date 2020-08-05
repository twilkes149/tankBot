import serial;
import keyboard;
import cv2;
import imutils;
import numpy as np;


cap = cv2.VideoCapture(0);
masks = [];
averaging = 7;

while True:

  # Take each frame
  _, frame = cap.read();
  height, width, channels = frame.shape#get height and width of video frame
 
  # Convert BGR to HSV
  hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
 
  # define range of blue color in HSV
  lower_green = np.array([29,86,0]);
  upper_green = np.array([105,255,250]);

 
  # Threshold the HSV image to get only blue colors
  mask = cv2.inRange(hsv, lower_green, upper_green)
  mask = cv2.erode(mask, None, iterations=2)
  mask = cv2.dilate(mask, None, iterations=2)

  #average the past n frames
  masks.append(mask);
  if len(masks) >= averaging:
    del masks[0];

  mask = masks[0];
  for i in range (1, len(masks)-1):
    mask = cv2.addWeighted(mask,.5,masks[i],.5,0);
  

  # find contours in the mask and initialize the current
  # (x, y) center of the ball
  cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
    cv2.CHAIN_APPROX_SIMPLE)[-2]
  center = None
 
  # only proceed if at least one contour was found
  if len(cnts) > 0:
    # find the largest contour in the mask, then use
    # it to compute the minimum enclosing circle and
    # centroid
    c = max(cnts, key=cv2.contourArea)
    ((x, y), radius) = cv2.minEnclosingCircle(c)
    M = cv2.moments(c)
    center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
 
    # only proceed if the radius meets a minimum size
    if radius > 10:
      # draw the circle and centroid on the frame,
      # then update the list of tracked points
      cv2.circle(frame, (int(x), int(y)), int(radius),
        (0, 255, 255), 2)
      cv2.circle(frame, center, 5, (0, 0, 255), -1)

      #tell arduino to move so tennis ball centers in frame
      x = center[0]; 
      y = center[1];

      if (x  > 2*width/3):
        print "move right";
      elif (x < width/3):
        print "move left";
      else:
        print "in center! :)";

 
  # Bitwise-AND mask and original image
  res = cv2.bitwise_and(frame,frame, mask= mask)
  cv2.imshow('frame',frame)
  cv2.imshow('mask',mask)
  cv2.imshow('res',res)
  k = cv2.waitKey(5) & 0xFF
 
  if k == 27:
      break
cv2.destroyAllWindows()