#!/usr/bin/env python
# Trying with pyqtgraph this time
import sys
from PyQt4 import QtGui, QtCore
from ui import Ui_MainWindow


class SamplePlot(QtGui.QMainWindow):
    def __init__(self,parent=None):
        QtGui.QWidget.__init__(self,parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.val = 1
        self.xdata=[]
        self.ydata=[]
        self.ui.plotArea11.setTitle('SamplePlot')
        self._timer = QtCore.QTimer(self)
        self._timer.timeout.connect(self.play)
        self._timer.start(100)
        #self.update()
    
    def play(self):
        self.ydata.append(self.val**2)
        self.xdata.append(self.val)
        self.val+=1
        self.ui.plotArea11.plot(self.xdata,self.ydata)
        
def main():
    app = QtGui.QApplication(sys.argv)
    window = SamplePlot()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
