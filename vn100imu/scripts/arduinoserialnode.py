#!/usr/bin/env python

from serial import Serial
from std_msgs.msg import String
from time import sleep
from vn100imu.msg import xyz_data

import rospy
ser=None

def uimessagecallback(msg):
    if(msg.data == 'Stop'):
        ser.write("s\n")
        print "s"
    elif(msg.data == 'Forward'):
        ser.write("f\n")
        print "f"
    elif(msg.data == 'Backward'):
        ser.write("b\n")
        print "b"
    elif(msg.data == 'Right'):
        ser.write("r\n")
        print "b"
    elif(msg.data == 'Left'):
        ser.write("l\n")
        print "b"
    elif(msg.data == 'TARE'):
        print "Tare Command Sent"
    else:
        msgstr = str(msg.data)
        ssplit = msgstr.split()
        if(ssplit[0] == 'PWM'):
            num = int(ssplit[1])
            if(num<0):
                num = 0
            elif(num>255):
                num = 255
            ser.write("p"+str(num)+"\n")
            print "p"+str(num)
        elif(ssplit[0] == 'Go'):
            num = int(ssplit[1])
            if(ssplit[2] == 'cm'):
                if(num<0):
                    ser.write("b"+str(-num)+"\n")
                    print "b"+str(-num)
                elif(num>0):
                    ser.write("f"+str(num)+"\n")
                    print "f"+str(num)
                else:
                    ser.write("s\n")
                    print "s"
            elif(ssplit[2] == 'sec'):
                if(num<=0):
                    ser.write("s\n")
                    print "s"
                else:
        #            ser.write( )  # TODO: put time in arduino code
                    print "t"+str(num)


def main():
    global ser
    rospy.init_node('arduinonode')
    ser = Serial('/dev/auv_nano',115200,timeout=1)
    sleep(1)
    bontrolsub = rospy.Subscriber('/botcontrol/uimessages',String,uimessagecallback)
    velpub = rospy.Publisher('/encoder/velocity_data',xyz_data,queue_size=1)
    velmsg = xyz_data()
    prevtime = rospy.Time.now()
    while not rospy.is_shutdown():
        inp = ser.readline()
        if inp!="":
            #print inp
            timediffvals = inp.split(",")
            timediffleft = float(timediffvals[0])
            timediffright = float(timediffvals[1])
            movementflag = float(timediffvals[2]);
            if timediffleft != 0:
                velleft = 17.2e4/timediffleft/32.0
            else:
                velleft = 0.0
            if timediffright != 0:
                velright = 17.2e4/timediffright/32.0
            else:
                velright = 0.0
            velmsg.XYZ.x = velleft
            velmsg.XYZ.y = velright
            velmsg.XYZ.z = movementflag
            velpub.publish(velmsg)
            timenow = rospy.Time.now()
            perDur = timenow - prevtime
            period = perDur.to_sec()*1e3;
        #    print "Frame Read, Period: " + str(period) + " ms"
            prevtime = timenow
            print str(velleft) + ',' + str(velright)

if __name__ == '__main__':
    main()
