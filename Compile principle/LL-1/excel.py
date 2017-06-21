# -*- coding: utf-8 -*-

#把分析过程写入Excel文件中

from pyexcelerate import Workbook
import Grammar

data = Grammar.step # data is a 2D array

wb = Workbook()
wb.new_sheet("fuck", data=data)
wb.save("output.xlsx")

from numpy import *

a=random.random((2,3))
print(a)