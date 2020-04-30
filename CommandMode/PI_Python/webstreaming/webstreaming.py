from imutils.video import VideoStream
from flask import Response
from flask import Flask
from flask import render_template
from flask_socketio import SocketIO
import threading
import argparse
import datetime
import imutils
import time
import cv2


outputFrame = None
lock = threading.Lock()

app = Flask(__name__)
app.config['SECRET_KEY'] = "tankBot!@#"
socketio = SocketIO(app)

vs = VideoStream(src=0).start()
time.sleep(2.0)

# Method for receiving socket messages
@socketio.on('json')
def handle_json(json):
    print(json['data'])
    print("message: " + str(json))


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
    ap = argparse.ArgumentParser()
    ap.add_argument("-i", "--ip", type=str, required=True, help="ip address of the device")
    ap.add_argument("-o", "--port", type=int, required=True, help="port number of the server")
    args = vars(ap.parse_args())

    t = threading.Thread(target=stream)
    t.daemon = True
    t.start()

    p = threading.Thread(target=start_socket_server)
    p.daemon = True
    p.start()

    app.run(host=args["ip"], port=args["port"], debug=True, threaded=True, use_reloader=False)

# Release video stream pointer
vs.stop()
