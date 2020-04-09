#!/usr/bin/env python

import rospy
import SimpleHTTPServer
import SocketServer

HTML_DIRECTORY = "html"
SERVER_PORT = 8181

# This ros node will start an html server that will serve
# ... any files in the html directory, that is within the
# ... same directory as this file
# The intent is for this node to serve a rosjs webpage to
# ... enable the tankBot to be controlled over the web

def startServer():
    Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
    httpd = SocketServer.TCPServer(("", SERVER_PORT), Handler)
    print("Listening on port: " + str(SERVER_PORT))
    httpd.serve_forever()
      
if __name__ == "__main__":
    rospy.init_node('html_server')
    startServer()
    rate = rospy.Rate(1) # 1 hz
    while not rospy.is_shutdown():
        rate.sleep()
