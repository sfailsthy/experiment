#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-05-30 18:43:20
# @Author  : sfailsthy (z1823339284@gmail.com)
# @Link    : http://example.org
# @Version : $Id$

import os
from tkinter import *
root=Tk()
def retrieve_input():
    inputValue=textBox.get("1.0","end-1c")
    return inputValue

def fuck():
    str=retrieve_input()
    if '\n' in str:
        print(True)
    else:
        print(False)

textBox=Text(root, height=20, width=40)
textBox.pack()
buttonCommit=Button(root, height=1, width=10, text="Commit",
                    command=lambda: fuck())
#command=lambda: retrieve_input() >>> just means do this when i press the button
buttonCommit.pack()

mainloop()