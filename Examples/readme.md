# Intro
This is the main repo for my tankBot project

# Part 1
The first part of this project is a server interface running on a raspberryPi which will render a control dashboard that streams video from the tank and has controls for the user to move the tank around.

## Motion
I used the motion library to stream video. You can view the streamed video from port 8081. Motion is designed as more of a security system application and initially the video stream was very poor (laggy). I did some research and found that motion saves jpgs every so often. After disabling that, the video stream improved enough to where you could drive the tankbot.

```bash
sudo apt-get motion
sudo service motion <start, restart, stop>
sudo motion
```

## Dashboard
The dashboard is build with bootstrapped html, with a simple image tag that streams the video. Buttons connected through sockets control the motors of the tank.

## Serial Communication
Im using the serialport npm library to comunicate with the arduino

## System
The system comprises mainly two parts: a state machine running on the arduino, and a simple nodeJS server running on a raspberry Pi. The arduino listens for an incoming byte and performs the instructed action based on the received byte:

  - 1. forward
  - 2. reverse
  - 3. left
  - 4. right
  - 5/default. stop

The nodeJS server renders a simple html page on a get request, which includes a basic javascript script that emits commands through the use of sockets. The nodeJS server will receive those commands and send an appropriate byte to the arduino.