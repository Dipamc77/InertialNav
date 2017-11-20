#!/usr/bin/env python

from serial import Serial
from std_msgs.msg import String
from time import sleep
from vn100imu.msg import xyz_data

import rospy
ser=None

def main():
    global ser
    rospy.init_node('imu2node')
    ser = Serial('/dev/imu_nano',115200,timeout=1)
    sleep(1)
    imu2pub = rospy.Publisher('/imu2/acceleration_data',xyz_data,queue_size=1)
    imu2msg = xyz_data()
    while not rospy.is_shutdown():
        inp = ser.readline()
        if inp!="":
            #print inp
            accvals = inp.split(",")
            accx = float(accvals[0])
            accy = float(accvals[1])
            accz = float(accvals[2])
            imu2msg.XYZ.x = accx
            imu2msg.XYZ.y = accy
            imu2msg.XYZ.z = accz
            imu2pub.publish(imu2msg)
        #    print "Frame Read, Period: " + str(period) + " ms"
            print str(accx) + ',' + str(accy) + ',' + str(accz)

if __name__ == '__main__':
    main()
