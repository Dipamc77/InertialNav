# File to generate plot for depth
# To change ui, QT is required
# http://www.pyqtgraph.org/documentation/how_to_use.html#embedding-widgets-inside-pyqt-applications
# See embedding widgets from the above link
# Create multiple instances of the PlotWidget to plot multiple things at once

import sys
import rospy
from vn100imu.msg import xyz_data
from std_msgs.msg import Bool
from PyQt4 import QtGui, QtCore
from ui import Ui_MainWindow
global subacc,subvel,_timer

class Plot(QtGui.QMainWindow):
    def __init__(self,parent=None):
        QtGui.QWidget.__init__(self,parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        rospy.init_node('plot')
        self.subacc = rospy.Subscriber('/vn100imu/acceleration_data',xyz_data,self.accelerationCallback)
        self.subvel = rospy.Subscriber('/vn100imu/velocity_data',xyz_data,self.velocityCallback)
        rospy.Subscriber('/vn100imu/sendtarecommand',Bool,self.tareCallback)
        self.startTimeNow = rospy.get_rostime()
        self.startTime = self.startTimeNow.secs + 10**-9*self.startTimeNow.nsecs
        #similarly add call backs for other topics
        self.AccX_X=[]
        self.AccX_Y=[]
        self.AccY_X=[]
        self.AccY_Y=[]
        self.AccZ_X=[]
        self.AccZ_Y=[]
        self.VelX_X=[]
        self.VelX_Y=[]
        self.VelY_X=[]
        self.VelY_Y=[]
        self.VelZ_X=[]
        self.VelZ_Y=[]
        self.ui.plotArea11.setTitle('Acceleration x')
        self.ui.plotArea12.setTitle('Acceleration y')
        self.ui.plotArea13.setTitle('Acceleration z')
        self.ui.plotArea21.setTitle('Velocity x')
        self.ui.plotArea22.setTitle('Velocity y')
        self.ui.plotArea23.setTitle('Velocity z')
        self._timer = QtCore.QTimer(self)
        self._timer.timeout.connect(self.play)
        # play function will plot all the graphs
        # keeping in same thread as callback was causing problems
        self._timer.start(50)
        #other widgets have names plotArea12, plotArea21 and plotArea22


    def play(self):
        self.ui.plotArea11.plot(self.AccX_X,self.AccX_Y)
        self.ui.plotArea12.plot(self.AccY_X,self.AccY_Y)
        self.ui.plotArea13.plot(self.AccZ_X,self.AccZ_Y)
        self.ui.plotArea21.plot(self.VelX_X,self.VelX_Y)
        self.ui.plotArea22.plot(self.VelY_X,self.VelY_Y)
        self.ui.plotArea23.plot(self.VelZ_X,self.VelZ_Y)

    def tareCallback(self,data):
        if data.data:
            self.subacc.unregister()
            self.subvel.unregister()
            self._timer.stop()
            self.AccX_X=[]
            self.AccX_Y=[]
            self.AccY_X=[]
            self.AccY_Y=[]
            self.AccZ_X=[]
            self.AccZ_Y=[]
            self.VelX_X=[]
            self.VelX_Y=[]
            self.VelY_X=[]
            self.VelY_Y=[]
            self.VelZ_X=[]
            self.VelZ_Y=[]
            self.ui.plotArea11.clear()
            self.ui.plotArea12.clear()
            self.ui.plotArea13.clear()
            self.ui.plotArea21.clear()
            self.ui.plotArea22.clear()
            self.ui.plotArea23.clear()
            self._timer.start(50)
            self.subacc = rospy.Subscriber('/vn100imu/acceleration_data',xyz_data,self.accelerationCallback)
            self.subvel = rospy.Subscriber('/vn100imu/acceleration_filtered',xyz_data,self.velocityCallback)
            print "Tare command recieved : Graphs Reset"
        self.startTimeNow = rospy.get_rostime()
        self.startTime = self.startTimeNow.secs + 10**-9*self.startTimeNow.nsecs

    def accelerationCallback(self,data):
        # depthTimeNow is a local variable
        # don't create any other variable with same name as different threads
        # will try to modify it at same time then
        self.accTimeNow = rospy.get_rostime()
        self.accTime = self.accTimeNow.secs + 10**-9*self.accTimeNow.nsecs
        self.AccX_Y.append(data.XYZ.x)
        self.AccY_Y.append(data.XYZ.y)
        self.AccZ_Y.append(data.XYZ.z)
        self.AccX_X.append(self.accTime-self.startTime)
        self.AccY_X.append(self.accTime-self.startTime)
        self.AccZ_X.append(self.accTime-self.startTime)

    def velocityCallback(self,data):
        self.velTimeNow = rospy.get_rostime()
        self.velTime = self.velTimeNow.secs + 10**-9*self.velTimeNow.nsecs
        self.VelX_Y.append(data.XYZ.x)
        self.VelY_Y.append(data.XYZ.y)
        self.VelZ_Y.append(data.XYZ.z)
        self.VelX_X.append(self.velTime-self.startTime)
        self.VelY_X.append(self.velTime-self.startTime)
        self.VelZ_X.append(self.velTime-self.startTime)

def main():
    app = QtGui.QApplication(sys.argv)
    window = Plot()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
