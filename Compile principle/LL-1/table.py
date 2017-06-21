#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-05-30 18:19:00
# @Author  : sfailsthy (z1823339284@gmail.com)
# @function : 画出预测分析时执行步骤的表格

import os
from tkinter import Tk, Scrollbar, Frame

from tkinter.ttk import Treeview
import tkinter

import grammar

# 创建tkinter应用程序窗口
root = Tk()

# 设置窗口大小和位置
root.geometry('500x300+400+300')

# 不允许改变窗口大小
root.resizable(True, True)

# 设置窗口标题
root.title('LL(1)文法：表驱动的预测分析')

# 使用Treeview组件实现表格功能
frame = Frame(root)

frame.place(x=0, y=10, width=480, height=280)

# 滚动条
scrollBar = tkinter.Scrollbar(frame)
scrollBar.pack(side=tkinter.RIGHT, fill=tkinter.Y)

# Treeview组件，4列，显示表头，带垂直滚动条

tree = Treeview(frame, columns=('c1', 'c2', 'c3', 'c4'), show="headings", yscrollcommand=scrollBar.set)

# 设置每列宽度和对齐方式
tree.column('c1', width=120, anchor='w')
tree.column('c2', width=120, anchor='e')
tree.column('c3', width=120, anchor='e')
tree.column('c4', width=120, anchor='w')


# 设置每列表头标题文本

tree.heading('c1', text='已匹配')
tree.heading('c2', text='栈')
tree.heading('c3', text='剩余输入串')
tree.heading('c4', text='动作')


tree.pack(side=tkinter.LEFT, fill=tkinter.Y)

# Treeview组件与垂直滚动条结合
scrollBar.config(command=tree.yview)


# 定义并绑定Treeview组件的鼠标单击事件
def treeviewClick(event):
    pass

tree.bind('<Button-1>', treeviewClick)

step=grammar.step

# 插入演示数据
for i in range(len(step)):
    tree.insert('', i, values=step[i])

# 运行程序，启动事件循环
root.mainloop()
