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

'''
S->BB
B->aB|b
'''

'''
S->aAd|bAc|aec|bed
A->e
'''

'''
E->TG
G->+TG|-TG
T->FS
S->*FS|/FS
F->(E)|i
'''



'''
E->TA
A->+TA
T->FB
B->*FB
F->(E)
F->i
'''
