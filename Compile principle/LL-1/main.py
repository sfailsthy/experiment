import sys
import GUI
from PyQt5.QtWidgets import QApplication, QMainWindow

if __name__ == '__main__':
    app = QApplication(sys.argv)
    MainWindow = QMainWindow()
    ui = GUI.Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

'''
E->E+T|T
T->T*F|F
F->(E)|i
'''

'''
S->BA
A->BS|d
B->aA|bS|c
'''