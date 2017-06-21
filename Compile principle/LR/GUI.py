# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created by: PyQt5 UI code generator 5.8.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
from Grammar import Grammar,adjust
# from Canonical_LR import Canonical_LR,adjust

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1198, 859)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(260, 30, 72, 15))
        self.label.setObjectName("label")
        self.textEdit = QtWidgets.QTextEdit(self.centralwidget)
        self.textEdit.setGeometry(QtCore.QRect(160, 60, 291, 171))
        self.textEdit.setObjectName("textEdit")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(210, 250, 161, 20))
        self.label_2.setObjectName("label_2")
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setGeometry(QtCore.QRect(160, 290, 271, 31))
        self.lineEdit.setObjectName("lineEdit")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(240, 340, 93, 28))
        self.pushButton.setObjectName("pushButton")
        self.pushButton.clicked.connect(self.click)
        self.tableWidget = QtWidgets.QTableWidget(self.centralwidget)
        self.tableWidget.setGeometry(QtCore.QRect(0, 420, 641, 381))
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(0)
        self.tableWidget.setRowCount(0)
        font = QtGui.QFont()
        font.setBold(True)
        font.setWeight(75)
        self.tableWidget.setFont(font)
        self.tableWidget_2 = QtWidgets.QTableWidget(self.centralwidget)
        self.tableWidget_2.setGeometry(QtCore.QRect(650, 40, 541, 761))
        self.tableWidget_2.setObjectName("tableWidget_2")
        self.tableWidget_2.setColumnCount(0)
        self.tableWidget_2.setRowCount(0)
        self.tableWidget_2.setFont(font)
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(231, 390, 91, 20))
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(831, 0, 191, 20))
        self.label_4.setObjectName("label_4")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1198, 26))
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
        self.label.setText(_translate("MainWindow", "输入文法"))
        self.label_2.setText(_translate("MainWindow", "   输入要分析的句子"))
        self.pushButton.setText(_translate("MainWindow", "点击分析"))
        self.label_3.setText(_translate("MainWindow", "  LR分析表"))
        self.label_4.setText(_translate("MainWindow", "  进行语法分析时执行的步骤"))

    def click(self):
        gram = self.textEdit.toPlainText().split('\n')
        sentence = self.lineEdit.text()
        grammar=Grammar()
        grammar.insert_from_arr(gram)
        # grammar = Grammar()
        # grammar.insert_from_arr(gram)

        self.tableWidget.setColumnCount(len(grammar.terminals)+len(grammar.nonterminals))
        self.tableWidget.setHorizontalHeaderLabels(grammar.terminals+['$']+grammar.nonterminals)
        for i in range(len(grammar.terminals)+2+len(grammar.nonterminals)):
            self.tableWidget.setColumnWidth(i,65)
        self.tableWidget_2.setColumnCount(4)
        self.tableWidget_2.setHorizontalHeaderLabels(['栈', '符号', '所剩输入串', '动作'])

        C=grammar.items()
        action, goto = grammar.Constructing_an_SLR_parsing_table()
        print(action)
        print(goto)

        for i in range(len(C)):
            self.tableWidget.insertRow(self.tableWidget.rowCount())
            for j,item in enumerate(grammar.terminals):
                self.tableWidget.setItem(i,j,QTableWidgetItem(action[(i,item)]))
            self.tableWidget.setItem(i,len(grammar.terminals),QTableWidgetItem(action[(i,'$')]))

            for j,item in enumerate(grammar.nonterminals):
                self.tableWidget.setItem(i,len(grammar.terminals)+1+j,QTableWidgetItem(str(goto[(i,item)])))

        step = grammar.LR_parsing_program(action,goto,sentence)
        print(step)
        for i in range(len(step)):
            self.tableWidget_2.insertRow(self.tableWidget_2.rowCount())
            for i in range(self.tableWidget_2.rowCount()):
                for j in range(self.tableWidget_2.columnCount()):
                    self.tableWidget_2.setItem(i, j, QTableWidgetItem(step[i][j]))

        # # LR(0)自动机
        transitions = []

        for item in C:
            for symbol in grammar.terminals + grammar.nonterminals:
                if grammar.goto(item, symbol) != []:
                    transitions.append((adjust(item), adjust(
                        grammar.goto(item, symbol)), symbol))

        # for item in transitions:
        #     print(item)

        from graphviz import Digraph

        f = Digraph('finite_state_machine', filename='fsm.gv')
        f.attr(rankdir='LR', size='8,5')

        f.attr('node', shape='square')
        for transition in transitions:
            f.edge(transition[0], transition[1], label=transition[2])

        f.view()


        #把分析过程写入Excel文件中

        from pyexcelerate import Workbook

        data = step # data is a 2D array
        wb = Workbook()
        wb.new_sheet("step", data=data)
        wb.save("output.xlsx")
