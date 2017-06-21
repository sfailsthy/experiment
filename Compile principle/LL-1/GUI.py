# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
from Grammar import Grammar

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1099, 884)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.tableWidget = QtWidgets.QTableWidget(self.centralwidget)
        self.tableWidget.setGeometry(QtCore.QRect(570, 50, 521, 761))
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(4)
        self.tableWidget.setRowCount(0)
        self.tableWidget.setHorizontalHeaderLabels(['已匹配', '分析栈','所剩输入串','动作'])
        font = QtGui.QFont()
        font.setBold(True)
        font.setWeight(75)
        self.tableWidget.setFont(font)
        self.textEdit = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit.setGeometry(QtCore.QRect(100, 120, 291, 171))#160, 60, 291, 171  10, 120, 511, 271
        self.textEdit.setObjectName("textEdit")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(200, 660, 93, 28))
        self.pushButton.setObjectName("pushButton")
        self.pushButton.clicked.connect(self.click)
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(710, 10, 191, 31))
        self.label.setObjectName("label")
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setGeometry(QtCore.QRect(110, 540, 271, 31))
        self.lineEdit.setObjectName("lineEdit")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(200, 80, 71, 21))
        self.label_2.setObjectName("label_2")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(150, 480, 181, 20))
        self.label_3.setObjectName("label_3")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1099, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.pushButton.setText(_translate("MainWindow", "点击分析"))
        self.label.setText(_translate("MainWindow", "进行语法分析时执行的步骤"))
        self.label_2.setText(_translate("MainWindow", "输入文法"))
        self.label_3.setText(_translate("MainWindow", "   输入要分析的句子"))


    def click(self):
        gram=self.textEdit.toPlainText().split('\n')
        sentence=self.lineEdit.text()
        grammar=Grammar()
        grammar.insert_from_arr(gram)
        if grammar.have_left_recursion()==True:
            grammar.eliminating_left_recursion()
        if grammar.have_left_factor()==True:
            grammar.left_factoring()
        follow=grammar.follow()

        if grammar.is_LL1()==False:
            print('不是LL(1)文法')
            return

        table=grammar.consuct_predictive_parsing_table(follow)
        step = grammar.predictive_parsing(table, sentence)
        for i in range(len(step)):
            self.tableWidget.insertRow(self.tableWidget.rowCount())
            for i in range(self.tableWidget.rowCount()):
                for j in range(self.tableWidget.columnCount()):
                    self.tableWidget.setItem(i,j,QTableWidgetItem(step[i][j]))


        #把分析过程写入Excel文件中

        from pyexcelerate import Workbook

        data = step # data is a 2D array
        wb = Workbook()
        wb.new_sheet("step", data=data)
        wb.save("output.xlsx")
