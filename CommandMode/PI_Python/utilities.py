#!/usr/bin/env python

import rospy
import os

from std_msgs.msg import String

def shutdown_callback(message):
	os.system("shutdown -h now")

if __name__ == "__main__":
	rospy.init_node('utilities')
	subscriber = rospy.Subscriber('/shutdown_cmd', String, shutdown_callback)
	rospy.spin()
