# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'depth_plot.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(1230, 600)
        MainWindow.setMinimumSize(QtCore.QSize(1230, 600))
        MainWindow.setMaximumSize(QtCore.QSize(1230, 600))
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.plotArea11 = PlotWidget(self.centralWidget)
        self.plotArea11.setGeometry(QtCore.QRect(10, 0, 390, 260))
        self.plotArea11.setObjectName(_fromUtf8("plotArea11"))
        self.plotArea12 = PlotWidget(self.centralWidget)
        self.plotArea12.setGeometry(QtCore.QRect(410, 0, 390, 260))
        self.plotArea12.setObjectName(_fromUtf8("plotArea12"))
        self.plotArea21 = PlotWidget(self.centralWidget)
        self.plotArea21.setGeometry(QtCore.QRect(10, 270, 390, 260))
        self.plotArea21.setObjectName(_fromUtf8("plotArea21"))
        self.plotArea22 = PlotWidget(self.centralWidget)
        self.plotArea22.setGeometry(QtCore.QRect(410, 270, 390, 260))
        self.plotArea22.setObjectName(_fromUtf8("plotArea22"))
        self.plotArea23 = PlotWidget(self.centralWidget)
        self.plotArea23.setGeometry(QtCore.QRect(810, 270, 390, 260))
        self.plotArea23.setObjectName(_fromUtf8("plotArea23"))
        self.plotArea13 = PlotWidget(self.centralWidget)
        self.plotArea13.setGeometry(QtCore.QRect(810, 0, 390, 260))
        self.plotArea13.setObjectName(_fromUtf8("plotArea13"))
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtGui.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 1230, 22))
        self.menuBar.setObjectName(_fromUtf8("menuBar"))
        MainWindow.setMenuBar(self.menuBar)
        self.mainToolBar = QtGui.QToolBar(MainWindow)
        self.mainToolBar.setObjectName(_fromUtf8("mainToolBar"))
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.mainToolBar)
        self.statusBar = QtGui.QStatusBar(MainWindow)
        self.statusBar.setObjectName(_fromUtf8("statusBar"))
        MainWindow.setStatusBar(self.statusBar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "AUV Plots", None))

from pyqtgraph import PlotWidget
