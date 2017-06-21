# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from lexer import *
from PyQt5.QtWidgets import *



class Ui_MainWindow(object):

    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1083, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.textEdit = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit.setGeometry(QtCore.QRect(20, 140, 431, 171))
        self.textEdit.setObjectName("textEdit")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(140, 400, 93, 28))
        self.pushButton.setObjectName("pushButton")
        self.pushButton.clicked.connect(self.click)
        self.tableWidget = QtWidgets.QTableWidget(self.centralwidget)
        self.tableWidget.setGeometry(QtCore.QRect(500, 10, 571, 541))
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(0)
        self.tableWidget.setRowCount(0)
        font = QtGui.QFont()
        font.setBold(True)
        font.setWeight(75)
        self.tableWidget.setFont(font)
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(130, 100, 251, 20))
        self.label_2.setObjectName("label_2")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1083, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "词法分析器"))
        self.pushButton.setText(_translate("MainWindow", "点我"))
        self.label_2.setText(_translate("MainWindow", "        输入代码"))

    def click(self):
        program = self.textEdit.toPlainText().split('\n')
        tokens = analy(program)

        self.tableWidget.setColumnCount(4)
        self.tableWidget.setHorizontalHeaderLabels(
            ['单词', '二元序列', '类型', '位置(行,列)'])

        for i, cur in enumerate(tokens):
            self.tableWidget.insertRow(self.tableWidget.rowCount())
            for j, item in enumerate(cur):
                if j == 1 or j == 3:
                    adjust=lambda item:'('+str(item[0])+','+str(item[1])+')'
                    self.tableWidget.setItem(
                        i, j, QTableWidgetItem(adjust(item)))
                else:
                    self.tableWidget.setItem(i, j, QTableWidgetItem(str(item)))
