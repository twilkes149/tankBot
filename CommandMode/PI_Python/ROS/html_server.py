#!/usr/bin/env python

import rospy
import netifaces as NI
import SimpleHTTPServer
import SocketServer
from flask import Flask
from flask import render_template

app = Flask(__name__)

HTML_DIRECTORY = "html"
SERVER_HOST = "0.0.0.0" # This allows flask to use the ip address of the machine
SERVER_PORT = 8181

# This ros node will start an html server that will serve
# ... the ./templates/index.html file when a client
# ... makes a GET request to the / route.
# The intent is for this node to serve a rosjs webpage to
# ... enable the tankBot to be controlled over the web

# Legacy function used to serve all files in the html directory
def startServer():
    Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
    httpd = SocketServer.TCPServer(("", SERVER_PORT), Handler)
    print("Listening on port: " + str(SERVER_PORT))
    httpd.serve_forever()

def getIPAddress():
  ip = NI.ifaddresses('wlan0') # Get information on the wifi connection
  return ip[NI.AF_INET][0]['addr']

@app.route('/')
def index():
  ip = getIPAddress()
  print(ip)
  return render_template('index.html', ip=ip)
      
if __name__ == "__main__":
    rospy.init_node('html_server')
    app.run(host=SERVER_HOST, port=SERVER_PORT)
    #startServer()
    rate = rospy.Rate(1) # 1 hz
    while not rospy.is_shutdown():
        rate.sleep()
