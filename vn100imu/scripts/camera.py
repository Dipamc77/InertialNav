#!/usr/bin/env python
import cv2
#import sys
#import subprocess
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

def main():
	cap = cv2.VideoCapture(0) # change this to cameraname
	pub = rospy.Publisher('overheadCamImage',Image,queue_size=1)
	rospy.init_node('image_capture',anonymous=False)
	rate = rospy.Rate(10) # for laptop's webcam this is max around 16 Hz
	bridge = CvBridge()
	img = None
	prevtime = rospy.Time.now()
	while not rospy.is_shutdown():
		try:
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


if __name__ == '__main__':
	main()
