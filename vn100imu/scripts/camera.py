#!/usr/bin/env python
import cv2
#import sys
#import subprocess
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge, CvBridgeError
global campubflag

def main():
	global campubflag
	campubflag = 0
	cap = cv2.VideoCapture(0) # change this to cameraname
	pub = rospy.Publisher('overheadCamImage',Image,queue_size=1)
	rospy.init_node('image_capture',anonymous=False)
	rate = rospy.Rate(10) # for laptop's webcam this is max around 16 Hz
	rospy.Subscriber('/tuning_ui/uimessages',String,uimessageCallback)
	bridge = CvBridge()
	img = None
	prevtime = rospy.Time.now()
	while not rospy.is_shutdown():
		try:
			#print campubflag
			if(campubflag == 1):
				ret,readFrame = cap.read()
				timenow = rospy.Time.now()
				perDur = timenow - prevtime
				period = perDur.to_sec()*1e3;
				if(ret == True):
					print "Frame Read, Period: " + str(period) + " ms"
					prevtime = timenow
					img = bridge.cv2_to_imgmsg(readFrame,"bgr8")
					pub.publish(img)
			rate.sleep()
		except KeyboardInterrupt:
			break

def uimessageCallback(data):
	global campubflag
	if data.data == "Camera Publish On":
		campubflag = 1
		#print campubflag
	elif data.data == "Camera Publish Off":
		campubflag = 0

if __name__ == '__main__':
	main()
