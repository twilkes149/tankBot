#!/usr/bin/env python
from imutils.video import VideoStream
from flask import Response
from flask import Flask
from flask import render_template
from flask_socketio import SocketIO
from tankBot_control import PI_Command

import threading
import argparse
import datetime
import imutils
import time
import cv2
import os


outputFrame = None
lock = threading.Lock()
arduino_lock = threading.Lock()

app = Flask(__name__)
app.config['SECRET_KEY'] = "tankBot!@#"
socketio = SocketIO(app)
arduino_control = PI_Command()

vs = VideoStream(src=0).start()
time.sleep(2.0)

def write_to_arduino():
    counter = 0
    TICK_FREQ = 4
    TICK_PERIOD = .01

    while True: # While loop "ticks every ms"
        if counter > TICK_FREQ:
            with arduino_lock:
                arduino_control.tank_tick()
            counter = 0
        with arduino_lock:
            arduino_control.write_to_arduino()

        time.sleep(TICK_PERIOD)
        counter += 1

# Method for receiving socket messages
@socketio.on('connected')
def connected(json):
    print(json['data'])

@socketio.on('shutdown')
def shutdown(message):
    print("Shutting down")
    os.system("sudo shutdown -h now")

@socketio.on('drive')
def drive_callback(json):
    #with arduino_lock:
    arduino_control.drive_commands_callback(json['data'])

@socketio.on('arm')
def arm_callback(json):
    #with arduino_lock:
    arduino_control.arm_commands_callback(json['data'])

@app.route("/")
def index():
    return render_template("index.html")

# Video streaming function
def stream():
    global vs, outputFrame, lock

    while True:
        # Grab the frame from the camera
        frame = vs.read()
        frame = imutils.resize(frame, width=400)
        with lock:
            outputFrame = frame.copy()

# convert video frame to jpg data
def generate():
    global outputFrame, lock

    while True:
        with lock:
            if outputFrame is None:
                continue
            (flag, encodedImage) = cv2.imencode(".jpg", outputFrame)

            if not flag:
                continue

        yield(b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' + bytearray(encodedImage) + b'\r\n')

@app.route("/video_feed")
def video_feed():
    return Response(generate(), mimetype = "multipart/x-mixed-replace; boundary=frame")


def start_socket_server():
    socketio.run(app)

if __name__ == "__main__":
    print("Parsing args")
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", "--ip", type=str, required=True, help="ip address of the device")
    ap.add_argument("-o", "--port", type=int, required=True, help="port number of the server")
    args = vars(ap.parse_args())

    arduino = threading.Thread(target=write_to_arduino)
    arduino.daemon = True
    arduino.start()

    video_stream = threading.Thread(target=stream)
    video_stream.daemon = True
    video_stream.start()

    socket_server = threading.Thread(target=start_socket_server)
    socket_server.daemon = True
    socket_server.start()

    app.run(host=args["ip"], port=args["port"], debug=True, threaded=True, use_reloader=False)

# Release video stream pointer
vs.stop()
